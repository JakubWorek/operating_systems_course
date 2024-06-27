#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    struct sockaddr_in addr;
    char id[32];
    int active;
} Client;

Client clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_socket;

void broadcast_message(const char* sender_id, const char* message, struct sockaddr_in* exclude_addr) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && memcmp(&clients[i].addr, exclude_addr, sizeof(struct sockaddr_in)) != 0) {
            char buffer[BUFFER_SIZE];
            snprintf(buffer, sizeof(buffer), "[%s]: %s", sender_id, message);
            sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void handle_client_message(char* buffer, int read_size, struct sockaddr_in* client_addr) {
    buffer[read_size] = '\0';
    char client_id[32];
    int client_found = 0;
    int client_index = -1;
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && memcmp(&clients[i].addr, client_addr, sizeof(struct sockaddr_in)) == 0) {
            strncpy(client_id, clients[i].id, sizeof(client_id) - 1);
            client_found = 1;
            client_index = i;
            break;
        }
    }
    if (!client_found) {
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (!clients[i].active) {
                clients[i].addr = *client_addr;
                strncpy(clients[i].id, buffer, sizeof(clients[i].id) - 1);
                clients[i].id[sizeof(clients[i].id) - 1] = '\0';
                clients[i].active = 1;
                strncpy(client_id, clients[i].id, sizeof(client_id) - 1);
                client_id[sizeof(client_id) - 1] = '\0';
                client_index = i;
                break;
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    buffer[read_size] = '\0';
    if (strncmp(buffer, "LIST", 4) == 0) {
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            printf(" ");
            if (clients[i].active) {
                char list_message[BUFFER_SIZE];
                snprintf(list_message, sizeof(list_message), "Client: %s\n", clients[i].id);
                sendto(server_socket, list_message, strlen(list_message), 0, (struct sockaddr*)&clients[client_index].addr, sizeof(clients[client_index].addr));
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    } else if (strncmp(buffer, "2ALL", 4) == 0) {
        broadcast_message(client_id, buffer + 5, client_addr);
    } else if (strncmp(buffer, "2ONE", 4) == 0) {
        char target_id[32];
        sscanf(buffer + 5, "%s", target_id);
        char* msg = strchr(buffer + 5, ' ') + 1;
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].active && strcmp(clients[i].id, target_id) == 0) {
                char private_message[BUFFER_SIZE];
                snprintf(private_message, sizeof(private_message), "[%s to %s]: %s", client_id, target_id, msg);
                sendto(server_socket, private_message, strlen(private_message), 0, (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    } else if (strncmp(buffer, "STOP", 4) == 0) {
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].active && memcmp(&clients[i].addr, client_addr, sizeof(struct sockaddr_in)) == 0) {
                clients[i].active = 0;
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }
}

void handle_sigint(int sig) {
    printf("\nShutting down server...\n");
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            sendto(server_socket, "Server is shutting down...\n", strlen("Server is shutting down...\n"), 0, (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
            sendto(server_socket, "STOP", strlen("STOP"), 0, (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    close(server_socket);
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, handle_sigint);

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    int port = atoi(argv[1]);

    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", port);

    while (1) {
        int read_size;

        if ((read_size = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_addr_len)) > 0) {
            handle_client_message(buffer, read_size, &client_addr);
        } else {
            perror("recvfrom");
        }
    }

    close(server_socket);
    return 0;
}
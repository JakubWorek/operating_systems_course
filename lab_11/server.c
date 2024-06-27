#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    int socket;
    char id[32];
    int active;
} Client;

Client clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_socket;

void broadcast_message(const char* sender_id, const char* message, int exclude_socket) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && clients[i].socket != exclude_socket) {
            char buffer[BUFFER_SIZE];
            snprintf(buffer, sizeof(buffer), "[%s]: %s", sender_id, message);
            send(clients[i].socket, buffer, strlen(buffer), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void* handle_client(void* arg) {
    int client_socket = *((int*)arg);
    char buffer[BUFFER_SIZE];
    char client_id[32];
    int read_size;

    // Receive client id
    if ((read_size = recv(client_socket, client_id, sizeof(client_id), 0)) <= 0) {
        perror("recv");
        close(client_socket);
        return NULL;
    }
    client_id[read_size] = '\0';

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i].active) {
            clients[i].socket = client_socket;
            strcpy(clients[i].id, client_id);
            clients[i].active = 1;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    while ((read_size = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[read_size] = '\0';
        if (strncmp(buffer, "LIST", 4) == 0) {
            pthread_mutex_lock(&clients_mutex);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].active) {
                    char list_message[BUFFER_SIZE];
                    snprintf(list_message, sizeof(list_message), "Client: %s\n", clients[i].id);
                    send(client_socket, list_message, strlen(list_message), 0);
                }
            }
            pthread_mutex_unlock(&clients_mutex);
        } else if (strncmp(buffer, "2ALL", 4) == 0) {
            broadcast_message(client_id, buffer + 5, client_socket);
        } else if (strncmp(buffer, "2ONE", 4) == 0) {
            char target_id[32];
            sscanf(buffer + 5, "%s", target_id);
            char* msg = strchr(buffer + 5, ' ') + 1;
            pthread_mutex_lock(&clients_mutex);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].active && strcmp(clients[i].id, target_id) == 0) {
                    char private_message[BUFFER_SIZE];
                    snprintf(private_message, sizeof(private_message), "[%s to %s]: %s", client_id, target_id, msg);
                    send(clients[i].socket, private_message, strlen(private_message), 0);
                    break;
                }
            }
            pthread_mutex_unlock(&clients_mutex);
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            break;
        }
    }

    close(client_socket);

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket == client_socket) {
            clients[i].active = 0;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    return NULL;
}

void handle_sigint(int sig) {
    printf("\nShutting down server...\n");
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            send(clients[i].socket, "Server is shutting down...\n", strlen("Server is shutting down...\n"), 0);
            send(clients[i].socket, "STOP", strlen("STOP"), 0); // Instruct client to exit
            close(clients[i].socket);
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

    int port = atoi(argv[1]);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
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

    if (listen(server_socket, 10) == -1) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    char server_ip[INET_ADDRSTRLEN];
    getsockname(server_socket, (struct sockaddr*)&server_addr, &client_addr_len);
    inet_ntop(AF_INET, &server_addr.sin_addr, server_ip, sizeof(server_ip));
    printf("Server listening on port %d on ip %s\n", port, server_ip);

    while (1) {
        int client_socket;
        if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len)) == -1) {
            perror("accept");
            continue;
        }

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, (void*)&client_socket);
        pthread_detach(thread);
    }

    close(server_socket);
    return 0;
}
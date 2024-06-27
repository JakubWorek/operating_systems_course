#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/select.h>

#define BUFFER_SIZE 1024

int server_socket;
struct sockaddr_in server_addr;
socklen_t addr_len;

void handle_exit() {
    sendto(server_socket, "STOP", 4, 0, (struct sockaddr*)&server_addr, addr_len);
    close(server_socket);
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <id> <server_ip> <server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* client_id = argv[1];
    char* server_ip = argv[2];
    int server_port = atoi(argv[3]);

    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    addr_len = sizeof(server_addr);

    printf("Client ID: %s\n", client_id);
    sendto(server_socket, client_id, strlen(client_id), 0, (struct sockaddr*)&server_addr, addr_len);

    signal(SIGINT, handle_exit);

    char buffer[BUFFER_SIZE];
    fd_set read_fds;
    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(server_socket, &read_fds);

        if (select(server_socket + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, addr_len);
            }
        }

        if (FD_ISSET(server_socket, &read_fds)) {
            int read_size;
            if ((read_size = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &addr_len)) > 0) {
                buffer[read_size] = '\0';
                if(strcmp(buffer, "STOP") == 0) {
                    handle_exit();
                }
                printf("%s", buffer);
            }
        }
    }

    handle_exit();
    return 0;
}
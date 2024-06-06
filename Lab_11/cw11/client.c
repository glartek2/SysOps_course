#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define ID_SIZE 32

int server_socket;
char client_id[ID_SIZE];

void *receive_handler(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(server_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            printf("Disconnected from server.\n");
            close(server_socket);
            exit(EXIT_FAILURE);
        }
        if (strncmp(buffer, "ALIVE", 5) == 0) {
            send(server_socket, "ALIVE", 5, 0);
        } else {
            printf("%s", buffer);
        }
    }
    return NULL;
}

void stop_client(int signum) {
    send(server_socket, "STOP", 4, 0);
    close(server_socket);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <id> <server_address> <server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strncpy(client_id, argv[1], ID_SIZE);
    char *server_address = argv[2];
    int server_port = atoi(argv[3]);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_address);
    server_addr.sin_port = htons(server_port);

    connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    send(server_socket, client_id, ID_SIZE, 0);

    signal(SIGINT, stop_client);

    pthread_t thread;
    pthread_create(&thread, NULL, receive_handler, NULL);

    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        if (strncmp(buffer, "STOP", 4) == 0) {
            stop_client(0);
        }
        send(server_socket, buffer, strlen(buffer), 0);
    }

    close(server_socket);
    return 0;
}

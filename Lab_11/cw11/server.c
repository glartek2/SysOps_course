#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define ID_SIZE 32

typedef struct {
    int socket;
    char id[ID_SIZE];
    struct sockaddr_in addr;
    int active;
} Client;

Client clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void add_client(Client client) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (!clients[i].active) {
            clients[i] = client;
            clients[i].active = 1;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int socket) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].socket == socket) {
            clients[i].active = 0;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void list_clients(int socket) {
    char buffer[BUFFER_SIZE] = "Active clients:\n";
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].active) {
            strcat(buffer, clients[i].id);
            strcat(buffer, "\n");
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    send(socket, buffer, strlen(buffer), 0);
}

void broadcast_message(const char *sender_id, const char *message) {
    char buffer[BUFFER_SIZE];
    time_t now = time(NULL);
    snprintf(buffer, sizeof(buffer), "[%s] %s: %s", ctime(&now), sender_id, message);
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].active) {
            send(clients[i].socket, buffer, strlen(buffer), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void send_message_to(const char *sender_id, const char *receiver_id, const char *message) {
    char buffer[BUFFER_SIZE];
    time_t now = time(NULL);
    snprintf(buffer, sizeof(buffer), "[%s] %s: %s", ctime(&now), sender_id, message);
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].active && strcmp(clients[i].id, receiver_id) == 0) {
            send(clients[i].socket, buffer, strlen(buffer), 0);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    char id[ID_SIZE];
    recv(client_socket, id, sizeof(id), 0);

    Client client = {client_socket, "", {0}, 1};
    strncpy(client.id, id, ID_SIZE);
    add_client(client);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            remove_client(client_socket);
            close(client_socket);
            break;
        }

        if (strncmp(buffer, "LIST", 4) == 0) {
            list_clients(client_socket);
        } else if (strncmp(buffer, "2ALL ", 5) == 0) {
            broadcast_message(client.id, buffer + 5);
        } else if (strncmp(buffer, "2ONE ", 5) == 0) {
            char *receiver_id = strtok(buffer + 5, " ");
            char *message = strtok(NULL, "");
            if (receiver_id && message) {
                send_message_to(client.id, receiver_id, message);
            }
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            remove_client(client_socket);
            close(client_socket);
            break;
        }
    }
}

void *alive_check(void *arg) {
    while (1) {
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (clients[i].active) {
                if (send(clients[i].socket, "ALIVE", 5, 0) <= 0) {
                    clients[i].active = 0;
                    close(clients[i].socket);
                }
            }
        }
        pthread_mutex_unlock(&clients_mutex);
        sleep(5);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <address> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *address = argv[1];
    int port = atoi(argv[2]);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address);
    server_addr.sin_port = htons(port);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 10);

    pthread_t alive_thread;
    pthread_create(&alive_thread, NULL, alive_check, NULL);

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        pthread_t thread;
        pthread_create(&thread, NULL, (void *(*)(void *))handle_client, (void *)(intptr_t)client_socket);
    }

    close(server_socket);
    return 0;
}

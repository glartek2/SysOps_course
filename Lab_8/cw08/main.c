#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <semaphore.h>

#define MAX_PRINTERS 5
#define MAX_PRINTER_BUFFER_SIZE 256
#define SHARED_MEMORY_KEY 1234
#define SEMAPHORE_KEY 5678

typedef enum {
    WAITING = 0,
    PRINTING = 1
} printer_state_t;

typedef struct {
    sem_t printer_semaphore;
    char printer_buffer[MAX_PRINTER_BUFFER_SIZE];
    size_t printer_buffer_size;
    printer_state_t printer_state;
} printer_t;

typedef struct {
    printer_t printers[MAX_PRINTERS];
    int number_of_printers;
} memory_map_t;

memory_map_t *memory_map;

volatile bool should_close = false;

void SIGNAL_handler(int signum) {
    should_close = true;
}

void printer_process(int printer_id) {
    while (!should_close) {
        if (memory_map->printers[printer_id].printer_state == PRINTING) {
            memory_map->printers[printer_id].printer_state = WAITING;
            for (int j = 0; j < memory_map->printers[printer_id].printer_buffer_size; j++) {
                putchar(memory_map->printers[printer_id].printer_buffer[j]);
                fflush(stdout);
                sleep(1);
            }
            putchar('\n');
            fflush(stdout);
            memory_map->printers[printer_id].printer_state = PRINTING;
            int n = sem_post(&memory_map->printers[printer_id].printer_semaphore);
            if (n != 0) perror("sem_post failed");
        } else {
            sleep(1);
        }

        int n = sem_post(&memory_map->printers[printer_id].printer_semaphore);
        if (n != 0) perror("sem_post failed");
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <number_of_printers>\n", argv[0]);
        return -1;
    }

    long number_of_printers = atol(argv[1]);

    if (number_of_printers > MAX_PRINTERS) {
        printf("Number of printers is too big, maximum number of printers is %d\n", MAX_PRINTERS);
        return -1;
    }

    int shmid = shmget(SHARED_MEMORY_KEY, sizeof(memory_map_t), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    memory_map = (memory_map_t *)shmat(shmid, NULL, 0);
    if (memory_map == (memory_map_t *)(-1)) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    memset(memory_map, 0, sizeof(memory_map_t));
    memory_map->number_of_printers = number_of_printers;

    for (int i = 0; i < number_of_printers; i++) {
        sem_init(&memory_map->printers[i].printer_semaphore, 1, 1);

        pid_t printer_pid = fork();
        if (printer_pid < 0) {
            perror("fork");
            return -1;
        } else if (printer_pid == 0) {
            printer_process(i);
            exit(0);
        }
    }

    for (int sig = 1; sig < SIGRTMAX; sig++) {
        signal(sig, SIGNAL_handler);
    }

    while (wait(NULL) > 0) {}

    for (int i = 0; i < number_of_printers; i++) {
        sem_destroy(&memory_map->printers[i].printer_semaphore);
    }

    shmdt(memory_map);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

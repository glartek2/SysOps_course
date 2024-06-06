#include "hope.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 128

enum Hopium hope;
int executed = 1;
int request_count = 0;
int pid;

void handle_signal(int signo, siginfo_t* info, void* context) {
    pid = info->si_pid;
    int value = info->si_status;

    if (value >= 1 && value <= 3) {
        request_count += 1;
        hope = value;
        executed = 0;
    } else {
        fprintf(stderr, "Invalid status [%d]\n", value);
    }

    kill(pid, SIGUSR1);
}

void set_sigaction() {
    struct sigaction act;
    act.sa_sigaction = handle_signal;
    act.sa_flags = SA_SIGINFO;
    sigemptyset(&act.sa_mask);

    sigaction(SIGUSR1, &act, NULL);
}


void print_numbers(int from, int to) {
    for (int i = from; i <= to; i++)
        printf("%d ", i);
    printf("\n");
}

void print_requests() {
    printf("Number of all requests: %d\n", request_count);
}


void close_p() {
    printf("Antydzieńdobrnik\n");
    fflush(stdout);
    exit(0);
}

int main(int argc, char **argv) {
    printf("Catcher (PID: %d)\n", getpid());
    printf("Waiting for signals...\n\n");

    set_sigaction();

    while (1) {
        if (executed)
            continue;

        switch (hope) {
            case M_PNUMS:
                print_numbers(1, 100);
                break;
            case M_PCHANGES:
                print_requests();
                break;
            case M_EXIT:
                close_p();
                break;
        }

        executed = 1;
    }
    
    return 0;
}
#include "hope.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int success = 0;

void handle_signal(int signo) {
    success = 1;
}

void set_sigaction() {
    struct sigaction act;
    act.sa_handler = handle_signal;
    sigemptyset(&act.sa_mask);

    sigaction(SIGUSR1, &act, NULL);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        perror("[ERROR] At least 2 arguments are required\n");
        return -1;
    }

    int pid = atoi(argv[1]);

    printf("Sender (PID: %d)\n", getpid());
    printf("Catcher's PID: %d\n", pid);

    for (int i = 2; i < argc; i++) {
        const int hope = atoi(argv[i]);
        success = 0;

        set_sigaction();

        sigval_t sigval = {hope};
        sigqueue(pid, SIGUSR1, sigval);
        printf("Sent [hope %d]\n", hope);

        while (!success);

        printf("Received confirmation from catcher [hopium %d]\n", hope);
    }

    return 0;
}
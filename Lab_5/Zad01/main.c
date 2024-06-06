#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <asm/signal.h>

// Obsługa sygnału SIGUSR1
void sigusr1_handler(int signum) {
    printf("Otrzymano sygnał SIGUSR1\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Użycie: %s [none | ignore | handler | mask]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Ustawienie reakcji na sygnał SIGUSR1 zgodnie z argumentem z linii poleceń
    if (strcmp(argv[1], "none") == 0) {
        // Nie zmienia reakcji na sygnał
    } else if (strcmp(argv[1], "ignore") == 0) {
        signal(SIGUSR1, SIG_IGN); // Ignorowanie sygnału
    } else if (strcmp(argv[1], "handler") == 0) {
        signal(SIGUSR1, sigusr1_handler); // Instalacja własnego handlera
    } else if (strcmp(argv[1], "mask") == 0) {
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &mask, NULL); // Maskowanie sygnału
    } else {
        printf("Niepoprawny argument\n");
        return EXIT_FAILURE;
    }
    

    // Wysłanie sygnału SIGUSR1 do samego siebie
    raise(SIGUSR1);

    // Sprawdzenie czy wiszący/oczekujący sygnał jest widoczny
    sigset_t pending_signals;
    sigpending(&pending_signals);
    if (sigismember(&pending_signals, SIGUSR1)) {
        printf("Wiszący/oczekujący sygnał SIGUSR1 jest widoczny\n");
    } else {
        printf("Brak widocznego wiszącego/oczekującego sygnału SIGUSR1\n");
    }

    // Pauza, aby dać czas na otrzymanie sygnału
    pause();

    return EXIT_SUCCESS;
}

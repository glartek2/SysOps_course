#include <stdio.h>     // For standard I/O operations
#include <stdlib.h>    // For standard library functions
#include <unistd.h>    // For process-related functions
#include <sys/wait.h>  // For waitpid()

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments are provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_processes>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Convert the argument to an integer
    int num_processes = atoi(argv[1]);
    pid_t child_pid;

    // Loop to create specified number of child processes
    for (int i = 0; i < num_processes; ++i) {
        child_pid = fork();
        if (child_pid < 0) {
            perror("Error creating child process");
            return EXIT_FAILURE;
        } else if (child_pid == 0) { // Child process
            printf("Parent process: %d, Process ID: %d\n", getppid(), getpid());
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process waits for all child processes to terminate
    for (int i = 0; i < num_processes; ++i) {
        if (wait(NULL) == -1) {
            perror("Error waiting for child process to terminate");
            return EXIT_FAILURE;
        }
    }

    // Parent process prints argv[1] at the end
    printf("%s\n", argv[1]);

    return EXIT_SUCCESS;
}

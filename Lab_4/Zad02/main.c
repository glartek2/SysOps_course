#include <stdio.h>     // For standard I/O operations
#include <stdlib.h>    // For standard library functions
#include <unistd.h>    // For process-related functions
#include <sys/wait.h>  // For waitpid()

int global = 0; // Global variable

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments are provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int local = 0; // Local variable
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error calling fork()");
        return EXIT_FAILURE;
    } else if (pid == 0) { // Child process
        printf("child process\n");
        global++; // Increment global variable in child process
        local++;  // Increment local variable in child process
        printf("child pid = %d, parent pid = %d\n", getpid(), getppid());
        printf("child's local = %d, child's global = %d\n", local, global);
        // Execute ls command with provided directory path
        execl("/bin/ls", "ls", argv[1], NULL);
        perror("Error calling execl()");
        return EXIT_FAILURE;
    } else { // Parent process
        printf("parent process\n");
        printf("parent pid = %d, child pid = %d\n", getpid(), pid);
        int status;
        wait(&status);
        if (WIFEXITED(status)) {
            printf("Child exit code: %d\n", WEXITSTATUS(status));
        }
        printf("Parent's local = %d, parent's global = %d\n", local, global);
        return EXIT_SUCCESS;
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FIFO_PATH "/tmp/calculator_fifo"

int main() {

    int p = mkfifo(FIFO_PATH, 0666);
    if(p == -1){
        perror("Nie udało się stworzyć potoku");
    }

    double start, end;

    printf("Podaj przedział (start end): ");
    scanf("%lf %lf", &start, &end);

    int fifo_fd = open(FIFO_PATH, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Nie można otworzyć potoku nazwanego do zapisu");
        exit(EXIT_FAILURE);
    }

    write(fifo_fd, &start, sizeof(double));
    write(fifo_fd, &end, sizeof(double));

    close(fifo_fd);

    sleep(1);

    fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Nie można otworzyć potoku nazwanego do odczytu");
        exit(EXIT_FAILURE);
    }

    double result;

    read(fifo_fd, &result, sizeof(double));

    printf("Wartość całki: %f\n", result);

    close(fifo_fd);

    unlink(FIFO_PATH);

    return 0;
}

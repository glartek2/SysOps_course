#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FIFO_PATH "/tmp/calculator_fifo"

#define FUNC(x) (4.0 / ((x) * (x) + 1))

double calculate_integral(double start, double end, double step) {
    double sum = 0.0;
    double x;
    for (x = start; x < end; x += step) {
        sum += FUNC(x) * step;
    }
    return sum;
}

int main() {
    double start, end;


    int fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Nie można otworzyć potoku nazwanego do odczytu");
        exit(EXIT_FAILURE);
    }


    read(fifo_fd, &start, sizeof(double));
    read(fifo_fd, &end, sizeof(double));


    close(fifo_fd);

 
    double step_width = 0.0001;
    double result = calculate_integral(start, end, step_width);


    fifo_fd = open(FIFO_PATH, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Nie można otworzyć potoku nazwanego do zapisu");
        exit(EXIT_FAILURE);
    }


    write(fifo_fd, &result, sizeof(double));


    close(fifo_fd);

    return 0;
}

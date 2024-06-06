#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>

#define FUNC(x) (4.0 / ((x) * (x) + 1))

double calculate_integral(double start, double end, double step) {
    double sum = 0.0;
    double x;
    for (x = start; x < end; x += step) {
        sum += FUNC(x) * step;
    }
    return sum;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <step_width> <num_processes>\n", argv[0]);
        return 1;
    }

    double step_width = atof(argv[1]);
    int num_processes = atoi(argv[2]);
    double interval_length = 1.0;
    int i;
    double total_integral = 0.0;
    struct timeval start_time, end_time;

    FILE *report_file = fopen("report.txt", "w");
    if (report_file == NULL) {
        perror("Nie mozna otworzyc pliku do raportu");
        return 1;
    }

    gettimeofday(&start_time, NULL);

    int pipes[num_processes][2];

    for (i = 0; i < num_processes; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Nie utworzono potoku");
            return 1;
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("Nie utworzyc nowego watku");
            return 1;
        } else if (pid == 0) {
            close(pipes[i][0]);

            double start = i * (interval_length / num_processes);
            double end = (i + 1) * (interval_length / num_processes);
            double partial_integral = calculate_integral(start, end, step_width);


            if (write(pipes[i][1], &partial_integral, sizeof(double)) == -1) {
                perror("Blad w zapisie");
                return 1;
            }

            close(pipes[i][1]);
            return 0;
        } else {
            close(pipes[i][1]);
        }
    }


    for (i = 0; i < num_processes; i++) {
        double partial_integral;
        if (read(pipes[i][0], &partial_integral, sizeof(double)) == -1) {
            perror("Blad w odczycie");
            return 1;
        }
        total_integral += partial_integral;
        close(pipes[i][0]);
    }

    gettimeofday(&end_time, NULL);

    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + 
                          (end_time.tv_usec - start_time.tv_usec) / 1e6;

    fprintf(report_file, "Number of processes: %d\n", num_processes);
    fprintf(report_file, "Step width: %f\n", step_width);
    fprintf(report_file, "Total integral: %f\n", total_integral);
    fprintf(report_file, "Elapsed time: %f seconds\n", elapsed_time);

    fclose(report_file);

    printf("Total integral: %f\n", total_integral);
    printf("Elapsed time: %f seconds\n", elapsed_time);

    return 0;
}

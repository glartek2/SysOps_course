#include <unistd.h>    // For file operations and POSIX functions
#include <sys/stat.h>  // For file permissions
#include <fcntl.h>     // For file control options
#include <stdio.h>     // For standard I/O operations
#include <stdlib.h>    // For standard library functions
#include <sys/time.h> // For measuring time

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments are provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    
    char c;
    int in, out;
    
    // Open the input file for reading
    in = open(argv[1], O_RDONLY);
    if (in == -1) {
        perror("Error opening input file");
        return 1;
    }
    
    // Open or create the output file for writing, with appropriate permissions
    out = open("out", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (out == -1) {
        perror("Error opening or creating output file");
        close(in);
        return 1;
    }

    // Get the start time
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Get the size of the input file
    off_t file_size = lseek(in, 0, SEEK_END);

    // Reverse copy the contents of the input file to the output file
    while (file_size-- > 0) {
        // Move the file pointer to the current position
        lseek(in, file_size, SEEK_SET);
        // Read a single character from the input file
        read(in, &c, 1);
        // Write the character to the output file
        write(out, &c, 1);
    }

    // Get the end time
    gettimeofday(&end, NULL);

    // Calculate and print elapsed time
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;

    // Save results to file
    FILE *fp;
    fp = fopen("pomiar_zad_2.txt", "w");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file for writing results.\n");
        close(in);
        close(out);
        return 1;
    }
    fprintf(fp, "Time elapsed: %.6f seconds\n", elapsed);
    fclose(fp);

    // Close the input and output files
    close(in);
    close(out);

    return 0;
}

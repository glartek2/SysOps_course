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
    
    char block[1024]; // Buffer to hold read data
    int in, out;
    int count; // Number of bytes read

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

    // Read data from the input file in blocks and reverse each block
    while ((count = read(in, block, sizeof(block))) > 0) {
        // Reverse the content of the block
        for (int i = 0; i < count / 2; i++) {
            char temp = block[i];
            block[i] = block[count - i - 1];
            block[count - i - 1] = temp;
        }
        // Write the reversed block to the output file
        write(out, block, count);
        // Seek back by the number of bytes read to process the next block
        lseek(in, -count * 2, SEEK_CUR);
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

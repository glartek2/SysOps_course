#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

int main() {
    DIR *directory;
    struct dirent *entry;
    struct stat file_stat;
    long long total_size = 0;

    directory = opendir(".");
    if (directory == NULL) {
        perror("Nie można otworzyć katalogu");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(directory)) != NULL) {
        if (stat(entry->d_name, &file_stat) == -1) {
            perror("Błąd podczas pobierania informacji o pliku");
            continue;
        }

        if (!S_ISDIR(file_stat.st_mode)) {
            printf("%lld %s\n", (long long)file_stat.st_size, entry->d_name);
            total_size += file_stat.st_size;
        }
    }

    closedir(directory);

    printf("Sumaryczny rozmiar wszystkich plików: %lld\n", total_size);

    return 0;
}

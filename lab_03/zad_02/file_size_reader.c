#include <fcntl.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <stdio.h> 
#include <dirent.h>

int main() {
    DIR* dir_stream = opendir(".");
    if (dir_stream == NULL) return 1;

    struct dirent* file;
    struct stat buffer;
    long long total_size;
    int counter = 1;

    while ((file = readdir(dir_stream)) != NULL) {
        stat(file->d_name, &buffer);
        if (!S_ISDIR(buffer.st_mode)) {
            total_size += buffer.st_size;
            printf("%s %ld\n", file->d_name, buffer.st_size);
            counter++;
        }
    }
    
    printf("razem %lld \n", total_size);
    closedir(dir_stream);
    return 0;
}
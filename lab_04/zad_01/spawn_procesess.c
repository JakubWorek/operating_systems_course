#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *args[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_processes>\n", args[0]);
        return 1;
    }
    int number_of_process = atoi(args[1]);
    if (number_of_process <= 0) {
        fprintf(stderr, "Number of processes must be positive.\n");
        return 1;
    }
    for (int i = 0; i < number_of_process; i++) {
        pid_t new_process = vfork();
        if (new_process == 0) {
            printf("Parent: %d Current:%d \n", getppid(), getpid());
            exit(0);
        }
    }
    printf("Number of processes: %d \n", number_of_process);
    return 0;
}
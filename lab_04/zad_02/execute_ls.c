#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int global_variable = 0;

int main(int argc, char *args[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_path>", args[0]);
    }
    printf("Program: %s \n", args[0]);

    int local_variable = 0;
    
    pid_t new_process = fork();
    
    if (new_process == 0) {
        printf("---------------------------- \n");
        printf("Child process \n");
        global_variable++;
        local_variable++;
        printf("child pid = %d, parent pid = %d \n", getpid(), getppid());
        printf("child's local = %d, child's global = %d \n", local_variable, global_variable);
        execl("/bin/ls", "ls", args[1], NULL);
    } else {
        int child_exit_code;
        wait(&child_exit_code);
        printf("---------------------------- \n");
        printf("Parent process \n");
        printf("parent pid = %d, child pid = %d \n", getpid(), new_process);
        printf("child exit code: %d\n", WEXITSTATUS(child_exit_code));
        printf("parent's local = %d, parent's global = %d\n", local_variable, global_variable);
        return child_exit_code;
    }
    return 0;
}
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

/*
1) program tworzy dwa procesy potomne. Nastepnie proces macierzysty co sekund�
wysy�a SIGUSR1 do procesu potomnego 1. Proces potomny 1 po otrzymaniu sygna�u
przesy�a kolejn� liczb� (rozpoczynajac od zera) przez potok nienazwany do
procesu potomnego 2, kt�ry wyswietla te liczbe.

2) Po wcisnieciu ctrl-c (SIGINT) powinno nastapic przerwanie wysy�ania sygnalow.
Powtorne wcisniecie ctrl-c powinno wznowic wysylanie sygnalow
*/

int fd[2];
int sync_pipe[2];
int flag = 1;
int number = 0;
pid_t pid1, pid2;

void sigusr1_handler(int signum){
    if (flag == 1){
        write(fd[1], &number, sizeof(int));
        number++;
    }
}

void sigint_handler(int signum){
    if (flag == 0){
        flag = 1;
    } else{
        flag = 0;
    }
}

int main (int lpar, char *tab[]){
    int d,i;
    pipe(fd);
    pipe(sync_pipe);
    pid1 = fork();
    if (pid1<0){
        perror("fork");
        return 0;
    }else if (pid1==0){//proces 1
        close(fd[0]);
        signal(SIGUSR1, sigusr1_handler);
        signal(SIGINT, sigint_handler);
        write(sync_pipe[1], &number, sizeof(int));
        while(1){
            pause();
        }
        close(fd[1]);
        return 0;
    }else{
        pid2 = fork();
        if (pid2<0){
            perror("fork");
            return 0;
        }else if (pid2==0){//proces 2
            close(fd[1]);
            signal(SIGINT, sigint_handler);
            while(1){
                if (flag == 1){
                    read(fd[0], &i, sizeof(int));
                    printf("przyjeto %d bajtow, wartosc:%d\n",sizeof(int),i);
                }
            }
            close(fd[0]);
            return 0;
        }
    }
    close(fd[0]);
    close(fd[1]);
    read(sync_pipe[0], &i, sizeof(int));
    signal(SIGINT, sigint_handler);
    while(1) {
        if (flag == 1){
            kill(pid1, SIGUSR1);
            printf("wyslano SIGUSR1\n");
        } else{
            printf("wylaczono wysylanie sygnalow\n");
        } 
        sleep(1);
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv){
    if (argc == 2){
        char* filename1 = argv[1];
        int fd[2];
        pipe(fd);
        pid_t pid = fork();
        if (pid == 0){
//  zamknij deskryptor do zapisu i wykonaj program sort na filename1
//  w przypadku błędu zwróć 3

            close(fd[1]);
            dup2(fd[0], 0);
            close(fd[0]);
            if ( execlp("sort", "sort", filename1, NULL) < 0){
                perror("execlp");
                return 3;
            }
//  koniec
        }else{
            close(fd[0]);
        }
    }
    else if (argc==3){
        char* filename1 = argv[1];
        char* filename2 = argv[2];
        int fd[2];
//  otwórz plik filename2 z prawami dostępu rwxr--r--, 
//  jeśli plik istnieje otwórz go i usuń jego zawartość
        //fclose(fopen(filename2, "w"));

        // plik nie jest czyszczony jeżeli istnieje, żadna opcja ze stacka nie działa
        int file = open(filename2, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRUSR | S_IROTH);
        if (file < 0) {
            perror("open");
            return 1;
        }
//  koniec
        pipe(fd);
        pid_t pid = fork();
        if (pid == 0){
//  zamknij deskryptor do zapisu,
//  przekieruj deskryptor standardowego wyjścia na deskryptor pliku filename2 i zamknij plik,
//  wykonaj program sort na filename1
//  w przypadku błędu zwróć 3.
            close(fd[1]);
            dup2(file, 1);
            close(file);
            if( execlp("sort", "sort", filename1, NULL) < 0 ) {
                perror("execlp");
                return 3;
            }
//  koniec
        }else{
            close(fd[0]);
        }
    }
    else
        printf("Wrong number of args! \n");

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    double a, b;
    printf("Podaj przedział całkowania (a b): ");
    scanf("%lf %lf", &a, &b);

    int fd1 = open("pipe1", O_WRONLY);
    write(fd1, &a, sizeof(double));
    write(fd1, &b, sizeof(double));
    close(fd1);

    int fd2 = open("pipe2", O_RDONLY);
    double result;
    read(fd2, &result, sizeof(double));
    close(fd2);

    printf("Wynik: %f\n", result);

    return 0;
}
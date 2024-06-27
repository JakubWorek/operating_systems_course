#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

double f(double x) {
    return 4.0 / (1.0 + x*x);
}

int main() {
    double a, b;
    const char *pipe1 = "pipe1";
    const char *pipe2 = "pipe2";

    mkfifo(pipe1, 0666);
    mkfifo(pipe2, 0666);
    int fd1 = open(pipe1, O_RDONLY);
    read(fd1, &a, sizeof(double));
    read(fd1, &b, sizeof(double));
    close(fd1);

    double w = 0.01;
    double sum = 0.0;
    for(double x = a; x < b; x += w) {
        sum += f(x) * w;
    }

    int fd2 = open(pipe2, O_WRONLY);
    write(fd2, &sum, sizeof(double));
    close(fd2);
    unlink(pipe1);
    unlink(pipe2);
    return 0;
}
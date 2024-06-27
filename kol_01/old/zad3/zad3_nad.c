#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

struct pak{
 int i;
 char lit;
} ob1;

int fd;
/*program przy kazdym uzyciu klawiszy ctrl-c (SIGINT) ma wyslac obiekt o1 przez
potok nazwany 'potok1'*/
//
//
void sig_handler(int signo) {
  if (signo == SIGINT) {
    write(fd, &ob1, sizeof(ob1));
  }
}

int main (int lpar, char *tab[]){
//
//
  if ((fd=open("potok1",O_WRONLY))==-1) {
    perror("open");
    return 1;
  }

  ob1.i=0;
  ob1.lit='a';
  if (signal(SIGINT, sig_handler) == SIG_ERR) {
    printf("\ncan't catch SIGINT\n");
    return 1;
  }
  while(1) {
    printf("%d %c\n",ob1.i,ob1.lit); fflush(stdout);
    ob1.i++;
    ob1.lit=ob1.lit<'e'?ob1.lit+1:'a';
    sleep(1);
  }
  return 0;
}

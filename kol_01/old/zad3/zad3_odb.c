#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct pak{
 int i;
 char lit;
};

int main (int lpar, char *tab[]){
  int w1;
  struct pak ob1;
// 1) utworzyc potok nazwany 'potok1'
//
//
  if (mkfifo("potok1", 0666) == -1) {
    perror("mkfifo");
    return 1;
  }

  if ((w1=open("potok1",O_RDONLY))==-1) {
    perror("open");
    return 1;
  }


  while (1){
// 2) wyswietlic obiekt otrzymany z potoku
//
//
    read(w1,&ob1,sizeof(ob1));
    printf("otrzymano: %d %c\n",ob1.i,ob1.lit); fflush(stdout);
  }

  return 0;
}

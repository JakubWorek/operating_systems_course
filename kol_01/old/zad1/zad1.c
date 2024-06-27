#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <dlfcn.h>

int main (int l_param, char * wparam[]){
  int i;
  int tab[20]={1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0};
/*
1) otworz biblioteke
2) przypisz wskaznikom f1 i f2 adresy funkcji z biblioteki sumuj i srednia
3) stworz Makefile kompilujacy biblioteke 'bibl1' ladowana dynamicznie
   oraz kompilujacy ten program
*/

  double (*f1)(int *, int);
  int (*f2)(int *, int);

  void *handle = dlopen("./build/libbibl1_shared.so", RTLD_LAZY);;
  if (!handle) {
    fprintf(stderr, "%s\n", dlerror());
    exit(1);
  }
  f1 = dlsym(handle, "sumuj");
  f2 = dlsym(handle, "srednia");

  for (i=0; i<5; i++) printf("Wynik: %lf, %d\n", f1(tab+i, 20-i), f2(tab+i, 20-i));
  return 0;
}

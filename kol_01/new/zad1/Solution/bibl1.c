#include <stdlib.h>
#include <stdio.h>
#include "bibl1.h"

/*napisz biblioteke ladowana dynamicznie przez program zawierajaca funkcje:

1) zliczajaca sume n elementow tablicy tab:
int sumuj(int *tab, int n);

2) zwracajaca wynik dzielenia argumentow a i b
double dziel(int a, int b);

*/

int sumuj(int *tab, int n){
    int sum = 0;
    for (int i = 0; i < n; i++){
        sum += *(tab + i);
    }
    return sum;
}

double dziel(int a, int b){
    return (double)a/b;
}


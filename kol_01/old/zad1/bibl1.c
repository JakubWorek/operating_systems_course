#include <stdlib.h>
#include <stdio.h>
#include "bibl1.h"

/*napisz biblioteke ladowana dynamicznie przez program zawierajaca funkcje:

1) zliczajaca sume n elementow tablicy tab:
int sumuj(int *tab, int n);

2) liczaca srednia n elementow tablicy tab
double srednia(int *tab, int n);

*/

int sumuj(int *tab, int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += tab[i];
    }
    return sum;
}

double srednia(int *tab, int n) {
    return (double) sumuj(tab, n) / n;
}

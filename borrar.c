#include <stdio.h>
#include <stdlib.h>


int main(){

    int *p;
    int x=5;

    p = &x; //pasamos al apuntador p la referencia de x 

    printf("%d", *p);






    return 0;
}
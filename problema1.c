#include <stdio.h>


int main(){
    
    unsigned int n, funcion;
    unsigned int aux_n_1 = 1;
    unsigned int aux_n_2 = 0;
    scanf("%d", &n);

    funcion = n;
    for(int i=2; i<=n; i=i+1){
    
        funcion = aux_n_1 + aux_n_2 +1;
        aux_n_2 = aux_n_1;
        aux_n_1 = funcion;
        

    }

    printf("%d\n", funcion);

    return 0;
}

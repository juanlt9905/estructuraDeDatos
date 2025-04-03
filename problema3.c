#include <stdio.h>
#include <stdlib.h>


void inicializar_arreglo(int *arreglo, int n){
    

    for(int i=0; i<n; i++){
        arreglo[i] = i;
    }


}

int main(int argc, char **argv){

    int n = atoi(argv[1]);
    int arreglo[n];
    inicializar_arreglo(&arreglo[0], n); // con el & indico que quiero apuntar a la posicion donde tengo guardado arreglo[0]


    for(int i=0; i<n; i++){
        printf("%d ", arreglo[i]);
    }



    return 0;
}
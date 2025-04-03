#include <stdio.h>
#include <stdlib.h>



int main(int argc, char **argv){


    int n = atoi(argv[1]);
    double arreglo[n];
    double prom=0;

    for (int i = 0; i<n; i++){
        prom += atof(argv[i+2]);
        arreglo[i] = atoi(argv[i+2]); 
    }
    
    
    prom = prom/(double)n;

    int alumnos_arriba_prom=0;

    for (int i = 0; i<n; i++){

        if(arreglo[i]>prom){
            alumnos_arriba_prom++;
        }
    }


    return 0;
}
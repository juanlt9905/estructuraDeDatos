#include <stdio.h>
#include <stdlib.h>

int main(){

    int num_vertices, num_aristas;

    printf("Ingrese el número de vertices:\n");
    scanf("%d", &num_vertices);
    printf("Ingrese el número de aristas:\n");
    scanf("%d", &num_aristas);

    int matriz_dispersa[num_vertices][num_vertices];

    /*
    int vertices[] = {1,2,3,4,5};

    int aristas[][2] = {{1,3},
                        {2,4},
                        {2,5},
                        {4,5}};*/


    //Inicializar la matriz dispersa con ceros y uno en la diagonal.
    for (int i = 0; i<num_vertices; i++){
        for (int j = 0; j<num_vertices; j++){
            if (i==j)
                matriz_dispersa[i][j]=1;
            else 
                matriz_dispersa[i][j]=0;
        }
    }
    

    //Pedir las aristas al usuario y construir la matriz dispersa.
    int a,b;// elementos de las aristas
    printf("Ingrese las aristas en el formato: NodoA NodoB \n");
    for (int i= 0; i<num_aristas; i++){

        scanf("%d %d", &a, &b);
        matriz_dispersa[a-1][b-1] = 1;
        matriz_dispersa[b-1][a-1] = 1;
        //matriz_dispersa[aristas[i][0]-1][aristas[i][1]-1] = 1;
        //matriz_dispersa[aristas[i][1]-1][aristas[i][0]-1] = 1;
        
        //printf("La arista %d es:(%d,%d)\n", i+1, aristas[i][0], aristas[i][1]);
    }
    
    printf("La matriz dispersa es:\n");
    for(int i=0; i<num_vertices; i++){
        for (int j=0; j<num_vertices; j++){
            printf("%d ", matriz_dispersa[i][j]);
        }

        printf("\n");
    }

}
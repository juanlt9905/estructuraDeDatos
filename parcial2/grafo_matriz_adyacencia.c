#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void transponerMatriz(int num_vertices, int matriz_dispersa[num_vertices][num_vertices], int matriz_transpuesta[num_vertices][num_vertices]) {
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            matriz_transpuesta[j][i] = matriz_dispersa[i][j];
        }
    }
}
int main (){

    int num_vertices, num_aristas;

    printf("Ingrese el número de vertices:\n");
    scanf("%d", &num_vertices);
    printf("Ingrese el número de aristas:\n");
    scanf("%d", &num_aristas);

    int matriz_dispersa[num_vertices][num_vertices];
    int matriz_transpuesta[num_vertices][num_vertices];

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
        //matriz_dispersa[b-1][a-1] = 1;
 
    }
    
    printf("La matriz dispersa es:\n");
    for(int i=0; i<num_vertices; i++){
        for (int j=0; j<num_vertices; j++){
            printf("%d ", matriz_dispersa[i][j]);
        }
        printf("\n");
    }
    transponerMatriz(num_vertices, matriz_dispersa, matriz_transpuesta);

    printf("Transpuesta de un grafo es:\n");
    for(int i=0; i<num_vertices; i++){
        for (int j=0; j<num_vertices; j++){
            printf("%d ", matriz_transpuesta[i][j]);
        }
        printf("\n");
    }


    return 0;
}
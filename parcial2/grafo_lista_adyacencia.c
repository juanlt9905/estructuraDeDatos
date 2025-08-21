#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct elemento {
    int info;
    struct elemento *sigElemento;
} TNodo;
void liberarLista(TNodo *lista) {
    TNodo *temp;
    while (lista != NULL) {
        temp = lista;
        lista = lista->sigElemento;
        free(temp);
    }
}

typedef struct conjunto {
    char nombreConjunto;
    int nElementos;
    struct conjunto *sigConjunto;    
    TNodo *primerElemento;         
} Conjunto;

typedef struct {
    int numVertices;
    Conjunto **listaAdyacencia; 
} Grafo;


int main(){









    return 0;
}
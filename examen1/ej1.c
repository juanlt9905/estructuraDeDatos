#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct nodo{
    int m;
    int n;
    struct nodo *sig;
}TNodo;

void AgregarInicio(TNodo**lista, int m, int n){
    //puntero doble, ya que modificamos la cabecera de la lista
    TNodo *nuevo=NULL;
    //Agregar memoria para el nuevo nodo
    nuevo=(TNodo*) malloc(sizeof(TNodo));
    nuevo->m=m; //Asigna el valor a nodo
    nuevo->n=n;
    nuevo->sig= *lista; //Coloca nodo al inicio de lista
    *lista = nuevo; //El inicio de lista cambia al nuevo

}


void VerTodos(TNodo*lista){ //Muestra todos los nodos de la lista

    while(lista!=NULL){
        printf("%d ", lista->info);

        lista=lista->sig;

    }
}

int main(){
    int m=2;
    int n=1;

    TNodo *Pila;

    AgregarInicio(&Pila, m,n);
    



    return 0;
}
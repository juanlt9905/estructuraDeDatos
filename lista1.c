#include <stdio.h>
#include <stdlib.h>

typedef struct nodo{
    int info;
    struct nodo *sig;
}TNodo;

void AgregarInicio(TNodo**lista, int dato){
    TNodo *nuevo=NULL;
    //Agregar memoria para el nuevo nodo
    nuevo=(TNodo*) malloc(sizeof(TNodo));
    nuevo->info=dato; //Asigna el valor a nodo
    nuevo->sig= *lista; //Coloca nodo al inicio de lista
    *lista = nuevo; //El inicio de lista cambia al nuevo

}

void VerTodos(TNodo*lista){ //Muestra todos los nodos de la lista

    while(lista!=NULL){
        printf("%d ", lista->info);

        lista=lista->sig;

    }
}

void eliminarPrimerNodo(TNodo **lista){
    if (*lista!= NULL){
        TNodo * x = *lista;
        *lista = (*lista)->sig; // hace a lista apuntar al segundo nodo
        free(x);
    }
}


int main(){

    TNodo *miEstructuraLista=NULL; //inicializando mi lista
    AgregarInicio(&miEstructuraLista,5); 
    AgregarInicio(&miEstructuraLista,8);
    Agregawewe5erInicio(&miEstructuraLista,-1);
    AgregarInicio(&miEstructuraLista,20);


    VerTodos(miEstructuraLista);
    printf("\n");

    eliminarPrimerNodo(&miEstructuraLista);
    eliminarPrimerNodo(&miEstructuraLista);

    VerTodos(miEstructuraLista);
    printf("\n");



    return 0; 
}
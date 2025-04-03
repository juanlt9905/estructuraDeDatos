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
        printf("%d\n", lista->info);

        lista=lista->sig;

    }
}


int main(){

    TNodo *miEstructuraLista; //inicializando mi lista
    AgregarInicio(&miEstructuraLista,2); //porque use &?
    AgregarInicio(&miEstructuraLista,3);
    AgregarInicio(&miEstructuraLista,4);
    AgregarInicio(&miEstructuraLista,5);
    AgregarInicio(&miEstructuraLista,6);


    VerTodos(miEstructuraLista);//porque solo miEstructuraLista?




    return 0; 
}
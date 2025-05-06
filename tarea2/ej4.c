#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct nodo{
    int info;
    struct nodo *sig;
}TNodo;

void liberarLista(TNodo *lista) {
    TNodo *temp;
    while (lista != NULL) {
        temp = lista;
        lista = lista->sig;
        free(temp);
    }
}

void AgregarInicio(TNodo**lista, int dato){
    TNodo *nuevo=NULL;
    //Agregar memoria para el nuevo nodo
    nuevo=(TNodo*) malloc(sizeof(TNodo));
    nuevo->info=dato; //Asigna el valor a nodo
    nuevo->sig= *lista; //Coloca nodo al inicio de lista
    *lista = nuevo; //El inicio de lista cambia al nuevo

}

void verTodos(TNodo*lista){ //Muestra todos los nodos de la lista

    while(lista!=NULL){
        printf("%d ", lista->info);

        lista=lista->sig;

    }
}

void AgregarFinal(TNodo**lista, int dato){
    TNodo *nuevo=NULL;
    nuevo=(TNodo*) malloc(sizeof(TNodo)); //memoria para el nuevo nodo
    
    nuevo->info=dato;
    nuevo->sig=NULL;

    if(*lista ==NULL)
        *lista = nuevo;//Lista vacia, el primer nodo sera *nuevo
    else{
        TNodo * ultimo_nodo = *lista;
        //Buscamos el ultimo nodo de la lista.
        while (ultimo_nodo ->sig !=NULL){
            ultimo_nodo = ultimo_nodo->sig;
        }
        //asignamos a siguiente del ultimo nodo, al nuevo nodo.
        ultimo_nodo->sig = nuevo;
    }
}

void extraerColaConPrioridad(TNodo ** cola){


    if (*cola == NULL) {
        printf("Cola vacia\n");
        return;
    }

    TNodo *temp = *cola; //puntero que recorrera la lista
    TNodo *max = *cola; //puntero que indicara donde esta el maximo
    TNodo *antesMaximo = NULL;
    TNodo *antes_temp = NULL;

    while(temp != NULL){
        if (max->info <= temp->info){
            max = temp;
            antesMaximo= antes_temp;   
        }
        antes_temp =temp;
        temp = temp->sig;
    }

    if(antesMaximo == NULL)
        //si antesMaximo es null, el maximo es el primer elemento.
        (*cola) = (*cola)->sig;
    else
        antesMaximo->sig = max->sig;
    free(max);

}


int main(){

    srand(time(NULL));
    //Defino a mi cola como una lista
    TNodo * cola = NULL;

    for (int i =0 ; i<5; i++) AgregarInicio(&cola, rand()%100);

    verTodos(cola);
    
    for (int i =0; i<6; i++){
        printf("\n");
        extraerColaConPrioridad(&cola);
        verTodos(cola);}

    return 0;
}
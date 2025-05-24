#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct nodo{
    int info;
    struct nodo *sig;
}TNodo;
void AgregarFinal(TNodo**lista, int dato){//funcion para agregar elemnetos al final de una lista
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


void VerTodos(TNodo*lista){ //Muestra todos los nodos de la lista

    while(lista!=NULL){
        printf("%d ", lista->info);

        lista=lista->sig;

    }
}


int main(int argc, char **argv){


    int n = atoi(argv[1]); //
    TNodo *lista_primos=NULL;

   // esPrimo = 0;// Bandera para saber si es primo
    for (int i=2; i<=n; i++){
        int esPrimo=1;
        for (int j=2; j<i; j++){
            if (i%j==0){
                esPrimo=0;
            }
        }
        if (esPrimo==1) AgregarFinal(&lista_primos, i);
    }

    VerTodos(lista_primos);


    return 0;
}
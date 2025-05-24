#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct nodo{
    int dato;
    struct * nodo hizoIzq;
    struct * nodo hijoDer;
}TNodo;

void AgregarHijos(TNodo**nodo, int datoIzq, int datoDer){
    //puntero doble, ya que modificamos la cabecera de la lista
    TNodo *hijoIzq =NULL;
    TNodo *hijoDer =NULL;
    hijoIzq=(TNodo*) malloc(sizeof(TNodo));
    hijoDer=(TNodo*) malloc(sizeof(TNodo));

    
    nuevo->estado=estado; //Asigna el valor a nodo
    nuevo->sig= *lista; //Coloca nodo al inicio de lista
    *lista = nuevo; //El inicio de lista cambia al nuevo

}

int main(){







    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct nodo{
    int info;
    struct nodo *izq;
    struct nodo *der;
}TNodo;

int agregarNodo(TNodo**raiz, int dato)
{
    TNodo*padre = NULL;
    TNodo*actual = *raiz;
    int enc=0;
    /* Buscar el dato en el árbol, manteniendo un puntero al nodo padre */
    while(actual!=NULL && enc==0) {
        padre = actual;
        if(dato < actual->info)
            actual = actual->izq;
        else if(dato > actual->info)
            actual = actual->der;
        else
            enc=1;
    }
    /* Si se ha encontrado el elemento, regresar sin insertar */
    if(enc==1) return 0;
    // Crea el nuevo nodo
    TNodo*nuevo = (TNodo*)malloc(sizeof(TNodo));
    nuevo->info= dato;
    nuevo->izq=NULL;
    nuevo->der=NULL;
    /* Si padre es NULL, entonces el árbol estaba vacío, el nuevo nodo será
    el nodo raiz*/
    if(padre==NULL) {
        *raiz=nuevo;
    }
    /* Si el dato es menor que el que contiene el nodo padre, lo insertamos
    en la rama izquierda */
    else if(dato < padre->info) //Si es menor que el padre lo insertamos a la izq.
        padre->izq= nuevo;
    else// Si es mayor lo insertamos a la derecha
        padre->der = nuevo;
    return 1;
}
TNodo*buscarNodo(TNodo*raiz, int dato){
    if(raiz==NULL)
        return NULL;    
    else if(raiz->info== dato)
        return raiz;
    else f(dato < raiz->info)
        return buscarNodo(raiz->izq, dato);
    else
        return buscarNodo(raiz->der, dato);
}
void inOrden(TNodo*raiz){
    if(raiz!=NULL){
    inOrden(raiz->izq);
    printf("%d ", raiz->info);
    inOrden(raiz->der);
    }
}
void preOrden(TNodo*raiz){
    if(raiz!=NULL){
    printf("%d ", raiz->info);
    preOrden(raiz->izq);
    preOrden(raiz->der);
    }
}
void posOrden(TNodo*raiz){
    if(raiz!=NULL){
    posOrden(raiz->izq);
    posOrden(raiz->der);
    printf("%d ", raiz->info);
    }
}

int main(){

    TNodo *raiz=NULL;
    agregarNodo(&raiz, 20);
    agregarNodo(&raiz, 10);
    agregarNodo(&raiz, 30);
    agregarNodo(&raiz, 8);
    agregarNodo(&raiz, 15);
    agregarNodo(&raiz, 25);
    agregarNodo(&raiz, 40);

    printf("In Order: \n");
    inOrden(raiz);
    printf("\n Pre Order: \n");
    preOrden(raiz);
    printf("\n Pos Order: \n");
    posOrden(raiz);
    printf("\n");




    return 0;
}
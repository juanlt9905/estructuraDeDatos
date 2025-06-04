#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct nodo {
    int info;
    struct nodo *izq;
    struct nodo *der;
    int rthread; // 1: es hebra, 0: no hebra
} TNodo;


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
void preOrden(TNodo*raiz){
    if(raiz!=NULL){
    printf("%d ", raiz->info);
    preOrden(raiz->izq);
    preOrden(raiz->der);
    }
}

void rotacionDerecha(TNodo **raiz, int dato){

    TNodo*padre = NULL;
    TNodo*actual = *raiz;
    
    /* Buscar el dato en el árbol, manteniendo un puntero al nodo padre */
    while(actual!=NULL && actual->info!=dato) {
        padre = actual;
        if(dato < actual->info)
            actual = actual->izq;
        else
            actual = actual->der;
    }
    if (actual ==NULL){
        printf("No se encontro el nodo a rotar.\n");
        return;
    }
    
    TNodo *subir= actual->izq;
    actual->izq = subir->der;
    subir->der = actual;
    
    if(padre==NULL){
        *raiz=subir;
        return;
    }
    //Ajustar padre
    if (padre->info>dato) padre->izq =subir;
    else padre->der =subir;

}
/*
void rotacionIzquierda(TNodo *raiz, int info_nodo_buscado){

    TNodo*padre = NULL;
    TNodo*actual = *raiz;
    int enc=0;
     Buscar el dato en el árbol, manteniendo un puntero al nodo padre
    while(actual!=NULL && enc==0) {
        padre = actual;
        if(dato < actual->info)
            actual = actual->izq;
        else if(dato > actual->info)
            actual = actual->der;
        else
            enc=1;
    }
    if (enc==0){
        printf("No se encontro el nodo a rotar.\n");
        return;
    }

    TNodo *subir= actual->izq;
    subir->der = actual;
    actual->left = subir->der;

    //Ajustar padre
    if (padre->info>dato) padre->izq =subir;
    else padre->der =subir;

}
*/

int main(){

    TNodo *raiz =NULL;

    agregarNodo(&raiz, 10);
    agregarNodo(&raiz, 9);
    agregarNodo(&raiz, 8);

    preOrden(raiz);

    rotacionDerecha(&raiz, 10);
    printf("despues de rotacion \n");
    preOrden(raiz);

    return 0;
}
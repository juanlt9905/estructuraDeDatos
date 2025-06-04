#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct nodo {
    int info;
    struct nodo *izq;
    struct nodo *der;
    int rthread; // 1: es hebra, 0: no hebra
} TNodo;

int agregarNodo_hiladoDerecha(TNodo**raiz, int dato)
{
    TNodo*padre = NULL;
    TNodo*actual = *raiz;
    int enc=0;
    /* Buscar el dato en el árbol, manteniendo un puntero al nodo padre */
    while(actual!=NULL && enc==0) {
        padre = actual;
        if(dato < actual->info)
            actual = actual->izq;
        else if(dato > actual->info){
            if (actual->rthread==0)
               actual = actual->der;
            else{
                actual=NULL; //salimos del bucle
            }
        }
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
    nuevo->rthread=0;
    /* Si padre es NULL, entonces el árbol estaba vacío, el nuevo nodo será
    el nodo raiz*/
    if(padre==NULL) {
        *raiz=nuevo;
    }
    /* Si el dato es menor que el que contiene el nodo padre, lo insertamos
    en la rama izquierda */
    else if(dato < padre->info) {//Si es menor que el padre lo insertamos a la izq.
        padre->izq= nuevo;
        nuevo->der =padre; //hilado al padre
        nuevo->rthread = 1; 
    }
    else{// Si es mayor lo insertamos a la derecha
        nuevo->der=padre->der; //si padre->der es null o es hebra, le hereda a nuevo su direccion
        nuevo->rthread = padre->rthread; //nuevo es hebra ahora si su padre lo fue
        
        if (padre->rthread==1) padre->rthread=0; //si el padre era hebra, deja de serlo
        padre->der = nuevo;  //asignamos como hijo derecho a padre
    }
    return 1;
}

void rotarDerecha(TNodo **raiz, int dato){

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

    


}
TNodo *leftmost(TNodo *n)
{
    TNodo *ans = n;
    if(ans == NULL)
        return NULL;
    while(ans->izq !=NULL)
        ans = ans->izq;
    return ans;
}

void inOrder_iterative(TNodo *n)
{
    TNodo *cur= leftmost(n);
    while(cur !=NULL) {
        printf("%d ", cur->info);
        if(cur->rthread == 1)
            cur = cur->der;
        else
            cur = leftmost(cur->der);

}
}
int main(){

    TNodo *raiz=NULL;
    agregarNodo_hiladoDerecha(&raiz, 20);
    agregarNodo_hiladoDerecha(&raiz, 10);
    agregarNodo_hiladoDerecha(&raiz, 30);
    agregarNodo_hiladoDerecha(&raiz, 8);
    agregarNodo_hiladoDerecha(&raiz, 15);
    agregarNodo_hiladoDerecha(&raiz, 25);
    agregarNodo_hiladoDerecha(&raiz, 40);

    printf("Arbol de ejemplo 1 (recorrido inOrder):\n");
    inOrder_iterative(raiz);
    printf("\n");

    TNodo *raiz2=NULL;

    
    agregarNodo_hiladoDerecha(&raiz2, 6);
    agregarNodo_hiladoDerecha(&raiz2, 3);
    agregarNodo_hiladoDerecha(&raiz2, 8);
    agregarNodo_hiladoDerecha(&raiz2, 1);
    agregarNodo_hiladoDerecha(&raiz2, 5);
    agregarNodo_hiladoDerecha(&raiz2, 7);
    agregarNodo_hiladoDerecha(&raiz2, 11);
    agregarNodo_hiladoDerecha(&raiz2, 9);
    agregarNodo_hiladoDerecha(&raiz2, 13);

    printf("Arbol de ejemplo 2 (recorrido inOrder):\n");
    inOrder_iterative(raiz2);
    printf("\n");

    TNodo *raiz3=NULL;
    agregarNodo_hiladoDerecha(&raiz3, 3);
    agregarNodo_hiladoDerecha(&raiz3, 2);
    agregarNodo_hiladoDerecha(&raiz3, 6);
    agregarNodo_hiladoDerecha(&raiz3, 1);
    agregarNodo_hiladoDerecha(&raiz3, 4);
    agregarNodo_hiladoDerecha(&raiz3, 8);
    agregarNodo_hiladoDerecha(&raiz3, 5);
    agregarNodo_hiladoDerecha(&raiz3, 7);
    agregarNodo_hiladoDerecha(&raiz3, 9);

    printf("Arbol de ejemplo 3 (recorrido inOrder):\n");
    inOrder_iterative(raiz3);
    printf("\n");

    return 0;
}
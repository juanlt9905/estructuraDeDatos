#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct nodo {
    int info;
    struct nodo *izq;
    struct nodo *der;
    struct nodo *nodoPadre;
    //    int hebra; // 1: es hebra, 0: no hebra
    int color; // 0 rojo, 1 negro
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
    nuevo->nodoPadre=padre;
    nuevo->color=0; //todo nodo insertado es rojo
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

TNodo * buscarNodo(TNodo *raiz, int dato_buscado){//regresa un apuntador al nodo buscado del arbol
    TNodo *actual=raiz;
    //busqueda del nodo
    while(actual!=NULL && actual->info!=dato_buscado) {
        if(dato_buscado < actual->info)
            actual = actual->izq;
        else
            actual = actual->der;
    }
    actual;
}

void rotacionDer(TNodo **raiz, int dato_rotar){
    //Buscar el nodo a rotar en el arbol
    TNodo *nodo_rotar= buscarNodo(*raiz, dato_rotar);
    if (nodo_rotar==NULL){ //si no se encuentra el nodo en el arbol
        printf("No se encontro el nodo a rotar. \n");
        return;
    }
    if (nodo_rotar->izq == NULL) { //VAlidacion de hijo izquierdo del nodo a rotar
        printf("No se puede hacer rotacion a la derecha porque hijoIzq es NULL.\n");
        return;
    }
    
    TNodo *nodo_subir= nodo_rotar->izq;//El nodo izquierdo del nodo a rotar sera el nodo que "subira"

    nodo_rotar->izq= nodo_subir->der;
    if(nodo_subir ->der!=NULL)nodo_subir->der->nodoPadre=nodo_rotar;

    nodo_subir->der = nodo_rotar;
    
    nodo_subir->nodoPadre = nodo_rotar->nodoPadre;
    nodo_rotar->nodoPadre = nodo_subir;

    if(nodo_subir->nodoPadre==NULL){
        *raiz= nodo_subir; //el nodo a subir es la raiz del arbol
    }else if (nodo_rotar==nodo_subir->nodoPadre->izq)
        nodo_subir->nodoPadre->izq= nodo_subir;
    else
        nodo_subir->nodoPadre->der= nodo_subir;

}

void rotacionIzq(TNodo **raiz, int dato_rotar){
    TNodo *nodo_rotar= buscarNodo(*raiz, dato_rotar);

    if (nodo_rotar==NULL){ //si no se encuentra el nodo en el arbol
        printf("No se encontro el nodo a rotar. \n");
        return;
    }
    if (nodo_rotar->der == NULL) { //Validacion de hijo derecho del nodo a rotar
        printf("No se puede hacer rotacion a la izquierda porque hijoDer es NULL.\n");
        return;
    }

    TNodo *nodo_subir= nodo_rotar->der;// el nodo derecho del nodo a rotar "subira"

    nodo_rotar->der= nodo_subir->izq;
    if(nodo_subir ->izq!=NULL)nodo_subir->izq->nodoPadre=nodo_rotar;

    nodo_subir->izq=nodo_rotar;

    //Asignacion de padres
    nodo_subir->nodoPadre= nodo_rotar->nodoPadre;
    nodo_rotar->nodoPadre= nodo_subir;

    if(nodo_subir->nodoPadre==NULL)
        *raiz=nodo_subir;
    else if (nodo_rotar==nodo_subir->nodoPadre->izq)
        nodo_subir->nodoPadre->izq= nodo_subir;
    else
        nodo_subir->nodoPadre->der= nodo_subir;
}
void preOrden(TNodo*raiz){
    if(raiz!=NULL){
        printf("%d ", raiz->info);
        preOrden(raiz->izq);
        preOrden(raiz->der);
    }
}


int main(){

    TNodo *raiz=NULL;
    //Insertar los nodos
    while(1){

        int nuevo_elemento, agregado;
        printf("Agregar nodo al arbol: \n");
        scanf("%d", &nuevo_elemento);
        if (nuevo_elemento==-1)break; //usuario termina la insercion de nodos
        
        agregado=agregarNodo(&raiz, nuevo_elemento);
        if (agregado==1)
            printf("Nodo %d agregado.\n", nuevo_elemento);
        else
            printf("El nodo %d ya existe en el arbol.\n", nuevo_elemento);
    }

    //Rotar dinamicamente el arbol.
    int opcion, nodoRotar;
    while (1) {
        //imprimir el árbol despues de cada rotación
        printf("\n***** Arbol (PreOrden)*****\n");
        if (raiz == NULL) {
            printf("El arbol esta vacio.\n");
        } else {
            preOrden(raiz);
        }
        printf("\n***************************************\n");

        // Muestra el menú de opciones al usuario.
        printf("Opciones de operación:\n");
        printf("1. Rotacion a la Derecha\n 2. Rotacion a la Izquierda\n 3. Terminar\n");
        scanf("%d", &opcion);

        if(opcion ==1){
            printf("Introduce el nodo a rotar a la derecha:\n ");
            scanf("%d", &nodoRotar);
            rotacionDer(&raiz, nodoRotar);
        }else if (opcion==2){
            printf("Introduce el nodo a rotar a la izquierda: \n");
            scanf("%d", &nodoRotar);
            rotacionIzq(&raiz, nodoRotar);
        }else{
            break;
        }

    }

    return 0;




}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 100

typedef struct{
    int elementos[MAX];
    int nElementos;
} Heap;

void swap(int *x, int *y) {
  int temp;
  temp = *x;
  *x = *y;
  *y = temp;
}

void insertarElementoHeap(Heap * heap, int dato){
    if (heap->nElementos==MAX){
        printf("Heap lleno\n");
        return;
    }

    heap->elementos[heap->nElementos++] = dato; //inserta el nuevo elemento al final.
    int aux, j;
    int k=heap->nElementos-1;
    while(k>0 && heap->elementos[k] > heap->elementos[(k-1)/2]){
        //saldra del ciclo while cuando encuentre su posicion (su padre sea mayor)
        aux = heap->elementos[(k-1)/2];
        heap->elementos[(k-1)/2] = heap->elementos[k];
        heap->elementos[k]=aux;
        k=(k-1)/2;    
        }

}

void inicializarHeap(Heap * heap){
    heap->nElementos=0;
}

void extraerMax(Heap * heap){
    if (heap->nElementos == 0) {
        printf("Cola vacia\n");
        return; 
    }

    int k=0;

    heap->elementos[0] = heap -> elementos[heap->nElementos-1];
    heap->nElementos--;

    int bandera=0;
    while(bandera ==0){

        int hijoIzq = 2*k+1; //indices de localizacion de los hijos 
        int hijoDer = 2*k+2;
        int mayor = k; //indice que almacenara el mayor elemento


        if(hijoIzq < heap->nElementos && heap->elementos[hijoIzq] > heap->elementos[mayor])
            mayor = hijoIzq;
        if (hijoDer < heap->nElementos  && heap->elementos[hijoDer] > heap->elementos[mayor])
            mayor=hijoDer;
        if (mayor!=k){
            swap(&heap->elementos[k], &heap->elementos[mayor]);
            k=mayor;
        }
        else
            bandera=1; //ya se encuentra en su posicion de la cola respetando el heap

}       

}



int main(){

    Heap heap;
    inicializarHeap(&heap);
    //inicializarCola(&miEstructuraCola);
    insertarElementoHeap(&heap, 22);
    insertarElementoHeap(&heap, 10);
    insertarElementoHeap(&heap, 52);
    insertarElementoHeap(&heap, 15);
    insertarElementoHeap(&heap, 33);
    insertarElementoHeap(&heap, 40);
    insertarElementoHeap(&heap, 38);
    insertarElementoHeap(&heap, 60);
    insertarElementoHeap(&heap, 73);
    insertarElementoHeap(&heap, 45);
    insertarElementoHeap(&heap, 95);
    insertarElementoHeap(&heap, 70);

    for (int i=0; i<heap.nElementos; i++){
        printf("%d ", heap.elementos[i]);
    }
    printf("\n");
    extraerMax(&heap);

    for (int i=0; i<heap.nElementos; i++){
        printf("%d ", heap.elementos[i]);
    }    

    printf("\n");








    return 0;
}
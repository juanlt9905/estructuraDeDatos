#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 100

typedef struct{
    int elementos[MAX];
    int nElementos;
} Heap;


void insertarElementoHeap(Heap * heap, int dato){
    if (heap->nElementos==MAX){
        printf("Heap lleno\n");
        return;
    }

    heap->elementos[heap->nElementos++] = dato; //inserta el nuevo elemento al final.
    int bandera=1, aux, j;
    int k=heap->nElementos-1;
    while(k>0 && bandera ==1){
        bandera =0;//saldra del ciclo while cuando encuentre su posicion (su padre sea mayor)
        if(heap->elementos[k]>heap->elementos[(k-1)/2]){
            aux = heap->elementos[(k-1)/2];
            heap->elementos[(k-1)/2] = heap->elementos[k];
            heap->elementos[k]=aux;
            k=(k-1)/2;
            bandera=1;
            }
        }

}

void inicializarHeap(Heap * heap){
    heap->nElementos=0;
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








    return 0;
}
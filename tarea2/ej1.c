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

void VerTodos(TNodo*lista){ //Muestra todos los nodos de la lista

    while(lista!=NULL){
        printf("%d ", lista->info);

        lista=lista->sig;

    }
}

int VerUltimoElemento(TNodo**lista){ //Muestra todos los nodos de la lista

    TNodo* temp = *lista;
    while(temp->sig !=NULL){
        temp=temp->sig;
    }
    return temp->info;
}

int existeElemento(TNodo **lista, int dato){

    int existe = 0;
    TNodo * temp = *lista;

    while(temp != NULL){
        if(temp->info == dato){
            existe=1;
            break;
        }
        temp=temp->sig;
    }
    return existe;
}

TNodo * ordenarListasOrdenadas_a(TNodo**listaA, TNodo **listaB){
    TNodo * listaC = NULL; //Lista ordenada

    TNodo * tempA = *listaA;
    TNodo * tempB = *listaB;

    while(tempA!= NULL && (tempB!= NULL)){
        if(tempA->info == tempB->info){
            if(existeElemento(&listaC, tempA->info)==0)AgregarFinal(&listaC, tempA->info);
            tempA=tempA->sig;
            tempB=tempB->sig;
        }
        else if(tempA->info < tempB->info){
            
            if(existeElemento(&listaC, tempA->info)==0)AgregarFinal(&listaC, tempA->info);
            tempA = tempA->sig;
        }
        else{
            if(existeElemento(&listaC, tempB->info)==0)AgregarFinal(&listaC, tempB->info);
            tempB = tempB->sig;
        }
    }
    //Vaciando en listaC la lista que no fue ordenada
    while(tempA!=NULL){ 
        if(existeElemento(&listaC, tempA->info)==0){
            AgregarFinal(&listaC, tempA->info);
        }
        tempA = tempA->sig;
    }    
    while(tempB!=NULL){
        if (existeElemento(&listaC, tempB->info)==0){
            AgregarFinal(&listaC, tempB->info); 
        }
        tempB = tempB->sig;
        
    }

    
    return listaC;
}

int main(int argc, int **argv){


    TNodo *lista = NULL;

    //Inicializamos las dos listas A y B
    TNodo *listaA = NULL;
    TNodo *listaB = NULL;
    int nA, nB; //longitud de las listas

    printf("Ingresa la longitud de las listas con el formato: LongitudA LongitudB:\n");
    scanf("%d %d", &nA, &nB);
    //Lista Ordenada A

    printf("Ingrese la lista Ordenada A:\n");
    for (int i = 0; i<nA;i++){
        int elemento;
        scanf("%d", &elemento);
        AgregarFinal(&listaA, elemento);
    }

    printf("Ingrese la lista Ordenada B:\n");
    for (int i = 0; i<nB; i++){
        int elemento;
        scanf("%d", &elemento);
        AgregarFinal(&listaB, elemento);
    }


    printf("La lista Ordenada es: \n");
    TNodo * listaC = ordenarListasOrdenadas_a(&listaA, &listaB);
    VerTodos(listaC);
    

    return 0;
}
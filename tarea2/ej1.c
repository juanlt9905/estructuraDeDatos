#include <stdio.h>
#include <stdlib.h>

/*************************************************************************************** 
1. Dadas dos listas ordenadas A y B. Desarrolle un programa que devuelvan una lista
C tal que:
a. C = A U B (sin duplicados)
b. C = A + B (con duplicados)
c. C = A intersección B
d. C = A – B (todos los elementos de A que no están en B)
e. C = (A U B ) – (A intersección B) (todos los elementos de A y de B excepto
los que están en ambas listas)

******************************************************************************************/

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

void AgregarFinal(TNodo**lista, int dato){//Función para agregar elementos al final de una lista enlazada
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

TNodo * ordenarListasOrdenadas_a(TNodo *listaA, TNodo *listaB){//AUB
    TNodo * listaC = NULL; //Lista ordenada

    TNodo * tempA = listaA;
    TNodo * tempB = listaB;
    int ultimoElementoC; //almacena el ultimo elemento insertado en la lista ordenada.
    int elementoInsertar;
    int bandera=1;// bandera en 1 indica que aun no se ha insertado el primer elemento en C

    while(tempA!= NULL && (tempB!= NULL)){
        if(tempA->info == tempB->info){ //si ambos elementos son iguales, avanzamos en ambas listas 
            elementoInsertar = tempA->info; 
            tempA=tempA->sig;
            tempB=tempB->sig;
        }
        else if(tempA->info < tempB->info){
            elementoInsertar = tempA->info;
            tempA = tempA->sig;  //solamente avanzamos en el elemento que posiblemente sera insertado
        }
        else{
            elementoInsertar = tempB->info;
            tempB = tempB->sig;
        }

        if (bandera==1 || elementoInsertar!=ultimoElementoC){
            AgregarFinal(&listaC, elementoInsertar);
            ultimoElementoC = elementoInsertar;
            bandera=0;//despues de insertar el primer elemento
        }

    }
    //Vaciando en listaC la lista que no quedo vacia
    while(tempA!=NULL){ 
        //utilizamos bandera en caso de que alguna lista de entrada estuviera vacia
        if(bandera==1 || tempA->info!=ultimoElementoC){ 
            AgregarFinal(&listaC, tempA->info);
            ultimoElementoC = tempA->info;
            bandera=0;
        }
        tempA = tempA->sig;
    }    
    while(tempB!=NULL){
        if (bandera==1 || tempB->info!=ultimoElementoC){
            AgregarFinal(&listaC, tempB->info); 
            ultimoElementoC = tempB->info;
            bandera=0;
        }
        tempB = tempB->sig;
        
    }

    
    return listaC;
}

TNodo * ordenarListasOrdenadas_b(TNodo*listaA, TNodo *listaB){//A+B (duplicados)
    TNodo * listaC = NULL; //Lista ordenada

    TNodo * tempA = listaA;
    TNodo * tempB = listaB;

    while(tempA!= NULL && (tempB!= NULL)){//Mientras ninguna sea nula, comparamos.
        
        if(tempA->info <= tempB->info){
            AgregarFinal(&listaC, tempA->info);
            tempA = tempA->sig;
        }
        else{
            AgregarFinal(&listaC, tempB->info);
            tempB = tempB->sig;
        }

    }
    //Vaciando en listaC la lista que no quedo vacia
    while(tempA!=NULL){ 
        AgregarFinal(&listaC, tempA->info);
        tempA = tempA->sig;
    }    
    while(tempB!=NULL){
        AgregarFinal(&listaC, tempB->info); 
        tempB = tempB->sig;
    }
    return listaC;
}


TNodo * ordenarListasOrdenadas_c(TNodo*listaA, TNodo *listaB){//AInterseccionB
    TNodo * listaC = NULL; //Lista ordenada

    TNodo * tempA = listaA;
    TNodo * tempB = listaB;

    int ultimoElementoC; //almacena el ultimo elemento insertado en la lista ordenada.
    int bandera=1;// bandera en 1 indica que aun no se ha insertado el primer elemento en C

    while(tempA!= NULL && (tempB!= NULL)){//Mientras ninguna sea nula, comparamos.
        //solo se insertaran en C cuand solos haya elementos iguales
        if(tempA->info == tempB->info){
            if(bandera==1 || tempA->info!=ultimoElementoC){
                AgregarFinal(&listaC, tempA->info);
                ultimoElementoC = tempA->info;
                bandera=0;
            }
            tempA = tempA->sig;
            tempB = tempB->sig;
        }
        else if(tempA->info < tempB->info){
            //cuando son distintos, solo avanzas
            tempA = tempA->sig;
        }
        else{
            tempB = tempB->sig;
        }

    }
    return listaC;
}

TNodo * ordenarListasOrdenadas_d(TNodo*listaA, TNodo *listaB){//A-B
    TNodo * listaC = NULL; //Lista ordenada

    TNodo * tempA = listaA;
    TNodo * tempB = listaB;

    int ultimoElementoC; //almacena el ultimo elemento insertado en la lista ordenada.
    int elemento_repetido;
    int bandera=1;// bandera en 1 indica que aun no se ha insertado el primer elemento en C
                    //en este caso se utiliza bandera solamente para los sobrantes de A.
    while(tempA!= NULL && (tempB!= NULL)){//Mientras ninguna sea nula, comparamos.
        
        if(tempA->info == tempB->info){
            elemento_repetido = tempA->info;
            //avanzamos en ambas listas hasta un valor distinto de elemento_repetido
            while(tempA!=NULL && tempA->info==elemento_repetido)tempA=tempA->sig;
            while(tempB!=NULL && tempB->info==elemento_repetido)tempB=tempB->sig;
        }
        else if(tempA->info < tempB->info){
            //cuando elemento de A es menor a elemento de B, insertamos elemento de A en lista C
            if (bandera || tempA->info != ultimoElementoC) {
                AgregarFinal(&listaC, tempA->info);
                ultimoElementoC = tempA->info;
                bandera = 0;
            }
            tempA = tempA->sig;
        }
        else{
            tempB = tempB->sig;
        }

    }
    //Para elementos sobrantes en A
    while (tempA != NULL) {
        if (bandera==1 || tempA->info != ultimoElementoC) {
            AgregarFinal(&listaC, tempA->info);
            ultimoElementoC = tempA->info;
            bandera = 0;
        }
        tempA = tempA->sig;}

    
    return listaC;
}

TNodo * ordenarListasOrdenadas_e(TNodo*listaA, TNodo *listaB){//(AUB)-(AinterseccionB)
    
    TNodo* unionListas = ordenarListasOrdenadas_a(listaA, listaB);
    TNodo* interseccioListas = ordenarListasOrdenadas_c(listaA, listaB);

    TNodo * listaC = ordenarListasOrdenadas_d(unionListas, interseccioListas);

    
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


    //AUB
    TNodo * listaC = ordenarListasOrdenadas_a(listaA, listaB);
    printf("La lista Ordenada sin duplicados(AUB): \n");
    VerTodos(listaC);
    printf("\n");
    //A+B
    TNodo * listaC2 = ordenarListasOrdenadas_b(listaA, listaB);
    printf("La lista Ordenada con duplicados (A+B): \n");
    VerTodos(listaC2);
    printf("\n");
    //AinterseccionB
    TNodo * listaC3 = ordenarListasOrdenadas_c(listaA, listaB);
    printf("La lista Ordenada (AinterseccionB): \n");
    VerTodos(listaC3);
    printf("\n");
    //A-B
    TNodo * listaC4 = ordenarListasOrdenadas_d(listaA, listaB);
    printf("La lista Ordenada (A-B): \n");
    VerTodos(listaC4);
    printf("\n");
    //(AUB)-(AinterseccionB)
    TNodo * listaC5 = ordenarListasOrdenadas_e(listaA, listaB);
    printf("La lista Ordenada (AUB)-(AinterseccionB): \n");
    VerTodos(listaC5);
    printf("\n");

    return 0;
}
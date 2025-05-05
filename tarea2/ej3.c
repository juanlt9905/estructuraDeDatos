#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct nodo{
    int info;
    struct nodo *sig;
}TNodo;

void liberarLista(TNodo *lista) {
    TNodo *temp;
    while (lista != NULL) {
        temp = lista;
        lista = lista->sig;
        free(temp);
    }
}

void AgregarInicio(TNodo**lista, int dato){
    TNodo *nuevo=NULL;
    //Agregar memoria para el nuevo nodo
    nuevo=(TNodo*) malloc(sizeof(TNodo));
    nuevo->info=dato; //Asigna el valor a nodo
    nuevo->sig= *lista; //Coloca nodo al inicio de lista
    *lista = nuevo; //El inicio de lista cambia al nuevo

}

void verTodos(TNodo*lista){ //Muestra todos los nodos de la lista

    while(lista!=NULL){
        printf("%d ", lista->info);

        lista=lista->sig;

    }
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

TNodo * ordenarListasOrdenadas_a(TNodo**listaA, TNodo **listaB){//AUB
    TNodo * listaC = NULL; //Lista ordenada

    TNodo * tempA = *listaA;
    TNodo * tempB = *listaB;
    int ultimoElementoC; //almacena el ultimo elemento insertado en la lista ordenada.
    int elementoInsertar;
    int bandera=1;// bandera en 1 indica que aun no se ha insertado el primer elemento en C

    while(tempA!= NULL && (tempB!= NULL)){
        if(tempA->info == tempB->info){ //si ambos elementos son iguales, avanzamos en ambas listas 
            elementoInsertar = tempA->info; //ya que no queremos duplicados
            tempA=tempA->sig;
            tempB=tempB->sig;
        }
        else if(tempA->info < tempB->info){
            elementoInsertar = tempA->info;
            tempA = tempA->sig;
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

TNodo * listaOrdenar(TNodo ** lista){
     
    int aux;
    TNodo * temp = *lista;

    while (temp!=NULL){
        
        //aux = temp->valor;
        TNodo * temp2 = temp->sig;
        while (temp2 != NULL){
            if (temp->info > temp2->info){
                aux = temp->info;
                temp->info = temp2->info;
                temp2->info = aux;
            }
            temp2 = temp2 ->sig;

        }

        temp=temp->sig;
    }
    return *lista;

}



TNodo * crearListaOrdenada(int nElementos){

    TNodo * lista=NULL;
    int i=0;
    while(i<nElementos){
        AgregarInicio(&lista, rand()%100);
        i++;
    }

    return listaOrdenar(&lista);


}

TNodo * ordenarListasOrdenadas_b(TNodo**listaA, TNodo **listaB){//A+B (duplicados)
    TNodo * listaC = NULL; //Lista ordenada

    TNodo * tempA = *listaA;
    TNodo * tempB = *listaB;

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

int main(int argc, char **argv){

    srand(time(NULL));

    //Para este ejercicio, creamos la funcion de lista ordenada para definir las listas
    //TNodo * lista1 = crearListaOrdenada(8);
    //VerTodos(lista1);
    int nListas = atoi(argv[1]); //numero de listas que contendra el verctor
    TNodo **vectorDeListas = (TNodo**) malloc(sizeof(TNodo*)*nListas);

    for (int i =0; i<nListas; i++){
        vectorDeListas[i] = crearListaOrdenada(atoi(argv[i+2]));
        printf("Lista %d\n", i+1);
        verTodos(vectorDeListas[i]);
        printf("\n");
    }
    
    TNodo * listaCombinada = vectorDeListas[0]; //inicializa con la primer lista
    TNodo * aux =NULL; //apuntador auxiliar

    for (int i = 1; i<nListas; i++){
        //ordenarListasOrdenadas_a() me da AUB (sin duplicados)
        aux = ordenarListasOrdenadas_a(&listaCombinada,&vectorDeListas[i]);
        liberarLista(listaCombinada);
        listaCombinada = aux;
    }

    printf("Lista Combinada sin Duplicados\n");
    verTodos(listaCombinada);
    printf("\n");

    // Liberar memoria
    for (int i = 1; i < nListas; i++)
        liberarLista(vectorDeListas[i]);
    free(vectorDeListas);


    return 0;
}
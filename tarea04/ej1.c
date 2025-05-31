#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct elemento {
    int info;
    struct elemento *sigElemento;
} TNodo;
void liberarLista(TNodo *lista) {
    TNodo *temp;
    while (lista != NULL) {
        temp = lista;
        lista = lista->sigElemento;
        free(temp);
    }
}

typedef struct conjunto {
    char nombreConjunto;
    int nElementos;
    struct conjunto *sigConjunto;    
    TNodo *primerElemento;         
} Conjunto;

// Liberar lista de conjuntos
void liberarListaDeConjuntos(Conjunto **miListaConjuntos) {
    Conjunto *conjuntoActual = *miListaConjuntos;
    Conjunto *siguienteConjunto = NULL;

    while (conjuntoActual != NULL) {
        siguienteConjunto = conjuntoActual->sigConjunto; // apuntador al siguiente conjunto

        //Liberar la lista de elementos del conjunto actual
        if (conjuntoActual->primerElemento != NULL) {
            liberarLista(conjuntoActual->primerElemento);
        }
        //Liberar el nodo del conjunto actual
        free(conjuntoActual);
        conjuntoActual = siguienteConjunto; // Avanzamos al siguiente conjunto en la lista
    }
}

void verTodos(TNodo*lista){ //Muestra todos los nodos de la lista

    while(lista!=NULL){
        printf("%d ", lista->info);

        lista=lista->sigElemento;

    }
}

void insercionAscendente(TNodo **lista, int dato){
    TNodo *nuevo=NULL;
    nuevo=(TNodo*)malloc(sizeof(TNodo));
    nuevo->info=dato;
    nuevo->sigElemento=NULL;
    if (*lista==NULL || (*lista)->info>=dato){
        nuevo->sigElemento =*lista;
        *lista=nuevo;
    }
    else{
        TNodo *temp=*lista;
        while(temp->sigElemento!=NULL && dato>temp->sigElemento->info){
            temp=temp->sigElemento;
        }
        nuevo->sigElemento=temp->sigElemento;
        temp->sigElemento=nuevo;
    }
}
TNodo * crearListaElementos(int nElementos){//Crea una lista de n elementos ordenados ascendentemente
    TNodo *miListaElementos =NULL;
    for (int i=0; i<nElementos; i++) insercionAscendente(&miListaElementos, rand()%100);

    return miListaElementos;

}

//funcion que crea un nuevo conjunto con nElementos aleatorios y 
Conjunto * crearConjunto(char nombre){ //crea un conjunto de nuevos elementos.
    Conjunto *nuevo_conjunto =NULL;
    nuevo_conjunto = (Conjunto*)malloc(sizeof(Conjunto));

    nuevo_conjunto->nombreConjunto = nombre;
    nuevo_conjunto->nElementos=rand()%8+3;
    nuevo_conjunto->primerElemento=crearListaElementos(nuevo_conjunto->nElementos);
    nuevo_conjunto->sigConjunto=NULL;

    return nuevo_conjunto;
}
//Funcion que agrega un nuevo conjunto a una lista de conjuntos
void AgregarConjunto(Conjunto **miListaConjuntos, char nombreConjunto){
    
    Conjunto *nuevo_conjunto = crearConjunto(nombreConjunto);

    Conjunto *temp=*miListaConjuntos;
    Conjunto *anterior=NULL; //almacena el elemento ante

    //Encontrar la posicion a insertar el nuevo conjunto (descendente respecto a nElementos)
    while(temp!=NULL && temp->nElementos > nuevo_conjunto->nElementos){
        anterior=temp;
        temp=temp->sigConjunto; 
    }

    //Insertar nuevo conjunto
    if (anterior ==NULL){
        nuevo_conjunto->sigConjunto = *miListaConjuntos;
        *miListaConjuntos =nuevo_conjunto;
    }
    else{
        anterior->sigConjunto = nuevo_conjunto;
        nuevo_conjunto->sigConjunto = temp;
    }


}
void verConjuntos(Conjunto *miListaConjuntos){
    Conjunto *temp = miListaConjuntos;
    while (temp != NULL) {
        printf("Conjunto %c (%d elementos): ", temp->nombreConjunto, temp->nElementos);
        verTodos(temp->primerElemento); 
        printf("\n");
        temp = temp->sigConjunto;
    }

}
//funciones para insertar elementos
void insertarConjuntoExistente(Conjunto **listaConjuntos, Conjunto *conjuntoAInsertar){

    Conjunto *actual=*listaConjuntos;
    Conjunto *anterior= NULL;

    while (actual!= NULL && actual->nElementos > conjuntoAInsertar->nElementos) {
        anterior = actual;
        actual = actual->sigConjunto;
    }

    if (anterior == NULL) { //insertar al inico de la lista
        conjuntoAInsertar->sigConjunto = *listaConjuntos;
        *listaConjuntos = conjuntoAInsertar;
    } else { //Intermedio o al final
        anterior->sigConjunto= conjuntoAInsertar;
        conjuntoAInsertar->sigConjunto= actual;
    }

    
}
void insertarElemento(Conjunto **listaConjuntos, char nombre, int elemento_nuevo){

    Conjunto *conjuntoObjetivo=NULL;
    Conjunto *anteriorDelObjetivo =NULL;
    Conjunto *temp= *listaConjuntos;

    //buscar el conjunto objetivo por su nombre
    while (temp != NULL) {
        if (temp->nombreConjunto == nombre) {
            conjuntoObjetivo = temp;
            break; 
        }
        anteriorDelObjetivo = temp;
        temp =temp->sigConjunto;
    }

    if (conjuntoObjetivo == NULL) { //no se encontro el conjunto
        printf("Error: El conjunto '%c' no fue encontrado\n", nombre);
        return;
    }
    //Desconectamos el conjunto objetivo de la lista
    if (anteriorDelObjetivo == NULL) { //el conjunto objetivo estaba en el inicio de la listaConjunto
        *listaConjuntos = conjuntoObjetivo->sigConjunto;
    } else { // el conjunto objetivo estaba en medio o al final
        anteriorDelObjetivo->sigConjunto = conjuntoObjetivo->sigConjunto;
    }
    conjuntoObjetivo->sigConjunto=NULL;

    //insercion del nuevo elemento del conjunto
    insercionAscendente(&(conjuntoObjetivo->primerElemento), elemento_nuevo);
    conjuntoObjetivo->nElementos++;//aumentamos el numero de elementos

    //reinsertar el conjunto Objetivo
    insertarConjuntoExistente(listaConjuntos, conjuntoObjetivo);

    //printf("Elemento %d insertado en conjunto '%c'.\n", elemento_nuevo, nombre);
    //printf("El conjunto '%c' ahora tiene %d elementos \n",
    //       conjuntoObjetivo->nombreConjunto, conjuntoObjetivo->nElementos);

}
//FUnciones para eliminar elementos
int eliminarElementoLista(TNodo **listaElementos, int elemento_eliminar){//funcion para eliminar un elemento de una lista
    
    TNodo *temp = *listaElementos;
    TNodo *anterior= NULL;

    //buscar elemento en la lista
    while (temp !=NULL && temp->info < elemento_eliminar) {
        anterior = temp;
        temp = temp->sigElemento;
    }

        
    if (temp!= NULL && temp->info ==elemento_eliminar){//verificar si el elemento es el bucado
        if (anterior == NULL) { 
            *listaElementos =temp->sigElemento;
        } else { // El elemento a eliminar está en medio o al final de la sublista
            anterior->sigElemento= temp->sigElemento;
        }
        free(temp);
        return 1; //el elemento se elimino
    }

    return 0; // no se encontro el elemento a eliminar
}
void eliminarElementoDeConjunto(Conjunto **listaConjuntos, char nombre, int elemento_eliminar){
    Conjunto *conjuntoObjetivo = NULL;
    Conjunto *anteriorDelObjetivo = NULL;
    Conjunto *temp= *listaConjuntos;

    //buscar conjunto por nombre
    while (temp!= NULL) {
        if (temp->nombreConjunto == nombre) {
            conjuntoObjetivo= temp;
            break; 
        }
        anteriorDelObjetivo =temp;
        temp =temp->sigConjunto;
    }

    //no se encontro el conjunto buscado
    if (conjuntoObjetivo == NULL) {
        printf("Error: El conjunto '%c' no fue encontrado.\n", nombre);
        return;
    }

    if (anteriorDelObjetivo== NULL) { // el conjunto buscado es el primero en la lista
        *listaConjuntos= conjuntoObjetivo->sigConjunto;
    } else { //DESCONECTAR CONJUNTO
        anteriorDelObjetivo->sigConjunto = conjuntoObjetivo->sigConjunto;
    }
    conjuntoObjetivo->sigConjunto=NULL;

    int bandera_eliminacion = eliminarElementoLista(&(conjuntoObjetivo->primerElemento), elemento_eliminar);
    
    if (bandera_eliminacion==1){
        conjuntoObjetivo->nElementos--;
        printf("Elemento %d eliminado del conjunto '%c'.\n", elemento_eliminar, nombre);
    }
    else {
        printf("Elemento %d no encontrado dentro del conjunto '%c'.\n", elemento_eliminar, nombre);
    }

    insertarConjuntoExistente(listaConjuntos, conjuntoObjetivo);

}   
//Funcion para eliminar un conjunto
void eliminarConjunto(Conjunto **listaConjuntos, char nombre_conjunto_eliminar){
    Conjunto * conjuntoAEliminar=NULL;
    Conjunto *anteriorConjunto=NULL;
    Conjunto *temp= *listaConjuntos;

    while(temp!= NULL) {
        if (temp->nombreConjunto == nombre_conjunto_eliminar) {
            conjuntoAEliminar = temp;
            break; // Conjunto encontrado
        }
        anteriorConjunto = temp;
        temp= temp->sigConjunto;
    }
    //si el conjunto no se encontro en la lista
    if (conjuntoAEliminar == NULL) {
        printf("Error: El conjunto '%c' no fue encontrado en la lista.\n", nombre_conjunto_eliminar);
        return;
    }

    if (anteriorConjunto == NULL) { //es el primer conjunto de la lista
        *listaConjuntos = conjuntoAEliminar->sigConjunto;
    } else {
        anteriorConjunto->sigConjunto = conjuntoAEliminar->sigConjunto;
    }
    conjuntoAEliminar->sigConjunto=NULL;
    
    //Liberar memoria
    liberarListaDeConjuntos(&conjuntoAEliminar);


}
int main(){

    srand(time(NULL)); 
    
    Conjunto *miListaConjuntos = NULL;

    AgregarConjunto(&miListaConjuntos, 'A');
    AgregarConjunto(&miListaConjuntos, 'B');
    AgregarConjunto(&miListaConjuntos, 'C');

    verConjuntos(miListaConjuntos);

    printf("INsercion de %d en lista 'A': \n", 90);

    insertarElemento(&miListaConjuntos, 'A', 90);

    verConjuntos(miListaConjuntos);


    eliminarElementoDeConjunto(&miListaConjuntos, 'A',80);
    printf("Eliminacion de %d en lista 'A': \n", 80);
    verConjuntos(miListaConjuntos);

    printf("Eliminacion de conjunto C:\n");
    eliminarConjunto(&miListaConjuntos, 'D');

    verConjuntos(miListaConjuntos);
    liberarListaDeConjuntos(&miListaConjuntos);


    
    
    return 0;
}
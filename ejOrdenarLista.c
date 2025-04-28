#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void VerTodos(TNodo*lista){ //Muestra todos los nodos de la lista

    while(lista!=NULL){
        printf("%d ", lista->info);

        lista=lista->sig;

    }
}

void eliminarPrimerNodo(TNodo **lista){
    if (*lista!= NULL){
        TNodo * x = *lista;
        *lista = (*lista)->sig; // hace a lista apuntar al segundo nodo
        free(x);
    }
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

int main(){

    TNodo *miLista=NULL; //inicializando mi lista
    srand(time(NULL));
    int i=0;
    while (i<10){
        AgregarInicio(&miLista, rand()%100);
        i++;
    }

    TNodo *listaOrdenada = listaOrdenar(&miLista);
    VerTodos(listaOrdenada);



    return 0; 
}
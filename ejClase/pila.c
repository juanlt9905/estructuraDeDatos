#include <stdio.h>
#include <stdlib.h>

typedef struct nodo{
    char info;
    struct nodo *sig;
}TNodo;

void push(TNodo**lista, char dato){ //Agregar elemento al inicio de la lista
    TNodo *nuevo=NULL;
    //Agregar memoria para el nuevo nodo
    nuevo=(TNodo*) malloc(sizeof(TNodo));
    nuevo->info=dato; //Asigna el valor a nodo
    nuevo->sig= *lista; //Coloca nodo al inicio de lista
    *lista = nuevo; //El inicio de lista cambia al nuevo

}
char verTope(TNodo *pila){
    return pila->info;
}
void VerTodos(TNodo*lista){ //Muestra todos los nodos de la lista

    while(lista!=NULL){
        printf("%c", lista->info);

        lista=lista->sig;

    }
}

void pop(TNodo **lista){//eliminar tope de la pila
    if (*lista!= NULL){
        TNodo * x = *lista;
        *lista = (*lista)->sig; // hace a lista apuntar al segundo nodo
        free(x);
    }
}

int pilaVacia(TNodo *pila){
    if (pila == NULL)
        return 1;
    else
        return 0;
}

int main(int argc, char **argv){

    char * a =argv[1];
    TNodo *pila=NULL; //inicializar pila

    for(int i=0; a[i]!='\0'; i++){
        if (a[i]!='0' && a[i]!='1' && a[i]!=':'){
            printf("La cadena NO PERTENECE al lenguaje X.\n");
            return 0;
        }
    }
    //metemos la cadena L a la pila
    int j=0;
    for(int i=0; a[i]!='\0' && a[i]!=':'; i++){
        push(&pila, a[i]);
        j++; //j almacena el indice donde encontremos a :
    }

    j++; //mover el indice de a despues del :
    int espejo = 1;
    //Comparamos R con los elementos de la pila
    while(a[j]!= '\0'){
        //si antes de terminar de recorrer a, la pila queda vacia
        //L!=R
        if(pilaVacia(pila) || a[j] != verTope(pila)){
            espejo = 0;
            break;
        }
        pop(&pila);
        j++;
    }
    //Verificamos que la pila quedara vacia despues de recorrer a.
    if(pilaVacia(pila)==0)espejo=0;

    if (espejo ==1)
        printf("La cadena SI PERTENECE al lenguaje X.\n");
    else 
        printf("La cadena NO PERTENECE al lenguaje X.\n");

    while (pilaVacia(pila)!=1) {
        pop(&pila);
    }
    
    
    //printf("%c", verTope(pila));

    return 0;
}
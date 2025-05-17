#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Identificadores de operadores
#define ninguno 0
#define llenar_jarra4 1
#define llenar_jarra3 2
#define vaciar_jarrra4 3
#define vaciar_jarra3 4
#define verter_3_en_4 5
#define verter_4_en_3 6

typedef struct estados{
    int jarra4;
    int jarra3;
    int op_aplicado_padre;
    struct estados * padre;
}EstadoJarras;

typedef struct nodo{
    EstadoJarras estado;
    struct nodo *sig;
}TNodo; //Nodo definido para el manejo de colas

int max(int a, int b){
    if (a>=b) return a;
    else return b;
}
int min(int a, int b){
    if (a<=b) return a;
    else return b;
}



void inicializarJarras(EstadoJarras * estado){
    estado->jarra4=0;
    estado->jarra3=0;
    estado->padre=NULL;
    estado->op_aplicado_padre =0;
}

int estadoFinal(EstadoJarras * estado){
    if (estado->jarra4 ==2)return 1;
    else return 0;
}

//Definir los operadores

void llenarJarra4(EstadoJarras *estado){ //1.Llena la jarra de 4lts
    if(estado->jarra4 < 4){
        estado->jarra4=4;
        estado->op_aplicado_padre=llenar_jarra4;
    }
}

void llenarJarra3(EstadoJarras * estado){ //2.Llena la jarra de 3lts
    if(estado->jarra3 < 3){
        estado->jarra3=3;
        estado->op_aplicado_padre=llenar_jarra3;
    }
}

void vaciarJarra4(EstadoJarras * estado){ //3.Vaciar la jarra de 4lts
    if(estado->jarra4 > 0){
        estado->jarra4=0;
        estado->op_aplicado_padre=vaciar_jarrra4;
    }
}


void vaciarJarra3(EstadoJarras * estado){ //4.Vaciar la jarra de 3lts
    if(estado->jarra3 > 0){
        estado->jarra3=0;
        estado->op_aplicado_padre=vaciar_jarra3;
    }
}

void verter3en4(EstadoJarras *estado){ //5.Verter el contenido de la jarra de 3lts en la jarra de 4lts
    //si la jarra de 3 no esta vacia y la jarra de 4 no esta llena
    if(estado->jarra3>0 && estado->jarra4<4){
        int faltante_jarra4 = 4-estado->jarra4;
        int actualizacion_jarra3 = max(estado->jarra3-faltante_jarra4,0);
        int actualizacion_jarra4 = min(estado->jarra4+estado->jarra3 - actualizacion_jarra3, 4);
        estado->jarra3 = actualizacion_jarra3;
        estado->jarra4 = actualizacion_jarra4;
        estado->op_aplicado_padre= verter_3_en_4;

    }
}

void verter4en3(EstadoJarras * estado){ //6.Verter el contenido de la jarra de 4lts en la jarra de 3lts
    //si la jarra de 4 no esta vacia y la jarra de 3 no esta llena
    if(estado->jarra4>0 && estado->jarra3<3){
        int faltante_jarra3 = 3-estado->jarra3;
        int actualizacion_jarra4 = max(estado->jarra4-faltante_jarra3,0);
        int actualizacion_jarra3 = min(estado->jarra3+estado->jarra4 - actualizacion_jarra4, 3);
        estado->jarra3 = actualizacion_jarra3;
        estado->jarra4 = actualizacion_jarra4;
        estado->op_aplicado_padre= verter_4_en_3;
    }
}
void AgregarFinal(TNodo**lista, EstadoJarras estado){//Función para agregar elementos al final de una lista enlazada
    TNodo *nuevo=NULL;
    nuevo=(TNodo*) malloc(sizeof(TNodo)); //memoria para el nuevo nodo
    
    nuevo->estado=estado;
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

TNodo *  sucesores(EstadoJarras *estado_actual){
    TNodo* lista_sucesores =NULL;
    EstadoJarras nuevo_estado;
    EstadoJarras vec[6]; //almacenamiento temporal
    int nelementos=0;//numero de elementos en vec

    // Llenar jarra 4 si es posiblee
    if (estado_actual->jarra4 < 4) { // Si la jarra no esta llena
        nuevo_estado = *estado_actual; // Copiamos el estado actual
        llenarJarra4(&nuevo_estado);
        nuevo_estado.padre= estado_actual;
        vec[nelementos]=nuevo_estado;
        nelementos++;
        //AgregarFinal(&lista_sucesores, nuevo_estado);
    }
    //Llenar jarra 3 si es posible
    if(estado_actual->jarra3<3){
        nuevo_estado =*estado_actual;
        llenarJarra3(&nuevo_estado);
        nuevo_estado.padre=estado_actual;
        vec[nelementos]=nuevo_estado;
        nelementos++;
        //AgregarFinal(&lista_sucesores, nuevo_estado);
    }
    //vaciar jarra de 3
    if(estado_actual->jarra3!=0){
        nuevo_estado=*estado_actual;
        vaciarJarra3(&nuevo_estado);
        nuevo_estado.padre=estado_actual;
        vec[nelementos]=nuevo_estado;
        nelementos++;
        //AgregarFinal(&lista_sucesores, nuevo_estado);
    }
    //vaciar jarra de 4
    if(estado_actual->jarra4!=0){
        nuevo_estado=*estado_actual;
        vaciarJarra4(&nuevo_estado);
        nuevo_estado.padre=estado_actual;
        vec[nelementos]=nuevo_estado;
        nelementos++;
        //AgregarFinal(&lista_sucesores, nuevo_estado);
    }
    //verter 3 en 4
    if(estado_actual->jarra3>0 && estado_actual->jarra4<4){
        nuevo_estado=*estado_actual;
        verter3en4(&nuevo_estado);
        nuevo_estado.padre=estado_actual;
        vec[nelementos]=nuevo_estado;
        nelementos++;
        //AgregarFinal(&lista_sucesores, nuevo_estado);
    }
    if(estado_actual->jarra4>0 && estado_actual->jarra3<3){
        nuevo_estado=*estado_actual;
        verter4en3(&nuevo_estado);
        nuevo_estado.padre=estado_actual;
        vec[nelementos]=nuevo_estado;
        nelementos++;
        //AgregarFinal(&lista_sucesores, nuevo_estado);
    }

    //insertar los estados en orden aletorio
    while(nelementos > 0){
        // Seleccionar un indice aleatorio
        int indice_aleatorio=rand()%nelementos;
            
        // agregar el estado del indice  a la lista
        AgregarFinal(&lista_sucesores, vec[indice_aleatorio]);
            
        vec[indice_aleatorio] = vec[nelementos- 1]; //llenar el hueco del elemento insertado con el ultimo elemento de la lista
        nelementos--;
    }


    return lista_sucesores;
}
void VerTodos(TNodo*lista){ //Muestra todos los nodos de la lista

    while(lista!=NULL){
        printf(" (%d, %d), Operacion a padre:%d\n", lista->estado.jarra4, lista->estado.jarra3, lista->estado.op_aplicado_padre);

        lista=lista->sig;

    }
}
int main(){

    srand(time(NULL)); 
    TNodo *nodo = (TNodo*)malloc(sizeof(TNodo));
    TNodo *nodo2 =(TNodo*)malloc(sizeof(TNodo));

    inicializarJarras(&nodo->estado);
    nodo2->estado.jarra3=2;
    nodo2->estado.jarra4=2;

    TNodo * lista_sucesores2 = sucesores(&(nodo2->estado));

    //printf("Jarra 3: %d, Jarra 4: %d\n", nodo->estado.jarra3, nodo->estado.jarra4);
    
    TNodo * lista_sucesores =sucesores(&(nodo->estado));

    printf("Primer Nodo: \n");
    VerTodos(lista_sucesores);

    printf("Segundo Nodo: \n");
    VerTodos(lista_sucesores2);


    free(nodo);



    return 0;
}
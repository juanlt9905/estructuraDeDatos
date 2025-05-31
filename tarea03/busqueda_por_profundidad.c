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

int max(int a, int b){
    if (a>=b) return a;
    else return b;
}
int min(int a, int b){
    if (a<=b) return a;
    else return b;
}
typedef struct estados{
    int jarra4;
    int jarra3;
    int op_aplicado_padre;
    //struct estados * padre;
}EstadoJarras;

typedef struct nodo{
    EstadoJarras estado;
    struct nodo *sig;
    struct nodo *padre;
}TNodo; //Nodo definido para el manejo de colas
void AgregarFinal_conPadre(TNodo**lista, EstadoJarras estado, TNodo *padre){//Función para agregar elementos al final de una lista enlazada
    TNodo *nuevo=NULL;
    nuevo=(TNodo*) malloc(sizeof(TNodo)); //memoria para el nuevo nodo

    nuevo->estado=estado;
    nuevo->sig=NULL;
    nuevo->padre=padre;

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
int estadoFinal(EstadoJarras * estado){
    if (estado->jarra4 ==2)return 1;
    else return 0;
}
void AgregarInicio_conPadre(TNodo**lista, EstadoJarras estado, TNodo *padre){
    //puntero doble, ya que modificamos la cabecera de la lista
    TNodo *nuevo=NULL;

    //Agregar memoria para el nuevo nodo
    nuevo=(TNodo*) malloc(sizeof(TNodo));
    nuevo->padre=padre;
    nuevo->estado=estado; //Asigna el valor a nodo
    nuevo->sig= *lista; //Coloca nodo al inicio de lista
    *lista = nuevo; //El inicio de lista cambia al nuevo

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
void AgregarInicio(TNodo**lista, EstadoJarras estado){
    //puntero doble, ya que modificamos la cabecera de la lista
    TNodo *nuevo=NULL;

    //Agregar memoria para el nuevo nodo
    nuevo=(TNodo*) malloc(sizeof(TNodo));
    nuevo->estado=estado; //Asigna el valor a nodo
    nuevo->sig= *lista; //Coloca nodo al inicio de lista
    *lista = nuevo; //El inicio de lista cambia al nuevo

}


void inicializarJarras(TNodo * nodo){
    nodo->estado.jarra4=0;
    nodo->estado.jarra3=0;
    nodo->padre=NULL;
    nodo->estado.op_aplicado_padre =0;
}
void VerTodos(TNodo*lista){ //Muestra todos los nodos de la lista

    printf("Mostrando lista de nodos:\n"); 
    while (lista != NULL) {
        if (lista->padre != NULL) {
           //el nodo tiene un padre
            int jarra4_padre = lista->padre->estado.jarra4;
            int jarra3_padre = lista->padre->estado.jarra3;
            printf("Estado: (%d, %d), Operacion desde padre: %d, Estado Padre: (%d, %d)\n",
                   lista->estado.jarra4,
                   lista->estado.jarra3,
                   lista->estado.op_aplicado_padre,
                   jarra4_padre,
                   jarra3_padre);
        } else {
            //el nodo no tiene padre 
            printf("Estado: (%d, %d), Operacion desde padre: %d, Estado Padre: (Es nodo raiz)\n",
                   lista->estado.jarra4,
                   lista->estado.jarra3,
                   lista->estado.op_aplicado_padre);
        }
        lista = lista->sig;
    }
    printf("Fin de la lista de nodos.\n");
}
void eliminarPrimerNodo(TNodo **lista){
    if (*lista!= NULL){
        TNodo * x = *lista;
        *lista = (*lista)->sig; // hace a lista apuntar al segundo nodo
        free(x);
    }
}
int existeEstado(TNodo *lista, EstadoJarras estado_buscado){// funcion que verifica la existencia de un estado en una lista
    TNodo *temp=lista;

    while(temp != NULL){
        if(temp->estado.jarra4 ==estado_buscado.jarra4 && temp->estado.jarra3 == estado_buscado.jarra3){
            return 1; // se encontro estado
        }
        temp = temp->sig;
    }
    return 0;//no se encontro estado
}

void liberarLista(TNodo *lista) {
    TNodo *temp;
    while (lista != NULL) {
        temp = lista;
        lista = lista->sig;
        free(temp);
    }
}
//Operadores de transicion de estados


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
TNodo *  sucesores(TNodo ** nodo_actual){ //Funcion para crear los nuevos sucesores de un estado
    TNodo* lista_sucesores =NULL;
    EstadoJarras nuevo_estado;
    EstadoJarras estado_actual = (*nodo_actual)->estado;
    EstadoJarras vec[6]; //almacenamiento temporal
    int nelementos=0;//numero de elementos en vec

    // Llenar jarra 4 si es posiblee
    if (estado_actual.jarra4 < 4) { // Si la jarra no esta llena
        nuevo_estado = estado_actual; // Copiamos el estado actual
        llenarJarra4(&nuevo_estado);
        vec[nelementos]=nuevo_estado;
        nelementos++;
    }
    //Llenar jarra 3 si es posible
    if(estado_actual.jarra3<3){
        nuevo_estado =estado_actual;
        llenarJarra3(&nuevo_estado);
        vec[nelementos]=nuevo_estado;
        nelementos++;
    }
    //vaciar jarra de 3
    if(estado_actual.jarra3!=0){
        nuevo_estado=estado_actual;
        vaciarJarra3(&nuevo_estado);
        vec[nelementos]=nuevo_estado;
        nelementos++;
    }
    //vaciar jarra de 4
    if(estado_actual.jarra4!=0){
        nuevo_estado=estado_actual;
        vaciarJarra4(&nuevo_estado);
        vec[nelementos]=nuevo_estado;
        nelementos++;
    }
    //verter 3 en 4
    if(estado_actual.jarra3>0 && estado_actual.jarra4<4){
        nuevo_estado=estado_actual;
        verter3en4(&nuevo_estado);
        vec[nelementos]=nuevo_estado;
        nelementos++;
    }
    if(estado_actual.jarra4>0 && estado_actual.jarra3<3){
        nuevo_estado=estado_actual;
        verter4en3(&nuevo_estado);
        vec[nelementos]=nuevo_estado;
        nelementos++;
    }

    //insertar los estados en orden aletorio
    while(nelementos > 0){
        // Seleccionar un indice aleatorio
        int indice_aleatorio=rand()%nelementos;
            
        // agregar el estado del indice  a la lista
        TNodo *nodo_padre = *nodo_actual;// definir el padre del nodo a insertar

        AgregarFinal_conPadre(&lista_sucesores, vec[indice_aleatorio], nodo_padre);  
        vec[indice_aleatorio] = vec[nelementos- 1]; //llenar el hueco del elemento insertado con el ultimo elemento de la lista
        nelementos--;
    }


    return lista_sucesores;
}


TNodo * nuevosSucesores(TNodo * nodo_actual, TNodo *abiertos, TNodo *cerrados){
    TNodo *lista_sucesores =sucesores(&nodo_actual);
    TNodo *nuevos_sucesores=NULL;

    //Guardar en nuevos suceros los nodos que no estan en abiertos ni cerrados
    TNodo *temp= lista_sucesores;
    while(temp!=NULL){
        if(existeEstado(abiertos, temp->estado)!=1 && existeEstado(cerrados, temp->estado)!=1){
            //Agregar a nuevos sucesores.
            AgregarFinal(&nuevos_sucesores, temp->estado);
        }
        temp=temp->sig;
    }
    liberarLista(lista_sucesores);

    return nuevos_sucesores;
}

TNodo * reconstruirCamino(TNodo * nodo_meta, TNodo * cerrados) {
    TNodo * camino =NULL;
    TNodo *recorre_por_padres=cerrados;

    //Reconstruir camino desde la meta al estado inicial/
    int i=0;
    while(recorre_por_padres !=NULL){
        //Agregar al inicio para mantener el orden de inicio a meta
        AgregarInicio_conPadre(&camino, recorre_por_padres->estado, recorre_por_padres->padre);
        //moverse al padre
        recorre_por_padres= recorre_por_padres->padre;
    }

    return camino;
}
TNodo *busquedaPorProfundidad(TNodo *nodo_inicial){ //regresa el camino (lista) a la solucion
    TNodo *abiertos=NULL;
    TNodo *cerrados=NULL;
    
    //hacer abiertos la cola formada por el nodo inicial
    AgregarFinal_conPadre(&abiertos, nodo_inicial->estado, nodo_inicial->padre);
    
    //Mientras abiertos no este vacio
    while(abiertos!=NULL){
        //Hacer estado actual el primer nodo de abiertos.
        TNodo *nodo_actual= abiertos;
        abiertos=abiertos->sig; //hacer abiertos el resto de abiertos
        
        AgregarInicio_conPadre(&cerrados, nodo_actual->estado, nodo_actual->padre);//poner nodo actual en cerrados
        
        //verificar si estado actual es meta
        if(estadoFinal(&nodo_actual->estado)==1){
            TNodo *camino = reconstruirCamino(nodo_actual, cerrados);
            return camino;
            liberarLista(abiertos);
            liberarLista(cerrados);
            free(nodo_actual);
        }
        TNodo * nuevos_sucesores = nuevosSucesores(nodo_actual, abiertos, cerrados);
        
        //AGREGAR NUEVOS SUCESORES AL FINAL DE ABIERTOS (BUSQUEDA POR ANCHURA)
        while(nuevos_sucesores!=NULL){
            AgregarInicio_conPadre(&abiertos, nuevos_sucesores->estado, nodo_actual);
            TNodo* temp = nuevos_sucesores;
            nuevos_sucesores = nuevos_sucesores->sig;
            free(temp);//liberar nodo de nuevos sucesores
        }
        
        
    }

    return NULL;
}

int main(){

    srand(time(NULL)); 
      
    TNodo *nodo_inicial=NULL;
    nodo_inicial=(TNodo*) malloc(sizeof(TNodo)); //memoria para el nuevo nodo
    inicializarJarras(nodo_inicial); //inicializar nodo incial
    
    TNodo *camino = busquedaPorProfundidad(nodo_inicial);
    
    VerTodos(camino);//reconstruir camino


    return 0;
}
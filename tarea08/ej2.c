#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define B 7 //tamaño de la tabla cerrada
#define M 9 //tamanio de la tabla abierta

typedef enum{vacio, ocupado, descartado}
state;

typedef struct moldecelda{
    int clave;
    state estado;
}tcelda;

typedef struct {
    tcelda celdas[B];
    int n; // Elementos en la tabla cerrada
} TablaCerrada;

TablaCerrada * hashtab[M]; //mi tabla abierta, ARRAY DE APUNTADORES A TABLAS CERRADAS
//int n; //n elementos ocupados en la tabla


int hashPrincipal(int clave){
    return (7*clave+1)%M;
}

int hash(int clave){
    return  (5*clave +1)%B;
}

void makeNull(){ //inicializar tabla abierta
    for (int i = 0; i < M; i++) {
        hashtab[i] = NULL;
    }
}
void inicializarTablaCerrada(TablaCerrada* tabla) {
    //contador en 0
    tabla->n = 0;

    for (int i = 0; i < B; i++) {//inicializar cada celda
        tabla->celdas[i].estado = vacio;
        tabla->celdas[i].clave = 0;
    }
}


void imprimirTablaCerrada(TablaCerrada *tabla) {
    
    if (tabla == NULL) {//balde vacio
        printf("NULL\n");
        return;
    }

    printf("[ (n=%d) ", tabla->n); //imprimir num de elementos
    for (int i = 0; i < B; i++) {
        char estado;

        if (tabla->celdas[i].estado == ocupado) {
            estado= 'o';
        } else if (tabla->celdas[i].estado == vacio) {
            estado= 'v';
        } else if (tabla->celdas[i].estado == descartado) {
            estado= 'd';
        }

        if (tabla->celdas[i].estado == vacio) {
            printf("(_, %c) ", estado);
        } else {
            printf("(%d, %c) ", tabla->celdas[i].clave, estado);
        }
    }
    printf("]\n");
}

void imprimirTablaAbierta() {
    printf("\n--- TABLA HASH PRINCIPAL ---\n");
    for (int i = 0; i < M; i++) {
        // indice de la cubeta
        printf("Cubeta [%d]: ", i);

        imprimirTablaCerrada(hashtab[i]);
    }
    printf("-----------------------------------------\n");
}
int buscarEnCerrado(TablaCerrada *tabla, int clave) {
    int i, last;
    if (tabla->n>0){
        for(i=hash(clave), last=(i-1+B)%B;  i!=last && tabla->celdas[i].estado != vacio; i=(i+1)%B)   {
            if (tabla->celdas[i].estado == descartado) continue;
            else if (tabla->celdas[i].clave == clave) break; //sólo compara clave si está ocupado
        }

    if (tabla->celdas[i].clave == clave && tabla->celdas[i].estado== ocupado ) return (i);
    else { printf("Error en búsqueda: No encontró %d\n", clave); return (-1);}
    }
    else { printf("Error en búsqueda de clave %d: Tabla vacía\n", clave); return (-2);}
}

int insertarEnCerrado(TablaCerrada *tabla, int clave){

    int i, last, pd=-1; //en pd se almacena posición de primer descartado.
    //Al inicio esa posición no existe.
    if (tabla->n<B) {
        for(i=hash(clave),last=(i-1+B) % B; i!=last && tabla->celdas[i].estado !=
            vacio; i=(i+1)% B){
            if (tabla->celdas[i].estado == descartado) {
                if(pd == -1) pd=i; 
                continue;
            }
            else if (tabla->celdas[i].clave == clave) 
                break; //sólo compara clave si está ocupado
            }
            if ( tabla->celdas[i].clave == clave && tabla->celdas[i].estado == ocupado ){
                printf("Error en inserción: Clave %d ya estaba en la tabla\n",clave); 
                return(1);
            }
            else{
                if (pd != -1) i=pd; //i apunta al primer descartado
                    tabla->celdas[i].clave = clave; tabla->celdas[i].estado = ocupado; tabla->n++;
                return(0);
            }
        }
        else{
            printf("Error en inserción de clave %d. Tabla llena\n", clave);
            return(2);
}
}

int buscarAbierta(int clave) {
    int balde =hashPrincipal(clave);

    //Verificar si hay tabla cerrada creada
    if (hashtab[balde] == NULL) {
        printf("No encontro %d.\n", clave);
        return -1;
    }

    // Para balde existente:
    return buscarEnCerrado(hashtab[balde], clave);
}



void insertarAbierta(int clave) {

    int balde = hashPrincipal(clave);

    if (hashtab[balde] ==NULL){ //Primer elemento del balde
        hashtab[balde] = (TablaCerrada*) malloc(sizeof(TablaCerrada));

        inicializarTablaCerrada(hashtab[balde]); //inicializar tabla cerrada en el balde correspondiente
    }
    //insertar en tabla cerrada encontrada

    insertarEnCerrado(hashtab[balde], clave);
}



int descartarEnCerrado(TablaCerrada *tabla, int clave){

    int i, last;
    if(tabla->n!=0){
        for(i=hash(clave),last=(i-1+B) % B; i!=last && tabla->celdas[i].estado!= vacio; i=(i+1)% B){
            if (tabla->celdas[i].estado == descartado) continue;
            else if (tabla->celdas[i].clave == clave) break; //sólo compara clave si está ocupado
        }
        if (tabla->celdas[i].clave == clave && tabla->celdas[i].estado == ocupado ){ 
            tabla->celdas[i].estado=descartado; tabla->n--; return (0);}
        else{
            printf("Error en descarte: No se encuentra activa la clave=%d\n",clave); return (1);}
    }
    else { printf("Error en descarte de clave %d: Tabla vacía\n", clave); return (2);}
}

int descartarAbierta(int clave){

    int balde= hashPrincipal(clave);

    if (hashtab[balde] == NULL) {
        printf("Error en descarte: No se encuentra la clave=%d, cubeta vaciaa.\n", clave);
        return 2;
    }

    return descartarEnCerrado(hashtab[balde], clave);
    
}


int main(){

    int operacion=-1;
    int clave;
    makeNull();

    while(operacion !=0){


        printf("Elija una operacion:\n");
        printf("1. Insertar clave\n");
        printf("2. Buscar clave\n");
        printf("3. Descartar clave\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &operacion);

        if (operacion ==1){
            printf("Ingrese la clave a insertar: ");
            scanf("%d", &clave);
            insertarAbierta(clave);
            imprimirTablaAbierta();
        }
        else if (operacion==2){
            printf("Ingrese la clave a buscar: ");
            scanf("%d", &clave);
            int pos = buscarAbierta(clave);
            if (pos >= 0) {
                printf("Clave %d encontrada en la posicion %d.\n", clave, pos);
            }
            imprimirTablaAbierta();
        }
        else if (operacion==3){
            printf("Ingrese la clave a descartar: \n");
            scanf("%d", &clave);
            descartarAbierta(clave);
            imprimirTablaAbierta();
        }
    
    } 


    return 0;
}
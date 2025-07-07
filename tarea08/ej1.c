#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define B 9 //tamaño de la tabla

typedef enum{vacio, ocupado, descartado}
state;

typedef struct moldecelda{
    int clave;
    state estado;
}tcelda;

tcelda hashtab[B];
int n; //n elementos ocupados en la tabla

int hash(int clave){
    return  (7*clave +1)%B;
}

void DejarTablaVacia() {
    int i;
    for ( i = 0; i < B; i++) 
        hashtab[i].estado  = vacio;
    n=0;
}

void imprimirItemHashCerrado(int i) {
    if( i>=0 && i<B ) {
        printf( "Indice= %d Clave=%d Estado=", i,hashtab[i].clave);
    if( hashtab[i].estado==vacio) 
        printf("vacio\n");
    else if (hashtab[i].estado==ocupado) 
        printf("ocupado\n");
    else{
        if  (hashtab[i].estado==descartado)
            printf("descartado\n");
        else printf("error en estado\n");
    }
}
else printf("Item inválido\n");
}

void imprimirTabla(void) {
    int i;
    if(n>0) {
        printf("Tabla\n");
        for(i=0; i<B; i++)
            imprimirItemHashCerrado(i);
        putchar('\n');
    }
    else printf("Tabla vacía\n");
}
int buscar(int clave) {
    int i, last;
    if (n>0){
        for(i=hash(clave), last=(i-1+B)%B;  i!=last && hashtab[i].estado != vacio; i=(i+1)%B)   {
            if (hashtab[i].estado == descartado) continue;
            else if (hashtab[i].clave == clave) break; //sólo compara clave si está ocupado
        }

    if (hashtab[i].clave == clave && hashtab[i].estado == ocupado ) return (i);
    else { printf("Error en búsqueda: No encontró %d\n", clave); return (-1);}
    }
    else { printf("Error en búsqueda de clave %d: Tabla vacía\n", clave); return (-2);}
}

int insertar(int clave){

    int i, last, pd=-1; //en pd se almacena posición de primer descartado.
    //Al inicio esa posición no existe.
    if (n<B) {
        for(i=hash(clave),last=(i-1+B) % B; i!=last && hashtab[i].estado !=
            vacio; i=(i+1)% B){
            if (hashtab[i].estado == descartado) {
                if(pd == -1) pd=i; 
                continue;
            }
            else if (hashtab[i].clave == clave) 
                break; //sólo compara clave si está ocupado
            }
            if ( hashtab[i].clave == clave && hashtab[i].estado == ocupado ){
                printf("Error en inserción: Clave %d ya estaba en la tabla\n",clave); 
                return(1);
            }
            else{
                if (pd != -1) i=pd; //i apunta al primer descartado
                    hashtab[i].clave = clave; hashtab[i].estado = ocupado; n++;
                return(0);
            }
        }
        else{
            printf("Error en inserción de clave %d. Tabla llena\n", clave);
            return(2);
}
}

int descartar(int clave){

    int i, last;
    if(n!=0){
        for(i=hash(clave),last=(i-1+B) % B; i!=last && hashtab[i].estado!= vacio; i=(i+1)% B){
            if (hashtab[i].estado == descartado) continue;
            else if (hashtab[i].clave == clave) break; //sólo compara clave si está ocupado
        }
        if (hashtab[i].clave == clave && hashtab[i].estado == ocupado ){ 
            hashtab[i].estado=descartado; n--; return (0);}
        else{
            printf("Error en descarte: No se encuentra activa la clave=%d\n",clave); return (1);}
    }
    else { printf("Error en descarte de clave %d: Tabla vacía\n", clave); return (2);}
}

int main(){

    int operacion=-1;
    int clave;
    DejarTablaVacia();

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
            insertar(clave);
            imprimirTabla();
        }
        else if (operacion==2){
            printf("Ingrese la clave a buscar: ");
            scanf("%d", &clave);
            int pos = buscar(clave);
            if (pos >= 0) {
                printf("Clave %d encontrada en la posicion %d.\n", clave, pos);
            }
            imprimirTabla();
        }
        else if (operacion==3){
            printf("Ingrese la clave a descartar: \n");
            scanf("%d", &clave);
            descartar(clave);
            imprimirTabla();
        }
    
    } 


    return 0;
}
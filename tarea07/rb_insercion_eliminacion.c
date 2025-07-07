#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct nodo {
    int info;
    struct nodo *izq;
    struct nodo *der;
    struct nodo *nodoPadre;
    int color; // 0 rojo, 1 negro
} TNodo;

TNodo *NIL;

void inicializarNIL() {
    NIL = (TNodo*)malloc(sizeof(TNodo));
    NIL->color = 1; 
    NIL->izq = NIL; 
    NIL->der = NIL;
    NIL->nodoPadre = NIL;
}

TNodo * buscarNodo(TNodo *raiz, int dato_buscado){//regresa un apuntador al nodo buscado del arbol
    TNodo *actual=raiz;
    //busqueda del nodo
    while(actual!=NIL && actual->info!=dato_buscado) {
        if(dato_buscado < actual->info)
            actual = actual->izq;
        else
            actual = actual->der;
    }
    return actual;
}

void rotacionDer(TNodo **raiz, int dato_rotar){
    //Buscar el nodo a rotar en el arbol
    TNodo *nodo_rotar= buscarNodo(*raiz, dato_rotar);
    if (nodo_rotar==NIL){ //si no se encuentra el nodo en el arbol
        printf("No se encontro el nodo a rotar. \n");
        return;
    }
    if (nodo_rotar->izq == NIL) { //VAlidacion de hijo izquierdo del nodo a rotar
        printf("No se puede hacer rotacion a la derecha porque hijoIzq es NULL.\n");
        return;
    }
    
    TNodo *nodo_subir= nodo_rotar->izq;//El nodo izquierdo del nodo a rotar sera el nodo que "subira"

    nodo_rotar->izq= nodo_subir->der;
    if(nodo_subir ->der!=NIL)nodo_subir->der->nodoPadre=nodo_rotar;

    nodo_subir->der = nodo_rotar;
    
    nodo_subir->nodoPadre = nodo_rotar->nodoPadre;
    nodo_rotar->nodoPadre = nodo_subir;

    if(nodo_subir->nodoPadre==NIL){
        *raiz= nodo_subir; //el nodo a subir es la raiz del arbol
    }else if (nodo_rotar==nodo_subir->nodoPadre->izq)
        nodo_subir->nodoPadre->izq= nodo_subir;
    else
        nodo_subir->nodoPadre->der= nodo_subir;

}

void rotacionIzq(TNodo **raiz, int dato_rotar){
    TNodo *nodo_rotar= buscarNodo(*raiz, dato_rotar);

    if (nodo_rotar==NIL){ //si no se encuentra el nodo en el arbol
        printf("No se encontro el nodo a rotar. \n");
        return;
    }
    if (nodo_rotar->der == NIL) { //Validacion de hijo derecho del nodo a rotar
        printf("No se puede hacer rotacion a la izquierda porque hijoDer es NULL.\n");
        return;
    }

    TNodo *nodo_subir= nodo_rotar->der;// el nodo derecho del nodo a rotar "subira"

    nodo_rotar->der= nodo_subir->izq;
    if(nodo_subir ->izq!=NIL)nodo_subir->izq->nodoPadre=nodo_rotar;

    nodo_subir->izq=nodo_rotar;

    //Asignacion de padres
    nodo_subir->nodoPadre= nodo_rotar->nodoPadre;
    nodo_rotar->nodoPadre= nodo_subir;

    if(nodo_subir->nodoPadre==NIL)
        *raiz=nodo_subir;
    else if (nodo_rotar==nodo_subir->nodoPadre->izq)
        nodo_subir->nodoPadre->izq= nodo_subir;
    else
        nodo_subir->nodoPadre->der= nodo_subir;
}

void RB_insertFixUp(TNodo **raiz, TNodo *z){

    while(z!=*raiz && z->nodoPadre->color==0){
        if(z->nodoPadre==z->nodoPadre->nodoPadre->izq){ //es papa de z un hijo izq?
            TNodo *y=z->nodoPadre->nodoPadre->der;
            if(y!=NIL && y->color==0){
                z->nodoPadre->color=1;
                y->color=1;
                z->nodoPadre->nodoPadre->color=0;
                z=z->nodoPadre->nodoPadre;
            }
            else {
                if(z==z->nodoPadre->der){
                    z=z->nodoPadre;
                    rotacionIzq(raiz,z->info);}
                z->nodoPadre->color=1;
                z->nodoPadre->nodoPadre->color=0;
                rotacionDer(raiz, z->nodoPadre->nodoPadre->info);
            }
        }else{
            TNodo *y=z->nodoPadre->nodoPadre->izq;
            if(y!=NIL && y->color==0){
                z->nodoPadre->color=1;
                y->color=1;
                z->nodoPadre->nodoPadre->color=0;
                z=z->nodoPadre->nodoPadre;
            }
            else {
                if(z==z->nodoPadre->izq){
                    z=z->nodoPadre;
                    rotacionDer(raiz,z->info);}
                z->nodoPadre->color=1;
                z->nodoPadre->nodoPadre->color=0;
                rotacionIzq(raiz, z->nodoPadre->nodoPadre->info);
            }
        }

    }
    (*raiz)->color=1;
}

int insertarNodoRB(TNodo **raiz, int dato_insertar){
    TNodo *actual= *raiz;
    TNodo *padre = NIL;

    int enc=0;
    while(actual !=NIL && enc==0){
        padre = actual;
        if (dato_insertar< actual->info) {
            actual = actual->izq;
        } else if (dato_insertar > actual->info) {
            actual = actual->der;
        } else {
            enc=1;
        }
    }

    if(enc==1)return 0;
    TNodo *nuevo_nodo=(TNodo*)malloc(sizeof(TNodo));
    nuevo_nodo->info=dato_insertar;
    nuevo_nodo->izq=NIL;
    nuevo_nodo->der=NIL;
    nuevo_nodo->nodoPadre =padre;
    //nuevo_nodo->color=0;

    /* Si padre es NULL, entonces el árbol estaba vacío, el nuevo nodo será
    el nodo raiz*/
    if(padre==NIL) {
        *raiz=nuevo_nodo;
        nuevo_nodo->color=1;//al ser raiz, el nodo se inserta como negro
        nuevo_nodo->nodoPadre=NIL;
    }
        /* Si el dato es menor que el que contiene el nodo padre, lo insertamos
    en la rama izquierda */
    else if(dato_insertar < padre->info){ //Si es menor que el padre lo insertamos a la izq.
        padre->izq= nuevo_nodo;
        nuevo_nodo->color=0;}//si no es raiz, el color se inserta como rojo
    else{// Si es mayor lo insertamos a la derecha
        padre->der = nuevo_nodo;
        nuevo_nodo->color=0;}
    //
    RB_insertFixUp(raiz,nuevo_nodo);
    return 1;
    
}

TNodo *treeMinimum(TNodo *x){
    if(x==NIL) return NIL;
    while(x->izq!=NIL){
        x=x->izq;
    }
    return x;
}
TNodo *TreeSucessor(TNodo *x){
    if(x==NIL) return NIL;
    if(x->der!=NIL) return treeMinimum(x->der);

    TNodo * y=x->nodoPadre;
    while(y!= NIL && x==y->der){
        x=y;
        y=y->nodoPadre;
    }
    return y;
}

void RB_deleteFixUp(TNodo **raiz, TNodo *x){//

    
    while(x!=*raiz && x->color==1){
        if (x==x->nodoPadre->izq){
            
            TNodo *w=x->nodoPadre->der;
            if(w->color==0){
                w->color=1;
                x->nodoPadre->color=0;
                rotacionIzq(raiz, x->nodoPadre->info);
                w=x->nodoPadre->der;            //Caso 1
            }

            if((w->izq->color==1) && (w->der->color==1)){
                w->color=0;
                x=x->nodoPadre;                 //Caso 2
            }else {
                if(w->der->color==1){
                    w->izq->color=1;
                    w->color=0;
                    rotacionDer(raiz, w->info);
                    w=x->nodoPadre->der;}       //caso 3
                w->color=x->nodoPadre->color;
                x->nodoPadre->color=1;
                w->der->color=1;
                rotacionIzq(raiz, x->nodoPadre->info);
                x=*raiz;                        //Caso 4
            }

        }else{

            TNodo *w=x->nodoPadre->izq;
            if(w->color==0){
                w->color=1;
                x->nodoPadre->color=0;
                rotacionDer(raiz, x->nodoPadre->info);
                w=x->nodoPadre->izq;            //Caso 1
            }

            if((w->der->color==1) && (w->izq->color==1)){
                w->color=0;
                x=x->nodoPadre;                 //Caso 2
            }else {
                if(w->izq->color==1){
                    w->der->color=1;
                    w->color=0;
                    rotacionIzq(raiz, w->info);
                    w=x->nodoPadre->izq;}       //caso 3
                w->color=x->nodoPadre->color;
                x->nodoPadre->color=1;
                w->izq->color=1;
                rotacionDer(raiz, x->nodoPadre->info);
                x=*raiz;                        //Caso 4
            }
            
    }
}
x->color=1;
}

int  deleteNodoRB(TNodo **raiz,  int nodo_borrar){

    TNodo *z= buscarNodo(*raiz, nodo_borrar);
    if (z==NIL) return 0;
    TNodo *y=NIL;
    if(z->izq ==NIL || z->der==NIL)y=z;
    else y=TreeSucessor(z);
    
    TNodo *x=NIL;
    if(y->izq !=NIL) x=y->izq;
    else x=y->der;

    if (x!=NIL)x->nodoPadre =y->nodoPadre;
    if(y->nodoPadre==NIL) *raiz=x;
    else if(y==y->nodoPadre->izq){
        y->nodoPadre->izq=x;
    }else y->nodoPadre->der=x;

    if (y!=z){
        z->info=y->info;
    };

    if (y->color==1) RB_deleteFixUp(raiz, x);//fixup;

    free(y);
    return 1;
}


void preOrden(TNodo*raiz){
    if(raiz!=NIL){
        printf("(Info:%d, Color: %d) ", raiz->info, raiz->color);
        preOrden(raiz->izq);
        preOrden(raiz->der);
    }
}


int main(){
     
    inicializarNIL();
    TNodo *raiz=NIL;
   
    //Insertar los nodos
    while(1){

        int nuevo_elemento, agregado;
        printf("Agregar nodo al arbol: \n");
        scanf("%d", &nuevo_elemento);
        if (nuevo_elemento==-1)break; //usuario termina la insercion de nodos
        
        agregado=insertarNodoRB(&raiz, nuevo_elemento);
        if (agregado==1)
            printf("Nodo %d agregado.\n", nuevo_elemento);
        else
            printf("El nodo %d ya existe en el arbol.\n", nuevo_elemento);
    }


    //Operar dinamicamente el arbol.
    int opcion, nodoOperar;
    while (1) {
        //imprimir el árbol despues de cada rotación
        printf("\n***** Arbol (PreOrden)*****\n");
        if (raiz == NIL) {
            printf("El arbol esta vacio.\n");
        } else {
            preOrden(raiz);
        }
        printf("\n***************************************\n");

        // Muestra el menú de opciones al usuario.
        printf("Opciones de operación:\n");
        printf("1. Insertar Nodo\n 2. Eliminar Nodo\n 3. Terminar\n");
        scanf("%d", &opcion);

        if(opcion ==1){
            printf("Introduce el nodo INSERTAR:\n ");
            scanf("%d", &nodoOperar);
            int ag =insertarNodoRB(&raiz, nodoOperar);
            if (ag==1)
                printf("Nodo %d agregado.\n", nodoOperar);
            else
                printf("El nodo %d ya existe en el arbol.\n", nodoOperar);
        }else if (opcion==2){
            printf("Introduce el nodo a BORRAR: \n");
            scanf("%d", &nodoOperar);
            int a= deleteNodoRB(&raiz, nodoOperar);
        }else{
            break;
        }

    }




    
}
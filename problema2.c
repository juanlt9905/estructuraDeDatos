#include <stdio.h>

typedef struct {
    char elementos[100];
    int tope_pila;
}Pila;

Pila insertar(Pila pila, char elemento_nuevo){
    if (pila.tope_pila >99){
        printf("Eror: Desbordamiento de pila.....");
    }
    else{
        pila.elementos[pila.tope_pila] = elemento_nuevo;
        pila.tope_pila++;
    }
    return pila;
}

void imprimir_pila(Pila pila){
    printf("Los elementos de la pila son:");
    for(int i = 0; i<pila.tope_pila;i++){
        printf("%c ", pila.elementos[i]);
    }
}

Pila botar_tope(Pila pila){
    pila.elementos[pila.tope_pila-1] = 'x';
    pila.tope_pila = pila.tope_pila - 1;

    return pila;
}

char elemento_tope(Pila pila){
    
    return pila.elementos[pila.tope_pila-1];
}

int main(){
    //inicializar pila
    Pila pila;
    pila.tope_pila = 0;

    /*char prueba = 'H';

    pila = insertar(pila, prueba);
    pila = insertar(pila, 'o');
    pila = insertar(pila, 'l');

    imprimir_pila(pila);

    pila = botar_tope(pila);

    imprimir_pila(pila);

    printf("El elemento tope es: %c", elemento_tope(pila));*/
    //printf("elemento instertado: %d", pila.elementos[pila.tope_pila-1]); 
     
    char cadena[] = "((()()()()()))";
    int n = sizeof(cadena)/sizeof(cadena[0]) - 1;
    int anidado = 1;

    for (int i = 0; i < n; i++) {
        if (cadena[i] == '(') {
            pila = insertar(pila, '(');
        } else if (cadena[i] == ')') {
            if (pila.tope_pila == 0) {
                anidado = 0; 
                break;
            }
            pila = botar_tope(pila);
        }
    }

    // Verificar si la pila esta vacia
    if (anidado ==1 && pila.tope_pila == 0) {
        printf("La cadena de parentesis esta bien anidada\n");
    } else {

        printf("La cadena de parentesis NO esta bien anidada\n");
    }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
    



    return 0;
}
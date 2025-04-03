//cifrador
#include <stdio.h>
#include <stdlib.h>
char alfabeto[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main(int argc, char **argv){
    int desplazamiento = atoi(argv[1]);
    int posicion_alfabeto;
    int longitud_cadena=0;

    for(int i =0; argv[2][i]!='\0'; i++){
        longitud_cadena++;
    }
    char cadena[longitud_cadena];

    for(int i=0; argv[2][i]!='\0'; i++){
        cadena[i] = argv[2][i];
    }
    

    for(int i=0; cadena[i]!='\0'; i++){ // recorrer la cadena a cifrar
        for (int j=0; alfabeto[j]!='\0'; j++){
            if(cadena[i]==alfabeto[j]){
                posicion_alfabeto=j;
                break;
            }
        }
        cadena[i] = alfabeto[posicion_alfabeto+ desplazamiento];
    }

    for(int i=0; cadena[i]!='\0'; i++){

        printf("%c\n", cadena[i]);
    }


    return 0;
}
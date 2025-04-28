//cifrador
#include <stdio.h>
#include <stdlib.h>
char alfabeto[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main(int argc, char **argv){
    int desplazamiento = atoi(argv[1]); //Lectura del desplazamiento.
    int longitud_cadena=0; //Variable que servira para medir la longitud de la cadena a cifrar.

    //Conteo de la longitud de la cadena a cifrar.
    for(int i =0; argv[2][i]!='\0'; i++){
        longitud_cadena++;
    }
    char cadena[longitud_cadena+1]; //cadena a cifrar.


    for(int i=0; argv[2][i]!='\0'; i++){
        cadena[i] = argv[2][i];
    }

    cadena[longitud_cadena] = '\0'; //caracter nulo al final de la cadena.
    

    for(int i=0; cadena[i]!='\0'; i++){ // recorrer la cadena a cifrar
        if(cadena[i]>='A' && cadena[i]<='Z'){ //solamente se realiza el cifrado con letras mayusculas.
            for (int j=0; alfabeto[j]!='\0'; j++){
                if(cadena[i]==alfabeto[j]){
                    cadena[i] = alfabeto[(j + desplazamiento)%26];
                    break;
                }
            }
        }
    }

    printf("\n La cadena cifrada es:");

    for(int i=0; cadena[i]!='\0'; i++){

        printf("%c", cadena[i]);
    }
    printf("\n");


    return 0;
}
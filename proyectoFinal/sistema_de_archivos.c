#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NOMBRE_ARCHIVO "mi_sistema_de_archivos"
#define NUMERO_DE_BLOQUES 10
#define LONGITUD_NOMBRES 100
#define ENTRADAS_POR_BLOQUE ((BLOCK_SIZE - sizeof(int)) / sizeof(Apuntador))

int bloque_actual =0; 
int bloques_libres[NUMERO_DE_BLOQUES];
FILE * aStream;

typedef struct {
    char nombre[LONGITUD_NOMBRES];
    int esDirectorio; //1 directorio, 0 archivo
    int numero_de_bloque;
    int size;
} Apuntador;

typedef struct{
    int contador_de_apuntadores;
    Apuntador apuntadores_de_objetos[ENTRADAS_POR_BLOQUE];
}Bloque_directorio; //contiene apuntadores.

typedef struct{
    int size;
    char data[BLOCK_SIZE - sizeof(int)];
}Bloque_archivo;




void WriteBlock(int i, char *blockAddr){

    //funcion para mover el puntero al bloque deseado (el i-esimo bloque)
    fseek (aStream, i * BLOCK_SIZE, SEEK_SET);

    //escribimos el contenido del puntero blockAddr en el archivo.
    fwrite(blockAddr, BLOCK_SIZE, 1, aStream);


}

void ReadBlock (int i, char *blockAddr){

    fseek (aStream, i*BLOCK_SIZE, SEEK_SET);

    fread(blockAddr, BLOCK_SIZE, 1, aStream);
}



//Funcion para reservar espacio para almacenar el sistema de archivos.

void createEmptyVirtualDisk(int numberOfRecords){
    
    //Crea un array de caracteres de tamaño BLOCK_SIZE.
    char block[BLOCK_SIZE]; 
    memset(block, '\0', BLOCK_SIZE);

    //Creacion de multiples bloques
    for (int i = 0; i < numberOfRecords; i++) {
        //escribe un bloque de nulos en la posicion actual
        WriteBlock(i, block);
        bloques_libres[i] = 1; // 1 libre, 0 ocupado
    }

    //Inicializar directorio raiz
    Bloque_directorio raiz;
    raiz.contador_de_apuntadores=0;
    WriteBlock(0, (char*)&raiz);
    bloques_libres[0]=0; //marcar raiz como ocupado.

    printf("Disco virtual de %d bloques de memoria.\n", numberOfRecords);

}

void inicializar_systema_de_archivos(){
    //init

    aStream = fopen("mi_sistema_de_archivos.txt", "w+r");
    bloque_actual=0;
    createEmptyVirtualDisk(NUMERO_DE_BLOQUES);


}
//buscar bloque libre
int encontrar_bloque_libre(){
    for(int i=1; i<NUMERO_DE_BLOQUES; i++){
        if(bloques_libres[i] == 1){
            return i;
        }
    }
    return -1;
}

//liberar bloque
void liberar_bloque(int bloque){
    char block[BLOCK_SIZE];
    memset(block, '\0', BLOCK_SIZE);
    WriteBlock(bloque, block);
    bloques_libres[bloque] = 1;
}

void mkdir(char *nombre_directorio){

    //Leemos el bloque actual para almacenar el puntero del nuevo directorio:
    Bloque_directorio directorio_actual;
    ReadBlock(bloque_actual, (char*)&directorio_actual); //almacenamos la info del bloque actual en directorio actual.

    //Verificar si ya existe el directorio
    for(int i = 0; i < directorio_actual.contador_de_apuntadores; i++){
        if(strcmp(directorio_actual.apuntadores_de_objetos[i].nombre, nombre_directorio) == 0){
            printf("Error: '%s' ya existe.\n", nombre_directorio);
            return;
        }
    }

    //buscar bloque libre para el nuevo directorio
    int nuevo_bloque = encontrar_bloque_libre();
    if(nuevo_bloque == -1){
        printf("Error: No hay bloques libres.\n");
        return;
    }

    //copiar el apuntador del nuevo directorio
    strcpy(directorio_actual.apuntadores_de_objetos[directorio_actual.contador_de_apuntadores].nombre, nombre_directorio);
    directorio_actual.apuntadores_de_objetos[directorio_actual.contador_de_apuntadores].esDirectorio=1;
    directorio_actual.apuntadores_de_objetos[directorio_actual.contador_de_apuntadores].numero_de_bloque=nuevo_bloque;
    directorio_actual.apuntadores_de_objetos[directorio_actual.contador_de_apuntadores].size=0;
    directorio_actual.contador_de_apuntadores++;

    WriteBlock(bloque_actual, (char*)&directorio_actual);

    Bloque_directorio bloque;
    bloque.contador_de_apuntadores=0;
    WriteBlock(nuevo_bloque, (char*)&bloque);
    bloques_libres[nuevo_bloque]=0;

    printf("Directorio '%s' creado en bloque %d.\n", nombre_directorio, nuevo_bloque);

}

void ls(){

     //Leemos el bloque actual para almacenar el puntero del nuevo directorio:
    char buffer[BLOCK_SIZE];
    ReadBlock(bloque_actual, buffer);

    Bloque_directorio* directorio_actual = (Bloque_directorio*)buffer;

    
     //Bloque_directorio directorio_actual;
    //ReadBlock(bloque_actual, (char*)&directorio_actual);

    printf("Contenido del directorio: \n");
    for(int i=0; i< directorio_actual->contador_de_apuntadores; i++){
        printf("%s %s\n", directorio_actual->apuntadores_de_objetos[i].nombre, 
            directorio_actual->apuntadores_de_objetos[i].esDirectorio ? "DIR":"FILE");
    }

}

int main(){

    
    //char arreglo[]="Juan";
    aStream = fopen("mi_sistema_de_archivos.txt", "r+");

    char comando[100];
    char arg1[50], arg2[50];

    while(1){
        printf(">");
        fgets(comando, 10, stdin);
        comando[strcspn(comando,"\n")]= 0;
        // Limpiar argumentos
        memset(arg1, 0, sizeof(arg1));
        memset(arg2, 0, sizeof(arg2));

        if (strcmp(comando, "init")==0){
            inicializar_systema_de_archivos();
        }
        else if(strcmp(comando, "write")==0){
            char cadena[BLOCK_SIZE];
            ReadBlock(0, cadena);

            strcat(cadena, "juan");
            WriteBlock(0, cadena);
        }
        else if(strcmp(comando, "read")==0){
            char cadena_lectura[BLOCK_SIZE];
            memset(cadena_lectura, '\0', BLOCK_SIZE);

            ReadBlock(0, cadena_lectura);

            printf("Contenido del bloque 0: '%s'\n", cadena_lectura);

        }
        else if (strcmp(comando, "quit")==0){
            break;
        }
        else if (strncmp(comando, "mkdir ", 6)==0){
            sscanf(comando, "mkdir %s", arg1);
            mkdir(arg1);
        }
        else if(strcmp(comando, "ls")==0){
            ls();
        }

    }

    return 0;
}
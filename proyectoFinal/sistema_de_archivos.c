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
    raiz.contador_de_apuntadores=2;

    //apuntador a el mismo "."
    strcpy(raiz.apuntadores_de_objetos[0].nombre, ".");
    raiz.apuntadores_de_objetos[0].esDirectorio = 1;
    raiz.apuntadores_de_objetos[0].numero_de_bloque = 0;
    raiz.apuntadores_de_objetos[0].size = 0;

    // apuntador a su dir padre ".." (el mismo)
    strcpy(raiz.apuntadores_de_objetos[1].nombre, "..");
    raiz.apuntadores_de_objetos[1].esDirectorio = 1;
    raiz.apuntadores_de_objetos[1].numero_de_bloque = 0;
    raiz.apuntadores_de_objetos[1].size = 0;
    
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
    //Bloque_directorio directorio_actual;
    //ReadBlock(bloque_actual, (char*)&directorio_actual); //almacenamos la info del bloque actual en directorio actual.

    char buffer[BLOCK_SIZE];
    ReadBlock(bloque_actual, buffer);

    Bloque_directorio * directorio_actual = (Bloque_directorio*)buffer;

    //Verificar si ya existe el directorio
    for(int i = 0; i < directorio_actual->contador_de_apuntadores; i++){
        if(strcmp(directorio_actual->apuntadores_de_objetos[i].nombre, nombre_directorio) == 0){
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
    strcpy(directorio_actual->apuntadores_de_objetos[directorio_actual->contador_de_apuntadores].nombre, nombre_directorio);
    directorio_actual->apuntadores_de_objetos[directorio_actual->contador_de_apuntadores].esDirectorio=1;
    directorio_actual->apuntadores_de_objetos[directorio_actual->contador_de_apuntadores].numero_de_bloque=nuevo_bloque;
    directorio_actual->apuntadores_de_objetos[directorio_actual->contador_de_apuntadores].size=0;
    directorio_actual->contador_de_apuntadores++;

    WriteBlock(bloque_actual, buffer);

    Bloque_directorio nuevo_bloque_directorio;
    
    nuevo_bloque_directorio.contador_de_apuntadores=2; //inicia con dos entradas, "." y ".."
    
    //apuntador "." para apuntar al directorio actual
    strcpy(nuevo_bloque_directorio.apuntadores_de_objetos[0].nombre, ".");
    nuevo_bloque_directorio.apuntadores_de_objetos[0].esDirectorio=1;
    nuevo_bloque_directorio.apuntadores_de_objetos[0].numero_de_bloque=nuevo_bloque;
    nuevo_bloque_directorio.apuntadores_de_objetos[0].size = 0;

    //apuntador ".." para apuntar al directorio padre
    strcpy(nuevo_bloque_directorio.apuntadores_de_objetos[1].nombre, "..");
    nuevo_bloque_directorio.apuntadores_de_objetos[1].esDirectorio=1;
    nuevo_bloque_directorio.apuntadores_de_objetos[1].numero_de_bloque=bloque_actual;
    nuevo_bloque_directorio.apuntadores_de_objetos[1].size = 0;

    WriteBlock(nuevo_bloque, (char*)&nuevo_bloque_directorio);
    bloques_libres[nuevo_bloque]=0;

    printf("Directorio '%s' creado en bloque %d.\n", nombre_directorio, nuevo_bloque);

}

void ls(){

     //Vaciamos el bloque actual en un buffer
    char buffer[BLOCK_SIZE];
    ReadBlock(bloque_actual, buffer);

    //apuntador tipo Bloque_directorio a buffer
    Bloque_directorio* directorio_actual = (Bloque_directorio*)buffer;

    
     //Bloque_directorio directorio_actual;//no es el mismo espacio que BLOCK_SIZE ERROR
    //ReadBlock(bloque_actual, (char*)&directorio_actual);

    printf("Contenido del directorio: \n");
    for(int i=0; i< directorio_actual->contador_de_apuntadores; i++){
        printf("%s %s\n", directorio_actual->apuntadores_de_objetos[i].nombre, 
            directorio_actual->apuntadores_de_objetos[i].esDirectorio ? "DIR":"FILE");
    }

}

void cd(char * nombre_directorio){

    if(strcmp(nombre_directorio, "/") == 0){
        bloque_actual = 0;
        printf("Cambiado al directorio raíz.\n");
        return;
    }

    char buffer[BLOCK_SIZE];
    ReadBlock(bloque_actual, buffer);
    Bloque_directorio *directorio_actual = (Bloque_directorio*)buffer;

    for (int i=0; i<directorio_actual->contador_de_apuntadores;i++){
        if (strcmp(nombre_directorio, directorio_actual->apuntadores_de_objetos[i].nombre)==0 &&
            directorio_actual->apuntadores_de_objetos[i].esDirectorio==1){
                bloque_actual= directorio_actual->apuntadores_de_objetos[i].numero_de_bloque;
                printf("Cambiado al directorio '%s' (bloque %d).\n", nombre_directorio, bloque_actual);
                return;
            }
    }
    printf("Error: Directorio '%s' no encontrado.\n", nombre_directorio);

    

}


void obtener_nombre_dir_actual(char* nombre_directorio) {
    // caso "/"
    if (bloque_actual == 0) {
        strcpy(nombre_directorio, "/");
        return;
    }

    // Leer el bloque actual y encontrar el bloque del padre ("..")
    char buffer_actual[BLOCK_SIZE];
    ReadBlock(bloque_actual, buffer_actual);
    Bloque_directorio* directorio_actual = (Bloque_directorio*)buffer_actual;

    int numero_bloque_padre = -1;
    for (int i = 0; i < directorio_actual->contador_de_apuntadores; i++) {
        if (strcmp(directorio_actual->apuntadores_de_objetos[i].nombre, "..") == 0) {
            numero_bloque_padre = directorio_actual->apuntadores_de_objetos[i].numero_de_bloque;
            break;
        }
    }
    // Leer el bloque del padre para encontrar nuestro propio nombre
    char buffer_padre[BLOCK_SIZE];
    ReadBlock(numero_bloque_padre, buffer_padre);
    Bloque_directorio* directorio_padre = (Bloque_directorio*)buffer_padre;

    for (int i = 0; i < directorio_padre->contador_de_apuntadores; i++) {
        // Buscar el Apuntador que apunta a nuestro bloque actual
        if (directorio_padre->apuntadores_de_objetos[i].numero_de_bloque == bloque_actual) {
            strcpy(nombre_directorio, directorio_padre->apuntadores_de_objetos[i].nombre);
            return;
        }
    }
}

int main(){

    
    //char arreglo[]="Juan";
    aStream = fopen("mi_sistema_de_archivos.txt", "r+");

    char comando[100];
    char arg1[50], arg2[50];
    char nombre_directorio_actual[100];
    


    while(1){

        obtener_nombre_dir_actual(nombre_directorio_actual);
        printf("%s/>", nombre_directorio_actual);
        fgets(comando, 50, stdin);
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
        else if (strncmp(comando, "cd ", 3)==0){
            sscanf(comando, "cd %s", arg1);
            cd(arg1);
        }

    }

    return 0;
}
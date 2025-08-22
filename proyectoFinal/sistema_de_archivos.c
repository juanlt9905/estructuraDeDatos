#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NOMBRE_ARCHIVO "mi_sistema_de_archivos"
#define NUMERO_DE_BLOQUES 10
#define LONGITUD_NOMBRES 100
#define ENTRADAS_POR_BLOQUE ((BLOCK_SIZE - sizeof(int)) / sizeof(Apuntador))

int bloque_actual =1; //BLOQUE RAIZ 
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
    fflush(aStream);
    


}

void ReadBlock (int i, char *blockAddr){

    fseek (aStream, i*BLOCK_SIZE, SEEK_SET);

    fread(blockAddr, BLOCK_SIZE, 1, aStream);
}

void guardar_mapa_de_bloques() {
    char buffer_mapa[BLOCK_SIZE] = {0};
    memcpy(buffer_mapa, bloques_libres, sizeof(bloques_libres));

    WriteBlock(0, (char*)buffer_mapa);
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
    raiz.apuntadores_de_objetos[0].numero_de_bloque = 1;
    raiz.apuntadores_de_objetos[0].size = 0;

    // apuntador a su dir padre ".." (el mismo)
    strcpy(raiz.apuntadores_de_objetos[1].nombre, "..");
    raiz.apuntadores_de_objetos[1].esDirectorio = 1;
    raiz.apuntadores_de_objetos[1].numero_de_bloque = 1;
    raiz.apuntadores_de_objetos[1].size = 0;
    
    WriteBlock(1, (char*)&raiz);


    bloques_libres[0]=0; //mapa de bloques
    bloques_libres[1]=0; //raiz del sistema

    // uardar el mapa de bloques en el bloque 0
    guardar_mapa_de_bloques();

    printf("Disco virtual de %d bloques de memoria.\n", numberOfRecords);

}


void inicializar_systema_de_archivos(){
    //init

    aStream = fopen("mi_sistema_de_archivos.txt", "w+r");
    bloque_actual=1;
    createEmptyVirtualDisk(NUMERO_DE_BLOQUES);


}
//buscar bloque libre
int encontrar_bloque_libre(){
    //empezamos del bloque 2
    for(int i=2; i<NUMERO_DE_BLOQUES; i++){
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
    guardar_mapa_de_bloques();
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
        printf("Error: No hay bloques libres. Inicializar disco con init.\n");
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

    guardar_mapa_de_bloques();

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
        bloque_actual = 1;
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
    if (bloque_actual == 1) {
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

//rmdir

void vaciar_directorio_recursivo(int numero_de_bloque) {
    // buffer para el directorio a vaciar
    char buffer[BLOCK_SIZE];
    ReadBlock(numero_de_bloque, buffer);
    Bloque_directorio* dir_a_vaciar = (Bloque_directorio*)buffer;

    // Recorremos todo el contenido del directorio
    for (int i=0; i<dir_a_vaciar->contador_de_apuntadores; i++) {
        Apuntador item=dir_a_vaciar->apuntadores_de_objetos[i];

        //omitir "." y ".."
        if (strcmp(item.nombre, ".")==0 || strcmp(item.nombre, "..")==0) {
            continue;
        }

        if (item.esDirectorio) {
            //si es directorio, llamamos a la funcion recursiva
            vaciar_directorio_recursivo(item.numero_de_bloque);
            // ya que se vacie el dicontenido, se libera su bloque
            liberar_bloque(item.numero_de_bloque);
        } else {
            //si es un archivo, se libera directamente
            liberar_bloque(item.numero_de_bloque);
        }
    }
}

void rmdir(char* nombre_directorio) {
    char buffer[BLOCK_SIZE];
    ReadBlock(bloque_actual, buffer);
    Bloque_directorio* directorio_actual = (Bloque_directorio*)buffer;
    
    // buscar apuntador al directorio a borrar
    int indice = -1;
    for (int i=0; i<directorio_actual->contador_de_apuntadores; i++) {
        if (strcmp(directorio_actual->apuntadores_de_objetos[i].nombre, nombre_directorio) == 0 && 
            directorio_actual->apuntadores_de_objetos[i].esDirectorio) {
            indice = i;
            break;
        }
    }
    
    if (indice == -1) {
        printf("Error: Directorio '%s' no encontrado.\n", nombre_directorio);
        return;
    }
    
    int bloque_a_borrar = directorio_actual->apuntadores_de_objetos[indice].numero_de_bloque;
    
    //vaciar directorio recursivamente
    vaciar_directorio_recursivo(bloque_a_borrar);
    
    // liberar el bloque del directorio aborrar
    liberar_bloque(bloque_a_borrar);
    
    // eliminar el apuntador del directorio borrado en el directorio actual
    for (int i =indice; i<directorio_actual->contador_de_apuntadores - 1; i++) {
        directorio_actual->apuntadores_de_objetos[i] = directorio_actual->apuntadores_de_objetos[i + 1];
    }
    directorio_actual->contador_de_apuntadores--;
    
    WriteBlock(bloque_actual, buffer);
    printf("Directorio '%s' ha sido eliminado.\n", nombre_directorio);
}

//file, creacion de archivo
void file(char* nombre_archivo) {
    // buffer para leer directorio actual
    char buffer[BLOCK_SIZE];
    ReadBlock(bloque_actual, buffer);
    Bloque_directorio* directorio_actual = (Bloque_directorio*)buffer;

    //Verificar existencia del archivo
    for (int i = 0; i<directorio_actual->contador_de_apuntadores; i++) {
        if (strcmp(directorio_actual->apuntadores_de_objetos[i].nombre, nombre_archivo) == 0) {
            printf("Error: '%s' ya existe.\n", nombre_archivo);
            return;
        }
    }

    //verificar espacio para el nuevo apuntador
    if (directorio_actual->contador_de_apuntadores >= ENTRADAS_POR_BLOQUE) {
        printf("Error: El directorio actual está lleno.\n");
        return;
    }

    //buscar bloque vacio para el contenido del archivo
    int nuevo_bloque_archivo = encontrar_bloque_libre();
    if (nuevo_bloque_archivo == -1) {
        printf("Error: No hay bloques libres en el disco.\n");
        return;
    }

    //solicitar contenido del archivo
    char contenido_archivo[BLOCK_SIZE - sizeof(int)];
    printf("Introduce el contenido para el archivo '%s':\n> ", nombre_archivo);
    fgets(contenido_archivo, sizeof(contenido_archivo), stdin);
    
    //quitar el salto de linea que fgets añade al final
    contenido_archivo[strcspn(contenido_archivo, "\n")] = 0;
    
    int tamano_contenido = strlen(contenido_archivo);

    // BLoque para la escitura
    Bloque_archivo nuevo_archivo;
    nuevo_archivo.size = tamano_contenido;
    strcpy(nuevo_archivo.data, contenido_archivo);
    WriteBlock(nuevo_bloque_archivo, (char*)&nuevo_archivo);

    //crear el apuntador en el directorio actual
    Apuntador* nuevo_apuntador = &directorio_actual->apuntadores_de_objetos[directorio_actual->contador_de_apuntadores];
    strcpy(nuevo_apuntador->nombre, nombre_archivo);
    nuevo_apuntador->esDirectorio = 0; //0 porque es un archivo
    nuevo_apuntador->numero_de_bloque = nuevo_bloque_archivo;
    nuevo_apuntador->size = tamano_contenido;
    directorio_actual->contador_de_apuntadores++;
    WriteBlock(bloque_actual, buffer);
    //bloque ocupado
    bloques_libres[nuevo_bloque_archivo] = 0;
    guardar_mapa_de_bloques();

    printf("Archivo '%s' creado en el bloque %d.\n", nombre_archivo, nuevo_bloque_archivo);
}

void cat(char * nombre_archivo){

    //obtener directorio actual
    char buffer[BLOCK_SIZE];
    ReadBlock(bloque_actual, buffer);
    Bloque_directorio*directorio_actual = (Bloque_directorio*)buffer;

    //buscar el apuntador del archivo en directorio actual
    int indice = -1;
    for (int i = 0; i <directorio_actual->contador_de_apuntadores; i++) {
        if (strcmp(directorio_actual->apuntadores_de_objetos[i].nombre, nombre_archivo) == 0) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Error: Archivo '%s' no encontrado.\n", nombre_archivo);
        return;
    }
    //verificar que no sea directorio
    Apuntador apuntador_archivo = directorio_actual->apuntadores_de_objetos[indice];
    if (apuntador_archivo.esDirectorio==1) {
        printf("Error: '%s' es un directorio, no se puede usar cat.\n", nombre_archivo);
        return;
    }

    //obtener bloque de datos del archivo

    char buffer_archivo[BLOCK_SIZE];
    ReadBlock(apuntador_archivo.numero_de_bloque, buffer_archivo);
    Bloque_archivo* archivo = (Bloque_archivo*)buffer_archivo;

    printf("%s\n", archivo->data);
}


void rm(char * nombre_archivo){
    // Obtener directorio actual.
    char buffer[BLOCK_SIZE];
    ReadBlock(bloque_actual, buffer);
    Bloque_directorio* directorio_actual = (Bloque_directorio*)buffer;
    
    // buscar archivo en dir actual
    int indice = -1;
    for (int i = 0; i < directorio_actual->contador_de_apuntadores; i++) {
        if (strcmp(directorio_actual->apuntadores_de_objetos[i].nombre, nombre_archivo) == 0) {
            indice = i;
            break;
        }
    }
    
    if (indice == -1) {
        printf("Error: Archivo '%s' no encontrado.\n", nombre_archivo);
        return;
    }
    
    if (directorio_actual->apuntadores_de_objetos[indice].esDirectorio) {
        printf("Error: '%s' es un directorio.\n", nombre_archivo);
        return;
    }
    
    // liberar bloque del archivo
    int bloque_a_liberar = directorio_actual->apuntadores_de_objetos[indice].numero_de_bloque;
    liberar_bloque(bloque_a_liberar);
    
    //desplazar apuntadores del dir actual
    for (int i = indice; i < directorio_actual->contador_de_apuntadores - 1; i++) {
        directorio_actual->apuntadores_de_objetos[i] = directorio_actual->apuntadores_de_objetos[i + 1];
    }
    directorio_actual->contador_de_apuntadores--;
    
    
    WriteBlock(bloque_actual, buffer);
    
    printf("Archivo '%s' eliminado.\n", nombre_archivo);
}

void copiar_archivo(Apuntador* source_apuntador, int dest_dir_block_num) {
    // obetener directorio destino
    char buffer_destino[BLOCK_SIZE];
    ReadBlock(dest_dir_block_num, buffer_destino);
    Bloque_directorio* dir_destino = (Bloque_directorio*)buffer_destino;

    //verificar si hay espacio en el nuevo directorio
    if (dir_destino->contador_de_apuntadores >= ENTRADAS_POR_BLOQUE) {
        printf("Error: El directorio destino está lleno.\n");
        return;
    }

    // 3. Verificar si ya existe un archivo con el mismo nombre en el destino
    for (int i = 0; i < dir_destino->contador_de_apuntadores; i++) {
        if (strcmp(dir_destino->apuntadores_de_objetos[i].nombre, source_apuntador->nombre) == 0) {
            printf("Error: El archivo '%s' ya existe en el directorio destino.\n", source_apuntador->nombre);
            return; // Detener la copia
        }
    }

    // 4. Encontrar un bloque libre para el contenido del nuevo archivo
    int nuevo_bloque = encontrar_bloque_libre();
    if (nuevo_bloque == -1) {
        printf("Error: No hay bloques libres en el disco.\n");
        return;
    }

    // 5. Copiar el contenido del bloque del archivo original al nuevo bloque
    char buffer_archivo[BLOCK_SIZE];
    ReadBlock(source_apuntador->numero_de_bloque, buffer_archivo);
    WriteBlock(nuevo_bloque, buffer_archivo);

    // 6. Crear el nuevo Apuntador en el directorio destino
    Apuntador* nuevo_apuntador = &dir_destino->apuntadores_de_objetos[dir_destino->contador_de_apuntadores];
    strcpy(nuevo_apuntador->nombre, source_apuntador->nombre);
    nuevo_apuntador->esDirectorio = 0; // Es un archivo
    nuevo_apuntador->numero_de_bloque = nuevo_bloque;
    nuevo_apuntador->size = source_apuntador->size;
    
    // 7. Actualizar el contador del directorio destino y guardarlo en el disco
    dir_destino->contador_de_apuntadores++;
    WriteBlock(dest_dir_block_num, buffer_dest);
    
    // 8. Actualizar el mapa de bloques en memoria y guardarlo en el disco
    bloques_libres[nuevo_bloque] = 0;
    guardar_mapa_de_bloques();
    
    printf("Archivo '%s' copiado con éxito.\n", source_apuntador->nombre);
}

int main(){

    
    //char arreglo[]="Juan";
    aStream = fopen("mi_sistema_de_archivos.txt", "r+");

    char buffer_mapa[BLOCK_SIZE];
    ReadBlock(0, buffer_mapa);

    memcpy(bloques_libres, buffer_mapa, sizeof(bloques_libres));

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
        else if(strncmp(comando, "rmdir ", 6)==0){
            sscanf(comando, "rmdir %s", arg1);
            rmdir(arg1);
        }
        else if(strncmp(comando, "file ", 5)==0){
            sscanf(comando, "file %s", arg1);
            file(arg1);
        }
        else if(strncmp(comando, "cat ", 4)==0){
            sscanf(comando, "cat %s", arg1);
            cat(arg1);
        }
        else if(strncmp(comando, "rm ", 3)==0){
            sscanf(comando, "rm %s", arg1);
            rm(arg1);
        }


    }

    fclose(aStream);
    

    return 0;
}
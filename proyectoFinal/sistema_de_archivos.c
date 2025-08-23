#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NOMBRE_ARCHIVO "mi_sistema_de_archivos"
#define NUMERO_DE_BLOQUES 50
#define LONGITUD_NOMBRES 28
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

    //verificar si ya existe un archivo con el mismo nombre en el destino
    for (int i = 0; i < dir_destino->contador_de_apuntadores; i++) {
        if (strcmp(dir_destino->apuntadores_de_objetos[i].nombre, source_apuntador->nombre) == 0) {
            printf("Error: El archivo '%s' ya existe en el directorio destino.\n", source_apuntador->nombre);
            return; // Detener la copia
        }
    }

    // encontrar nuevo bloque para el archivo destino
    int nuevo_bloque = encontrar_bloque_libre();
    if (nuevo_bloque == -1) {
        printf("Error: No hay bloques libres en el disco.\n");
        return;
    }

    // copiar el contenido del bloque del archivo original al nuevo bloque
    char buffer_archivo[BLOCK_SIZE];
    ReadBlock(source_apuntador->numero_de_bloque, buffer_archivo);
    WriteBlock(nuevo_bloque, buffer_archivo);

    //crear el nuevo apuntador en el directorio destino
    Apuntador* nuevo_apuntador = &dir_destino->apuntadores_de_objetos[dir_destino->contador_de_apuntadores];
    strcpy(nuevo_apuntador->nombre, source_apuntador->nombre);
    nuevo_apuntador->esDirectorio = 0; // Es un archivo
    nuevo_apuntador->numero_de_bloque = nuevo_bloque;
    nuevo_apuntador->size = source_apuntador->size;
    
    // actualizar el contador del directorio destino y guardarlo en el disco
    dir_destino->contador_de_apuntadores++;
    WriteBlock(dest_dir_block_num, buffer_destino);
    
    // actualizar el mapa de bloques en memoria y guardarlo en el disco
    bloques_libres[nuevo_bloque] = 0;
    guardar_mapa_de_bloques();
    
    printf("Archivo '%s' copiado.\n", source_apuntador->nombre);
}

void copiar_directorio_recursivo(Apuntador* source_dir_apuntador, int parent_dest_dir_block_num) {
    // cargar el directorio padre de destino para añadir la nueva carpeta
    char buffer_parent_dest[BLOCK_SIZE];
    ReadBlock(parent_dest_dir_block_num, buffer_parent_dest);
    Bloque_directorio* dir_padre_destino = (Bloque_directorio*)buffer_parent_dest;

    
    //verificar si hay espacio en el directorio padre de destino
    if (dir_padre_destino->contador_de_apuntadores >= ENTRADAS_POR_BLOQUE) {
        printf("Error: El directorio destino está lleno, no se puede copiar '%s'.\n", source_dir_apuntador->nombre);
        return;
    }
    
    // verificar si ya existe un objeto con el mismo nombre en el destino
    for (int i = 0; i < dir_padre_destino->contador_de_apuntadores; i++) {
        if (strcmp(dir_padre_destino->apuntadores_de_objetos[i].nombre, source_dir_apuntador->nombre) == 0) {
            printf("Error: '%s' ya existe en el directorio destino.\n", source_dir_apuntador->nombre);
            return;
        }
    }

    // Encontrar un bloque libre para el nuevo directorio
    int nuevo_bloque_dir = encontrar_bloque_libre();
    if (nuevo_bloque_dir == -1) {
        printf("Error: No hay bloques libres en el disco para crear la copia de '%s'.\n", source_dir_apuntador->nombre);
        return;
    }

    // crear el Apuntador para este nuevo directorio en su directorio padre
    Apuntador* nuevo_dir_apuntador = &dir_padre_destino->apuntadores_de_objetos[dir_padre_destino->contador_de_apuntadores];
    strcpy(nuevo_dir_apuntador->nombre, source_dir_apuntador->nombre);
    nuevo_dir_apuntador->esDirectorio = 1;
    nuevo_dir_apuntador->numero_de_bloque = nuevo_bloque_dir;
    nuevo_dir_apuntador->size = 0;
    
    dir_padre_destino->contador_de_apuntadores++;
    WriteBlock(parent_dest_dir_block_num, buffer_parent_dest);

    //inicializar el contenido del nuevo directorio con sus entradas "." y ".."
    Bloque_directorio nuevo_dir_contenido;
    nuevo_dir_contenido.contador_de_apuntadores = 2;

    //entrada para "."
    strcpy(nuevo_dir_contenido.apuntadores_de_objetos[0].nombre, ".");
    nuevo_dir_contenido.apuntadores_de_objetos[0].esDirectorio = 1;
    nuevo_dir_contenido.apuntadores_de_objetos[0].numero_de_bloque = nuevo_bloque_dir;
    nuevo_dir_contenido.apuntadores_de_objetos[0].size = 0;

    //entrada para ".." 
    strcpy(nuevo_dir_contenido.apuntadores_de_objetos[1].nombre, "..");
    nuevo_dir_contenido.apuntadores_de_objetos[1].esDirectorio = 1;
    nuevo_dir_contenido.apuntadores_de_objetos[1].numero_de_bloque = parent_dest_dir_block_num;
    nuevo_dir_contenido.apuntadores_de_objetos[1].size = 0;

    WriteBlock(nuevo_bloque_dir, (char*)&nuevo_dir_contenido);
    
    // marcar el bloque como usado y guardar el mapa
    bloques_libres[nuevo_bloque_dir] = 0;
    guardar_mapa_de_bloques();
    printf("Directorio '%s' copiado.\n", source_dir_apuntador->nombre);

    // cargar el directorio origen para iterar sobre su contenido
    char buffer_source[BLOCK_SIZE];
    ReadBlock(source_dir_apuntador->numero_de_bloque, buffer_source);
    Bloque_directorio* dir_origen = (Bloque_directorio*)buffer_source;

    //copiar cada elemento del directorio origen al nuevo directorio
    for (int i = 0; i < dir_origen->contador_de_apuntadores; i++) {
        Apuntador* item_a_copiar = &dir_origen->apuntadores_de_objetos[i];

        // Omitir "." y ".." 
        if (strcmp(item_a_copiar->nombre, ".") == 0 || strcmp(item_a_copiar->nombre, "..") == 0) {
            continue;
        }

        // copiar un archivo o un subdirectorio
        if (item_a_copiar->esDirectorio) {
            copiar_directorio_recursivo(item_a_copiar, nuevo_bloque_dir);
        } else {
            copiar_archivo(item_a_copiar, nuevo_bloque_dir);
        }
    }
}
void cp(char* nombre_origen, char* nombre_destino_dir) {
    //Obtener directorio actual
    char buffer[BLOCK_SIZE];
    ReadBlock(bloque_actual, buffer);
    Bloque_directorio* directorio_actual_ptr = (Bloque_directorio*)buffer;

    // Apuntador del archivo/directorio de origen
    int indice_origen = -1;
    for (int i = 0; i < directorio_actual_ptr->contador_de_apuntadores; i++) {
        if (strcmp(directorio_actual_ptr->apuntadores_de_objetos[i].nombre, nombre_origen) == 0) {
            indice_origen = i;
            break;
        }
    }

    if (indice_origen == -1) {
        printf("Error: El origen '%s' no existe.\n", nombre_origen);
        return;
    }
    
    //buscar el bloque del directorio destino
    int bloque_destino_dir = -1;
    //caso raiz "/"
    if (strcmp(nombre_destino_dir, "/") == 0) {
        bloque_destino_dir = 1;
    } else {
        //buscar el directorio destino en el directorio actual
        for (int i = 0; i < directorio_actual_ptr->contador_de_apuntadores; i++) {
            if (strcmp(directorio_actual_ptr->apuntadores_de_objetos[i].nombre, nombre_destino_dir) == 0) {
                // verificar que el destino sea realmente un directorio
                if (directorio_actual_ptr->apuntadores_de_objetos[i].esDirectorio==0) {
                    printf("Error: El destino '%s' no es un directorio.\n", nombre_destino_dir);
                    return;
                }
                bloque_destino_dir = directorio_actual_ptr->apuntadores_de_objetos[i].numero_de_bloque;
                break;
            }
        }
    }

    if (bloque_destino_dir == -1) {
        printf("Error: El directorio destino '%s' no existe.\n", nombre_destino_dir);
        return;
    }

    // 4. Decidir qué función auxiliar llamar basándose en el tipo del origen
    Apuntador apuntador_origen = directorio_actual_ptr->apuntadores_de_objetos[indice_origen];
    if (apuntador_origen.esDirectorio) {
        copiar_directorio_recursivo(&apuntador_origen, bloque_destino_dir);
    } else {
        copiar_archivo(&apuntador_origen, bloque_destino_dir);
    }
}

void mv(char* nombre_origen, char* nombre_destino) {
    //obtener directorio actual
    char buffer_actual[BLOCK_SIZE];
    ReadBlock(bloque_actual, buffer_actual);
    Bloque_directorio* directorio_actual = (Bloque_directorio*)buffer_actual;

    // buscar apuntador de origen
    int indice_origen = -1;
    for (int i = 0; i < directorio_actual->contador_de_apuntadores; i++) {
        if (strcmp(directorio_actual->apuntadores_de_objetos[i].nombre, nombre_origen) == 0) {
            indice_origen = i;
            break;
        }
    }

    if (indice_origen == -1) {
        printf("Error: El origen '%s' no existe.\n", nombre_origen);
        return;
    }
    //guardamos una copia del Apuntador de origen antes de cualquier cambio
    Apuntador apuntador_a_mover = directorio_actual->apuntadores_de_objetos[indice_origen];

    //verificar el destino
    int indice_destino_dir = -1;
    for (int i = 0; i < directorio_actual->contador_de_apuntadores; i++) {
        if (strcmp(directorio_actual->apuntadores_de_objetos[i].nombre, nombre_destino) == 0) {
            if (directorio_actual->apuntadores_de_objetos[i].esDirectorio) {
                indice_destino_dir = i;
            }
            break;
        }
    }

    // decidir si mover o renombrar
    if (indice_destino_dir != -1) {
        // MOver a un directorio
        int bloque_destino_dir = directorio_actual->apuntadores_de_objetos[indice_destino_dir].numero_de_bloque;
        
        // obtener directorio destino.
        char buffer_destino[BLOCK_SIZE];
        ReadBlock(bloque_destino_dir, buffer_destino);
        Bloque_directorio* dir_destino = (Bloque_directorio*)buffer_destino;

        //verificar si hay espacio en el directorio destino
        if (dir_destino->contador_de_apuntadores >= ENTRADAS_POR_BLOQUE) {
            printf("Error: El directorio destino '%s' está lleno.\n", nombre_destino);
            return;
        }
        
        //verificar si ya existe el objeto
        for (int i = 0; i < dir_destino->contador_de_apuntadores; i++) {
            if (strcmp(dir_destino->apuntadores_de_objetos[i].nombre, apuntador_a_mover.nombre) == 0) {
                printf("Error: Ya existe un objeto llamado '%s' en el directorio destino.\n", apuntador_a_mover.nombre);
                return;
            }
        }

        // Añadir el apuntador al directorio destino
        dir_destino->apuntadores_de_objetos[dir_destino->contador_de_apuntadores] = apuntador_a_mover;
        dir_destino->contador_de_apuntadores++;
        WriteBlock(bloque_destino_dir, buffer_destino);

        // eliminar apuntador del direectorio origen.
        for (int i = indice_origen; i < directorio_actual->contador_de_apuntadores - 1; i++) {
            directorio_actual->apuntadores_de_objetos[i] = directorio_actual->apuntadores_de_objetos[i + 1];
        }
        directorio_actual->contador_de_apuntadores--;
        WriteBlock(bloque_actual, buffer_actual);

        printf("'%s' movido a '%s'.\n", nombre_origen, nombre_destino);

    } else {//Renombrar
        for (int i = 0; i < directorio_actual->contador_de_apuntadores; i++) {
            if (strcmp(directorio_actual->apuntadores_de_objetos[i].nombre, nombre_destino) == 0) {
                printf("Error: Ya existe un objeto llamado '%s'.\n", nombre_destino);
                return;
            }
        }

        // cambiar el nombre del apuntador existente.
        strcpy(directorio_actual->apuntadores_de_objetos[indice_origen].nombre, nombre_destino);
        WriteBlock(bloque_actual, buffer_actual);
        printf("'%s' renombrado a '%s'.\n", nombre_origen, nombre_destino);
    }
}


int coincidencia(char* texto, char* patron_b) {
    
    char patron[LONGITUD_NOMBRES];
    strcpy(patron, patron_b);
    
    char* asterisco = strchr(patron, '*');
    int len_patron = strlen(patron);

    if (asterisco != NULL) {
        // existe el *
        *asterisco = '\0'; // divide el patron en dos en el lugar del '*'
        const char* prefijo = patron;
        const char* sufijo = asterisco + 1;
        
        //la linea COMIENZA CON EL PREFIJO
        if (strncmp(texto, prefijo, strlen(prefijo)) != 0) {
            return 0; // No coincide
        }
        
        // Y el resto de la línea debe contener el sufijo
        if (strstr(texto + strlen(prefijo), sufijo) == NULL) { //verificamos el sufijo
            return 0; // No coincide
        }

        return 1; // Coincide

    } else if (len_patron > 0 && patron[len_patron - 1] == '$') {
        // caso2: termina con $
        char patron_sin_dolar[LONGITUD_NOMBRES];
        strncpy(patron_sin_dolar, patron, len_patron - 1);
        patron_sin_dolar[len_patron - 1] = '\0';
        
        int len_texto = strlen(texto);
        int len_patron_sin_dolar = strlen(patron_sin_dolar);

        if (len_texto < len_patron_sin_dolar) {
            return 0; // el texto es mas corto que el patrón
        }
        
        // comparar si el final del texto es igual que el patron
        if (strcmp(texto+len_texto-len_patron_sin_dolar, patron_sin_dolar) == 0) {
            return 1; // Coincide
        }

    } else {
        // busqueda sin comodin
        if (strstr(texto, patron) != NULL) {
            return 1; // Coincide
        }
    }

    return 0; // No se encontro coincidencia
}

void grep(char* patron, char* nombre_archivo) {
    //obtener directorio actual
    char buffer_dir[BLOCK_SIZE];
    ReadBlock(bloque_actual, buffer_dir);
    Bloque_directorio* directorio_actual = (Bloque_directorio*)buffer_dir;

    //Buscar el Apuntador del archivo
    int indice_encontrado = -1;
    for (int i = 0; i < directorio_actual->contador_de_apuntadores; i++) {
        if (strcmp(directorio_actual->apuntadores_de_objetos[i].nombre, nombre_archivo) == 0) {
            if(directorio_actual->apuntadores_de_objetos[i].esDirectorio) {
                printf("Error: No se puede usar grep en un directorio.\n");
                return;
            }
            indice_encontrado = i;
            break;
        }
    }

    if (indice_encontrado == -1) {
        printf("Error: Archivo '%s' no encontrado.\n", nombre_archivo);
        return;
    }
    
    // leer contenido del archivo
    Apuntador apuntador_archivo = directorio_actual->apuntadores_de_objetos[indice_encontrado];
    char buffer_archivo[BLOCK_SIZE];
    ReadBlock(apuntador_archivo.numero_de_bloque, buffer_archivo);
    Bloque_archivo* archivo = (Bloque_archivo*)buffer_archivo;

    // usar la función auxiliar para ver si hay coincidencia
    if (coincidencia(archivo->data, patron)==1) {
        // Si coincide, imprimir el contenido del archivo
        printf("%s\n", archivo->data);
    }
}

void create(char* nombre_destino, char* ruta_origen) {
    
    //copimos ruta de origen para editarla
    char ruta_origen_editable[LONGITUD_NOMBRES];
    strcpy(ruta_origen_editable, ruta_origen);

    Apuntador apuntador_origen;
    int encontrado = 0;

    // Buscamos si la ruta contiene un '/' para saber si es una ruta o un nombre simple
    char* separador = strchr(ruta_origen_editable, '/');
    
    if (separador != NULL) {
        // caso 1: el archivo que tiene el contenido esta en un directorio dentro del directorio actual.
        *separador = '\0'; // Divide la cadena en directorio y archivo
        char* nombre_dir_origen = ruta_origen_editable;
        char* nombre_archivo_origen = separador + 1;

        // obtener el diretorio que contiene el archivo con el contenido
        char buffer_actual[BLOCK_SIZE];
        ReadBlock(bloque_actual, buffer_actual);
        Bloque_directorio* dir_actual = (Bloque_directorio*)buffer_actual;

        int bloque_dir_origen = -1;
        for (int i = 0; i < dir_actual->contador_de_apuntadores; i++) {
            if (strcmp(dir_actual->apuntadores_de_objetos[i].nombre, nombre_dir_origen) == 0 && dir_actual->apuntadores_de_objetos[i].esDirectorio) {
                bloque_dir_origen = dir_actual->apuntadores_de_objetos[i].numero_de_bloque;
                break;
            }
        }

        if (bloque_dir_origen != -1) {
            // obtener archivo 
            char buffer_origen[BLOCK_SIZE];
            ReadBlock(bloque_dir_origen, buffer_origen);
            Bloque_directorio* dir_origen = (Bloque_directorio*)buffer_origen;

            for (int i = 0; i < dir_origen->contador_de_apuntadores; i++) {
                if (strcmp(dir_origen->apuntadores_de_objetos[i].nombre, nombre_archivo_origen) == 0) {
                    apuntador_origen = dir_origen->apuntadores_de_objetos[i];
                    encontrado = 1;
                    break;
                }
            }
        }

    } else {
        //el archivo con el contenido esta en el directorio actual
        char buffer_actual[BLOCK_SIZE];
        ReadBlock(bloque_actual, buffer_actual);
        Bloque_directorio* dir_actual = (Bloque_directorio*)buffer_actual;
        for (int i = 0; i < dir_actual->contador_de_apuntadores; i++) {
            if (strcmp(dir_actual->apuntadores_de_objetos[i].nombre, ruta_origen) == 0) {
                apuntador_origen = dir_actual->apuntadores_de_objetos[i];
                encontrado = 1;
                break;
            }
        }
    }

    if (encontrado==0) {
        printf("Error: No se pudo encontrar el origen '%s'.\n", ruta_origen);
        return;
    }
    if (apuntador_origen.esDirectorio==1) {
        printf("Error: No se puede usar este comando para copiar directorios.\n");
        return;
    }

    //copiar el archivo en el directorio actual
    
    char buffer_dest[BLOCK_SIZE];
    ReadBlock(bloque_actual, buffer_dest); // El destino es el directorio actual
    Bloque_directorio* dir_destino = (Bloque_directorio*)buffer_dest;

    if (dir_destino->contador_de_apuntadores >= ENTRADAS_POR_BLOQUE) {
        printf("Error: El directorio actual está lleno, no se puede crear '%s'.\n", nombre_destino);
        return;
    }

    // verificacion de nombre en el dir actua;
    for (int i = 0; i < dir_destino->contador_de_apuntadores; i++) {
        if (strcmp(dir_destino->apuntadores_de_objetos[i].nombre, nombre_destino) == 0) {
            printf("Error: Ya existe un objeto llamado '%s' en el directorio actual.\n", nombre_destino);
            return;
        }
    }

    int nuevo_bloque = encontrar_bloque_libre();
    if (nuevo_bloque == -1) { printf("No hay bloques libres en el disco.\n"); return; }

    //copiar el contenido del bloque del archivo
    char buffer_archivo[BLOCK_SIZE];
    ReadBlock(apuntador_origen.numero_de_bloque, buffer_archivo);
    WriteBlock(nuevo_bloque, buffer_archivo);

    // Crear el nuevo Apuntador en el directorio actual con el nuevo nombre
    Apuntador* nuevo_apuntador = &dir_destino->apuntadores_de_objetos[dir_destino->contador_de_apuntadores];
    strcpy(nuevo_apuntador->nombre, nombre_destino); // Usar el nuevo nombre
    nuevo_apuntador->esDirectorio = 0;
    nuevo_apuntador->numero_de_bloque = nuevo_bloque;
    nuevo_apuntador->size = apuntador_origen.size;
    
    dir_destino->contador_de_apuntadores++;
    WriteBlock(bloque_actual, buffer_dest);
    
    bloques_libres[nuevo_bloque] = 0;
    guardar_mapa_de_bloques();
    printf("Archivo '%s' creado como '%s'.\n", ruta_origen, nombre_destino);
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
        else if(strncmp(comando, "cp ", 3) == 0){
            sscanf(comando, "cp %s %s", arg1, arg2);
            cp(arg1, arg2);
        }else if(strncmp(comando, "mv ", 3) == 0){
            sscanf(comando, "mv %s %s", arg1, arg2);
            mv(arg1, arg2);
        }
        else if(strncmp(comando, "grep ", 5) == 0){
            sscanf(comando, "grep %s %s", arg1, arg2);
            grep(arg1, arg2);
        }
        else if (strncmp(comando, "create ", 7) == 0){
            sscanf(comando, "create %s %s", arg1, arg2);
            create(arg1, arg2); 
        }

    }

    fclose(aStream);
    

    return 0;
}
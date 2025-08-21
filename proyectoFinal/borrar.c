#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NOMBRE_ARCHIVO "mi_sistema_de_archivos"
#define NUMERO_DE_BLOQUES 100
#define MAX_NAME_LEN 32
#define MAX_ENTRIES_PER_BLOCK ((BLOCK_SIZE - sizeof(int)) / sizeof(DirectoryEntry))

int bloque_actual = 0; 
FILE * aStream;

// Estructura para entrada de directorio
typedef struct {
    char name[MAX_NAME_LEN];
    int is_directory;  // 1 = directorio, 0 = archivo
    int block_number;
    int size;
} DirectoryEntry;

// Estructura para un bloque de directorio
typedef struct {
    int entry_count;
    DirectoryEntry entries[MAX_ENTRIES_PER_BLOCK];
} DirectoryBlock;

// Estructura para un bloque de archivo
typedef struct {
    int size;
    char data[BLOCK_SIZE - sizeof(int)];
} FileBlock;

// Array para rastrear bloques libres (simplificación)
int bloques_libres[NUMERO_DE_BLOQUES];

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

//Funcion para reservar espacio para almacenar el sistema de archivos.
void createEmptyVirtualDisk(int numberOfRecords){
    //Crea un array de caracteres de tamaño BLOCK_SIZE.
    char block[BLOCK_SIZE]; 
    memset(block, '\0', BLOCK_SIZE);
    
    // Escribir todos los bloques vacíos
    for(int i = 0; i < numberOfRecords; i++){
        WriteBlock(i, block);
        bloques_libres[i] = 1; // Marcar como libre
    }
    
    // Crear directorio raíz en bloque 0
    DirectoryBlock root;
    root.entry_count = 0;
    WriteBlock(0, (char*)&root);
    bloques_libres[0] = 0; // Marcar bloque 0 como ocupado
    
    printf("Disco virtual creado con %d bloques.\n", numberOfRecords);
    printf("Directorio raíz creado en bloque 0.\n");
}

void inicializar_systema_de_archivos(){
    //init
    if(aStream) fclose(aStream);
    aStream = fopen("mi_sistema_de_archivos.txt", "w+");
    if(!aStream){
        printf("Error: No se pudo crear el archivo del sistema.\n");
        return;
    }
    
    createEmptyVirtualDisk(NUMERO_DE_BLOQUES);
    bloque_actual = 0;
    printf("Sistema de archivos inicializado.\n");
    
    fclose(aStream);
    aStream = fopen("mi_sistema_de_archivos.txt", "r+");
}

// Buscar bloque libre
int encontrar_bloque_libre(){
    for(int i = 1; i < NUMERO_DE_BLOQUES; i++){
        if(bloques_libres[i] == 1){
            return i;
        }
    }
    return -1;
}

// Liberar bloque
void liberar_bloque(int block_num){
    char block[BLOCK_SIZE];
    memset(block, '\0', BLOCK_SIZE);
    WriteBlock(block_num, block);
    bloques_libres[block_num] = 1;
}

// Comando mkdir
void cmd_mkdir(const char* dirname){
    if(!aStream){
        printf("Error: Sistema no inicializado. Ejecuta 'init' primero.\n");
        return;
    }
    
    if(strlen(dirname) >= MAX_NAME_LEN){
        printf("Error: Nombre demasiado largo.\n");
        return;
    }
    
    DirectoryBlock current_dir;
    ReadBlock(bloque_actual, (char*)&current_dir);
    
    // Verificar si ya existe
    for(int i = 0; i < current_dir.entry_count; i++){
        if(strcmp(current_dir.entries[i].name, dirname) == 0){
            printf("Error: '%s' ya existe.\n", dirname);
            return;
        }
    }
    
    // Verificar espacio en directorio
    if(current_dir.entry_count >= MAX_ENTRIES_PER_BLOCK){
        printf("Error: Directorio lleno.\n");
        return;
    }
    
    // Buscar bloque libre
    int nuevo_bloque = encontrar_bloque_libre();
    if(nuevo_bloque == -1){
        printf("Error: No hay bloques libres.\n");
        return;
    }
    
    // Crear entrada en directorio actual
    strcpy(current_dir.entries[current_dir.entry_count].name, dirname);
    current_dir.entries[current_dir.entry_count].is_directory = 1;
    current_dir.entries[current_dir.entry_count].block_number = nuevo_bloque;
    current_dir.entries[current_dir.entry_count].size = 0;
    current_dir.entry_count++;
    
    WriteBlock(bloque_actual, (char*)&current_dir);
    
    // Crear nuevo directorio vacío
    DirectoryBlock nuevo_dir;
    nuevo_dir.entry_count = 0;
    WriteBlock(nuevo_bloque, (char*)&nuevo_dir);
    bloques_libres[nuevo_bloque] = 0; // Marcar como ocupado
    
    printf("Directorio '%s' creado en bloque %d.\n", dirname, nuevo_bloque);
}

// Comando rmdir
void cmd_rmdir(const char* dirname){
    if(!aStream){
        printf("Error: Sistema no inicializado.\n");
        return;
    }
    
    DirectoryBlock current_dir;
    ReadBlock(bloque_actual, (char*)&current_dir);
    
    int found_index = -1;
    for(int i = 0; i < current_dir.entry_count; i++){
        if(strcmp(current_dir.entries[i].name, dirname) == 0 && 
           current_dir.entries[i].is_directory){
            found_index = i;
            break;
        }
    }
    
    if(found_index == -1){
        printf("Error: Directorio '%s' no encontrado.\n", dirname);
        return;
    }
    
    int dir_block = current_dir.entries[found_index].block_number;
    
    // Verificar que esté vacío
    DirectoryBlock target_dir;
    ReadBlock(dir_block, (char*)&target_dir);
    
    if(target_dir.entry_count > 0){
        printf("Error: El directorio '%s' no está vacío.\n", dirname);
        return;
    }
    
    // Liberar bloque
    liberar_bloque(dir_block);
    
    // Remover entrada del directorio padre
    for(int i = found_index; i < current_dir.entry_count - 1; i++){
        current_dir.entries[i] = current_dir.entries[i + 1];
    }
    current_dir.entry_count--;
    
    WriteBlock(bloque_actual, (char*)&current_dir);
    printf("Directorio '%s' eliminado.\n", dirname);
}

// Comando cd
void cmd_cd(const char* dirname){
    if(!aStream){
        printf("Error: Sistema no inicializado.\n");
        return;
    }
    
    if(strcmp(dirname, "/") == 0){
        bloque_actual = 0;
        printf("Cambiado al directorio raíz.\n");
        return;
    }
    
    DirectoryBlock current_dir;
    ReadBlock(bloque_actual, (char*)&current_dir);
    
    for(int i = 0; i < current_dir.entry_count; i++){
        if(strcmp(current_dir.entries[i].name, dirname) == 0 && 
           current_dir.entries[i].is_directory){
            bloque_actual = current_dir.entries[i].block_number;
            printf("Cambiado al directorio '%s' (bloque %d).\n", dirname, bloque_actual);
            return;
        }
    }
    
    printf("Error: Directorio '%s' no encontrado.\n", dirname);
}

// Comando ls
void cmd_ls(){
    if(!aStream){
        printf("Error: Sistema no inicializado.\n");
        return;
    }
    
    DirectoryBlock current_dir;
    ReadBlock(bloque_actual, (char*)&current_dir);
    
    printf("Contenido del directorio (bloque %d):\n", bloque_actual);
    printf("Tipo\tNombre\t\t\tBloque\tTamaño\n");
    printf("----\t------\t\t\t------\t------\n");
    
    for(int i = 0; i < current_dir.entry_count; i++){
        printf("%s\t%-20s\t%d\t%d\n",
               current_dir.entries[i].is_directory ? "DIR" : "FILE",
               current_dir.entries[i].name,
               current_dir.entries[i].block_number,
               current_dir.entries[i].size);
    }
    
    if(current_dir.entry_count == 0){
        printf("(directorio vacío)\n");
    }
}

// Comando touch (crear archivo)
void cmd_touch(const char* filename){
    if(!aStream){
        printf("Error: Sistema no inicializado.\n");
        return;
    }
    
    DirectoryBlock current_dir;
    ReadBlock(bloque_actual, (char*)&current_dir);
    
    // Verificar si ya existe
    for(int i = 0; i < current_dir.entry_count; i++){
        if(strcmp(current_dir.entries[i].name, filename) == 0){
            printf("Error: '%s' ya existe.\n", filename);
            return;
        }
    }
    
    if(current_dir.entry_count >= MAX_ENTRIES_PER_BLOCK){
        printf("Error: Directorio lleno.\n");
        return;
    }
    
    int nuevo_bloque = encontrar_bloque_libre();
    if(nuevo_bloque == -1){
        printf("Error: No hay bloques libres.\n");
        return;
    }
    
    // Crear entrada
    strcpy(current_dir.entries[current_dir.entry_count].name, filename);
    current_dir.entries[current_dir.entry_count].is_directory = 0;
    current_dir.entries[current_dir.entry_count].block_number = nuevo_bloque;
    current_dir.entries[current_dir.entry_count].size = 0;
    current_dir.entry_count++;
    
    WriteBlock(bloque_actual, (char*)&current_dir);
    
    // Crear archivo vacío
    FileBlock nuevo_archivo;
    nuevo_archivo.size = 0;
    memset(nuevo_archivo.data, 0, sizeof(nuevo_archivo.data));
    WriteBlock(nuevo_bloque, (char*)&nuevo_archivo);
    bloques_libres[nuevo_bloque] = 0;
    
    printf("Archivo '%s' creado en bloque %d.\n", filename, nuevo_bloque);
}

// Comando write (escribir en archivo)
void cmd_write(const char* filename, const char* content){
    if(!aStream){
        printf("Error: Sistema no inicializado.\n");
        return;
    }
    
    DirectoryBlock current_dir;
    ReadBlock(bloque_actual, (char*)&current_dir);
    
    int found_index = -1;
    for(int i = 0; i < current_dir.entry_count; i++){
        if(strcmp(current_dir.entries[i].name, filename) == 0 && 
           !current_dir.entries[i].is_directory){
            found_index = i;
            break;
        }
    }
    
    if(found_index == -1){
        printf("Error: Archivo '%s' no encontrado.\n", filename);
        return;
    }
    
    int file_block = current_dir.entries[found_index].block_number;
    
    if(strlen(content) >= sizeof(((FileBlock*)0)->data)){
        printf("Error: Contenido demasiado largo.\n");
        return;
    }
    
    FileBlock archivo;
    strcpy(archivo.data, content);
    archivo.size = strlen(content);
    WriteBlock(file_block, (char*)&archivo);
    
    // Actualizar tamaño en directorio
    current_dir.entries[found_index].size = archivo.size;
    WriteBlock(bloque_actual, (char*)&current_dir);
    
    printf("Contenido escrito en '%s' (%d bytes).\n", filename, archivo.size);
}

// Comando read (leer archivo)
void cmd_read_file(const char* filename){
    if(!aStream){
        printf("Error: Sistema no inicializado.\n");
        return;
    }
    
    DirectoryBlock current_dir;
    ReadBlock(bloque_actual, (char*)&current_dir);
    
    for(int i = 0; i < current_dir.entry_count; i++){
        if(strcmp(current_dir.entries[i].name, filename) == 0 && 
           !current_dir.entries[i].is_directory){
            FileBlock archivo;
            ReadBlock(current_dir.entries[i].block_number, (char*)&archivo);
            printf("Contenido de '%s' (%d bytes):\n", filename, archivo.size);
            printf("'%s'\n", archivo.data);
            return;
        }
    }
    
    printf("Error: Archivo '%s' no encontrado.\n", filename);
}

int main(){
    // Inicializar array de bloques libres
    for(int i = 0; i < NUMERO_DE_BLOQUES; i++){
        bloques_libres[i] = 1;
    }
    
    aStream = fopen("mi_sistema_de_archivos.txt", "r+");
    
    char comando[100];
    char arg1[50], arg2[200];
    
    printf("=== Sistema de Archivos ===\n");
    printf("Comandos: init, mkdir, rmdir, cd, ls, touch, write, read, quit\n\n");

    while(1){
        printf("fs[%d]> ", bloque_actual);
        fgets(comando, sizeof(comando), stdin);
        comando[strcspn(comando,"\n")] = 0;
        
        // Limpiar argumentos
        memset(arg1, 0, sizeof(arg1));
        memset(arg2, 0, sizeof(arg2));
        
        if (strcmp(comando, "init")==0){
            inicializar_systema_de_archivos();
        }
        else if(strncmp(comando, "mkdir ", 6)==0){
            sscanf(comando, "mkdir %s", arg1);
            cmd_mkdir(arg1);
        }
        else if(strncmp(comando, "rmdir ", 6)==0){
            sscanf(comando, "rmdir %s", arg1);
            cmd_rmdir(arg1);
        }
        else if(strncmp(comando, "cd ", 3)==0){
            sscanf(comando, "cd %s", arg1);
            cmd_cd(arg1);
        }
        else if(strcmp(comando, "ls")==0){
            cmd_ls();
        }
        else if(strncmp(comando, "touch ", 6)==0){
            sscanf(comando, "touch %s", arg1);
            cmd_touch(arg1);
        }
        else if(strncmp(comando, "write ", 6)==0){
            // Extraer nombre de archivo y contenido
            char *space = strchr(comando + 6, ' ');
            if(space){
                strncpy(arg1, comando + 6, space - (comando + 6));
                arg1[space - (comando + 6)] = '\0';
                strcpy(arg2, space + 1);
                cmd_write(arg1, arg2);
            } else {
                printf("Uso: write <archivo> <contenido>\n");
            }
        }
        else if(strncmp(comando, "read ", 5)==0){
            sscanf(comando, "read %s", arg1);
            cmd_read_file(arg1);
        }
        else if(strcmp(comando, "read")==0){
            // Mantener tu comando original para leer bloque 0
            char cadena_lectura[BLOCK_SIZE];
            memset(cadena_lectura, '\0', BLOCK_SIZE);
            ReadBlock(0, cadena_lectura);
            printf("Contenido del bloque 0: '%s'\n", cadena_lectura);
        }
        else if (strcmp(comando, "quit")==0){
            break;
        }
        else {
            printf("Comando no reconocido: %s\n", comando);
        }
    }

    if(aStream) fclose(aStream);
    return 0;
}
/*

Carrera: Ing de Sistemas
Semestre 4
Materia: Lenguaje de programacion I
Seccion: D3
Docente: Juan Jose Rial Somma
Grupo 8

Integrantes:
-Miguel Antonio 31951249
-Luis Díaz 31755591
-Gabriel Aponte 31680138

*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#define MAXIMO_TAREAS 100

typedef struct 
{
    char titulo_tarea[100];
    char descripcion[300];   // nota de la tarea
    char fecha[15]; 
    int prioridad; // 1-prioridad alta, 2-prioridad media, 3 prioridad baja
    int estado_tarea; // 0-incompleta, 1-competa 
}Tareas;

Tareas tareas[MAXIMO_TAREAS];

int contador_tareas = 0;
int contadoractivas = 0;
int contadorcompletadas = 0;
int cambios_pendientes = 0; // 0 = no hay cambios, 1 = hay cambios
char archivo_cargado[100] = "";  // Variable para almacenar el nombre del archivo cargado

void limpiarBufferEntrada(void);

/* ======== LECTURA SEGURA ======== */

int leerEnteroSeguro(const char *mensaje, int min, int max)
{
    char buffer[100];
    char *fin;
    long numero;

    while (1)
    {
        printf("%s", mensaje);

        if (!fgets(buffer, sizeof(buffer), stdin))
            continue;

        numero = strtol(buffer, &fin, 10); 

        if (fin == buffer || (*fin != '\n' && *fin != '\0'))
        {
            printf("*Error: solo numeros validos.\n");
            continue;
        }

        if (numero < min || numero > max)
        {
            printf("*Error: rango permitido (%d - %d).\n", min, max);
            continue;
        }

        return (int)numero;
    }
}

void leerTextoSeguro(const char *mensaje, char *destino, int tam)
{
    while (1)
    {
        printf("%s", mensaje);

        if (!fgets(destino, tam, stdin))
            continue;

        destino[strcspn(destino, "\n")] = 0;

        if (strlen(destino) == 0)
        {
            printf("*Error: no puede estar vacio.\n");
            continue;
        }

        return;
    }
}


void recalcular_contadores(void);


void guardar_tareas(const char *nombre_archivo);
void cargar_tareas(const char *nombre_archivo);

void agregartarea();
void mostrar_tarea(void);
void mostrar_menu(void); 
void editar_tarea(void);
void eliminar_tarea(void);  // Nueva función para eliminar tarea
void mostrar_contadores(void); // Mostrar el contador de tareas
void marcar_completada(void); // Función para marcar tarea como completada
/* -------------------- Inicio/menu del programa -------------------- */
int main() {
    // Mostrar el menú inicial recursivamente
    mostrar_menu();
    return 0;
}

void mostrar_menu() {
    char nombre_archivo[100];
    int opcion = 0;

    // Imprime el menú de opciones iniciales
    printf("\nBienvenido a nuestro gestor de tareas personales, realizado por el grupo 8 de ING. En Sistemas.\n");
    printf("\n*Nota: solo se cargan archivos .dat creados por este programa.\n");
    printf("=== Que accion desea realizar? ===\n");
    printf("\n1. Cargar tareas de un archivo\n");
    printf("2. Crear nuevo archivo de tareas\n");
    
    opcion = leerEnteroSeguro("\n*Seleccione una opcion: ", 1, 2);

    switch(opcion) {
        case 1:
            // Cargar tareas desde un archivo
            printf("*Ingrese el nombre del archivo para cargar: ");
            fgets(nombre_archivo, sizeof(nombre_archivo), stdin);
            nombre_archivo[strcspn(nombre_archivo, "\n")] = '\0'; // Limpiar salto de línea
            cargar_tareas(nombre_archivo);
            strcpy(archivo_cargado, nombre_archivo);
            break;
        case 2:
            // Crear un nuevo archivo
    contador_tareas = 0;
    contadoractivas = 0;
    contadorcompletadas = 0;

    printf("*Ingrese nombre del nuevo archivo(Maximo 99 caracteres. Use solo letras y numeros para el nombre del archivo): ");
    
    fgets(nombre_archivo, sizeof(nombre_archivo), stdin);
nombre_archivo[strcspn(nombre_archivo, "\n")] = '\0';

strcpy(archivo_cargado, nombre_archivo);

contador_tareas = 0;
contadoractivas = 0;
contadorcompletadas = 0;

printf("*Archivo nuevo creado: '%s.dat'\n", archivo_cargado);
    break;
        default:
            printf("*Opcion invalida. Volviendo a intentar...\n");
            mostrar_menu();  // Si la opcion es incorrecta, volvemos al menú
            return;
    }

    // Mostrar el menú secundario con las opciones de gestión de tareas

    int ejecutando = 1;


    while (ejecutando) {
        // Mostrar encabezado con nombre del archivo cargado y contadores
        mostrar_contadores();

        printf("\n=== Que desea hacer ahora? ===\n");
        printf("1. Agregar una nueva tarea\n");
       
        printf("2. Mostrar tareas\n");
        printf("3. Guardar tareas\n");
        
        printf("4. Salir del programa\n");
        printf("5. Regresar al menu principal\n");

       opcion = leerEnteroSeguro("\n*Seleccione una opcion numerica (1-5): ", 1, 5);

        switch(opcion) {
            case 1:
                agregartarea();  // Función para agregar tarea
                break;
            
            case 2:
                mostrar_tarea(); // Mostrar todas las tareas
                break;
            case 3:
                guardar_tareas(archivo_cargado);
                cambios_pendientes = 0; // No hay cambios pendientes después de guardar
                break;
            
          case 4:
{
    char respuesta[10];

    printf("*Desea guardar antes de salir? (s/n): ");
    fgets(respuesta, sizeof(respuesta), stdin);

    if (respuesta[0] == 's' || respuesta[0] == 'S') {
        guardar_tareas(archivo_cargado);
    }

    printf("*Saliendo del programa...\n");
    return;

}// Termina la ejecución del programa
            case 5:
                printf("*Regresando al menu principal...\n");
                mostrar_menu();  // Volver al menú principal
                return;
            default:
                printf("*Opcion invalida. Intente nuevamente.\n");
        }

        // Mostrar el menú nuevamente si no se ha salido
    }
}

void agregartarea(){
if (contador_tareas >= MAXIMO_TAREAS)
{
   printf("*Lo siento, has alcanzado el maximo de tareas)\n");
   return;
}

printf("===Agregar una tarea===\n");
printf("\n*Recuerde que el programa no es capaz de almacenar mas de 100 archivos de tareas\npor lo tanto, si cuenta con un numero de tareas superior a 99 se recomienda hacer una limpieza de las mismas para poder seguir agregando mas sin problemas.\n\n");
leerTextoSeguro("*Ingrese el titulo de la tarea(maximo 99 caracteres): ",
                tareas[contador_tareas].titulo_tarea, 100);

    leerTextoSeguro("*Ingrese una descripcion o nota(maximo 299 caracteres): ",
                tareas[contador_tareas].descripcion, 300);

    time_t t = time(NULL);
struct tm *tm_info = localtime(&t);
strftime(tareas[contador_tareas].fecha, 15, "%d/%m/%Y", tm_info);

tareas[contador_tareas].prioridad =
    leerEnteroSeguro("*Ingrese la prioridad (SOLO NUMEROS: 1-Alta, 2-Media, 3-Baja): ", 1, 3);

tareas[contador_tareas].estado_tarea = 0; // por defecto incompleta

contador_tareas++;

recalcular_contadores();

cambios_pendientes = 1; // Hay cambios pendientes para guardar

printf("===Tarea agregada correctamente.\n");
}

void eliminar_tarea() {
    if (contador_tareas == 0) {
        printf("*No hay tareas para eliminar.\n");
        return;
    }

    int indice;
   indice = leerEnteroSeguro("*Ingrese el numero de la tarea que desea eliminar: ", 1, contador_tareas);

    if (indice < 1 || indice > contador_tareas) {
        printf("*Numero de tarea invalido.\n");
        return;
    }

    // Eliminar la tarea
    for (int i = indice - 1; i < contador_tareas - 1; i++) {
        tareas[i] = tareas[i + 1]; // Desplazar tareas hacia arriba
    }

    contador_tareas--; // Reducir el contador de tareas

    recalcular_contadores();
    cambios_pendientes = 1; // Hay cambios pendientes para guardar

    printf("*Tarea eliminada correctamente.\n");

    // Después de eliminar tarea, volvemos al menú principal recursivamente
}

void mostrar_tarea(void){

    int opcion;

    if (contador_tareas == 0){
        printf("*No hay tareas.\n");
        return;
    }

    while(1)   // ← CLAVE PARA NO SALIR AL MENU PRINCIPAL
    {
        int filtro = 0;

        printf("\n=== Filtro ===\n");
        printf("1. Todas\n");
        printf("2. Solo activas\n");
        printf("3. Solo completadas\n");
       filtro = leerEnteroSeguro("Seleccione una opcion numerica (1-3): ", 1, 3);

        printf("\n=== Lista de Tareas ===\n");

        int encontradas = 0;

        for (int i = 0; i < contador_tareas; i++) {

            if (filtro == 2 && tareas[i].estado_tarea == 1)
                continue;

            if (filtro == 3 && tareas[i].estado_tarea == 0)
                continue;

            printf("\nTarea #%d\n", i + 1);
            printf("Titulo: %s\n", tareas[i].titulo_tarea);
            printf("Descripcion: %s\n", tareas[i].descripcion);
            printf("Fecha: %s\n", tareas[i].fecha);
            printf("Prioridad: %d\n", tareas[i].prioridad);
            printf("Estado: %s\n",
                   tareas[i].estado_tarea ? "Completada" : "No completada");

            encontradas++;
        }

        if (encontradas == 0) {
            printf("*No hay tareas que coincidan con el filtro.\n");
        }

        printf("\n=== Opciones ===\n");
        printf("1. Marcar completada\n");
        printf("2. Eliminar\n");
        printf("3. Editar\n");
        printf("4. Volver\n");
        opcion = leerEnteroSeguro("Seleccione una opcion numerica (1-4): ", 1, 4);

        switch(opcion) {
            case 1: marcar_completada(); break;
            case 2: eliminar_tarea(); break;
            case 3: editar_tarea(); break;
            case 4: return;   // ← SOLO vuelve al menú secundario
            default: printf("*Opcion invalida.\n");
        }
    }
}


/* -------------------- ARCHIVOS -------------------- */

void guardar_tareas(const char *nombre_archivo)
{
    char archivo_final[140];
   
    if (strstr(nombre_archivo, ".dat") == NULL)
    {
        // Si no tiene ".dat", lo agregamos
        snprintf(archivo_final, sizeof(archivo_final), "%s.dat", nombre_archivo);
    }
    else
    {
        // Si ya tiene ".dat", usamos el nombre tal cual
        snprintf(archivo_final, sizeof(archivo_final), "%s", nombre_archivo);
    }

    FILE *f = fopen(archivo_final, "wb");
    if (f == NULL)
    {
        printf("*Error: No se pudo abrir el archivo para guardar.\n");
        return;
    }


    // Guardar cuántas tareas hay
    fwrite(&contador_tareas, sizeof(int), 1, f);

    // Guardar solo las tareas que existen
    fwrite(tareas, sizeof(Tareas), contador_tareas, f);

    fclose(f);
    printf("*Tareas guardadas en '%s'.\n", archivo_final);
    cambios_pendientes = 0; // No hay cambios pendientes después de guardar

}

void cargar_tareas(const char *nombre_archivo)
{
    char archivo_final[140];
    snprintf(archivo_final, sizeof(archivo_final), "%s.dat", nombre_archivo);

    FILE *f = fopen(archivo_final, "rb");
    if (f == NULL)
    {
        printf("*No existe '%s'. Se iniciara sin tareas.\n", archivo_final);
        contador_tareas = 0;
        contadoractivas = 0;
        contadorcompletadas = 0;
        return;
    }

    if (fread(&contador_tareas, sizeof(int), 1, f) != 1)
    {
        printf("*Archivo corrupto o vacio. Se iniciara sin tareas.\n");
        contador_tareas = 0;
        fclose(f);
        return;
    }

    if (contador_tareas < 0 || contador_tareas > MAXIMO_TAREAS)
    {
        printf("*Archivo invalido. Se iniciara sin tareas.\n");
        contador_tareas = 0;
        fclose(f);
        return;
    }

    fread(tareas, sizeof(Tareas), contador_tareas, f);
    fclose(f);

    recalcular_contadores();

    strcpy(archivo_cargado, nombre_archivo);

    printf("*Tareas cargadas desde '%s'.\n", archivo_final);
}
/* -------------------- Utilidades -------------------- */
void limpiarBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void recalcular_contadores(void) {
    contadoractivas = 0;
    contadorcompletadas = 0;

    for (int i = 0; i < contador_tareas; i++) {
        if (tareas[i].estado_tarea == 1)
            contadorcompletadas++;
        else
            contadoractivas++;
    }
}

void mostrar_contadores(void) {
    printf("\n=== Archivo \"%s\" cargado ===\n", archivo_cargado);
    printf("*Tareas agregadas: %d\n", contador_tareas);
    printf("*Tareas activas: %d\n", contadoractivas);
    printf("*Tareas completadas: %d\n", contadorcompletadas);
}

void marcar_completada(void) {
    int indice;
    if (contador_tareas == 0) {
        printf("*No hay tareas para marcar como completadas.\n");
        return;
    }

    indice = leerEnteroSeguro("*Ingrese el numero de la tarea que desea marcar como completada: ", 1, contador_tareas);

    if (indice < 1 || indice > contador_tareas) {
        printf("*Numero de tarea invalido.\n");
        return;
    }

    if (tareas[indice - 1].estado_tarea == 1) {
        printf("*La tarea ya esta completada.\n");
        return;
    }

    tareas[indice - 1].estado_tarea = 1;
    recalcular_contadores();
    cambios_pendientes = 1; // Hay cambios pendientes para guardar

    printf("*Tarea marcada como completada.\n");

    // Después de marcar la tarea como completada, volvemos al menú principal
}

void editar_tarea(){

    int indice, opcion;

    if (contador_tareas == 0){
        printf("*No hay tareas para editar.\n");
        return; // vuelve al submenu automaticamente
    }

   indice = leerEnteroSeguro("*Numero de tarea a editar: ", 1, contador_tareas);

    if(indice < 1 || indice > contador_tareas){
        printf("*Numero invalido.\n");
        return;
    }

    indice--;

    printf("\n1. Cambiar titulo\n");
    printf("2. Cambiar descripcion\n");
    printf("3. Cambiar prioridad\n");
    opcion = leerEnteroSeguro("Seleccione: ", 1, 3);
    switch(opcion){

        case 1:
            leerTextoSeguro("Nuevo titulo: ", tareas[indice].titulo_tarea, 100);
            break;

        case 2:
           leerTextoSeguro("Nueva descripcion: ", tareas[indice].descripcion, 300);
            break;

        case 3:
            printf("Nueva prioridad: ");
           tareas[indice].prioridad =
    leerEnteroSeguro("Nueva prioridad (1-3): ", 1, 3);
            break;
    }

    cambios_pendientes = 1;

    printf("*Tarea editada correctamente.\n");
}

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
#define MAXIMO_TAREAS 100

typedef struct 
{
    char titulo_tarea[100];
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
void recalcular_contadores(void);


void guardar_tareas(const char *nombre_archivo);
void cargar_tareas(const char *nombre_archivo);

void agregartarea();
void mostrar_tarea(void);
void mostrar_menu(void); 
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
    printf("=== Que accion desea realizar? ===\n");
    printf("\n1. Cargar tareas de un archivo\n");
    printf("2. Crear nuevo archivo de tareas\n");

    printf("\n*Seleccione una opcion: ");
    scanf("%d", &opcion);
    limpiarBufferEntrada();  // Limpiar el buffer de entrada

    switch(opcion) {
        case 1:
            // Cargar tareas desde un archivo
            printf("*Ingrese el nombre del archivo para cargar: ");
            fgets(nombre_archivo, sizeof(nombre_archivo), stdin);
            nombre_archivo[strcspn(nombre_archivo, "\n")] = '\0'; // Limpiar salto de línea
            cargar_tareas(nombre_archivo);
            break;
        case 2:
            // Crear un nuevo archivo
            contador_tareas = 0;
            contadoractivas = 0;
            contadorcompletadas = 0;
            printf("*Archivo nuevo creado.\n");
            break;
        default:
            printf("*Opcion invalida. Volviendo a intentar...\n");
            mostrar_menu();  // Si la opcion es incorrecta, volvemos al menú
            return;
    }

    // Mostrar el menú secundario con las opciones de gestión de tareas
    while (1) {
        // Mostrar encabezado con nombre del archivo cargado y contadores
        mostrar_contadores();

        printf("\n=== Que desea hacer ahora? ===\n");
        printf("1. Agregar una nueva tarea\n");
        printf("2. Eliminar una tarea\n");
        printf("3. Mostrar tareas\n");
        printf("4. Guardar tareas\n");
        printf("5. Marcar tarea como completada\n");
        printf("6. Salir del programa\n");
        printf("7. Regresar al menu principal\n");

        printf("\n*Seleccione una opcion: ");
        scanf("%d", &opcion);
        limpiarBufferEntrada();  // Limpiar el buffer de entrada

        switch(opcion) {
            case 1:
                agregartarea();  // Función para agregar tarea
                break;
            case 2:
                eliminar_tarea(); // Función para eliminar tarea
                break;
            case 3:
                mostrar_tarea(); // Mostrar todas las tareas
                break;
            case 4:
                printf("*Ingrese el nombre del archivo para guardar: ");
                fgets(nombre_archivo, sizeof(nombre_archivo), stdin);
                nombre_archivo[strcspn(nombre_archivo, "\n")] = '\0'; // Limpiar salto de línea
                guardar_tareas(nombre_archivo);  
                break;
            case 5:
                marcar_completada();  // Función para marcar tarea como completada
                break;
            case 6:
                if (cambios_pendientes) {
                    printf("*Hay cambios pendientes. ¿Desea guardar antes de salir? (s/n): ");
                    char guardar;
                    scanf(" %c", &guardar);
                    if (guardar == 's' || guardar == 'S') {
                        printf("*Ingrese el nombre del archivo para guardar: ");
                        fgets(nombre_archivo, sizeof(nombre_archivo), stdin);
                        nombre_archivo[strcspn(nombre_archivo, "\n")] = '\0'; // Limpiar salto de línea
                        guardar_tareas(nombre_archivo);
                    }
                }
                printf("*Saliendo del programa...\n");
                return;  // Termina la ejecución del programa
            case 7:
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
printf("*Ingrese el titulo de la tarea: ");
fgets(tareas[contador_tareas].titulo_tarea, 100, stdin); 

 // Quitar salto de línea que deja fgets
    tareas[contador_tareas].titulo_tarea[strcspn(tareas[contador_tareas].titulo_tarea, "\n")] = '\0';

printf("*Ingrese la prioridad de la tarea (1-Alta, 2-Media, 3-Baja): ");
scanf("%d", &tareas[contador_tareas].prioridad);
limpiarBufferEntrada(); // importante para que el próximo fgets no se salte

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
    printf("*Ingrese el numero de la tarea que desea eliminar: ");
    scanf("%d", &indice);

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

void mostrar_tarea(void) {
    if (contador_tareas == 0) {
        printf("*No hay tareas agregadas.\n");
        return;
    }

    printf("\n=== Lista de Tareas ===\n");
    for (int i = 0; i < contador_tareas; i++) {
        printf("*Tarea #%d: %s\n", i + 1, tareas[i].titulo_tarea);
        printf("*Prioridad: %d\n", tareas[i].prioridad);
        printf("*Estado: %s\n", tareas[i].estado_tarea ? "Completada" : "No completada");
    }

    // Después de mostrar las tareas, volvemos al menú principal recursivamente
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

    printf("*Ingrese el numero de la tarea que desea marcar como completada: ");
    scanf("%d", &indice);

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

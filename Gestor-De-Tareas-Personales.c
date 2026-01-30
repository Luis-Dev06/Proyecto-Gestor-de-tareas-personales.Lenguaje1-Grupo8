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

void limpiarBufferEntrada(void);
void recalcular_contadores(void);


void guardar_tareas(const char *nombre_archivo);
void cargar_tareas(const char *nombre_archivo);

void agregartarea();
void mostrar_tarea(void);
void mostrar_menu(void); 

int main(){

  char nombre_archivo[100];
  char opcion_mostrar;// Opcion para decidir si se muestran las tareas1
  int opcion = 0;

    //Mostrar menu y permitir al usuario elegir o ingresar archivo
    mostrar_menu();
    scanf("%d", &opcion);
    limpiarBufferEntrada();
    
    printf("Ingrese el nombre del archivo (sin extension, ej. 'tareas1'): ");
    fgets(nombre_archivo, sizeof(nombre_archivo), stdin);
    nombre_archivo[strcspn(nombre_archivo, "\n")] = '\0'; //Limpiar salto de línea

    //Intentar cargar las tareas del archivo especificado
    if (opcion == 1)
    {
        cargar_tareas(nombre_archivo);
    }
    else if (opcion == 2)
    {
        // Nuevo archivo: arrancar vacío
        contador_tareas = 0;
        contadoractivas = 0;
        contadorcompletadas = 0;
        printf("* Archivo nuevo seleccionado: '%s.dat'\n", nombre_archivo);
    }
    else
    {
        printf("* Opcion invalida. Saliendo...\n");
        return 0;
    }

printf("=====================================\n");
    
    printf("===Bienvenido al Gestor de Tareas Personales\n\n");
    printf("===Tareas agregadas: %d\n", contador_tareas);
    printf("===Tareas activas: %d\n", contadoractivas);
    printf("===Tareas completadas: %d\n\n", contadorcompletadas);


// ----- OPCION PARA MOSTRAR TAREAS DESPUES DE LA BIENVENIDA -----

printf("Desea ver sus tareas actuales? (s/n): ");

scanf(" %c", &opcion_mostrar);

limpiarBufferEntrada();

if (opcion_mostrar == 's' || opcion_mostrar == 'S')
{
    mostrar_tarea
    ();
}

agregartarea();


if (cambios_pendientes)
{
    guardar_tareas(nombre_archivo);
}

return 0;
}

void mostrar_menu()
{
    printf("=== Menu ===\n");
    printf("\n1. Cargar tareas de un archivo\n");
    printf("2. Crear nuevo archivo de tareas\n");
    printf("\nElija una opcion: ");
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

/* -------------------- UTILIDADES -------------------- */

void limpiarBufferEntrada(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void recalcular_contadores(void)
{
    int i;
    contadoractivas = 0;
    contadorcompletadas = 0;

    for (i = 0; i < contador_tareas; i++)
    {
        if (tareas[i].estado_tarea == 1)
            contadorcompletadas++;
        else
            contadoractivas++;
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
        printf("* Error: No se pudo abrir el archivo para guardar.\n");
        return;
    }


    // Guardar cuántas tareas hay
    fwrite(&contador_tareas, sizeof(int), 1, f);

    // Guardar solo las tareas que existen
    fwrite(tareas, sizeof(Tareas), contador_tareas, f);

    fclose(f);
    printf("* Tareas guardadas en '%s'.\n", archivo_final);
}

void cargar_tareas(const char *nombre_archivo)
{
    char archivo_final[140];
    snprintf(archivo_final, sizeof(archivo_final), "%s.dat", nombre_archivo);

    FILE *f = fopen(archivo_final, "rb");
    if (f == NULL)
    {
        printf("* No existe '%s'. Se iniciara sin tareas.\n", archivo_final);
        contador_tareas = 0;
        contadoractivas = 0;
        contadorcompletadas = 0;
        return;
    }

    if (fread(&contador_tareas, sizeof(int), 1, f) != 1)
    {
        printf("* Archivo corrupto o vacio. Se iniciara sin tareas.\n");
        contador_tareas = 0;
        fclose(f);
        return;
    }

    if (contador_tareas < 0 || contador_tareas > MAXIMO_TAREAS)
    {
        printf("* Archivo invalido. Se iniciara sin tareas.\n");
        contador_tareas = 0;
        fclose(f);
        return;
    }

    fread(tareas, sizeof(Tareas), contador_tareas, f);
    fclose(f);

    recalcular_contadores();
    printf("* Tareas cargadas desde '%s'.\n", archivo_final);
}

void mostrar_tarea(void){ // Muestra todas las tareas registradas en el sistema
    int i;
    int opcion;
    int indice;

    printf("\n===Lista de Tareas===\n");

    if (contador_tareas == 0){
        printf("No hay tareas agregadas.\n");

    }
// Recorrer todas las tareas almacenadas
    for (i=0; i<contador_tareas; i++){ 

        printf("\nTarea #%d\n", i + 1);

        printf("Titulo: %s\n", tareas[i].titulo_tarea);


        printf("Prioridad: ");

        if (tareas[i].prioridad == 1)

            printf("Alta\n");

        else if (tareas[i].prioridad == 2)

            printf("Media\n");

        else if (tareas[i].prioridad == 3)
            printf("Baja\n");

        printf("Estado: ");

        if (tareas[i].estado_tarea == 1)

            printf("Completada\n");
        else
            printf("No Completada\n");
    }

    // ---- Preguntar si desea marcar una tarea ----
    printf("\n¿Desea marcar una tarea como completada?\n");

    printf("1. Si\n");

    printf("2. Volver\n");

    printf("Seleccione una opcion: ");

    scanf("%d", &opcion);

    limpiarBufferEntrada();

    if (opcion == 1)
    {
        printf("Ingrese el numero de la tarea: ");
        scanf("%d", &indice);
        limpiarBufferEntrada();

        indice--; // Ajuste para arreglo (usuario ve desde 1)

        if (indice < 0 || indice >= contador_tareas)
        {
            printf("* Numero de tarea invalido.\n");
            return;
        }

        if (tareas[indice].estado_tarea == 1)
        {
            printf("* La tarea ya esta completada.\n");
        }
        else
{
     tareas[indice].estado_tarea = 1;
    cambios_pendientes = 1;   // <-- CAMBIO IMPORTANTE
    recalcular_contadores();
    printf("* Tarea marcada como completada.\n");

    // Mostrar nuevamente la lista actualizada
    mostrar_tarea();
}
    }
}

    



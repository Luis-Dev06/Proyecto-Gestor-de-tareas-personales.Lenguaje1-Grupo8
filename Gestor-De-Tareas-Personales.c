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

void agregartarea();


int main(){

printf("=====================================\n");
    
    printf("===Bienvenido al Gestor de Tareas Personales\n\n");
    printf("===Tareas agregadas: %d\n", contador_tareas);
    printf("===Tareas activas: %d\n", contadoractivas);
    printf("===Tareas completadas: %d\n\n", contadorcompletadas);

agregartarea();

return 0;
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

printf("*Ingrese la prioridad de la tarea (1-Alta, 2-Media, 3-Baja): ");
scanf("%d", &tareas[contador_tareas].prioridad);

tareas[contador_tareas].estado_tarea = 0;

contador_tareas++;

    printf("===Tarea agregada correctamente.\n");





}
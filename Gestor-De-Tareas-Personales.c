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
}Tarea;

Tarea Tareas[MAXIMO_TAREAS];

int contador_tareas = 0;

void agregartarea();


int main(){

printf("=====================================\n");
    
    printf("Bienvenido al Gestor de Tareas Personales\n");

    printf("Aqui podra:\n");
    printf("1. Registrar tareas\n");
    printf("2. Marcar tareas completadas(no activo)\n");
    printf("3. Mostrar tareas activas y complementarias(no activo)\n");
    printf("4. Guardar en archivo (no activo)\n");
    printf("5. Contar tareas activas (no activo)\n");
    printf("=====================================\n");
    
agregartarea();

return 0;
}
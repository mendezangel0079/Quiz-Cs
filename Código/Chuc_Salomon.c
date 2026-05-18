/*
* Proyecto: Quiz-Cs (Software de Quizzes)
* Equipo: SOFTTRIDDLES
* Integrantes:
* - Balam Chuc Salomon Jesus
* - Lopez Ambrosio Jennifer Karina
* - Mendez Baas Angel Said
* - Zavala Chable Luis Fernando
* Materia: Programacion Estructurada
* Profesor: Emilio Gabriel Rejon Herrera
* Fecha: 10 de Mayo de 2026
* Version: 1.1
* Compilador: DevC++ (GCC)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definicion de macros y constantes del sistema
#define VERSION "1.1"
#define MAX_OPCIONES 4
#define LONGITUD_MAX 256
#define ARCHIVO_DB_USUARIOS "usuarios.txt"
#define ARCHIVO_DB_PREGUNTAS "banco_preguntas.txt"
#define ARCHIVO_DB_ESTADISTICAS "estadisticas.txt"
#define ARCHIVO_MANUAL "manual.txt"

// Estructura para manejar los datos de la sesion actual
typedef struct {
	char nombre_usuario[50];
	char contrasena[50];
	int rol; // 1 = Admin, 0 = Estudiante
} Usuario;

// Declaracion de funciones (Prototipos)
void limpiar_buffer();
void eliminar_salto_linea(char *cadena);
int leer_entero(); // Nueva funcion para lectura segura de numeros
char leer_caracter(); // Nueva funcion para lectura segura de caracteres
void pausar_pantalla();
void inicializar_archivos();
void actualizar_estadisticas(char *nombre_usuario, int correctas, int incorrectas);
void iniciar_sistema();
int iniciar_sesion(Usuario *usuario_actual);
void registrar_cuenta();
void menu_administrador(Usuario usuario_actual);
void menu_usuario(Usuario usuario_actual);
void agregar_acertijo(int es_admin);
void consultar_reactivo(); // Renombrado segun el documento
void seleccionar_tema(Usuario usuario_actual);
void reporte_desempeno(Usuario usuario_actual);
int menu_secundario(int mostrar_siguiente, int mostrar_atras);
void consultar_manual();
void consultar_registro_usuarios();
void gestionar_roles(Usuario admin_actual);
void modificar_credenciales(Usuario *usuario_actual);


/*
* Funcion principal que arranca el sistema
* Inicializa la base de datos y lanza el menu de autenticacion
*/

int main() {
	inicializar_archivos();
	iniciar_sistema();
	return 0;
}


/*
* Lee el archivo de estadisticas, busca al usuario y le suma
* los aciertos e incorrectas de su ultima sesion. Sobrescribe usando
* un archivo temporal.
*/


void actualizar_estadisticas(char *nombre_usuario, int correctas, int incorrectas) {
	if (correctas == 0 && incorrectas == 0) return;
	
	FILE *archivo = fopen(ARCHIVO_DB_ESTADISTICAS, "r");
	FILE *temp = fopen("temp_estadisticas.txt", "w");
	int encontrado = 0;
	
	if (archivo != NULL) {
		char linea[LONGITUD_MAX];
		while (fgets(linea, sizeof(linea), archivo) != NULL) {
			eliminar_salto_linea(linea);
			if (strlen(linea) == 0) continue;
			
			// Tokenizacion de datos CSV
			char *usr = strtok(linea, ",");
			char *q_str = strtok(NULL, ",");
			char *c_str = strtok(NULL, ",");
			char *i_str = strtok(NULL, ",");
			
			if (usr && q_str && c_str && i_str) {
				if (strcmp(usr, nombre_usuario) == 0) {
					// Actualizacion de metricas existentes
					int q = atoi(q_str) + 1; 
					int c = atoi(c_str) + correctas;
					int i = atoi(i_str) + incorrectas;
					fprintf(temp, "%s,%d,%d,%d\n", usr, q, c, i);
					encontrado = 1;
				} else {
					fprintf(temp, "%s,%s,%s,%s\n", usr, q_str, c_str, i_str);
				}
			}
		}
		fclose(archivo);
	}
	
	// Si el usuario es nuevo, se crea su registro
	if (!encontrado) {
		fprintf(temp, "%s,1,%d,%d\n", nombre_usuario, correctas, incorrectas);
	}
	
	fclose(temp);
	remove(ARCHIVO_DB_ESTADISTICAS);
	rename("temp_estadisticas.txt", ARCHIVO_DB_ESTADISTICAS);
}


/*
* Lee los datos consolidados en estadisticas.txt e imprime
* la sumatoria total del historial del usuario actual.
*/

void reporte_desempeno(Usuario usuario_actual) {
	system("cls");
	system("cls");
	printf("----- Opcion [3]: Reporte de desempeno -----\n");
	printf("Usuario: [%s]\n", usuario_actual.nombre_usuario);
	printf("Estadisticas:\n");
	printf("--- Estadisticas Historicas ---\n");
	
	int quizzes_totales = 0, correctas = 0, incorrectas = 0;
	
	FILE *archivo = fopen(ARCHIVO_DB_ESTADISTICAS, "r");
	if (archivo != NULL) {
		char linea[LONGITUD_MAX];
		while (fgets(linea, sizeof(linea), archivo) != NULL) {
			eliminar_salto_linea(linea);
			char *usr = strtok(linea, ",");
			char *q_str = strtok(NULL, ",");
			char *c_str = strtok(NULL, ",");
			char *i_str = strtok(NULL, ",");
			
			if (usr && strcmp(usr, usuario_actual.nombre_usuario) == 0) {
				quizzes_totales = atoi(q_str);
				correctas = atoi(c_str);
				incorrectas = atoi(i_str);
				break;
			}
		}
		fclose(archivo);
	}
	
	printf("Quizzes/Sesiones de estudio finalizadas: %d\n", quizzes_totales);
	printf("No. historico de respuestas correctas: %d\n", correctas);
	printf("No. historico de respuestas incorrectas: %d\n", incorrectas);
	
	pausar_pantalla();
}


/*
* Despliega las directrices y reglas del programa para el usuario,
* iterando sobre las lineas guardadas en manual.txt.
*/
void consultar_manual() {
	system("cls");
	printf("--- Consultar manual de usuario ---\n\n");
	FILE *archivo = fopen(ARCHIVO_MANUAL, "r");
	if (archivo != NULL) {
		char linea[LONGITUD_MAX];
		while(fgets(linea, sizeof(linea), archivo)) {
			printf("%s", linea);
		}
		fclose(archivo);
	} else {
		printf("El manual de usuario no se encuentra disponible.\n");
	}
	printf("\n");
	pausar_pantalla();
}



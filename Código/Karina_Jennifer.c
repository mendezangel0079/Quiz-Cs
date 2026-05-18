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
* Limpia el buffer de entrada para evitar saltos de linea residuales
* que afecten el uso de fgets o scanf posteriores.
*/

void limpiar_buffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}


/*
* Elimina el caracter de salto de linea '\n' al final de una cadena
* generada por fgets, reemplazandolo por un caracter nulo.
*/

void eliminar_salto_linea(char *cadena) {
	size_t longitud = strlen(cadena);
	if (longitud > 0 && cadena[longitud - 1] == '\n') {
		cadena[longitud - 1] = '\0';
	}
}


/*
* Módulo de Autenticacion.
* Valida de forma directa las credenciales contra la base de datos local.
*/

int iniciar_sesion(Usuario *usuario_actual) {
	system("cls");
	printf("---------Boton [1]: Iniciar sesion--------\n");
	
	char input_usuario[50];
	char input_contrasena[50];
	
	printf("Ingrese nombre de usuario: ");
	fgets(input_usuario, sizeof(input_usuario), stdin);
	eliminar_salto_linea(input_usuario);
	
	printf("Ingrese contrasena: ");
	fgets(input_contrasena, sizeof(input_contrasena), stdin);
	eliminar_salto_linea(input_contrasena);
	
	FILE *archivo = fopen(ARCHIVO_DB_USUARIOS, "r");
	if (archivo == NULL) {
		printf("\nNo hay cuentas registradas en el sistema.\n");
		pausar_pantalla();
		return 0;
	}
	
	char linea[LONGITUD_MAX];
	// Iteracion en la base de datos buscando "match"
	while (fgets(linea, sizeof(linea), archivo) != NULL) {
		eliminar_salto_linea(linea);
		char *usr = strtok(linea, ",");
		char *pwd = strtok(NULL, ",");
		char *rol = strtok(NULL, ",");
		
		if (usr && pwd && rol && strcmp(input_usuario, usr) == 0 && strcmp(input_contrasena, pwd) == 0) {
			strcpy(usuario_actual->nombre_usuario, usr);
			strcpy(usuario_actual->contrasena, pwd);
			usuario_actual->rol = atoi(rol);
			fclose(archivo);
			return 1;
		}
	}
	fclose(archivo);
	printf("\nCredenciales incorrectas.\n");
	pausar_pantalla();
	return 0;
}


/*
* Permite a nuevos usuarios registrar sus credenciales.
* Las almacena por defecto con el rol de estudiante (0).
*/

void registrar_cuenta() {
	system("cls");
	printf("-------------Boton [2]: Registrar cuenta--------------\n");
	// ... luego siguen tus printf pidiendo datos
	
	char nuevo_usuario[50];
	char nueva_contrasena[50];
	
	printf("Cree un nombre de usuario: ");
	fgets(nuevo_usuario, sizeof(nuevo_usuario), stdin);
	eliminar_salto_linea(nuevo_usuario);
	
	printf("Cree una contrasena: ");
	fgets(nueva_contrasena, sizeof(nueva_contrasena), stdin);
	eliminar_salto_linea(nueva_contrasena);
	
	FILE *archivo = fopen(ARCHIVO_DB_USUARIOS, "a");
	if (archivo != NULL) {
		fprintf(archivo, "%s,%s,0\n", nuevo_usuario, nueva_contrasena);
		fclose(archivo);
		printf("\nCuenta registrada exitosamente.\n");
	}
	pausar_pantalla();
}


/*
* Permite al usuario actual modificar su nombre de usuario y contrasena.
* Actualiza los archivos usuarios.txt y estadisticas.txt
*/

void modificar_credenciales(Usuario *usuario_actual) {
	system("cls");
	printf("--- Modificar Credenciales ---\n\n");
	
	char nuevo_usuario[50];
	char nueva_contrasena[50];
	
	printf("Ingrese su NUEVO nombre de usuario (o presione Enter para dejarlo igual): ");
	fgets(nuevo_usuario, sizeof(nuevo_usuario), stdin);
	eliminar_salto_linea(nuevo_usuario);
	if (strlen(nuevo_usuario) == 0) {
		strcpy(nuevo_usuario, usuario_actual->nombre_usuario);
	}
	
	printf("Ingrese su NUEVA contrasena (o presione Enter para dejarla igual): ");
	fgets(nueva_contrasena, sizeof(nueva_contrasena), stdin);
	eliminar_salto_linea(nueva_contrasena);
	if (strlen(nueva_contrasena) == 0) {
		strcpy(nueva_contrasena, usuario_actual->contrasena);
	}
	
	// 1. Actualizar base de datos de usuarios
	FILE *archivo = fopen(ARCHIVO_DB_USUARIOS, "r");
	FILE *temp = fopen("temp_usr.txt", "w");
	
	if (archivo && temp) {
		char linea[LONGITUD_MAX];
		while(fgets(linea, sizeof(linea), archivo)) {
			char linea_copia[LONGITUD_MAX];
			strcpy(linea_copia, linea);
			eliminar_salto_linea(linea_copia);
			
			char *usr = strtok(linea_copia, ",");
			char *pwd = strtok(NULL, ",");
			char *rol = strtok(NULL, ",");
			
			if (usr && strcmp(usr, usuario_actual->nombre_usuario) == 0) {
				// Reescribimos los datos nuevos para este usuario
				fprintf(temp, "%s,%s,%s\n", nuevo_usuario, nueva_contrasena, rol);
			} else if (usr) {
				// Copiamos a los demas usuarios tal cual
				fprintf(temp, "%s", linea);
			}
		}
		fclose(archivo);
		fclose(temp);
		remove(ARCHIVO_DB_USUARIOS);
		rename("temp_usr.txt", ARCHIVO_DB_USUARIOS);
		
		// 2. Actualizar archivo de estadisticas si el nombre cambio
		if (strcmp(usuario_actual->nombre_usuario, nuevo_usuario) != 0) {
			FILE *archivo_est = fopen(ARCHIVO_DB_ESTADISTICAS, "r");
			FILE *temp_est = fopen("temp_estadisticas_rename.txt", "w");
			if (archivo_est && temp_est) {
				char linea_est[LONGITUD_MAX];
				while(fgets(linea_est, sizeof(linea_est), archivo_est)) {
					char linea_est_copia[LONGITUD_MAX];
					strcpy(linea_est_copia, linea_est);
					eliminar_salto_linea(linea_est_copia);
					
					char *usr_est = strtok(linea_est_copia, ",");
					char *resto = strtok(NULL, ""); // Capturamos (q,c,i)
					
					if (usr_est && strcmp(usr_est, usuario_actual->nombre_usuario) == 0) {
						fprintf(temp_est, "%s,%s\n", nuevo_usuario, resto);
					} else if (usr_est) {
						fprintf(temp_est, "%s", linea_est);
					}
				}
				fclose(archivo_est);
				fclose(temp_est);
				remove(ARCHIVO_DB_ESTADISTICAS);
				rename("temp_estadisticas_rename.txt", ARCHIVO_DB_ESTADISTICAS);
			}
		}
		
		// 3. Actualizamos la sesion activa en memoria
		strcpy(usuario_actual->nombre_usuario, nuevo_usuario);
		strcpy(usuario_actual->contrasena, nueva_contrasena);
		
		printf("\n¡Credenciales actualizadas exitosamente!\n");
	} else {
		printf("\nError al acceder a la base de datos de usuarios.\n");
	}
	pausar_pantalla();
}

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
* Modulo compartido (Admin/Estudiante) para ingresar preguntas al archivo txt.
* Utiliza modo append ("a") para persistencia de datos segura.
*/

void agregar_acertijo(int es_admin) {
	int tema, correcta;
	char pregunta[LONGITUD_MAX], o1[100], o2[100], o3[100], o4[100];
	
	system("cls");
	if(es_admin) {
		printf("Admin\n");
		printf("Opcion [1]: Agregar acertijo\n\n");
	} else {
		printf("Usuario\n");
		printf("Opcion [2]: Registrar nuevo cuestionario\n\n");
	}
	
	while(1) {
		printf("Ingrese tema relacionado a su pregunta:\n");
		printf("[1] Tipos de ciclos\n");
		printf("[2] Tipos de variables\n");
		printf("[3] Librerias\n");
		printf("[4] Funciones\n");
		printf("[5] Apuntadores\n");
		printf("Seleccion: ");
		tema = leer_entero();
		if(tema >= 1 && tema <= 5) break;
		printf("\nTema invalido. Intente de nuevo.\n\n");
	}
	
	printf("\nIngrese pregunta o acertijo:\n");
	fgets(pregunta, sizeof(pregunta), stdin); eliminar_salto_linea(pregunta);
	
	printf("Ingrese la primera opcion de respuesta:\n");
	fgets(o1, sizeof(o1), stdin); eliminar_salto_linea(o1);
	
	printf("Ingrese la segunda opcion de respuesta:\n");
	fgets(o2, sizeof(o2), stdin); eliminar_salto_linea(o2);
	
	printf("Ingrese la tercera opcion de respuesta:\n");
	fgets(o3, sizeof(o3), stdin); eliminar_salto_linea(o3);
	
	printf("Ingrese la cuarta opcion de respuesta:\n");
	fgets(o4, sizeof(o4), stdin); eliminar_salto_linea(o4);
	
	while(1) {
		printf("\nIngrese el numero de la respuesta correcta (1-4): ");
		correcta = leer_entero();
		if(correcta >= 1 && correcta <= 4) break;
		printf("Error: La respuesta correcta debe ser un numero entre 1 y 4.\n");
	}
	
	FILE *archivo = fopen(ARCHIVO_DB_PREGUNTAS, "a");
	if(archivo) {
		fprintf(archivo, "%d|%s|%s|%s|%s|%s|%d\n", tema, pregunta, o1, o2, o3, o4, correcta);
		fclose(archivo);
		printf("\n¡Reactivo guardado con exito!\n");
	}
	
	pausar_pantalla();
}


/*
* Permite a los administradores visualizar el listado completo de reactivos
* por tema, leyendo desde banco_preguntas.txt.
*/

void consultar_reactivo() {
	int tema_buscado;
	const char *nombres_temas[] = {"", "Tipos de ciclos", "Tipos de variables", "Librerias", "Funciones", "Apuntadores"};
	
	system("cls");
	printf("Admin\n");
	printf("Opcion [2]: Ver banco de preguntas\n\n");
	
	while(1) {
		printf("Seleccionar tema a revisar:\n");
		printf("[1] Tipos de ciclos\n");
		printf("[2] Tipos de variables\n");
		printf("[3] Librerias\n");
		printf("[4] Funciones\n");
		printf("[5] Apuntadores\n");
		printf("[6] Volver al menu\n");
		printf("Seleccion: ");
		tema_buscado = leer_entero();
		
		if(tema_buscado == 6) return;
		if(tema_buscado >= 1 && tema_buscado <= 5) break;
		printf("\nTema invalido. Intente de nuevo.\n\n");
	}
	
	system("cls");
	printf("Admin\n");
	printf("--- Mostrando Banco de Preguntas: [%s] ---\n\n", nombres_temas[tema_buscado]);
	
	FILE *archivo = fopen(ARCHIVO_DB_PREGUNTAS, "r");
	if(archivo) {
		char linea[1024];
		int contador = 1;
		while(fgets(linea, sizeof(linea), archivo)) {
			eliminar_salto_linea(linea);
			char *tema_str = strtok(linea, "|");
			if(atoi(tema_str) == tema_buscado) {
				char *preg = strtok(NULL, "|");
				// Saltamos las opciones incorrectas para ir a la respuesta final
				strtok(NULL, "|"); strtok(NULL, "|"); strtok(NULL, "|"); strtok(NULL, "|");
				char *corr = strtok(NULL, "|");
				
				printf("Pregunta %d: %s\n", contador++, preg);
				printf("Respuesta correcta: %s\n\n", corr);
			}
		}
		fclose(archivo);
	}
	
	pausar_pantalla();
}



/*
* Proporciona un listado completo de cuentas de acceso,
* evaluando su flag numerico para denotar sus privilegios.
*/

void consultar_registro_usuarios() {
	system("cls");
	printf("Admin\n");
	printf("Opcion [3]: Consultar registro de usuarios\n\n");
	
	FILE *archivo = fopen(ARCHIVO_DB_USUARIOS, "r");
	if (archivo != NULL) {
		char linea[LONGITUD_MAX];
		while(fgets(linea, sizeof(linea), archivo)) {
			eliminar_salto_linea(linea);
			char linea_copia[LONGITUD_MAX];
			strcpy(linea_copia, linea);
			
			char *usr = strtok(linea_copia, ",");
			char *pwd = strtok(NULL, ","); // Omitimos imprimir pass
			char *rol = strtok(NULL, ",");
			
			if(usr && rol) {
				printf("- Usuario: %s | Rol: %s\n", usr, atoi(rol) == 1 ? "Administrador" : "Estudiante");
			}
		}
		fclose(archivo);
	}
	printf("\n");
	pausar_pantalla();
}
 

/*
* Permite cambiar los privilegios de un usuario existente buscando su
* cadena de nombre. Incorpora un bloqueo "anti-suicidio" de permisos.
*/

void gestionar_roles(Usuario admin_actual) {
	system("cls");
	printf("Admin\n");
	printf("Opcion [4]: Gestionar roles de usuarios\n\n");
	
	char objetivo[50];
	printf("Ingrese el nombre de usuario a modificar (o escriba 'salir' para cancelar): ");
	fgets(objetivo, sizeof(objetivo), stdin);
	eliminar_salto_linea(objetivo);
	
	if (strcmp(objetivo, "salir") == 0) return;
	
	// Control de seguridad, el administrador actual no se puede auto-revocar
	if (strcmp(objetivo, admin_actual.nombre_usuario) == 0) {
		printf("\nAccion denegada: No puedes revocar tus propios privilegios por seguridad.\n");
		pausar_pantalla();
		return;
	}
	
	FILE *archivo = fopen(ARCHIVO_DB_USUARIOS, "r");
	FILE *temp = fopen("temp_usr.txt", "w");
	int encontrado = 0;
	
	if (archivo && temp) {
		char linea[LONGITUD_MAX];
		while(fgets(linea, sizeof(linea), archivo)) {
			char linea_copia[LONGITUD_MAX];
			strcpy(linea_copia, linea);
			eliminar_salto_linea(linea_copia);
			
			char *usr = strtok(linea_copia, ",");
			char *pwd = strtok(NULL, ",");
			char *rol = strtok(NULL, ",");
			
			if (usr && strcmp(usr, objetivo) == 0) {
				encontrado = 1;
				// Swap logico de banderas de rol (1 -> 0 o 0 -> 1)
				int nuevo_rol = atoi(rol) == 1 ? 0 : 1;
				fprintf(temp, "%s,%s,%d\n", usr, pwd, nuevo_rol);
				printf("\nExito: El rol de '%s' ha sido actualizado a %s.\n", usr, nuevo_rol == 1 ? "Administrador" : "Estudiante");
			} else {
				fprintf(temp, "%s", linea);
			}
		}
		fclose(archivo);
		fclose(temp);
		// Persistencia definitiva reemplazando el txt local
		remove(ARCHIVO_DB_USUARIOS);
		rename("temp_usr.txt", ARCHIVO_DB_USUARIOS);
	}
	
	if (!encontrado) {
		printf("\nUsuario '%s' no encontrado en la base de datos.\n", objetivo);
		remove("temp_usr.txt");
	}
	pausar_pantalla();
}

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
// Nueva función para confirmar salida
/* -------------------------------------------------------------------------
 * AUTOR: Zavala Chable Luis Fernando (Arquitecto)
 * RESPONSABILIDAD: Motor de evaluacion, validacion matematica de respuestas,
 * logica iterativa de menus principales y entradas seguras (RF-03 y RF-04).
 * ------------------------------------------------------------------------- */
void confirmar_salida() {
    char confirmacion;
    while(1) {
        printf("\n¿Esta seguro que desea salir del programa? (S/N): ");
        confirmacion = leer_caracter();
        if (confirmacion == 'S' || confirmacion == 's') {
            printf("\nSaliendo del sistema. ¡Hasta pronto!\n");
            confirmar_salida();
        } else if (confirmacion == 'N' || confirmacion == 'n') {
            printf("Cancelando salida...\n");
            return; // Regresa a donde estaba
        } else {
            printf("Opcion invalida. Ingrese 'S' para si o 'N' para no.\n");
        }
    }
}

/*
 * Funcion principal que arranca el sistema
 * Inicializa la base de datos y lanza el menu de autenticacion
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Balam Chuc Salomon Jesus (Lider)
 * RESPONSABILIDAD: Rutinas de seguimiento de desempeno, manejo de historial,
 * uso de tokenizacion (strtok) y revision estricta del estandar snake_case (RF-05).
 * ------------------------------------------------------------------------- */
int main() {
    inicializar_archivos();
    iniciar_sistema();
    return 0;
}

/*
 * Limpia el buffer de entrada para evitar saltos de linea residuales
 * que afecten el uso de fgets o scanf posteriores.
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Lopez Ambrosio Jennifer Karina (Analista)
 * RESPONSABILIDAD: Programacion integra de la funcion de Acceso, prevencion
 * de corrupcion de datos (limpieza de buffer/saltos) y Casos de Uso (RF-01 y RF-02).
 * ------------------------------------------------------------------------- */
void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * Elimina el caracter de salto de linea '\n' al final de una cadena
 * generada por fgets, reemplazandolo por un caracter nulo.
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Lopez Ambrosio Jennifer Karina (Analista)
 * RESPONSABILIDAD: Programacion integra de la funcion de Acceso, prevencion
 * de corrupcion de datos (limpieza de buffer/saltos) y Casos de Uso (RF-01 y RF-02).
 * ------------------------------------------------------------------------- */
void eliminar_salto_linea(char *cadena) {
    size_t longitud = strlen(cadena);
    if (longitud > 0 && cadena[longitud - 1] == '\n') {
        cadena[longitud - 1] = '\0';
    }
}

/*
 * Lee una linea entera de la consola y la convierte a un numero entero.
 * Esto evita los bucles infinitos causados por scanf cuando el usuario
 * ingresa letras por accidente. A prueba de errores.
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Zavala Chable Luis Fernando (Arquitecto)
 * RESPONSABILIDAD: Motor de evaluacion, validacion matematica de respuestas,
 * logica iterativa de menus principales y entradas seguras (RF-03 y RF-04).
 * ------------------------------------------------------------------------- */
int leer_entero() {
    char buffer[50];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Si el usuario solo dio Enter, retornamos -1 (invalido)
        if (buffer[0] == '\n') return -1; 
        return atoi(buffer);
    }
    return -1;
}

/*
 * Lee un caracter de forma segura ignorando el resto de la linea.
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Zavala Chable Luis Fernando (Arquitecto)
 * RESPONSABILIDAD: Motor de evaluacion, validacion matematica de respuestas,
 * logica iterativa de menus principales y entradas seguras (RF-03 y RF-04).
 * ------------------------------------------------------------------------- */
char leer_caracter() {
    char buffer[50];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        return buffer[0];
    }
    return '\0';
}

/*
 * Detiene la ejecucion en pantalla hasta que el usuario presione Enter.
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Zavala Chable Luis Fernando (Arquitecto)
 * RESPONSABILIDAD: Motor de evaluacion, validacion matematica de respuestas,
 * logica iterativa de menus principales y entradas seguras (RF-03 y RF-04).
 * ------------------------------------------------------------------------- */
void pausar_pantalla() {
    printf("\nPresione Enter para continuar...");
    char buffer[50];
    fgets(buffer, sizeof(buffer), stdin); // Espera silenciosa
}

/*
 * Verifica la existencia de los archivos de texto locales.
 * Si no existen, los crea e inserta datos semilla por defecto, incluyendo
 * 50 preguntas pre-cargadas (10 por tema).
 */
void inicializar_archivos() {
    FILE *archivo_usuarios = fopen(ARCHIVO_DB_USUARIOS, "r");
    if (archivo_usuarios == NULL) {
        archivo_usuarios = fopen(ARCHIVO_DB_USUARIOS, "w");
        if (archivo_usuarios != NULL) {
            // Se inyecta un administrador por defecto si la BD es nueva
            fprintf(archivo_usuarios, "admin,1234,1\n"); 
            fclose(archivo_usuarios);
        }
    } else {
        fclose(archivo_usuarios);
    }

    FILE *archivo_preguntas = fopen(ARCHIVO_DB_PREGUNTAS, "r");
    if (archivo_preguntas == NULL) {
        archivo_preguntas = fopen(ARCHIVO_DB_PREGUNTAS, "w");
        if (archivo_preguntas != NULL) {
            // --- TEMA 1: Tipos de ciclos (10 preguntas) ---
            fprintf(archivo_preguntas, "1|Cual es el ciclo que se utiliza principalmente cuando se conoce de antemano el numero exacto de iteraciones?|while|for|do-while|switch|2\n");
            fprintf(archivo_preguntas, "1|Cual ciclo evalua la condicion al final, asegurando que se ejecute al menos una vez?|for|while|do-while|if|3\n");
            fprintf(archivo_preguntas, "1|Que instruccion se utiliza para salir de un ciclo prematuramente?|continue|exit|break|return|3\n");
            fprintf(archivo_preguntas, "1|Que instruccion salta el resto del cuerpo del ciclo y pasa a la siguiente iteracion?|break|continue|pass|skip|2\n");
            fprintf(archivo_preguntas, "1|En un ciclo while, si la condicion es falsa desde el principio, cuantas veces se ejecuta el cuerpo?|1|Infinitas|Depende del compilador|0|4\n");
            fprintf(archivo_preguntas, "1|Cual es la sintaxis correcta de un ciclo infinito usando for?|for()|for(;;)|for(true)|for(1)|2\n");
            fprintf(archivo_preguntas, "1|Que ciclo es ideal para leer un archivo hasta llegar al final (EOF)?|for|do-while|while|switch|3\n");
            fprintf(archivo_preguntas, "1|Un ciclo do-while siempre termina con un punto y coma (;).|Falso|Verdadero|Depende|Solo en C++|2\n");
            fprintf(archivo_preguntas, "1|Se pueden anidar ciclos en C?|No|Solo 2|Solo for|Si, sin limite teorico|4\n");
            fprintf(archivo_preguntas, "1|En un ciclo for(A; B; C), en que orden se ejecutan las partes en la primera iteracion?|A, B, C|A, B|B, C|C, A, B|2\n");

            // --- TEMA 2: Tipos de variables (10 preguntas) ---
            fprintf(archivo_preguntas, "2|Que tipo de dato se usa para almacenar enteros cortos?|int|short|long|char|2\n");
            fprintf(archivo_preguntas, "2|Cuantos bytes ocupa tipicamente un int en sistemas modernos de 32/64 bits?|2|4|8|1|2\n");
            fprintf(archivo_preguntas, "2|Que palabra clave se usa para definir un numero con decimales de doble precision?|float|real|double|decimal|3\n");
            fprintf(archivo_preguntas, "2|Que modificador hace que una variable entera solo almacene valores positivos?|positive|unsigned|signed|abs|2\n");
            fprintf(archivo_preguntas, "2|Como se declara una constante en C?|constant|#define|const|final|3\n");
            fprintf(archivo_preguntas, "2|Que tipo de dato devuelve la funcion sizeof()?|int|long|size_t|unsigned int|3\n");
            fprintf(archivo_preguntas, "2|Que valor tiene una variable local no inicializada en C?|0|Null|Basura (Indeterminado)|Error|3\n");
            fprintf(archivo_preguntas, "2|Que tipo de dato se usa para valores booleanos en C (estandar C99)?|boolean|bool|bit|logico|2\n");
            fprintf(archivo_preguntas, "2|Si sumas un int y un float, cual es el tipo del resultado?|int|float|double|error|2\n");
            fprintf(archivo_preguntas, "2|Como se declara una variable global que solo es visible en su archivo?|extern|global|static|private|3\n");

            // --- TEMA 3: Librerias (10 preguntas) ---
            fprintf(archivo_preguntas, "3|Que libreria se necesita para usar printf y scanf?|stdlib.h|stdio.h|conio.h|math.h|2\n");
            fprintf(archivo_preguntas, "3|Que libreria contiene funciones para memoria dinamica como malloc y free?|malloc.h|stdlib.h|memory.h|stdio.h|2\n");
            fprintf(archivo_preguntas, "3|Que libreria se usa para operaciones matematicas avanzadas como sin() o pow()?|math.h|calc.h|numbers.h|stdlib.h|1\n");
            fprintf(archivo_preguntas, "3|Que libreria proporciona utilidades para manipular cadenas como strlen y strcpy?|chars.h|text.h|string.h|stdio.h|3\n");
            fprintf(archivo_preguntas, "3|Que libreria se utiliza para trabajar con fechas y horas?|date.h|time.h|clock.h|datetime.h|2\n");
            fprintf(archivo_preguntas, "3|Que funcion de stdlib.h se usa para generar numeros aleatorios?|random|rand|rnd|generate|2\n");
            fprintf(archivo_preguntas, "3|Que funcion de ctype.h verifica si un caracter es un numero?|isnum|isalpha|isdigit|isnumeric|3\n");
            fprintf(archivo_preguntas, "3|Como se incluye una libreria creada por el usuario en el mismo directorio?|#include <archivo.h>|#import \"archivo.h\"|#include \"archivo.h\"|import archivo.h|3\n");
            fprintf(archivo_preguntas, "3|Que libreria estandar de C se usa para limites enteros como INT_MAX?|limits.h|math.h|stdlib.h|int.h|1\n");
            fprintf(archivo_preguntas, "3|Que macro se define en stdio.h para representar el final de un archivo?|END|NULL|EOF|-1|3\n");

            // --- TEMA 4: Funciones (10 preguntas) ---
            fprintf(archivo_preguntas, "4|Cual es la funcion principal que todo programa en C debe tener?|start|main|init|run|2\n");
            fprintf(archivo_preguntas, "4|Que palabra clave se usa para indicar que una funcion no devuelve ningun valor?|null|empty|void|nothing|3\n");
            fprintf(archivo_preguntas, "4|Como se pasa una variable a una funcion para modificar su valor original?|Por valor|Por copia|Por referencia|Por nombre|3\n");
            fprintf(archivo_preguntas, "4|Que es el prototipo de una funcion?|La definicion|La llamada|La declaracion sin cuerpo|El retorno|3\n");
            fprintf(archivo_preguntas, "4|Que sucede si no se especifica el tipo de retorno de una funcion en C antiguo?|Error|Asume void|Asume int|Asume char|3\n");
            fprintf(archivo_preguntas, "4|Se pueden definir funciones dentro de otras funciones en C estandar?|Si|No|Solo main|Depende|2\n");
            fprintf(archivo_preguntas, "4|Que enunciado se usa para devolver un valor desde una funcion?|send|yield|return|give|3\n");
            fprintf(archivo_preguntas, "4|Como se llama a una funcion que se llama a si misma?|Iterativa|Recursiva|Infinita|Anidada|2\n");
            fprintf(archivo_preguntas, "4|En la firma int func(char x), que tipo de dato recibe la funcion?|int|void|char|float|3\n");
            fprintf(archivo_preguntas, "4|Que indica la palabra extern al declarar una funcion?|Es estatica|Definida en otro archivo|Es global|No tiene cuerpo|2\n");

            // --- TEMA 5: Apuntadores (10 preguntas) ---
            fprintf(archivo_preguntas, "5|Que operador se usa para obtener la direccion de memoria de una variable?|*|&|%%|#|2\n");
            fprintf(archivo_preguntas, "5|Que operador se usa para acceder al valor apuntado por un puntero?|&|*|->|@|2\n");
            fprintf(archivo_preguntas, "5|Como se declara un apuntador a un entero?|int p*;|int *p;|pointer int p;|*int p;|2\n");
            fprintf(archivo_preguntas, "5|Que valor tiene un apuntador nulo en C?|0|NULL|-1|Indefinido|2\n");
            fprintf(archivo_preguntas, "5|Si p es un puntero a int, que hace p++?|Avanza 1 byte|Avanza sizeof(int) bytes|Error|Suma 1 al valor|2\n");
            fprintf(archivo_preguntas, "5|Como se pasa un arreglo a una funcion en C?|Por valor|Como copia|Como un puntero|No se puede|3\n");
            fprintf(archivo_preguntas, "5|Que es void * en C?|Puntero nulo|Puntero generico|Puntero a funcion|Error|2\n");
            fprintf(archivo_preguntas, "5|Que significa int **p?|Doble entero|Puntero doble|Puntero a puntero int|Array 2D|3\n");
            fprintf(archivo_preguntas, "5|Como se libera la memoria reservada dinamicamente apuntada por p?|delete p;|free(p);|clear(p);|remove(p);|2\n");
            fprintf(archivo_preguntas, "5|El nombre de un arreglo funciona como un puntero constante al primer elemento.|Falso|Verdadero|Depende|Solo en char|2\n");

            fclose(archivo_preguntas);
        }
    } else {
        fclose(archivo_preguntas);
    }
    
    FILE *archivo_manual = fopen(ARCHIVO_MANUAL, "r");
    if (archivo_manual == NULL) {
        archivo_manual = fopen(ARCHIVO_MANUAL, "w");
        if (archivo_manual != NULL) {
            // Instrucciones basicas de uso del sistema
            fprintf(archivo_manual, "--- MANUAL DE USUARIO - QUIZ-CS ---\n\n");
            fprintf(archivo_manual, "1. NAVEGACION GENERAL:\n");
            fprintf(archivo_manual, "   - Utilice los numeros mostrados en los corchetes [X] para interactuar.\n");
            fprintf(archivo_manual, "   - Para opciones de retroceso o salida, utilice las letras indicadas (a, b, c).\n\n");
            fprintf(archivo_manual, "2. ROLES DE USUARIO:\n");
            fprintf(archivo_manual, "   - ESTUDIANTE: Puede seleccionar temas, contestar quizzes, proponer nuevas\n");
            fprintf(archivo_manual, "     preguntas y ver su reporte historico de desempeno.\n");
            fprintf(archivo_manual, "   - ADMINISTRADOR: Tiene control total. Puede ver el banco de preguntas completo,\n");
            fprintf(archivo_manual, "     ver usuarios registrados y gestionar permisos de otros usuarios.\n\n");
            fprintf(archivo_manual, "3. GUIA DE ERRORES COMUNES:\n");
            fprintf(archivo_manual, "   - Error de Entrada Invalida: Ocurre al ingresar una letra cuando se pide\n");
            fprintf(archivo_manual, "     un numero. El sistema no colapsara, simplemente pedira el dato nuevamente.\n");
            fprintf(archivo_manual, "   - Credenciales Incorrectas: Si olvida su usuario/contrasena, contacte al Admin.\n");
            fprintf(archivo_manual, "   - Banco de preguntas no disponible: Ocurre si el archivo .txt se borra del\n");
            fprintf(archivo_manual, "     disco duro accidentalmente. Contacte a soporte.\n\n");
            fprintf(archivo_manual, "4. REGLAS PARA AGREGAR REACTIVOS:\n");
            fprintf(archivo_manual, "   - No utilice acentos ni caracteres especiales (como la enye).\n");
            fprintf(archivo_manual, "   - El indice de la respuesta correcta DEBE ser un numero entre 1 y 4.\n");
            fclose(archivo_manual);
        }
    } else {
        fclose(archivo_manual);
    }
}

/*
 * Lee el archivo de estadisticas, busca al usuario y le suma
 * los aciertos e incorrectas de su ultima sesion. Sobrescribe usando
 * un archivo temporal.
 */
 
 /* -------------------------------------------------------------------------
 * AUTOR: Balam Chuc Salomon Jesus (Lider)
 * RESPONSABILIDAD: Rutinas de seguimiento de desempeno, manejo de historial,
 * uso de tokenizacion (strtok) y revision estricta del estandar snake_case (RF-05).
 * ------------------------------------------------------------------------- */
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
 * Imprime las opciones secundarias dinamicas en consola.
 * Retorna un entero (0, 1, 2) dependiendo de la navegacion solicitada.
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Zavala Chable Luis Fernando (Arquitecto)
 * RESPONSABILIDAD: Motor de evaluacion, validacion matematica de respuestas,
 * logica iterativa de menus principales y entradas seguras (RF-03 y RF-04).
 * ------------------------------------------------------------------------- */
int menu_secundario(int mostrar_siguiente, int mostrar_atras) {
    char opcion;
    printf("\nOtras opciones:\n");
    printf("a) Regresar al Inicio\n");
    if (mostrar_atras) {
        printf("b) Menu principal\n");
        printf("c) Atras\n");
        printf("d) Salir del programa\n");
    } else if (mostrar_siguiente) {
        printf("b) Volver al menu principal\n");
        printf("c) Siguiente pregunta\n");
        printf("d) Salir del programa\n");
    } else {
        printf("b) Salir del sistema\n");
        printf("c) Continuar\n");
    }
    
    // Validacion logica iterativa de entradas para evitar caracteres invalidos
    while(1) {
        printf("Seleccion: ");
        opcion = leer_caracter();
        
        if (opcion == 'd' || opcion == 'D' || (!mostrar_siguiente && !mostrar_atras && (opcion == 'b' || opcion == 'B'))) confirmar_salida();
        if (opcion == 'c' || opcion == 'C') return 2; 
        if (opcion == 'b' || opcion == 'B') return 1; 
        if (opcion == 'a' || opcion == 'A') return 0; 
        
        printf("Opcion invalida. Intente de nuevo.\n");
    }
    return 0;
}

/*
 * Modulo General de Acceso. 
 * Muestra el menu raiz del sistema de quizzes.
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Zavala Chable Luis Fernando (Arquitecto)
 * RESPONSABILIDAD: Motor de evaluacion, validacion matematica de respuestas,
 * logica iterativa de menus principales y entradas seguras (RF-03 y RF-04).
 * ------------------------------------------------------------------------- */
void iniciar_sistema() {
    int opcion;
    Usuario usuario_actual;
    
    do {
       system("cls");
        printf("--- QuizzC ---\n");
        printf("INICIO\n");
        printf("Bienvendi@s a QuizzC, un programa para empezar en el mundo del lenguaje C.\n");
        printf("Seleccione una opcion para continuar:\n");
        printf("[1] Iniciar sesion\n");
        printf("[2] Registrar cuenta\n");
        printf("[3] Salir del sistema\n");
        printf("Seleccion: ");
        
        opcion = leer_entero();
        
        switch (opcion) {
            case 1:
                if(iniciar_sesion(&usuario_actual)) {
                    // Redireccion por roles
                    if(usuario_actual.rol == 1) menu_administrador(usuario_actual);
                    else menu_usuario(usuario_actual);
                }
                break;
            case 2:
                registrar_cuenta();
                break;
            case 3:
                confirmar_salida();
            default:
                printf("\nOpcion invalida. Intente de nuevo.\n");
                pausar_pantalla();
                break;
        }
    } while (opcion != 3);
}

/*
 * Módulo de Autenticacion.
 * Valida de forma directa las credenciales contra la base de datos local.
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Lopez Ambrosio Jennifer Karina (Analista)
 * RESPONSABILIDAD: Programacion integra de la funcion de Acceso, prevencion
 * de corrupcion de datos (limpieza de buffer/saltos) y Casos de Uso (RF-01 y RF-02).
 * ------------------------------------------------------------------------- */
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
 /* -------------------------------------------------------------------------
 * AUTOR: Lopez Ambrosio Jennifer Karina (Analista)
 * RESPONSABILIDAD: Programacion integra de la funcion de Acceso, prevencion
 * de corrupcion de datos (limpieza de buffer/saltos) y Casos de Uso (RF-01 y RF-02).
 * ------------------------------------------------------------------------- */
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
 * Menu principal exclusivo para los usuarios con privilegios Administrativos.
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Zavala Chable Luis Fernando (Arquitecto)
 * RESPONSABILIDAD: Motor de evaluacion, validacion matematica de respuestas,
 * logica iterativa de menus principales y entradas seguras (RF-03 y RF-04).
 * ------------------------------------------------------------------------- */
void menu_administrador(Usuario usuario_actual) {
    int opcion;
    do {
        system("cls");
        printf("----- Administrador: [%s] -----\n", usuario_actual.nombre_usuario);
        printf("Menu Principal\n");
        printf("Seleccione una opcion:\n");
        printf("[1] Agregar acertijo\n");
        printf("[2] Ver banco de preguntas\n");
        printf("[3] Ver registro de usuarios\n");
        printf("[4] Gestionar roles de usuarios\n");
        printf("[5] Consultar manual de usuario\n");
        printf("[6] Modificar mis credenciales\n");
        printf("[7] Cerrar sesion\n");
        printf("[8] Salir del sistema\n");
        printf("Seleccion: ");
        
        opcion = leer_entero();
        
        switch (opcion) {
            case 1: agregar_acertijo(1); break;
            case 2: consultar_reactivo(); break;
            case 3: consultar_registro_usuarios(); break;
            case 4: gestionar_roles(usuario_actual); break;
            case 5: consultar_manual(); break;
            case 6: modificar_credenciales(&usuario_actual); break;
            case 7: return; // Cierra sesion
            case 8: confirmar_salida(); break;
            default:
                printf("\nOpcion invalida. Intente de nuevo.\n");
                pausar_pantalla();
                break;
        }
    } while (1);
}

/*
 * Menu principal exclusivo para los estudiantes.
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Zavala Chable Luis Fernando (Arquitecto)
 * RESPONSABILIDAD: Motor de evaluacion, validacion matematica de respuestas,
 * logica iterativa de menus principales y entradas seguras (RF-03 y RF-04).
 * ------------------------------------------------------------------------- */
void menu_usuario(Usuario usuario_actual) {
    int opcion;
    do {
        system("cls");
        printf("----- User: [%s] -----\n", usuario_actual.nombre_usuario);
        printf("Menu Principal\n");
        printf("Seleccione una opcion:\n");
        printf("[1] Seleccionar tema\n");
        printf("[2] Registrar nuevo cuestionario\n");
        printf("[3] Reporte de desempeno\n");
        printf("[4] Consultar manual de usuario\n");
        printf("[5] Modificar mis credenciales\n");
        printf("[6] Cerrar sesion\n");
        printf("[7] Salir del sistema\n");
        printf("Seleccion: ");
        
        opcion = leer_entero();
        
        switch (opcion) {
            case 1: seleccionar_tema(usuario_actual); break;
            case 2: agregar_acertijo(0); break;
            case 3: reporte_desempeno(usuario_actual); break;
            case 4: consultar_manual(); break;
            case 5: modificar_credenciales(&usuario_actual); break;
            case 6: return; // Cierra sesion
            case 7: confirmar_salida(); break;
            default:
                printf("\nOpcion invalida. Intente de nuevo.\n");
                pausar_pantalla();
                break;
        }
    } while (1);
}

/*
 * Modulo compartido (Admin/Estudiante) para ingresar preguntas al archivo txt.
 * Utiliza modo append ("a") para persistencia de datos segura.
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Mendez Baas Angel Said (Desarrollador)
 * RESPONSABILIDAD: Funciones operativas de Administracion, logica CRUD en
 * archivos .txt y persistencia segura en particiones de temas (RF-06 y RF-07).
 * ------------------------------------------------------------------------- */
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
 /* -------------------------------------------------------------------------
 * AUTOR: Mendez Baas Angel Said (Desarrollador)
 * RESPONSABILIDAD: Funciones operativas de Administracion, logica CRUD en
 * archivos .txt y persistencia segura en particiones de temas (RF-06 y RF-07).
 * ------------------------------------------------------------------------- */
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
 * Motor de evaluacion para el estudiante.
 * Lee preguntas, evalua la entrada e implementa retroalimentacion inmediata.
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Zavala Chable Luis Fernando (Arquitecto)
 * RESPONSABILIDAD: Motor de evaluacion, validacion matematica de respuestas,
 * logica iterativa de menus principales y entradas seguras (RF-03 y RF-04).
 * ------------------------------------------------------------------------- */
void seleccionar_tema(Usuario usuario_actual) {
    int tema_buscado;
    const char *nombres_temas[] = {"", "Tipos de ciclos", "Tipos de variables", "Librerias", "Funciones", "Apuntadores"};
    
    system("cls");
    printf("----- Opcion [1]: Seleccionar tema ---\n");
    
    while(1) {
        printf("Seleccione el tema sobre el que desea aprender hoy:\n");
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
        printf("\nOpcion invalida. Intente de nuevo.\n\n");
    }
    
    FILE *archivo = fopen(ARCHIVO_DB_PREGUNTAS, "r");
    if(archivo) {
        char linea[1024];
        int contador = 1;
        int continuar = 1;
        int aciertos_sesion = 0;
        int errores_sesion = 0;
        
        while(fgets(linea, sizeof(linea), archivo) && continuar) {
            eliminar_salto_linea(linea);
            char linea_copia[1024];
            strcpy(linea_copia, linea); 
            
            char *tema_str = strtok(linea_copia, "|");
            if(atoi(tema_str) == tema_buscado) {
                system("cls");
                printf("--- Quiz Activo: %s ---\n\n", nombres_temas[tema_buscado]);
                
                char *preg = strtok(NULL, "|");
                char *o1 = strtok(NULL, "|");
                char *o2 = strtok(NULL, "|");
                char *o3 = strtok(NULL, "|");
                char *o4 = strtok(NULL, "|");
                int corr = atoi(strtok(NULL, "|"));
                
                printf("Pregunta %d: %s\n", contador++, preg);
                printf("[1] %s\n", o1);
                printf("[2] %s\n", o2);
                printf("[3] %s\n", o3);
                printf("[4] %s\n", o4);
                
                int respuesta;
                while(1) {
                    printf("\nEntrada(respuesta): ");
                    respuesta = leer_entero();
                    if(respuesta >= 1 && respuesta <= 4) break;
                    printf("Por favor, ingrese un numero de opcion valido (1-4).\n");
                }
                
                if(respuesta == corr) {
                    printf("\nResultado: ¡Su respuesta es correcta!\n");
                    aciertos_sesion++;
                } else {
                    printf("\nResultado: Su respuesta es incorrecta, la respuesta correcta es %d\n", corr);
                    errores_sesion++;
                }
                
                int accion = menu_secundario(1, 0);
                if(accion == 2) {
                    continuar = 1; // Siguiente pregunta
                } else {
                    continuar = 0;
                    actualizar_estadisticas(usuario_actual.nombre_usuario, aciertos_sesion, errores_sesion);
                    fclose(archivo);
                    if (accion == 1) return; // Volver al menu principal
                    if (accion == 0) confirmar_salida(); // Forzar reinicio si pide inicio
                }
            }
        }
        fclose(archivo);
        
        if (continuar == 1) {
            actualizar_estadisticas(usuario_actual.nombre_usuario, aciertos_sesion, errores_sesion);
            printf("\n¡Felicidades! Has respondido todas las preguntas disponibles de este tema.\n");
            pausar_pantalla();
        }

    } else {
        printf("\nEl banco de preguntas no esta disponible.\n");
        pausar_pantalla();
    }
}

/*
 * Lee los datos consolidados en estadisticas.txt e imprime
 * la sumatoria total del historial del usuario actual.
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Balam Chuc Salomon Jesus (Lider)
 * RESPONSABILIDAD: Rutinas de seguimiento de desempeno, manejo de historial,
 * uso de tokenizacion (strtok) y revision estricta del estandar snake_case (RF-05).
 * ------------------------------------------------------------------------- */
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

/*
 * Proporciona un listado completo de cuentas de acceso,
 * evaluando su flag numerico para denotar sus privilegios.
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Mendez Baas Angel Said (Desarrollador)
 * RESPONSABILIDAD: Funciones operativas de Administracion, logica CRUD en
 * archivos .txt y persistencia segura en particiones de temas (RF-06 y RF-07).
 * ------------------------------------------------------------------------- */
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
 /* -------------------------------------------------------------------------
 * AUTOR: Mendez Baas Angel Said (Desarrollador)
 * RESPONSABILIDAD: Funciones operativas de Administracion, logica CRUD en
 * archivos .txt y persistencia segura en particiones de temas (RF-06 y RF-07).
 * ------------------------------------------------------------------------- */
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

/*
 * Permite al usuario actual modificar su nombre de usuario y contrasena.
 * Actualiza los archivos usuarios.txt y estadisticas.txt
 */
 /* -------------------------------------------------------------------------
 * AUTOR: Lopez Ambrosio Jennifer Karina (Analista)
 * RESPONSABILIDAD: Programacion integra de la funcion de Acceso, prevencion
 * de corrupcion de datos (limpieza de buffer/saltos) y Casos de Uso (RF-01 y RF-02).
 * ------------------------------------------------------------------------- */
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

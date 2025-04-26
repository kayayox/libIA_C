/** Añadir protección para multi-inclusión en estructuras*/
#pragma once
#ifndef IA_C_H_INCLUDED
#define IA_C_H_INCLUDED
/**
 * @file IA_C.h
 * @brief API pública para integración con GUI
 *
 * Funciones seguras para hilos:
 * - tsInsertarAppend()
 * - tsBuscarEnLista()
 * - lista_operacion_segura()
 *
 * Funciones que requieren configuración previa:
 * - inicia_BD_Variant() (necesita DATA_BASE_PATH)
 * - guardar_Datos_variant()
 *
 * Callbacks necesarios:
 * - set_event_handler()
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <ctype.h>
#include <stddef.h>
#include <limits.h>
#include <curl/curl.h>
#include <math.h>
#include <locale.h>
#include <time.h>

#define MAX_DATA 1024
#define MAX_TOKEN 512
#define WORD_LENGTH 22
#define MAX_VARIANTES 16
#define MAX_FILENAME 128
#define MAX_BLOQUES 15
#define NEURAL_NET_SIZE 32
#define VARIANT_INIT {.type=TYPE_UNKNOWN,.value={0},.certeza=1.0}


#ifndef GUI_OUTPUT
#define print_debug(...) printf(__VA_ARGS__)
#else
#define print_debug(...) (void)0
#endif

/**contendra todos los enum necesarios para la toma de decisiones*/
///en el analisis de datos desde y hacia la BDLP,BDT,BDTT,y BDD.

typedef enum _bool{
    _false, ///coinsidencia total
    _true,  ///coinsidencia nula
    _duda   ///coinsidencia parcial
}_bool;

typedef enum{
    DECISION_SI,    ///100% si
    DECISION_NO,    ///100% no
    DECISION_DUDA,  ///entre 50% y 70%
    DECISION_RAND,  ///solo si esta en el rango anterior
    DECISION_LEARN  ///desconocido o nuevo enfoque
}DecisionState;

typedef enum{
    ALTERNATIVA_1,    ///primera alternativa de la lista
    ALTERNATIVA_2,    ///segunda alternativa de la lista
    ALTERNATIVA_3,    ///tercera alternativa de la lista
    ALTERNATIVA_4,    ///cuarta alternativa de la lista
    ALTERNATIVA_5     ///quinta alternativa de la lista
}Alternativa;


/**Definicion de tipos para tokenizacion de texto plano*/
typedef enum{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_LONG,
    TYPE_LONG_LONG,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_UNKNOWN
}DataType;

typedef union{
    int intValue;
    float floatValue;
    double doubleValue;
    long longValue;
    long long longLongValue;
    char charValue;
    char stringValue[MAX_DATA+1];
}DataValue;
/**
    *@struct Variant
    *@brief Almacena un valor de tipo variable(int,float,string,etc)
    */
typedef struct{
    DataType type;  ///<Tipo del dato almacenado
    DataValue value;///<Valor del dato
}Variant;
/**Estructura de control para tokens*/
typedef struct Word_couter{
    char arr_string[MAX_DATA][WORD_LENGTH];
    int num_caracteres;///cantidad de caracteres en el texto plano
    int num_letras;///cantidad de letras {A-Z,a-z} o numeros{0-9} incluidos decimales
    int num_palabras;///numero de tokens
    int letras_por_palabra_x[MAX_TOKEN];///caracteres por token aumulativo
    int letras_por_palabra_y[MAX_TOKEN];///caracteres por token definido
}Word_couter;

///funcion que recibe la entrada en texto plano
size_t recibe_Dato(Word_couter* cp,const char* archivo);
size_t tokenizar(Word_couter* cp,const char* input);
///funcion que reconoce cada token por separado y los clasifica
void reconocer(Variant* variant,const char* input);
///imprime el contenido de una variable Variant
void imprime(Variant* variant);
///comparador de variables del tipo Variant
_bool Variant_Comparator(Variant* var1,Variant* var2);
///Inicializa a 0 todas las variables de la estructura
void inicia_Word_couter(Word_couter* cp);

/**
*@struct Var
*@brief Nodo de lista doble que almacena un Variant y punteros al siguiente/anterior*/

typedef struct Var{
    Variant variable;
    struct Var* sig;
    struct Var* prev;
}Var;
typedef struct{
    Var* cabeza;
    Var* cola;
    size_t contador;
}Lista_Var;

///crea una variable clasificada
Var* crear_Var(Variant variable);
///inicializa la lista vacia de Variant
void inic_Lista_Doble(Lista_Var* lista_D);
///inserta una nueva variable en la lista
void insertar_append_Doble(Lista_Var* lista_D,Variant variable);
///busca una variable especifica en la lista
_bool buscar_en_lista_Doble(Lista_Var* listaActual,Variant* variable);
///elimina una variable de la lista
_bool eliminar_Variant(Lista_Var* lista_D,Variant* temp);
///guarda la lista en un archivo binario
///se usara luego para contexto
void guardar_Datos_variant(Lista_Var* datos);
///carga la lista desde el archivo binario
///tambien servira para contexto
void cargar_Datos_variant(Lista_Var* datos);

typedef struct {
    Lista_Var* lista;
    pthread_mutex_t mutex;
} ThreadSafeList;

void initThreadSafeList(ThreadSafeList* tsList, Lista_Var* lista);
void tsInsertarAppend(ThreadSafeList* tsList, Variant variable);
_bool tsBuscarEnLista(ThreadSafeList* tsList, Variant* variable);

///Guarda una Variant por tipo en su archivo respectivo
void guardar(Variant* variant);
///guarda tokens extendidos, solo separados por signos de puntuacion
void guardar_tokens(char arr_tokens[MAX_DATA][MAX_TOKEN],size_t len);
/**Cargan los datos en un array respectivamente por tipo de dato*/
size_t cargar_int(int* arr_int);
size_t cargar_float(float* arr_float);
size_t cargar_double(double* arr_double);
size_t cargar_char(char* arr_char);
size_t cargar_string(char arr_string[MAX_DATA][WORD_LENGTH]);
///borra el contenido del archivo binario temporal
void free_archivo_variant(int flag);

_bool isInputSafe(const char* input);

/**calificador de tipo para analisis comparativo*/
///define los articulos
typedef enum Tipo_cant{
    SING,
    PLUR,
    NONE
}Tipo_cant;
///define los verbos
typedef enum Tipo_tiempo{
    PASS,
    PRES,
    FUTR,
    INDETERMINADO
}Tipo_tiempo;
///califica el grado de la palabra o la oracion
typedef enum Tipo_grado{
    COMPARA,
    SUPERLA,
    POSIT,
    INTENS,
    INTERRG,
    NEGAT,
    RELAT,
    CUANTI,
    NON
}Tipo_grado;
///califica el tipo de persona de la que se habla
typedef enum Tipo_persona{
    PRIM,
    SEGU,
    TERC,
    NIN
}Tipo_persona;
///califica el genero de la palabra si lo tiene
typedef enum Tipo_genero{
    MASC,
    FEME,
    NEUT
}Tipo_genero;
///clasifica las palabras
typedef enum Tipo_palabra{
    PRON,
    ART,
    ADJT,
    SUST,
    VERB,
    PREG,
    ADV,
    SENS,
    PREP,
    POSE,
    RELT,
    NUM,
    CONJ,
    CONT,
    CUANT,
    DEMS,
    INDEFINIDO
}Tipo_palabra;
///calificador de tiempo verbal para analisis comparativo
typedef enum Tiempo_oracion{
    PASADO,
    PRESENTE,
    FUTURO,
    INFINITIVO
}Tiempo_oracion;
///estructura de datos ampliable para analisis de cada palabra
typedef struct Wd{
    char palabra[WORD_LENGTH];
    char significado[256];
    int num_relasionadas;
    int num_variantes;
    char variantes[MAX_VARIANTES][WORD_LENGTH];
    char relasionadas[MAX_VARIANTES][WORD_LENGTH];
    double valor_relasion[MAX_VARIANTES];
    Tipo_palabra Tipo;
    Tipo_cant Cant;
    Tipo_tiempo Tiempo;
    Tipo_genero Gen;
    Tipo_grado Grado;
    Tipo_persona Per;
}Wd;
///nodo para fraces
typedef struct Word{
    Wd palabra;
    struct Word* sig;
    struct Word* pre;
}Word;
///lista doble para contener fraces
typedef struct{
    Word* inicio;
    Word* cola;
}Frace;
///crea una nueva palabra para su analisis
Wd* crea_palabra(char* pala);
///asigna los elementos relevantes de dicha palabra
void inic_Wd(Wd* nuevaPalabra);
void asigna_palabra(Wd* nuevaPalabra,char* significado);
void asignar_Tipo_A(Wd* nueva,const char* significado);
void asignar_typo_B(Wd* nueva,const char* significado);
_bool tiene_terminacion(const char* palabra,const char* terminacion[],int num_terminacion);
_bool es_sustantivo(const char* palabra);
_bool es_verbo(const char* palabra);
_bool es_adjetivo(const char* palabra);
_bool es_adverbio(const char* palabra);
_bool es_plural(const char* palabra);
_bool es_genero_femenino(const char* palabra);
_bool es_genero_masculino(const char* palabra);
Tipo_tiempo es_tiempo(const char* palabra);
void asignar_Tipo(Wd* nueva);
///devuelven un string con el respectivo tipo
const char* tipo_a_texto(Tipo_palabra tipo);
const char* tiempo_a_texto(Tipo_tiempo tiempo);
const char* genero_a_texto(Tipo_genero genero);
const char* grado_a_texto(Tipo_grado grado);
///busca el archivo individual de cada palabra en BDLP
_bool buscar_palabra(Wd* nuevaPalabra);
///asigna variantes relacionales por contexto
void asignar_variante_relacional(Wd* nuevaPalabra,Word* temp);
///crea un nodo de la lista doble Frace
Word* crear_Word(Wd nuevaPalabra);
///inicializa la lista (lista vacia)
void inic_Frace_Word(Frace* frace);
///agrega nodos al final de la lista
void insertar_append_Frace(Frace* frace,Wd nuevaPalabra);
///guarda la frace en un archivo unico de BDT
void guardar_Frace_respuesta(Frace* frace,const char* filename);
///carga el archivo para su analisis y posterior incersion en BDLP
void cargar_Frace_respuesta(Frace* frace,const char* filename);
///metodo de eleccion primaria de BDT
_bool eleccion_primaria();
///eleccion aleatoria,caotica,para compensar "_duda"
DecisionState eleccion_secundaria(_bool eleccion,Wd* palabra_clave);
///Definicion de alternativa para switch de opciones
///de procesamiento BDT
Alternativa definicion_primaria(DecisionState* desicion);
DecisionState tomar_decision_aprendizaje(Wd* palabra);
_bool inicia_BD_Variant(Word_couter* cp,char* entrada);
void generator_BDT(Frace* frace_actual);
/**Estructuras para el manejo de oraciones, patrones,
relaciones entre oraciones dentro de una misma idea,
creacion de ideas BDTT y BDLP*/

///patron tipo en la sentencia(oracion)
typedef struct SENS_T{
    Tipo_palabra A[MAX_BLOQUES];
}SENS_T;
///bloque fundamental de analisis
typedef struct Bloque{
    char block[WORD_LENGTH];
    Tipo_palabra typo_b;
}Bloque;
///contiene la oracion, el tiempo verbal y el patron
typedef struct Oracion{
    Bloque b[MAX_BLOQUES];///tamaño maximo de oracion 15 palabras
    Tiempo_oracion tim;///Tiempoverbal de la oracion
    Bloque clave;///palabra principal del contexto de la oracion(sujeto o verbo)
    SENS_T Typos;///patron de tipos
    float frecuencia;///veces que se usa este patron
    int num_bloques;///cantidad de palabras en la oracion
}Oracion;
///estructura relacional en el texto(parrafo)
typedef struct Sentencia{
    Oracion idea;
    struct Sentencia* izq;///no contiene sustantivo
    struct Sentencia* der;///contiene sustantivo
    struct Sentencia* bro;///contiene el mismo sustantivo de origen
    struct Sentencia* raiz_orig;///origende la oracion actual
    struct Sentencia* raiz_pri;///origen del parrafo
}Sentencia;
///cadena que une indistintamente el parrafo oracion por oracion
typedef struct{
    Sentencia oracion_actual;
    Sentencia* sig;
}Texto;
///Genera una oracion para estructura sentencia
Oracion* cargar_Oracion_de_Frace(Frace* fraceActual);
///Inicializa una Sentencia con una oOracion
Sentencia* inic_vinculo_oracion(Oracion* oracionActual);
///Crea los siguientes vinculos(insercion de nodos)
Sentencia* crea_vinculo_oracion(Oracion* oracionActual,Sentencia* anterior);
///guarda una oracion
void guardar_Oracion(Oracion* actual);
///Guarda cada oracion dependiendo de su sustantivo
void guardar_Sentencia(Sentencia* actual);
///Guarda cada oracion dependiendo de su sustantivo
void guardar_Sentencia(Sentencia* raiz);
///Carga una oracion dependiendo de un sustantivo clave
void cargar_Sentencia(Sentencia** actual,Wd* clave);

/**Identifica el tipo de patron para coincidir
con la oracion de ingreso*/
typedef enum Tipo_patron{
    AFIRMACION_SIMP,
    AFIRMACION_COMP,
    NEGACION_SIMP,
    NEGACION_COMP,
    PREGUNTA_SIMP,
    PREGUNTA_COMP,
    MIXTO,
    SENTENCIAS
}Tipo_patron;
///estructura que guarda cada patron
///estos seran algunos precargados y otros
///se estableceran por analisis
typedef struct Patron{
    int p[MAX_VARIANTES];
    Tipo_patron patern;
}Patron;
typedef struct{
    Patron patron;
    float efectividad;
    int usos_exit;
    int usos_total;
}PatronAprendido;
///lista para guardadr y cargar los patrones desde archivo
typedef struct Patrones{
    Patron actual;
    struct Patrones* sig;
}Patrones;
///estructura de control de dialogo
typedef struct Dialogo{
    Oracion* premisa;///existente real valida y usada por alguna de las partes
    Oracion* hipotesis;///nueva creada a partir del random el patron inicial y la palabra clave de la anterior
    Patron* patronUsed;///patron usado en premisa
    PatronAprendido* generado;///patron creado a partir del anterior y el contexto
    float creatividad;///valor de la validez de la hipotesis
    struct Dialogo* sig;
}Dialogo;

typedef struct{
    Dialogo* historico;///lista de dialogos validos o no(sera depurada antes de guardar)
    unsigned int num_dialogos;///cantidad de dialogos almacenados
    float umbral_creatividad;///resultante media de los dialogos(si es bajo se depuran los dialogos)
}BDLP_Struct;

typedef struct {
    float weights[NEURAL_NET_SIZE][NEURAL_NET_SIZE];
    float biases[NEURAL_NET_SIZE];
} SimpleNeuralNet;

DecisionState neuralDecision(Wd* palabra, SimpleNeuralNet* net);
void trainNeuralNet(SimpleNeuralNet* net, Lista_Var* trainingData);
///generaran las listas de palabras a usar en el pensamiento
///obtenidas desde BDT de manera aleatoria
void genera_lista_Rand(Frace* fraceRand);
///genera un array con varias fraces con ekl mismo sustantivo
///con diferentes patrones
void genera_lista_Fraces(Frace* fraces,char arr_Fraces[MAX_VARIANTES][MAX_TOKEN]);
///crea un nodo del tipo Patron
Patrones* crear_Nodo(Patron p);
///carga una lista de patrones desde archivo
void cargar_patron(Patrones* p);
///guarda la lista de patrones en archivo
void guardar_patron(Patrones* actual);
///inserta un nuevo patron en la lista
void insertar_patron(Patrones** lista,Patron* actual);
///selecciona un patron a usar
void seleccionar_patron(Patron* actual);

/// IA_conector con Api
typedef struct {
    char url[MAX_DATA];
    char response[MAX_DATA*4];
} APIConnector;

/// Funciones nuevas
void initAPIConnector(APIConnector* connector, const char* api_url);
_bool fetchAPIData(APIConnector* connector, const char* params);
void parseAPIResponse(APIConnector* connector, Lista_Var* outputList);

///calbacks para GUI
typedef void (*EventCallback)(int event_type, const char* message, void* user_data);

typedef struct {
    EventCallback callback;
    void* user_data;
} EventHandler;

void set_event_handler(EventHandler* handler);

#endif /// IA_C_H_INCLUDED

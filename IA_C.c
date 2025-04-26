#include "IA_C.h"

const char* DATA_BASE_PATH = NULL;

/**recepcion de datos por teclado (texto plano)
o por archivo mientras el texto se pace por puntero char*/
size_t recibe_Dato(Word_couter* cp,const char* archivo){
    int i,j=0,k=0,aux=0;
    char argv1[1024];
    cp->num_caracteres=strlen(archivo);
///tokenizasion del texto ingrezado
    for(i=0;i<cp->num_caracteres;i++){
        if(isalpha(archivo[i]) || isalnum(archivo[i]) || archivo[i]=='.' || archivo[i]==','){
            argv1[cp->num_letras]=archivo[i];
            cp->num_letras++;
        }
        else {
            cp->num_palabras++;
        }cp->letras_por_palabra_x[cp->num_palabras]=cp->num_letras;
    }
///calcula el tamaño de cada bloque
    for(i=1;i<=cp->num_palabras;i++){
        aux+=cp->letras_por_palabra_x[i-1];
        cp->letras_por_palabra_x[i]=cp->letras_por_palabra_x[i]-aux;
    }
    for(i=0;i<=cp->num_palabras;i++){
        if(cp->letras_por_palabra_x[i]==0){
            cp->letras_por_palabra_x[i];
        }else{
            cp->letras_por_palabra_y[j]=cp->letras_por_palabra_x[i];
            j++;
        }
    }
///ingresa cada caracter en su respectivo bloque
    for(i=0;i<cp->num_palabras+1;i++){
        for(int j=0;j<cp->letras_por_palabra_y[i];j++){
            cp->arr_string[i][j]=argv1[k];
            k++;
        }
    }
    for(i=0;i<cp->num_palabras+1;i++){
        for(int j=0;j<cp->letras_por_palabra_y[i];j++){///para verificar en pruebas
            ///printf("%c",arr_string[i][j]);
        }///printf("\n");
    }
    return cp->num_palabras+1;
}
///tokeniza por conjunto separado por ",.:;="
size_t tokenizar(Word_couter* cp,const char* input){
    int i,j=0,k=0,aux=0;
    char argv1[1024];
    cp->num_caracteres=strlen(input);
    for(i=0;i<cp->num_caracteres;i++){
        if((isalpha(input[i]) || isalnum(input[i])) || input[i]==' ' || ((isalnum(input[i+1]) && isalnum(input[i-1])) && (input[i]=='.'))){
            argv1[cp->num_letras]=input[i];
            cp->num_letras++;
        }
        else {
            cp->num_palabras++;
        }cp->letras_por_palabra_x[cp->num_palabras]=cp->num_letras;
    }
///calcula el tamaño de cada bloque
    for(i=1;i<=cp->num_palabras;i++){
        aux+=cp->letras_por_palabra_x[i-1];
        cp->letras_por_palabra_x[i]=cp->letras_por_palabra_x[i]-aux;
    }
    for(i=0;i<=cp->num_palabras;i++){
        if(cp->letras_por_palabra_x[i]==0){
            cp->letras_por_palabra_x[i];
        }else{
            cp->letras_por_palabra_y[j]=cp->letras_por_palabra_x[i];
            j++;
        }
    }
///ingresa cada caracter en su respectivo bloque
    for(i=0;i<cp->num_palabras+1;i++){
        for(int j=0;j<cp->letras_por_palabra_y[i];j++){
            cp->arr_string[i][j]=argv1[k];
            k++;
        }
    }
    for(i=0;i<cp->num_palabras+1;i++){
        for(int j=0;j<cp->letras_por_palabra_y[i];j++){///para verificar en pruebas
            ///printf("%c",arr_string[i][j]);
        }///printf("\n");
    }
    return cp->num_palabras+1;
}

_bool isInputSafe(const char* input) {
    const char* blacklist[] = {";", "\"", "\'", "\\", "|", "&", "$", ">", "<", "`"};

    for(size_t i=0;i<sizeof(blacklist)/sizeof(blacklist[0]);i++){
        if(strstr(input,blacklist[i])){
            return _false;
        }
    }

    // Validar longitud máxima usando constante existente
    if(strlen(input) > MAX_DATA) {
        return _false;
    }

    return _true;
}

///Etapa de clasificacion de datos int,float,double,char,string
///seran mas en el futuro(long,long long,etc)->listo
void reconocer(Variant* variant,const char* ptinput){
    if(!variant || !ptinput)return;
    char* endPtr;
    size_t j=0;
    char input[MAX_DATA+1];
    strncpy(input,ptinput,MAX_DATA);
    input[MAX_DATA]='\0';
    size_t n=strlen(input);
    ///verifica si todos los caracteres del bloque son letras
    for(size_t i=0;i<n;i++){
        if(isalpha(input[i])){
            j+=0;
        }
        else{
            j+=1;
        }
    }
    ///si es asi lo guarda como palabra
    if(j==0){
        variant->type=TYPE_STRING;
        strncpy(variant->value.stringValue,input,MAX_DATA-1);
        variant->value.stringValue[MAX_DATA-1]='\0';
    }else{///de lo contrario se empieza a clasificar en los diferentes formatos de numeros
        long long longLongValue=strtoll(input,&endPtr,10);
        if(*endPtr=='\0'){
            if(longLongValue>=INT_MIN && longLongValue<=INT_MAX){
                variant->type=TYPE_INT;
                variant->value.intValue=(int)longLongValue;
            }else if(longLongValue>=LONG_MIN && longLongValue<=LONG_MAX){
                variant->type=TYPE_LONG;
                variant->value.longValue=(long)longLongValue;
            }else{
                variant->type=TYPE_LONG_LONG;
                variant->value.longLongValue=longLongValue;
            }
            return;
        }
        float floatValue=strtof(input,&endPtr);
        if(*endPtr=='\0'){
            variant->type=TYPE_FLOAT;
            variant->value.floatValue=floatValue;
            return;
        }
        double doubleValue=strtod(input,&endPtr);
        if(*endPtr=='\0'){
            variant->type=TYPE_DOUBLE;
            variant->value.doubleValue=doubleValue;
            return;
        }
        if(strlen(input)==1){
            variant->type=TYPE_CHAR;
            variant->value.charValue=input[0];
            return;
        }
    }
    return;
}
///comparador de variable nueva typo "Variant"
_bool Variant_Comparator(Variant* var1,Variant* var2){
    if(var1->type!=var2->type)return _false;
    switch(var1->type){
        case TYPE_INT:
            return (var1->value.intValue==var2->value.intValue) ? _true : _false;
            break;
        case TYPE_FLOAT:
            return fabs(var1->value.floatValue-var2->value.floatValue)<1e-6 ? _true : _false;
            break;
        case TYPE_DOUBLE:
            return fabs(var1->value.doubleValue-var2->value.doubleValue)<1e-9 ? _true : _false;
            break;
        case TYPE_LONG:
            return (var1->value.longValue==var2->value.longValue) ? _true : _false;
            break;
        case TYPE_LONG_LONG:
            return (var1->value.longLongValue==var2->value.longLongValue) ? _true : _false;
            break;
        case TYPE_CHAR:
            return (strcmp(&var1->value.charValue,&var2->value.charValue)==0)? _true : _false;
            break;
        case TYPE_STRING:
            return (strcmp(var1->value.stringValue,var2->value.stringValue)==0) ? _true : _false;
            break;
        default:
            break;
    }
    return _false;
}
///Inicializa a 0 todas las variables de la estructura
void inicia_Word_couter(Word_couter* cp){
    for(int i=0;i<MAX_DATA;i++){
        for(int j=0;j<WORD_LENGTH;j++){
            cp->arr_string[i][j]='\0';
        }
    }
    for(int i=0;i<MAX_TOKEN;i++){
        cp->letras_por_palabra_x[i]=0;
        cp->letras_por_palabra_y[i]=0;
    }
    cp->num_caracteres=0;
    cp->num_letras=0;
    cp->num_palabras=0;
}
/**modulo para la creacion y el manejo de listas
de datos mixtas o por tipo, doblemente enlazadas,
y las diferentes funciones para su manipulacion*/
void initThreadSafeList(ThreadSafeList* tsList, Lista_Var* lista) {
    tsList->lista = lista;
    pthread_mutex_init(&tsList->mutex, NULL);
}

void tsInsertarAppend(ThreadSafeList* tsList, Variant variable) {
    pthread_mutex_lock(&tsList->mutex);
    insertar_append_Doble(tsList->lista, variable);
    pthread_mutex_unlock(&tsList->mutex);
}

_bool tsBuscarEnLista(ThreadSafeList* tsList, Variant* variable) {
    pthread_mutex_lock(&tsList->mutex);
    _bool result = buscar_en_lista_Doble(tsList->lista, variable);
    pthread_mutex_unlock(&tsList->mutex);
    return result;
}
Var* crear_Var(Variant variable){
    Var* nuevoVar=(Var*)malloc(sizeof(Var));
    if(nuevoVar==NULL){
        return NULL;
    }
    nuevoVar->variable=variable;
    nuevoVar->sig=NULL;
    nuevoVar->prev=NULL;
    return nuevoVar;
}
/// Función para inicializar la lista
void inic_Lista_Doble(Lista_Var* lista_D){
    lista_D->cabeza=NULL;
    lista_D->cola=NULL;
    lista_D->contador=0;
}
/// Función para insertar un usuario al final de la lista
void insertar_append_Doble(Lista_Var* lista_D,Variant variable){
    Var* nuevoVar=crear_Var(variable);
    if(!nuevoVar)return;
    if(lista_D->cola){
        lista_D->cola->sig=nuevoVar;
    }else{
        lista_D->cabeza=nuevoVar;
    }
    lista_D->cola=nuevoVar;
    lista_D->contador++;
}
///funcion para buscar en lista y evitar duplicados de ser necesario
_bool buscar_en_lista_Doble(Lista_Var* listaActual,Variant* variable){
    if(!listaActual || !variable)return _duda;
    Var* actual=listaActual->cabeza;
    listaActual->contador=0;
    while(actual!=NULL){
        if(Variant_Comparator(&actual->variable,variable)==_true){
            return _true;
        }
        actual=actual->sig;
    }
    return _false;
}
/// Función para eliminar un Variant de la lista
_bool eliminar_Variant(Lista_Var* lista_D,Variant* temp){
    if(lista_D->cabeza==NULL || !temp || lista_D->contador==0){
        return _duda;
    }
    Var* actual=lista_D->cabeza;
    while(actual!=NULL){
        if(Variant_Comparator(&actual->variable,temp)==_true){
            if(actual->prev!=NULL){
                actual->prev->sig=actual->sig;
            }else{
                lista_D->cabeza=actual->sig;
            }
            if(actual->sig!=NULL){
                actual->sig->prev=actual->prev;
            }else{
                lista_D->cola=actual->prev;
            }
            free(actual);
            temp=NULL;
            lista_D->contador--;
            return _true;
        }
        actual=actual->sig;
    }
    return _false;
}
///guarda una lista corrida de los ingresos del usuario
void guardar_Datos_variant(Lista_Var* datos){
    if(!DATA_BASE_PATH) return;
    char fullpath[MAX_FILENAME*2];
    snprintf(fullpath, sizeof(fullpath), "%s/.-data.dat", DATA_BASE_PATH);
    FILE* fp = fopen(fullpath, "wb");
    if(fp==NULL){
        printf("ERROR...al abrir el Archivo\n");
        return;
    }
    Var* actual=datos->cabeza;
    while(actual!=NULL){
        fwrite(&actual->variable,sizeof(Variant),1,fp);
        actual=actual->sig;
    }
    fclose(fp);
}
///recupera la lista de entrada de datos del usuario
void cargar_Datos_variant(Lista_Var* datos){
    if(!DATA_BASE_PATH) return;
    char fullpath[MAX_FILENAME*2];
    snprintf(fullpath, sizeof(fullpath), "%s/.-data.dat", DATA_BASE_PATH);
    FILE* fp = fopen(fullpath, "rb");
    if(fp==NULL){
        return;
    }
    Variant variable;
    while(fread(&variable,sizeof(Variant),1,fp)){
        insertar_append_Doble(datos,variable);
    }
    fclose(fp);
}

/**modulo para la manipulasion de Archivos BDT
//Base de Datos Temporal
//guarda cada variable reconocida en su respectivo archivo*/
void guardar(Variant* variant){
    if(!DATA_BASE_PATH) return;
    char fullpath[MAX_FILENAME*2];
    FILE* fp;
    switch(variant->type){
        case TYPE_INT:
            snprintf(fullpath, sizeof(fullpath), "%s/.temp_int", DATA_BASE_PATH);
            fp = fopen(fullpath, "ab");
            fwrite(&variant->value.intValue,sizeof(int),1,fp);
            fclose(fp);
            break;
        case TYPE_FLOAT:
            snprintf(fullpath, sizeof(fullpath), "%s/.temp_float", DATA_BASE_PATH);
            fp = fopen(fullpath, "ab");
            fwrite(&variant->value.floatValue,sizeof(float),1,fp);
            fclose(fp);
            break;
        case TYPE_DOUBLE:
            snprintf(fullpath, sizeof(fullpath), "%s/.temp_double", DATA_BASE_PATH);
            fp = fopen(fullpath, "ab");
            fwrite(&variant->value.doubleValue,sizeof(double),1,fp);
            fclose(fp);
            break;
        case TYPE_LONG:
            snprintf(fullpath, sizeof(fullpath), "%s/.temp_long", DATA_BASE_PATH);
            fp=fopen(fullpath, "ab");
            fwrite(&variant->value.longValue,sizeof(long),1,fp);
            fclose(fp);
        break;
            case TYPE_LONG_LONG:
            snprintf(fullpath, sizeof(fullpath), "%s/.temp_long_long", DATA_BASE_PATH);
            fp = fopen(fullpath, "ab");
            fwrite(&variant->value.longLongValue,sizeof(long long),1,fp);
            fclose(fp);
            break;
        case TYPE_CHAR:
            snprintf(fullpath, sizeof(fullpath), "%s/.temp_char", DATA_BASE_PATH);
            fp = fopen(fullpath, "ab");
            fwrite(&variant->value.charValue,sizeof(char),1,fp);
            fclose(fp);
            break;
        case TYPE_STRING:
            snprintf(fullpath, sizeof(fullpath), "%s/.temp_string", DATA_BASE_PATH);
            fp = fopen(fullpath, "ab");
            if(variant->value.stringValue[strlen(variant->value.stringValue)-1]=='\n')variant->value.stringValue[strlen(variant->value.stringValue)-1]='\0';
            fwrite(&variant->value.stringValue,1024*sizeof(char),1,fp);
            fclose(fp);
            break;
        default:
            break;
    }
}
///Guardar tokens de entrada
void guardar_tokens(char arr_tokens[MAX_DATA][MAX_TOKEN],size_t len){
    if(!DATA_BASE_PATH) return;
    char fullpath[MAX_FILENAME*2];
    snprintf(fullpath, sizeof(fullpath), "%s/.temp_tokens", DATA_BASE_PATH);
    FILE* fp = fopen(fullpath, "a");
    if(!fp)return;
    for(size_t i=0;i<len;i++){
        fprintf(fp,"%s|",arr_tokens[i]);
    }
    fclose(fp);
}
///cargar tokens desde archivo
void cargar_tokens(char arr_tokens[MAX_DATA][MAX_TOKEN],size_t* len){
    if(!DATA_BASE_PATH) return;
    char fullpath[MAX_FILENAME*2];
    snprintf(fullpath, sizeof(fullpath), "%s/.temp_tokens", DATA_BASE_PATH);
    FILE* fp = fopen(fullpath, "r");
    if(!fp)return;
    size_t cantidad=0;
    while(!feof(fp)){
        int contado=fgetc(fp);
        if(contado=='|')cantidad++;
    }
    fseek(fp,0,SEEK_SET);
    char leido[MAX_DATA*MAX_TOKEN];
    while(fgets(leido,MAX_DATA*MAX_TOKEN,fp)){
        char* token=strtok(leido,"|");
        strncpy(arr_tokens[(*len)++],token,strlen(token));
        arr_tokens[*len-1][strlen(arr_tokens[*len-1])+1]='\0';
        for(size_t i=0;i<(cantidad-1);i++){
            token=strtok(NULL,"|");
            strncpy(arr_tokens[(*len)++],token,strlen(token)+1);
            arr_tokens[*len-1][strlen(arr_tokens[*len-1])+1]='\0';
        }
    }
    fclose(fp);
}
///carga del archivo los dastos (int) en un array
size_t cargar_int(int* arr_int){
    int arr_temp=0;
    size_t i=0;
    if(!DATA_BASE_PATH) return;
    char fullpath[MAX_FILENAME*2];
    snprintf(fullpath, sizeof(fullpath), "%s/.temp_int", DATA_BASE_PATH);
    FILE* fp = fopen(fullpath, "rb");
    if(!fp){
        return 0;
    }
    else{
        while(fread(&arr_temp,sizeof(int),1,fp)){
            arr_int[i]=arr_temp;
            i++;
        }
        fclose(fp);
    }
    return i;
}
///carga del archivo los dastos (float) en un array
size_t cargar_float(float* arr_float){
    float arr_temp=0;
    size_t i=0;
    if(!DATA_BASE_PATH) return;
    char fullpath[MAX_FILENAME*2];
    snprintf(fullpath, sizeof(fullpath), "%s/.temp_float", DATA_BASE_PATH);
    FILE* fp = fopen(fullpath, "rb");
    if(!fp){
        return 0;
    }
    else{
        while(fread(&arr_temp,sizeof(float),1,fp)){
            arr_float[i]=arr_temp;
            i++;
        }
        fclose(fp);
    }
    return i;
}
///carga del archivo los dastos (double) en un array
size_t cargar_double(double* arr_double){
    double arr_temp=0;
    size_t i=0;
    if(!DATA_BASE_PATH) return;
    char fullpath[MAX_FILENAME*2];
    snprintf(fullpath, sizeof(fullpath), "%s/.temp_double", DATA_BASE_PATH);
    FILE* fp = fopen(fullpath, "rb");
    if(!fp){
        return 0;
    }
    else{
        while(fread(&arr_temp,sizeof(double),1,fp)){
            arr_double[i]=arr_temp;
            i++;
        }
        fclose(fp);
    }
    return i;
}
///carga del archivo los dastos (char) en un array
size_t cargar_char(char* arr_char){
    char arr_temp=0;
    size_t i=0;
    if(!DATA_BASE_PATH) return;
    char fullpath[MAX_FILENAME*2];
    snprintf(fullpath, sizeof(fullpath), "%s/.temp_char", DATA_BASE_PATH);
    FILE* fp = fopen(fullpath, "rb");
    if(!fp){
        return 0;
    }
    else{
        while(fread(&arr_temp,sizeof(char),1,fp)){
            arr_char[i]=arr_temp;
            i++;
        }
        fclose(fp);
    }
    return i;
}
size_t cargar_long(long* arr_long){
    long arr_temp=0;
    size_t i=0;
    if(!DATA_BASE_PATH) return;
    char fullpath[MAX_FILENAME*2];
    snprintf(fullpath, sizeof(fullpath), "%s/.temp_long", DATA_BASE_PATH);
    FILE* fp = fopen(fullpath, "rb");
    if(!fp){
        return 0;
    }
    else{
        while(fread(&arr_temp,sizeof(long),1,fp)){
            arr_long[i]=arr_temp;
            i++;
        }
        fclose(fp);
    }
    return i;
}
size_t cargar_long_long(long long* arr_long_long){
    long long arr_temp=0;
    size_t i=0;
    if(!DATA_BASE_PATH) return;
    char fullpath[MAX_FILENAME*2];
    snprintf(fullpath, sizeof(fullpath), "%s/.temp_long_long", DATA_BASE_PATH);
    FILE* fp = fopen(fullpath, "rb");
    if(!fp){
        return 0;
    }
    else{
        while(fread(&arr_temp,sizeof(long long),1,fp)){
            arr_long_long[i]=arr_temp;
            i++;
        }
        fclose(fp);
    }
    return i;
}
///carga del archivo los dastos (string) en un array
size_t cargar_string(char arr_string[MAX_DATA][WORD_LENGTH]){
    if(!arr_string)return 0;
    size_t i=0;
    if(!DATA_BASE_PATH) return;
    char fullpath[MAX_FILENAME*2];
    snprintf(fullpath, sizeof(fullpath), "%s/.temp_string", DATA_BASE_PATH);
    FILE* fp = fopen(fullpath, "rb");
    if(!fp){
        return 0;
    }
    else{
        Variant variable;
        variable.type=4;
        while(i<MAX_DATA && fread(&variable.value.stringValue,1024*sizeof(char),1,fp)==1){
            strncpy(arr_string[i],variable.value.stringValue,MAX_DATA);
            arr_string[i][MAX_DATA-1]='\0';
            i++;
        }fclose(fp);
    }
    return i;
}
void free_archivo_variant(int flag){
    if(!DATA_BASE_PATH) return;
    char fullpath[MAX_FILENAME*2];
    FILE* fp;
    switch(flag){
        case TYPE_INT:
            snprintf(fullpath, sizeof(fullpath), "%s/.temp_int", DATA_BASE_PATH);
            fp = fopen(fullpath, "wb");
            fclose(fp);
            break;
        case TYPE_FLOAT:
            snprintf(fullpath, sizeof(fullpath), "%s/.temp_float", DATA_BASE_PATH);
            fp = fopen(fullpath, "wb");
            fclose(fp);
            break;
        case TYPE_DOUBLE:
            snprintf(fullpath, sizeof(fullpath), "%s/.temp_double", DATA_BASE_PATH);
            fp = fopen(fullpath, "wb");
            fclose(fp);
            break;
        case TYPE_LONG:
            snprintf(fullpath, sizeof(fullpath), "%s/.temp_long", DATA_BASE_PATH);
            fp = fopen(fullpath, "wb");
            fclose(fp);
        break;
            case TYPE_LONG_LONG:
            snprintf(fullpath, sizeof(fullpath), "%s/.temp_long_long", DATA_BASE_PATH);
            fp = fopen(fullpath, "wb");
            fclose(fp);
            break;
        case TYPE_CHAR:
            snprintf(fullpath, sizeof(fullpath), "%s/.temp_char", DATA_BASE_PATH);
            fp = fopen(fullpath, "wb");
            fclose(fp);
            break;
        case TYPE_STRING:
            snprintf(fullpath, sizeof(fullpath), "%s/.temp_string", DATA_BASE_PATH);
            fp = fopen(fullpath, "wb");
            fclose(fp);
            break;
        default:
            break;
    }
}
///proximamente funciones que carguen de los archivos
///a listas de "Variant"

///Fin de las funciones de tokenizaciony almacenamiento temp BDV

/**funcion para crear una variable del tipo palabra y la manipulacion
de sus partes incluida la busqueda y almacenamiento*/
///normalizar primero
void normalizar_palabra(char* palabra){
    int i=0,j=0;
    while(palabra[i]){
        if(isalpha(palabra[i]) || palabra[i]=='\''){
            palabra[j]=tolower(palabra[i]);
            j++;
        }
        i++;
    }
    palabra[j]='\0';
}
///crea una estructura de palabra sin iniciar valores
Wd* crea_palabra(char* palabra){
    if(!palabra)return NULL;
    Wd* nueva_palabra=(Wd*)malloc(sizeof(Wd));
    if(!nueva_palabra)return NULL;
    normalizar_palabra(palabra);
    strncpy(nueva_palabra->palabra,palabra,strlen(palabra));
    if(nueva_palabra->palabra[strlen(palabra)]!='\0')nueva_palabra->palabra[strlen(palabra)]='\0';
    return nueva_palabra;
}
///funciones para inicializar elementos relevantes a dicha variable por defecto
void inic_Wd(Wd* nuevaPalabra){
    nuevaPalabra->Cant=NONE;
    nuevaPalabra->Gen=NEUT;
    nuevaPalabra->Grado=NON;
    nuevaPalabra->num_relasionadas=0;
    nuevaPalabra->Per=NIN;
    nuevaPalabra->Tiempo=INFINITIVO;
    nuevaPalabra->Tipo=INDEFINIDO;
}
///funcion generica para verificar terminaciones de palabras
_bool tiene_terminacion(const char* palabra,const char* terminacion[],int num_terminacion){
    size_t len_palabra=strlen(palabra);
    for(int i=0;i<num_terminacion;i++){
        size_t len_terminacion=strlen(terminacion[i]);

        if(len_palabra>len_terminacion){
            if(strcmp(palabra+len_palabra-len_terminacion,terminacion[i])==0){
                return _true;
            }
        }
    }
    return _false;
}
///verifica terminacion de sustantivo(ampliable y depurable aun)
_bool es_sustantivo(const char* palabra){
    const char* terminacion[]={"cion","dad","tad","sion","aje",
        "ente","ante","or","ora","umbre","ez","eza"};
    int num_terminacion=sizeof(terminacion)/sizeof(terminacion[0]);
    return tiene_terminacion(palabra,terminacion,num_terminacion);
}
///verifica terminacion de verbo (ampliable y depurable aun)
_bool es_verbo(const char* palabra){
    const char* terminacion[]={"ar","er","ir","ando","iendo",
        "aba","ia","are","ere","ire","aria","eria","iria","aste",
        "iste","io","amos","imos","aron","ieron"};
    int num_terminacion=sizeof(terminacion)/sizeof(terminacion[0]);
    return tiene_terminacion(palabra,terminacion,num_terminacion);
}
///verifica terminacion de adjetivo (ampliable y depurable aun)
_bool es_adjetivo(const char* palabra){
    const char* terminacion[]={"oso","osa","able","ible","ivo",
        "iva","al","il","ble","ar","or","ora","ante","ista",
        "ario","aria","ero","era"};
    int num_terminacion=sizeof(terminacion)/sizeof(terminacion[0]);
    return tiene_terminacion(palabra,terminacion,num_terminacion);
}
///verifica terminacion de adverbio (ampliable y depurable aun)
_bool es_adverbio(const char* palabra){
    const char* terminacion[]={"mente","ando","endo","amente"};
    int num_terminacion=sizeof(terminacion)/sizeof(terminacion[0]);
    return tiene_terminacion(palabra,terminacion,num_terminacion);
} ///verifica terminacion de plural (ampliable y depurable aun)
_bool es_plural(const char* palabra){
    const char* terminacion[]={"s","es","as","on","an"};
    int num_terminacion=sizeof(terminacion)/sizeof(terminacion[0]);
    return tiene_terminacion(palabra,terminacion,num_terminacion);
}
///verifica terminacion de genero (ampliable y depurable aun)
_bool es_genero_femenino(const char* palabra){
    const char* terminacion[]={"e","a","es","as"};
    int num_terminacion=sizeof(terminacion)/sizeof(terminacion[0]);
    return tiene_terminacion(palabra,terminacion,num_terminacion);
}
_bool es_genero_masculino(const char* palabra){
    const char* terminacion[]={"o","l","as","on","an"};
    int num_terminacion=sizeof(terminacion)/sizeof(terminacion[0]);
    return tiene_terminacion(palabra,terminacion,num_terminacion);
}
///verifica terminacion de verbo para tiempo (ampliable y depurable aun)
Tipo_tiempo es_tiempo(const char* palabra){
    const char* terminacion[]={"iste","io","imos","eros","aba","amos","ban"};
    const char* terminacion1[]={"ra","an","as","os"," "," "," "};
    for(size_t i=0;i<7;i++){
        if(strstr(palabra+strlen(palabra)-4,terminacion[i])!=NULL){
            return PASS;
        }
        else if(strcmp(palabra+strlen(palabra)-2,terminacion1[i])==0){
            return FUTR;
        }
    }
    return PRES;
}
///primera posibiledad de asignar tipo por verificacion terminal
void asignar_Tipo_A(Wd* nueva,const char* significado){
    if(es_sustantivo(nueva->palabra)==_true)nueva->Tipo=SUST;
    if(es_verbo(nueva->palabra)==_true)nueva->Tipo=VERB;
    if(es_adjetivo(nueva->palabra)==_true)nueva->Tipo=ADJT;
    if(es_adverbio(nueva->palabra)==_true)nueva->Tipo=ADV;
    switch(nueva->Tipo){
        case SUST:{
            nueva->Cant=(es_plural(nueva->palabra)==_true)? PLUR : SING;
            if(es_genero_femenino(nueva->palabra)==_true)nueva->Gen=FEME;
            else if(es_genero_masculino(nueva->palabra)==_true)nueva->Gen=MASC;
            else{nueva->Gen=NEUT;}
        }break;
        case VERB:{
            nueva->Tiempo=es_tiempo(nueva->palabra);
            nueva->Cant=(nueva->palabra[-1]=='n')?PLUR : SING;
        }break;
        case ADJT:{
            if(es_genero_femenino(nueva->palabra)==_true)nueva->Gen=FEME;
            else if(es_genero_masculino(nueva->palabra)==_true)nueva->Gen=MASC;
            else{nueva->Gen=NEUT;}
        }break;
        case ADV:
            break;
        case INDEFINIDO:
            asignar_typo_B(nueva,significado);
            break;
    }
}
///devuelve un string con el tipo de la palabra
const char* tipo_a_texto(Tipo_palabra tipo){
    static const char* tipos[]={
        "Pronombre","Articulo","Adjetivo","Sustantivo","Verbo",
        "Pregunta","Adverbio","Sentencia","Prepocision","Posesivo",
        "Relativo","Numeral","Conjuncion","Contraccion","Cuantitativo",
        "Determinante","Indefinido"
    };
    return tipos[tipo];
}
///devuelve un string con el tiempo de la palabra o oracion
const char* tiempo_a_texto(Tipo_tiempo tiempo){
    static const char* tiempos[]={
        "Pasado","Presente","Futuro","Indeterminado"
    };
    return tiempos[tiempo];
}
///devuelve un string con el genero de la palabra
const char* genero_a_texto(Tipo_genero genero){
    static const char* generos[]={"Masculino","Femenino","Neutral"};
    return generos[genero];
}
///devuelve un string con el grado de la palabra
const char* grado_a_texto(Tipo_grado grado){
    static const char* grados[]={"Comparativo","Superlativo","Positivo",
        "Intension","Interrogativo","Negativo","Relativo","Cuantitativo","None"};
        return grados[grado];
}
///segunda posibiledad de asignar tipo por verificacion terminal
void asignar_typo_B(Wd* nuevaPalabra,const char* significado){
    char clave[][22]={"pronombre","articulo","adjetivo","sustantivo",
            "verbo","pregunta","adverbio","sentencia","prepocision",
            "Posesivo","Relativo","Numeral","Conjuncion","Contraccion",
            "Cuantitativo","Demostrativo","indefinido"};
    for(int i=0;i<9;i++){
        if(strstr(significado,clave[i])!=NULL){
            nuevaPalabra->Tipo=(Tipo_palabra)i;
            if(nuevaPalabra->Tipo==ART){
                size_t len=strlen(nuevaPalabra->palabra);
                if(nuevaPalabra->palabra[len-1]=='s'){
                    nuevaPalabra->Cant=(Tipo_cant)PLUR;
                }else{
                    nuevaPalabra->Cant=(Tipo_cant)SING;
                }
            }
            if(nuevaPalabra->Tipo==VERB){
                nuevaPalabra->Tiempo=es_tiempo(nuevaPalabra->palabra);
            }
            return;
        }
    }
    nuevaPalabra->Tipo=INDEFINIDO;
}
///asigna valores a los elementos de la palabra
void asigna_palabra(Wd* nueva_palabra,char* significado){
    size_t len;
    len=strlen(significado);
    if(len>0 && significado[len-1]=='\n'){
        significado[len-1]='\0';
    }
    strncpy(nueva_palabra->significado,significado,len);
    asignar_typo_B(nueva_palabra,significado);
    asignar_Tipo_A(nueva_palabra,significado);
}
///revisar contexto y corregir de ser necesario (cuando my_bool genera DUDA)
void asignar_palabra_revision_A(Wd* nuevaPalabra,const char* contexto){
    Word_couter* cp=(Word_couter*)malloc(sizeof(Word_couter));
    size_t len;
    if(nuevaPalabra->Tipo==INDEFINIDO){
        len=recibe_Dato(cp,contexto);
        for(size_t i=0;i<len;i++){
            if(strncmp(cp->arr_string[i],nuevaPalabra->palabra,strlen(nuevaPalabra->palabra)-2)==0){
                strcpy(nuevaPalabra->variantes[nuevaPalabra->num_variantes],cp->arr_string[i]);
                nuevaPalabra->num_variantes++;
            }
        }
    }

}
///funcion para guardar la informacion de la variable en un archivo unico temporal
///para analisis y depuracion en el proceso de aprendizaje
void guardarWd(Wd* nuevapalabra){
    char filename[MAX_FILENAME];
    snprintf(filename,MAX_FILENAME,"BDT/%s",nuevapalabra->palabra);
    mkdir("BDT",0755);
    FILE* fp=fopen(filename,"wb");
    fwrite(nuevapalabra,sizeof(Wd),1,fp);
    fclose(fp);
}
///funcion para determinar la existencia de una palabra en el proceso de aprendizaje
_bool buscar_palabra(Wd* nuevaPalabra){
    if(!nuevaPalabra)return _false;
    char filename[MAX_FILENAME];
    snprintf(filename,MAX_FILENAME,"BDT/%s",nuevaPalabra->palabra);
    FILE* fp;
    DIR* dir;
    struct dirent* ent;
    if((dir=opendir("BDT"))!=NULL){
        while((ent=readdir(dir))!=NULL ){
            if(strcmp(ent->d_name,nuevaPalabra->palabra)==0){
                fp=fopen(filename,"rb");
                if(!fp){
                    closedir(dir);
                    return _false;
                }
                fread(nuevaPalabra,sizeof(Wd),1,fp);
                fclose(fp);
                return _true;
            }
        }
        closedir(dir);
    }
    return _false;
}
///proceso experimental para relacionar palabras y valorar la relacion
void asignar_variante_relacional(Wd* nuevaPalabra,Word* temp){
    size_t j=0,k=1;
    size_t n=strlen(nuevaPalabra->palabra);
    size_t n1=strlen(temp->palabra.palabra);
    size_t n2;
    double valor=0;
    if(!temp->palabra.num_relasionadas)temp->palabra.num_relasionadas=0;
    if(n>n1){
        n2=n-n1;
        for(size_t i=0;i<n1;i++){
            if((int)nuevaPalabra->palabra[i]==(int)temp->palabra.palabra[i]){
                j++;
            }else{
                k++;
            }
        }
        valor=((double)n/k)-((double)n/j);
        if(valor>((double)n1/n) && j>n2){
            strncpy(temp->palabra.relasionadas[temp->palabra.num_relasionadas],nuevaPalabra->palabra,strlen(nuevaPalabra->palabra));
            temp->palabra.valor_relasion[temp->palabra.num_relasionadas]=valor;
            temp->palabra.num_relasionadas++;
        }
    }
    if(n<n1){
        n2=n1-n;
        for(size_t i=0;i<n;i++){
            if((int)temp->palabra.palabra[i]==(int)nuevaPalabra->palabra[i]){
                j++;
            }else{
                k++;
            }
        }
        valor=((double)n1/k)-((double)n1/j);
        if(valor>((double)n/n1) && j>n2){
            strncpy(temp->palabra.relasionadas[temp->palabra.num_relasionadas],nuevaPalabra->palabra,strlen(nuevaPalabra->palabra));
            temp->palabra.valor_relasion[temp->palabra.num_relasionadas]=valor;
            temp->palabra.num_relasionadas++;
        }
    }
    if(n==n1 && strcmp(temp->palabra.palabra,nuevaPalabra->palabra)!=0){
        for(size_t i=0;i<n;i++){
            if((int)temp->palabra.palabra[i]==(int)nuevaPalabra->palabra[i]){
                j++;
            }else{
                k++;
            }
        }
        valor=((double)n/k)-((double)n/j);
        if(valor>(double)(n/(n1*k))){
            strncpy(temp->palabra.relasionadas[temp->palabra.num_relasionadas],nuevaPalabra->palabra,strlen(nuevaPalabra->palabra));
            temp->palabra.valor_relasion[temp->palabra.num_relasionadas]=valor;
            temp->palabra.num_relasionadas++;
        }
    }
}
///funcion para crear un nodo en la lista frace
Word* crear_Word(Wd nuevaPalabra){
    Word* nuevoWord=(Word*)malloc(sizeof(Word));
    if(nuevoWord==NULL){
        return NULL;
    }
    nuevoWord->palabra=nuevaPalabra;
    nuevoWord->sig = NULL;
    nuevoWord->pre = NULL;
    return nuevoWord;
}
///inicializar la lista frace vacia
void inic_Frace_Word(Frace* frace){
    frace->inicio=NULL;
    frace->cola=NULL;
}
///insertar nodos al final de la lista frace
void insertar_append_Frace(Frace* frace,Wd nuevaPalabra){
    Word* nuevoWord=crear_Word(nuevaPalabra);
    if(frace->inicio==NULL){
        frace->inicio=nuevoWord;
        frace->cola=nuevoWord;
    } else {
        frace->cola->sig=nuevoWord;
        nuevoWord->pre=frace->cola;
        frace->cola=nuevoWord;
    }
}
///guardar la informacion de la lista frace en un archivo temporal
void guardar_Frace_respuesta(Frace* frace,const char* filename){
    FILE* fp=fopen(filename,"wb");
    if(fp==NULL){
        return;
    }
    Word* actual=frace->inicio;
    while(actual!=NULL){
        fwrite(&actual->palabra,sizeof(Wd),1,fp);
        actual=actual->sig;
    }
    fclose(fp);
}
///cargar del archivo temporal la lista frace
void cargar_Frace_respuesta(Frace* frace,const char* filename){
    FILE* fp=fopen(filename,"rb");
    if(fp==NULL){
        return;
    }
    Wd nuevapalabra;
    while(fread(&nuevapalabra,sizeof(Wd),1,fp)){
        insertar_append_Frace(frace,nuevapalabra);
    }
    fclose(fp);
}
///relaciona palabras en la lista
void analizar_relacionar(Frace* frace){
    Word* actual=frace->inicio;
    while(actual!=NULL){
        Word* comparar=frace->inicio;
        while(comparar!=NULL){
            if(actual!=comparar){
                asignar_variante_relacional(&actual->palabra,comparar);
            }
            comparar=comparar->sig;
        }
        actual=actual->sig;
    }
}
/**Esta etapa esta sometida a revision*/
///metodo de eleccion primaria de BDT
_bool eleccion_primaria(){
    srand(time(NULL));
    _bool eleccion=(random()%3+1)-1;
    return eleccion;
}
///eleccion aleatoria,caotica,para compensar "_duda"
DecisionState eleccion_secundaria(_bool eleccion,Wd* palabra_clave){
    if(eleccion==_true && (palabra_clave->Tipo==SUST || palabra_clave->Tipo==VERB) && palabra_clave->num_relasionadas==0){
        return DECISION_SI;
    }
    else if(eleccion==_true && (palabra_clave->Tipo==SUST || palabra_clave->Tipo==VERB) && palabra_clave->num_relasionadas>0){
        return DECISION_NO;
    }
    else if((eleccion==_duda && (palabra_clave->Tipo==SUST || palabra_clave->Tipo==VERB) || palabra_clave->num_relasionadas>0)){
        return DECISION_DUDA;
    }
    else if(eleccion==_duda || palabra_clave->Tipo==SUST || palabra_clave->Tipo==VERB || palabra_clave->num_relasionadas==0){
        return eleccion_primaria();
    }
    else{
        return DECISION_LEARN;
    }
}
///Definicion de alternativa para switch de opciones
///de procesamiento BDT
Alternativa definicion_primaria(DecisionState* desicion){
    srand(time(NULL));
    Alternativa opcion;
    if(desicion==DECISION_SI)return ALTERNATIVA_1;
    else if(desicion==DECISION_NO)return ALTERNATIVA_2;
    else if(desicion==DECISION_DUDA){
        opcion=(random()%5+1)-1;
        return opcion;
    }
    else if(desicion==DECISION_RAND){
        opcion=(random()%3+1)-1;
        return opcion;
    }
    else{
        return ALTERNATIVA_5;
    }
}
DecisionState tomar_decision_aprendizaje(Wd* palabra){
    _bool eleccion=eleccion_primaria();
    return eleccion_secundaria(eleccion,palabra);
}
DecisionState neuralDecision(Wd* palabra, SimpleNeuralNet* net) {
    float input[NEURAL_NET_SIZE] = {0};

    // Usar valores existentes como entrada
    input[0] = (float)palabra->Tipo;
    input[1] = (float)palabra->num_relasionadas;
    // ... otros campos relevantes

    float output = 0;
    for(int i=0; i<NEURAL_NET_SIZE; i++) {
        for(int j=0; j<NEURAL_NET_SIZE; j++) {
            output += input[i] * net->weights[i][j];
        }
        output += net->biases[i];
    }

    // Mapear a DecisionState existente
    if(output > 0.8) return DECISION_SI;
    if(output < -0.8) return DECISION_NO;
    if(output > -0.2 && output < 0.2) return DECISION_DUDA;
    return DECISION_RAND;
}
/**Funciones para la manipulacion de oraciones y sus partes
aun falta estrucurar redes neuronales y ML para la valoracion, evaluacion,
almacenamiento o eliminacion*/
///Laestructura Oracion dependera de los sustantivos
Oracion* cargar_Oracion_de_Frace(Frace* fraceActual){
    if(!fraceActual)return NULL;
    Oracion* nueva=(Oracion*)malloc(sizeof(Oracion));
    if(!nueva)return NULL;
    nueva->num_bloques=0;
    nueva->tim=INDETERMINADO;
    Word* temp=fraceActual->inicio;
    while(temp!=NULL && nueva->num_bloques<MAX_BLOQUES){
        strncpy(nueva->b[nueva->num_bloques].block,temp->palabra.palabra,WORD_LENGTH-1);
        nueva->b[nueva->num_bloques].block[WORD_LENGTH-1]='\0';
        nueva->b[nueva->num_bloques].typo_b=temp->palabra.Tipo;
        nueva->Typos.A[nueva->num_bloques]=temp->palabra.Tipo;
        if(temp->palabra.Tipo==VERB){
            nueva->tim=temp->palabra.Tiempo;
        }
        if(temp->palabra.Tipo==SUST){
            strcpy(nueva->clave.block,temp->palabra.palabra);
            nueva->clave.typo_b=temp->palabra.Tipo;
            nueva->frecuencia=1.0f;
        }
        nueva->num_bloques++;
        temp=temp->sig;
    }
    if(nueva->clave.typo_b==SUST){
        return nueva;
    }else{
        for(int i=0;i<nueva->num_bloques;i++){
            if(nueva->b[i].typo_b==VERB){
                strcpy(nueva->clave.block,nueva->b[i].block);
                nueva->clave.typo_b=nueva->b[i].typo_b;
                nueva->frecuencia=1.2f;
                return nueva;
            }
        }
    }
    return nueva;
}

Sentencia* inic_vinculo_oracion(Oracion* oracionActual){
    if(!oracionActual)return NULL;
    Sentencia* nueva=(Sentencia*)malloc(sizeof(Sentencia));
    if(!nueva)return NULL;
    nueva->idea=*oracionActual;
    nueva->der=NULL;
    nueva->izq=NULL;
    nueva->bro=NULL;
    nueva->raiz_orig=nueva;
    nueva->raiz_pri=nueva;
    return nueva;
}
Sentencia* crear_vinculo_oracion(Oracion* oracionActual,Sentencia* anterior){
    if(!oracionActual || !anterior)return NULL;
    Sentencia* nueva=(Sentencia*)malloc(sizeof(Sentencia));
    if(!nueva)return NULL;
    nueva->idea=*oracionActual;
    nueva->der=NULL;
    nueva->izq=NULL;
    nueva->bro=NULL;
    nueva->raiz_orig=anterior;
    nueva->raiz_pri=anterior->raiz_pri;

    _bool sustantivo=_false;
    for(int i=0;i<oracionActual->num_bloques;i++){
        if(oracionActual->b[i].typo_b==SUST){
            sustantivo=_true;
            break;
        }
    }
    if(sustantivo==_true){
        for(int i=0;i<anterior->idea.num_bloques;i++){
            if(anterior->idea.b[i].typo_b==SUST){
                for(int j=0;j<oracionActual->num_bloques;j++){
                    if(oracionActual->b[j].typo_b==SUST && strcmp(oracionActual->b[j].block,anterior->idea.b[i].block)==0){
                        anterior->bro=nueva;
                        return nueva;
                    }
                }
            }
        }
        anterior->der=nueva;
    }else{
        anterior->izq=nueva;
    }
    return nueva;
}
///guarda una oracion en BDTT
void guardar_Oracion(Oracion* actual){
    if(!actual)return;
    FILE* fp;
    char filename[MAX_FILENAME];
    mkdir("BDTT",0755);
    sprintf(filename,"BDTT/%s_%s",actual->clave.block,tipo_a_texto(actual->clave.typo_b));
    fp=fopen(filename,"ab");
    fwrite(&actual->num_bloques,sizeof(int),1,fp);
    fwrite(&actual->frecuencia,sizeof(float),1,fp);
    fwrite(&actual->tim,sizeof(Tiempo_oracion),1,fp);
    for(int i=0;i<actual->num_bloques;i++){
        fwrite(&actual->b[i],sizeof(Bloque),1,fp);
    }fclose(fp);
}
///obtiene una oracion de BDTT
Oracion* obtener_Oracion(Wd* palabraclave){
    if(!palabraclave)return NULL;
    Oracion* tempo=(Oracion*)malloc(sizeof(Oracion));
    FILE* fp;
    char filename[MAX_FILENAME];
    sprintf(filename,"BDTT/%s_%s",palabraclave->palabra,tipo_a_texto(palabraclave->Tipo));
    fp=fopen(filename,"rb");
    if(!fp)return NULL;
    fread(&tempo->num_bloques,sizeof(int),1,fp);
    fread(&tempo->frecuencia,sizeof(float),1,fp);
    fread(&tempo->tim,sizeof(Tiempo_oracion),1,fp);
    for(int i=0;i<tempo->num_bloques;i++){
        fread(&tempo->b[i],sizeof(Bloque),1,fp);
    }fclose(fp);
    for(int i=0;i<tempo->num_bloques;i++){
        tempo->Typos.A[i]=tempo->b[i].typo_b;
        if(tempo->b[i].typo_b==3){
            strcpy(tempo->clave.block,tempo->b[i].block);
            tempo->clave.typo_b=tempo->b[i].typo_b;
        }
    }
    if(tempo->clave.typo_b==3)return tempo;
    for(int i=0;i<tempo->num_bloques;i++){
        if(tempo->b[i].typo_b==4){
            strcpy(tempo->clave.block,tempo->b[i].block);
            tempo->clave.typo_b=tempo->b[i].typo_b;
        }
    }
    return tempo;
}
///Guarda cada oracion dependiendo de su sustantivo
void guardar_Sentencia(Sentencia* raiz){
    if(!raiz)return;
    guardar_Oracion(&raiz->idea);
    guardar_Sentencia(raiz->izq);
    guardar_Sentencia(raiz->der);
}
///Carga una oracion dependiendo de un sustantivo clave
void cargar_Sentencia(Sentencia** actual,Wd* clave){
    Sentencia* temp=(Sentencia*)malloc(sizeof(Sentencia));
    if(actual==NULL){
        temp=inic_vinculo_oracion(obtener_Oracion(clave));
    }
    Oracion* corriente=obtener_Oracion(clave);
    temp=crear_vinculo_oracion(corriente,*actual);
    *actual=temp;
}
///aun en progreso,requiere del modulo pensamiento profundo
void generar_respuesta_autonoma(Frace* frace){
    Oracion* oracion=cargar_Oracion_de_Frace(frace);
    if(oracion==NULL)return;

    Sentencia* sentencia=inic_vinculo_oracion(oracion);
///falta desarrollar esta parte
    DecisionState decision=eleccion_primaria();
    switch(decision){
        case DECISION_SI:
            break;
        case DECISION_NO:
            break;
        case DECISION_DUDA:
            break;
        case DECISION_RAND:
            break;
        case DECISION_LEARN:
            break;
    }
    free(oracion);
    free(sentencia);
}
///Genera una lista de 100 palabras al azar de BDT
void genera_lista_Rand(Frace* fraceRand){
    srand(time(NULL));
    _bool activador;
    char filename[128];
    FILE* fp;
    DIR* dir;
    int i=0;
    struct dirent* env;
    if((dir=opendir("BDT"))!=NULL){
        while((env=readdir(dir))!=NULL){
            if(i>1000){
                break;
            }
            activador=(random()%3+1)-1;
            if(activador==_true){
                Wd* temp=(Wd*)malloc(sizeof(Wd));
                inic_Wd(temp);
                strcpy(temp->palabra,env->d_name);
                sprintf(filename,"BDT/%s",temp->palabra);
                fp=fopen(filename,"rb");///funcion con problemas o aun no la entiendo
                if(!fp)return;
                fread(temp,sizeof(Wd),1,fp);
                insertar_append_Frace(fraceRand,*temp);
                i++;
                fclose(fp);
            }
        }
        closedir(dir);
    }
}
///genera un array de fraces obtenido a partir de fraceRand y un patron selectivo
void genera_lista_Fraces(Frace* fraces,char arr_Fraces[MAX_VARIANTES][MAX_TOKEN]){
    genera_lista_Rand(fraces);
    Patron* p=(Patron*)malloc(sizeof(Patron));
    int i=0,j=0,k=0;
    seleccionar_patron(p);
    Word* temp=fraces->inicio;
    while(temp!=NULL){///realizar revision exaustiva a esta funcion
        if(temp->palabra.Tipo==p->p[i]){
            strcat(arr_Fraces[j][k++],&temp->palabra.palabra);
            strcat(arr_Fraces[j][k++]," ");
            i++;
        }
        temp=temp->sig;
    }
}
Patrones* crear_Nodo(Patron p){
    Patrones* nuevo_Nodo=(Patrones*)malloc(sizeof(Patrones));
    if(nuevo_Nodo==NULL){///verificacion importante
        fprintf(stderr,"ERROR: no se pudo asignar memoria\n");
        exit(EXIT_FAILURE);
    }
    nuevo_Nodo->actual=p;///se carga el valor en la variable del Nodo
    nuevo_Nodo->sig=NULL;///importante para evitar fugas
    return nuevo_Nodo;
}
void cargar_patron(Patrones* p){
    FILE* fp=fopen("BDTT/.Patrones","rb");
    if(!fp)return;
    Patron* temp=(Patron*)malloc(sizeof(Patron));
    while(fread(temp,sizeof(Patron),1,fp)){
        insertar_patron(&p,temp);
    }
    free(temp);
    fclose(fp);
}
void guardar_patron(Patrones* actual){
    FILE* fp=fopen("BDTT/.Patrones","wb");
    if(!fp)return;
    while(fwrite(&actual->actual,sizeof(Patron),1,fp)){
        actual=actual->sig;
    }
    fclose(fp);
}
void insertar_patron(Patrones** lista,Patron* actual){
    Patrones* nuevo_Nodo=crear_Nodo(*actual);
    if(*lista==NULL){
        *lista=nuevo_Nodo;
    }else{
        nuevo_Nodo->sig=*lista;
        *lista=nuevo_Nodo;
    }
}
///selecciona el patron a usar en generar_lista_fraces
///para la conversacion interna y la creacion de nuevas fraces
///de manera autonoma
void seleccionar_patron(Patron* patron){
    Patrones* ps=(Patrones*)malloc(sizeof(Patrones));
    cargar_patron(ps);
    srand(time(NULL));
    _bool activador;
    while(ps!=NULL){
        activador=(random()%5+1)-1;
        if(activador==_true){
            *patron=ps->actual;
            break;
        }
        else if(activador==_duda){
            patron->p[0]=PREG;
            patron->p[1]=ART;
            patron->p[2]=SUST;
            patron->p[3]=VERB;
            patron->patern=PREGUNTA_SIMP;
            break;
        }
        ps=ps->sig;
    }
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    APIConnector* connector = (APIConnector*)userp;
    size_t total_size = size * nmemb;
    strncat(connector->response, (char*)contents, total_size);
    return total_size;
}

void initAPIConnector(APIConnector* connector, const char* api_url) {
    strncpy(connector->url, api_url, MAX_DATA-1);
    connector->response[0] = '\0';
}

_bool fetchAPIData(APIConnector* connector, const char* params) {
    CURL* curl = curl_easy_init();
    if(!curl) return _false;

    char full_url[MAX_DATA*2];
    snprintf(full_url, sizeof(full_url), "%s?%s", connector->url, params);

    curl_easy_setopt(curl, CURLOPT_URL, full_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, connector);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return (res == CURLE_OK) ? _true : _false;
}

void parseAPIResponse(APIConnector* connector, Lista_Var* outputList) {
    Word_couter counter;
    inicia_Word_couter(&counter);

    size_t len = tokenizar(&counter, connector->response);
    for(size_t i=0; i<len; i++) {
        Variant var;
        reconocer(&var, counter.arr_string[i]);
        insertar_append_Doble(outputList, var);
    }
}

#include "IA_C.h"

_bool inicia_BD_Variant(Word_couter* cp,char* entrada){
    setlocale(LC_ALL,"es_ESUTF-8");
    Lista_Var* listaActual=(Lista_Var*)malloc(sizeof(Lista_Var));
    if(listaActual==NULL)return _false;
    size_t len_entrada=0;
    size_t len_recibe=0;
    len_entrada=strlen(entrada);
    if(len_entrada>1){
        inicia_Word_couter(cp);
        len_recibe=recibe_Dato(cp,entrada);
        for(size_t i=0;i<len_recibe;i++){
            Variant* variable=(Variant*)malloc(sizeof(Variant));
            reconocer(variable,cp->arr_string[i]);
            guardar(variable);
            insertar_append_Doble(listaActual,*variable);
        }
        guardar_Datos_variant(listaActual);
        return _true;
    }else{
        return _duda;
    }
}
void generator_BDT(Frace* frace_actual){
    char arr_string[MAX_DATA][WORD_LENGTH];
    for(int i=0;i<MAX_DATA;i++){
        for(int j=0;j<WORD_LENGTH;j++){
            arr_string[i][j]='\0';
        }
    }
    size_t len=cargar_string(arr_string);
    for(size_t i=0;i<len;i++){
        Wd temp;
        strcpy(temp.palabra,arr_string[i]);
        inic_Wd(&temp);
        insertar_append_Frace(frace_actual,temp);
    }
}

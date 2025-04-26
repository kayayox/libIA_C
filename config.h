#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include "IA_C.h"

typedef struct {
    char data_path[MAX_FILENAME];
    _bool enable_logging;
    int max_threads;
} IAC_Config;

extern IAC_Config global_config;

void init_config(const char* path, _bool logging, int threads);


#endif // CONFIG_H_INCLUDED

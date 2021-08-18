//
// Created by 12131 on 2021/7/14.
//

#ifndef C_LOGGING_ZJF_H
#define C_LOGGING_ZJF_H

#include <stdio.h>

typedef struct {
    char desc[128];
    char pos[128];
    char log[256];
    char pathname[128];
    int mode;
    int level;
} LogZJF;

//mode
//1 write mode
//0 print mode
//level
//n notice 10000
//f fatal 1000
//e error 100
//w warning 10
//d debug (other it is)

void logging_init_config(int mode, const char *level, const char *pathname);

#define log_ZJF(level, ...) logging_ZJF(\
level,\
snprintf(logging_retrieve_handle()->pos,\
sizeof(logging_retrieve_handle()->pos),\
"%d::%s::%s",__LINE__, __func__, __FILE__),\
snprintf(logging_retrieve_handle()->desc,\
sizeof(logging_retrieve_handle()->desc),\
__VA_ARGS__))


//using by log_ZJF
LogZJF *logging_retrieve_handle();
void logging_ZJF(const char *level,...);

#endif //C_LOGGING_ZJF_H

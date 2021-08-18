//
// Created by 12131 on 2021/7/14.
//

#include <time.h>
#include <string.h>
#include "logging_ZJF.h"

static void logging_write(const char *buffer, int size, const char *pathname);

static void level_value(const char *level, char **desc, int *value);

static void init_time(char *buffer, int size);

LogZJF *logging_retrieve_handle() {
    static LogZJF handle;
    return &handle;
}

void logging_init_config(int mode, const char *level, const char *pathname) {
    LogZJF *handle = logging_retrieve_handle();
    handle->mode = mode;
    level_value(level, NULL, &handle->level);
    char *buffer = handle->pathname;
    int size = sizeof(handle->pathname);
    strncpy(buffer, pathname, size);
}

void logging_ZJF(const char *level, ...) {
    char *desc, *buffer;
    int value, size;
//    init time string
    char time[24];
    init_time(time, sizeof(time));
//    filter init logging
    LogZJF *handle = logging_retrieve_handle();
    if (!handle->level) {
        printf("%s   %s   fatal: logging config is not init\n", time, handle->pos);
        return;
    }
    level_value(level, &desc, &value);
    if (handle->level > value) {
        return;
    }
//    fill log to buffer
    buffer = handle->log;
    size = sizeof(handle->log);
    size = snprintf(buffer, size, "%s   %s   %s: %s\n",
                    time, handle->pos,
                    desc, handle->desc);

//    0: print log to stdout
//    1 write log to file
    if (!handle->mode) {
        printf("%s", buffer);
    } else {
        logging_write(buffer, size, handle->pathname);
    }
}

void level_value(const char *level, char **desc, int *value) {
    if (!level) {
        level = "d";
    }
    int val;
    char *ptr;
    switch (*level) {
        case 'n':
            ptr = "notice";
            val = 10000;
            break;
        case 'f':
            ptr = "fatal";
            val = 1000;
            break;
        case 'e':
            ptr = "error";
            val = 100;
            break;
        case 'w':
            ptr = "warning";
            val = 10;
            break;
        default:
            ptr = "debug";
            val = 1;
            break;
    }
    *value = val;
    if (desc) {
        *desc = ptr;
    }
}

void init_time(char *buffer, int size) {
    time_t st;
    struct tm *info;
    time(&st);
    info = localtime(&st);
    strftime(buffer, size, "%Y-%m-%d %X", info);
}

void logging_write(const char *buffer, int size, const char *pathname) {
    FILE *file = NULL;
    const char *pattern = "ab";
    file = fopen(pathname, pattern);
    if (!file) {
        printf("%d:%s:%s   open file error: %s::%s",
               __LINE__, __func__, __FILE__,
               pattern, pathname);
        goto End;
    }
    fwrite(buffer, sizeof(char), size, file);
    if (ferror(file)) {
        printf("%d:%s:%s   write file error: %s::%s",
               __LINE__, __func__, __FILE__,
               pattern, pathname);
        goto End;
    }
    End:
    if (file) {
        fclose(file);
    }
}
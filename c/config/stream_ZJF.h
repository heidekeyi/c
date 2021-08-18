//
// Created by 12131 on 2021/7/14.
//

#ifndef C_STREAM_ZJF_H
#define C_STREAM_ZJF_H

#include "logging_ZJF.h"

int stream_init_ZJF(void **handle, void  *ini);
int stream_free_ZJF(void **handle);
int stream_read_ZJF(void *handle, char **line);
int stream_write_ZJF(void *handle, const char *buffer, size_t *size);


#endif //C_STREAM_ZJF_H

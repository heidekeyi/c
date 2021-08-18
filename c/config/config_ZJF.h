//
// Created by 12131 on 2021/7/15.
//

#ifndef C_CONFIG_ZJF_H
#define C_CONFIG_ZJF_H

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "stream_ZJF.h"
#include "logging_ZJF.h"


int config_init_ZJF(void **handle, void *ini);

int config_free_ZJF(void **handle);

int config_read_ZJF(void *handle, const char *key, char **value);

int config_write_ZJF(void *handle, const char *key, const char *value);

#endif //C_CONFIG_ZJF_H

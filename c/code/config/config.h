//
// Created by 12131 on 2021/8/1.
//

#ifndef LIB_CONFIG_H
#define LIB_CONFIG_H


int config_file_read(const char *pathname,
                     const char *key,
                     const char *value,
                     int size);

int config_file_write(const char *pathname,
                      const char *key,
                      const char *value);

#endif //LIB_CONFIG_H

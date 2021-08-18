//
// Created by 12131 on 2021/8/2.
//

#include <stdio.h>
#include "../../../log/logging.h"
#include "test_config_file.h"
#include "../config_file.h"

#define READ_BUFFER_SIZE 128

static void test_read_arg();

static void test_read();

static void test_write();

void test_config_file(void) {
//    test_read();
    test_write();
}

static void test_read_arg() {
    char buffer[READ_BUFFER_SIZE];
    char *data;
    config_file_read(NULL, NULL, NULL, 1, NULL);
    config_file_read("", NULL, NULL, 1, NULL);
    config_file_read("./appName.log", NULL, NULL, 1, NULL);
    config_file_read("./appName.log", "test", NULL, 1, NULL);
    config_file_read("./appName.log", "test", buffer, 1, NULL);
    config_file_read("./appName.log", "test", buffer, READ_BUFFER_SIZE, NULL);
    config_file_read("./appName.log", "test", buffer, READ_BUFFER_SIZE, &data);
}

static void test_read() {
    char buffer[READ_BUFFER_SIZE];
    char *data;
    const char *pathname = "./appName.log";
    char *str_arr[] = {"a", "b", "c", "d", "e"};
    int size = (int) (sizeof(str_arr) / sizeof(*str_arr));
    int code, index;
    printf("********%s********\n", LOG_FUNC_NAME);
//    test_read_arg();

    for (index = 0; index < size; ++index) {
        code = config_file_read(pathname, str_arr[index], buffer, READ_BUFFER_SIZE, &data);
        if (code) {
            printf("%d:::%s:::config_file_read("
                   "pathname, key, buffer, READ_BUFFER_SIZE, &value)\n",
                   code, LOG_FUNC_NAME);
            return;
        }
        if (data) {
            printf("%s: %s==>%s\n", LOG_FUNC_NAME, str_arr[index], data);
        } else {
            printf("%s: key = %s is not exist\n", LOG_FUNC_NAME, str_arr[index]);
        }
    }
    printf("********%s********\n", LOG_FUNC_NAME);
}

static void test_write() {
    const char *pathname = "./bak.log";
    const char *key = "hello";
    const char *value = "world";
    config_file_write(pathname, key, value);
}
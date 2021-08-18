//
// Created by 12131 on 2021/7/15.
//

#include "test_config_ZJF.h"


void test_config_init_free_ZJF() {
    void *handle = NULL;
    char *pathname = "./app.log";
    int code = config_init_ZJF(&handle, pathname);
    if (code) {
        log_ZJF("e", "init config error");
        goto End;
    }
    End:
    if (!handle) {
        return;
    }
    code = config_free_ZJF(&handle);
    if (code) {
        log_ZJF("e", "free config error");
        goto End;
    }
    log_ZJF("n", "test End");
}

void test_config_read_ZJF() {
    void *handle = NULL;
    char *pathname = "./app.log";
    int code = config_init_ZJF(&handle, pathname);
    if (code) {
        log_ZJF("e", "init config error");
        goto End;
    }
    char *keys[] = {"k1", "k4"};
    char *value;
    for (int i = 0; i < sizeof(keys) / sizeof(*keys); ++i) {
        code = config_read_ZJF(handle, keys[i], &value);
        if (code) {
            log_ZJF("e", "init config error");
            goto End;
        }
        if (value) {
            printf("val: %s\n", value);
        }
    }
    End:
    if (!handle) {
        return;
    }
    code = config_free_ZJF(&handle);
    if (code) {
        log_ZJF("e", "free config error");
        goto End;
    }
    log_ZJF("n", "test End");
}

void test_config_write_ZJF() {
    void *handle = NULL;
    char *pathname = "./app.log";
    int code = config_init_ZJF(&handle, pathname);
    if (code) {
        log_ZJF("e", "init config error");
        goto End;
    }
    char *key[] = {"hello", "c", "mouse"};
    char *val[] = {"world", "c++", "keyboard"};
    for (int i = 0; i < sizeof(key) / sizeof(*key); ++i) {
        code = config_write_ZJF(handle, key[i], val[i]);
        if (code) {
            log_ZJF("e", "write config error");
            goto End;
        }
    }
    char *keys[] = {"k1", "it", "c", "k4", "mouse"};
    char *value;
    for (int i = 0; i < sizeof(keys) / sizeof(*keys); ++i) {
        code = config_read_ZJF(handle, keys[i], &value);
        if (code) {
            log_ZJF("e", "init config error");
            goto End;
        }
        if (value) {
            printf("%s=%s\n", keys[i], value);
        }
    }
    End:
    if (!handle) {
        return;
    }
    code = config_free_ZJF(&handle);
    if (code) {
        log_ZJF("e", "free config error");
        goto End;
    }
    log_ZJF("n", "test End");
}
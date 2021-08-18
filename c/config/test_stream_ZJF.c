//
// Created by 12131 on 2021/7/14.
//

#include "test_stream_ZJF.h"


void test_stream_init_free_ZJF() {
    void *handle = NULL;
    char *pathname = "./app.log";
    int code = stream_init_ZJF(&handle, pathname);
    if (code) {
        log_ZJF("e", "init stream error");
        goto End;
    }
    End:
    if (!handle) {
        return;
    }
    code = stream_free_ZJF(&handle);
    if (code) {
        log_ZJF("e", "free stream error");
        goto End;
    }
    log_ZJF("n", "test End");
}

void test_stream_read_ZJF() {
    void *handle = NULL;
    char *pathname = "./test_stream_ZJF.c";
    int code = stream_init_ZJF(&handle, pathname);
    if (code) {
        log_ZJF("e", "init stream error");
        goto End;
    }
    char *line;
    int cnt = 0;
    while(1){
        code = stream_read_ZJF(handle, &line);
        if (code) {
            log_ZJF("e", "read stream error: %d", code);
            goto End;
        }
        if(!line){
            break;
        }
        printf("%3d %s\n", ++cnt, line);
    }
    End:
    if (!handle) {
        return;
    }
    code = stream_free_ZJF(&handle);
    if (code) {
        log_ZJF("e", "free stream error");
        goto End;
    }
    log_ZJF("n", "test End");
}

void test_stream_write_ZJF() {
    void *handle = NULL;
    char *pathname = "./app.log";
    int code = stream_init_ZJF(&handle, pathname);
    if (code) {
        log_ZJF("e", "init stream error");
        goto End;
    }
    char buffer[] = "key=value";
    size_t size;
    code = stream_write_ZJF(handle, buffer, &size);
    if (code) {
        log_ZJF("e", "init stream error");
        goto End;
    }
    log_ZJF("n", "%d", size);
    End:
    if (!handle) {
        return;
    }
    code = stream_free_ZJF(&handle);
    if (code) {
        log_ZJF("e", "free stream error");
        goto End;
    }
    log_ZJF("n", "test End");
}
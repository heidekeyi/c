//
// Created by 12131 on 2021/7/14.
//
#include <string.h>
#include <stdlib.h>
#include "stream_ZJF.h"

typedef struct {
    size_t size;
    size_t len;
    char *buffer;
} LineBuffer;

typedef struct {
    char *pos;
    char *buffer;
    size_t size;
    size_t len;
} FileBuffer;

typedef struct {
    char *pathname;
    FileBuffer *file;
    LineBuffer *line;
} Stream;

static int load_file(const char *pathname, const char *mode, FileBuffer **file);

static int init_line(const char *str, size_t size, LineBuffer **line);

int stream_init_ZJF(void **handle, void *ini) {
    int rv = -111;
    Stream *stream = NULL;
    if (!handle) {
        log_ZJF("e", "handle == NULL");
        goto End;
    }
    if (!ini) {
        log_ZJF("e", "ini == NULL");
        goto End;
    }
    char *pathname = (char *) ini;
    if (!*pathname) {
        log_ZJF("e", "ini == \"\"");
        goto End;
    }
    rv = 0;

//    Stream + pathname
    const size_t sz = strlen(pathname) + 1;
    const size_t size = sizeof(Stream) + sz;
    stream = (Stream *) malloc(size);
    if (!stream) {
        rv = -333;
        log_ZJF("f", "malloc %llu error", size);
        goto End;
    }

//    init
    memset(stream, 0, size);
    char *ptr = (char *) (stream + 1);
    memcpy(ptr, pathname, sz);
    stream->pathname = ptr;

//    return
    *handle = stream;
    End:
    return rv;
}

int stream_free_ZJF(void **handle) {
    int rv = -111;
    if (!handle) {
        log_ZJF("e", "handle == NULL");
        goto End;
    }
    rv = 0;
    Stream *ptr = *handle;
    if (!ptr) {
        goto End;
    }
    if (ptr->file) {
        free(ptr->file);
    }
    if (ptr->line) {
        free(ptr->line);
    }
    free(ptr);
    *handle = NULL;
    End:
    return rv;
}

int stream_read_ZJF(void *handle, char **line) {
    int rv = -111;
    char *buffer = NULL;
    if (!handle) {
        log_ZJF("e", "handle == NULL");
        goto End;
    }
    if (!line) {
        log_ZJF("e", "line == NULL");
        goto End;
    }
    rv = 0;

//    load file
    Stream *stream = (Stream *) handle;
    FileBuffer *file = stream->file;
    if (!file) {
        rv = load_file(stream->pathname, "rb", &file);
        if (rv) {
            log_ZJF("e", "load file error");
            goto End;
        }
        stream->file = file;
    }

//    read line
    char *pos = file->pos, *beg = pos;
//    reach End of file
    if (!*beg) {
        goto End;
    }
    char ch;
    while ((ch = *pos) && ch != '\n') {
        ++pos;
    }
//    copy line
    size_t size = pos - beg;
    LineBuffer *lineBuffer = stream->line;
    rv = init_line(beg, size + 1, &lineBuffer);
    if (rv) {
        log_ZJF("e", "init line error");
        goto End;
    }
//    reset line
    stream->line = lineBuffer;

//    set position
    if (*pos) {
        ++pos;
    }
    file->pos = pos;
    buffer = lineBuffer->buffer;
    End:
    *line = buffer;
    return rv;
}

int stream_write_ZJF(void *handle, const char *buffer, size_t *size) {
    FILE *file = NULL;
    int rv = -111;
    if (!handle) {
        log_ZJF("e", "handle == NULL");
        goto End;
    }
    if (!buffer) {
        log_ZJF("e", "buffer == NULL");
        goto End;
    }
    rv = 0;
    Stream *ptr = (Stream *) handle;
    char *pathname = ptr->pathname;
    char *mode = "wb";
    file = fopen(pathname, mode);
    if (!file) {
        rv = -221;
        log_ZJF("e", "open error: %s::%s", mode, pathname);
        goto End;
    }
    size_t count = fwrite(buffer, sizeof(char), strlen(buffer), file);
    if (ferror(file)) {
        rv = -223;
        log_ZJF("e", "write error: %s::%s", mode, pathname);
        goto End;
    }
    if (size) {
        *size = count;
    }
    End:
    if (file) {
        fclose(file);
    }
    return rv;
}

int load_file(const char *pathname, const char *mode, FileBuffer **file) {
    int rv = -111;
    FILE *ptr = NULL;
    if(!pathname){
        log_ZJF("e", "pathname == NULL");
        goto End;
    }
    if(!*pathname){
        log_ZJF("e", "pathname == \"\"");
        goto End;
    }
    if(!mode){
        log_ZJF("e", "mode == NULL");
        goto End;
    }
    if(!*mode){
        log_ZJF("e", "mode == \"\"");
        goto End;
    }
    if(!file){
        log_ZJF("e", "mode == NULL");
        goto End;
    }
    rv = 0;
//    open file
    ptr = fopen(pathname, mode);
    if (!ptr) {
        rv = -221;
        log_ZJF("e", "open error: %s::%s", mode, pathname);
        goto End;
    }

//  retrieve size of file
    fseek(ptr, 0L, SEEK_END);
    long size = ftell(ptr) + 1;
    const long max = 9 * 1024 * 1024;
    fseek(ptr, 0L, SEEK_SET);
    if (size > max) {
        rv = -255;
        log_ZJF("e", "(size == %l)>(max == %l)", size, max);
        goto End;
    }

    FileBuffer *stBuffer = *file;
    if(!stBuffer || stBuffer->size < size){
        if(stBuffer){
            free(stBuffer);
            *file = NULL;
        }
        stBuffer = (FileBuffer *) malloc(size + sizeof(FileBuffer));
        if (!stBuffer) {
            rv = -333;
            log_ZJF("f", "malloc %l error", size);
            goto End;
        }
        stBuffer->buffer = (char *) (stBuffer + 1);
        stBuffer->size = size;
        *file = stBuffer;
    }

//    reset pos
    stBuffer->pos = stBuffer->buffer;

//    read file
    char *buf = stBuffer->buffer;
    --size;
    fread(buf, sizeof(char), size, ptr);
    if (ferror(ptr)) {
        rv = -222;
        log_ZJF("e", "read error: %s::%s", mode, pathname);
        goto End;
    }
    buf[size] = '\0';
    stBuffer->len = size;
    End:
    if (ptr) {
        fclose(ptr);
    }
    return rv;
}

int init_line(const char *str, size_t size, LineBuffer **line) {
    int rv = -111;
    if (!str) {
        log_ZJF("e", "str == NULL");
        goto End;
    }
    if (size < 2) {
        log_ZJF("e", "size == %llu, do nothing", size);
        goto End;
    }
    if (!line) {
        log_ZJF("e", "line == NULL");
        goto End;
    }
    rv = 0;
    LineBuffer *stBuffer = *line;
    if (!stBuffer || stBuffer->size < size) {
//        free old line
        if (stBuffer) {
            free(stBuffer);
            *line = NULL;
        }
//        resize line
        size_t sz = size + sizeof(LineBuffer);
        stBuffer = (LineBuffer *) malloc(sz);
        if (!stBuffer) {
            rv = -333;
            log_ZJF("f", "malloc %llu error", sz);
            goto End;
        }
//        init new line
        stBuffer->size = size;
        stBuffer->buffer = (char *) (stBuffer + 1);
        *line = stBuffer;
    }
//    copy data
    --size;
    char *buf = stBuffer->buffer;
    memcpy(buf, str, size);
    buf[size] = '\0';
    stBuffer->len = size;
    End:
    return rv;
}
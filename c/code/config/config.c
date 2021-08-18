//
// Created by 12131 on 2021/8/1.
//

#include "config.h"
#include "../adt/list/list_generic_ui.h"
#include "../adt/list/list_generic_impl.h"

typedef struct {
    int size;
    char buffer[1];
} Buffer;

typedef struct {
    Buffer key;
    Buffer value;
} Map;

LIST_INTERFACE_GENERIC(Buffer *, buffer)

LIST_IMPLEMENT_GENERIC(Buffer *, buffer)

LIST_INTERFACE_GENERIC(Map *, map)

//LIST_IMPLEMENT_GENERIC(Map *, map)

static int map_init();

static int line_init();

static int map_parse();


int config_file_read(const char *pathname,
                     const char *key,
                     const char *value,
                     int size) {

    return 0;
}

int config_file_write(const char *pathname,
                      const char *key,
                      const char *value) {
    return 0;
}


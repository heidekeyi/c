//
// Created by 12131 on 2021/8/1.
//
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../../log/logging.h"
#include "../../code_error/code_error.h"
#include "config_file.h"
#include "../../adt/list/list_generic_ui.h"
#include "../../adt/list/list_generic_impl.h"

typedef struct {
    int size;
    char *buffer;
} Buffer;

typedef struct {
    Buffer *key;
    Buffer *value;
} Map;

LIST_INTERFACE_GENERIC(Buffer *, buffer)

LIST_IMPLEMENT_GENERIC(Buffer *, buffer)

LIST_INTERFACE_GENERIC(Map *, map)

LIST_IMPLEMENT_GENERIC(Map *, map)

#define READ_BUFFER_CACHE_SIZE 8

// open file
#ifdef _MSC_VER
#define STREAM_OPEN_FILE(pathname, mode, pp_file) \
fopen_s(pp_file, pathname, mode)
#else
#define STREAM_OPEN_FILE(pathname, mode, pp_file) \
*pp_file = fopen(pathname, mode)
#endif

static int map_init(List_map *list_map, const char *pathname);

static int map_parse(List_map *list_map, List_buffer *list_line);

static int map_retrieve_item(const Buffer *line, Map **item);

static int map_update_item(List_map *list_map, int index,
                           const char *key, const char *value);

static int map_logging_persistent(const char *pathname, List_map *list_map);

static void map_free_item(Map *map);

static int map_free(List_map *list_map);

static int line_init(List_buffer *list_line, const char *pathname);

static int line_read_file(List_buffer *list_line, FILE *ptr_file);

static int line_append(List_buffer *list_line, Buffer *line);

static int buffer_data_copy(Buffer *in_line,
                            const char *str,
                            int count,
                            Buffer **out_line);

static int buffer_retrieve_with_trim(const char *beg,
                                     const char *end,
                                     Buffer **out_buffer);

static int line_free(List_buffer *list_line);

static int is_match_map_key(Map *data, Map *elem);

int config_file_read(const char *pathname,
                     const char *in_key,
                     char in_buffer[],
                     int sz_buffer,
                     char **out_buffer) {
    int rv, index, len;
    Buffer buf_key;
    Map map;
    int code = 0;
    List_map *list_map = NULL;
    Map *map_data = NULL;
    buf_key.buffer = (char *) in_key;
    map.key = &buf_key;
//    check arguments
    if (NULL == pathname) {
        logging("e", "NULL == pathname");
        return CODE_ERROR_ARG_NULL;
    }
    if ('\0' == *pathname) {
        logging("e", "'\\0' == *pathname");
        return CODE_ERROR_ARG_EMPTY_STRING;
    }
    if (NULL == in_key) {
        logging("e", "NULL == in_key");
        return CODE_ERROR_ARG_NULL;
    }
    if (NULL == in_buffer) {
        logging("e", "NULL == in_buffer");
        return CODE_ERROR_ARG_NULL;
    }
    if (sz_buffer < 2) {
        logging("e", "sz_buffer=%d < 2, do nothing", sz_buffer);
        return CODE_ERROR_ARG_EMPTY_BUFFER;
    }
    if (NULL == out_buffer) {
        logging("e", "NULL == out_buffer");
        return CODE_ERROR_ARG_NULL;
    }
//    init list
    rv = list_map_create(&list_map);
    if (rv) {
        logging("e", "%d = list_map_create(&list_map=0x%p)",
                rv, &list_map);
        return rv;
    }
//    init map
    rv = map_init(list_map, pathname);
    if (rv) {
        logging("e", "%d = map_init(list_map=0x%p)",
                rv, list_map);
        goto e;
    }
//    search map
    rv = list_map_search(list_map, &map, &index, is_match_map_key);
    if (rv) {
        logging("e", "%d = list_map_search("
                     "list_map=0x%p, &map=0x%p, &index=0x%p, "
                     "is_match_map_key=0x%p)",
                rv, list_map, &map, &index, is_match_map_key);
        goto e;
    }
//    not found key
    if (index < 0) {
        *out_buffer = NULL;
        goto e;
    }
//    peek map
    rv = list_map_peek(list_map, index, &map_data);
    if (rv) {
        logging("e", "%d = list_map_peek("
                     "list_map=0x%p, index=%d, &map_data=0x%p)",
                rv, list_map, index, &map_data);
        goto e;
    }
//    copy data
    len = map_data->value->size;
    if (len >= sz_buffer) {
        len = sz_buffer - 1;
    }
    in_buffer[len] = '\0';
    memcpy(in_buffer, map_data->value->buffer, len);
    *out_buffer = in_buffer;
    e:
//    free map
    rv = map_free(list_map);
    if (rv) {
        logging("e", "%d = map_free(list_map=0x%p)",
                rv, list_map);
        code = rv;
    }
//    free list
    rv = list_map_destroy(&list_map);
    if (rv) {
        logging("e", "%d = list_map_destroy(&list_map=0x%p)",
                rv, &list_map);
        code = rv;
    }
    return code;
}

int config_file_write(const char *pathname,
                      const char *key,
                      const char *value) {
    int rv, index;
    int code = 0;
    List_map *list_map = NULL;
    Buffer buf_key;
    Map map;
    buf_key.buffer = (char *) key;
    map.key = &buf_key;

//    check arguments
    if (NULL == pathname) {
        logging("e", "NULL == pathname");
        return CODE_ERROR_ARG_NULL;
    }
    if ('\0' == *pathname) {
        logging("e", "'\\0' == *pathname");
        return CODE_ERROR_ARG_EMPTY_STRING;
    }
    if (NULL == key) {
        logging("e", "NULL == key");
        return CODE_ERROR_ARG_NULL;
    }
    if (NULL == value) {
        logging("e", "NULL == value");
        return CODE_ERROR_ARG_NULL;
    }
//    init list
    rv = list_map_create(&list_map);
    if (rv) {
        logging("e", "%d = list_map_create(&list_map=0x%p)",
                rv, &list_map);
        return rv;
    }
//    init map
    rv = map_init(list_map, pathname);
    if (rv) {
        logging("e", "%d = map_init(list_map=0x%p)",
                rv, list_map);
        goto e;
    }
//    search map
    rv = list_map_search(list_map, &map, &index, is_match_map_key);
    if (rv) {
        logging("e", "%d = list_map_search("
                     "list_map=0x%p, &map=0x%p, &index=0x%p, "
                     "is_match_map_key=0x%p)",
                rv, list_map, &map, &index, is_match_map_key);
        goto e;
    }
    rv = map_update_item(list_map, index, key, value);
    if (rv) {
        logging("e", "%d = map_update_item("
                     "list_map=0x%p, index=%d, key=0x%p, value=0x%p)",
                rv, list_map, index, key, value);
        goto e;
    }
    rv = map_logging_persistent(pathname, list_map);
    if (rv) {
        logging("e", "%d = map_logging_persistent("
                     "pathname=\"%s\", list_map=0x%p)",
                rv, pathname, list_map);
        goto e;
    }
    e:
//    free map
    rv = map_free(list_map);
    if (rv) {
        logging("e", "%d = map_free(list_map=0x%p)",
                rv, list_map);
        code = rv;
    }
//    free list
    rv = list_map_destroy(&list_map);
    if (rv) {
        logging("e", "%d = list_map_destroy(&list_map=0x%p)",
                rv, &list_map);
        code = rv;
    }
    return code;
}

static int is_match_map_key(Map *data, Map *elem) {
    return !strcmp(data->key->buffer, elem->key->buffer);
}

static int map_init(List_map *list_map, const char *pathname) {
    int rv;
    int code = 0;
    List_buffer *list_line = NULL;
//    init list
    rv = list_buffer_create(&list_line);
    if (rv) {
        logging("e", "%d = list_buffer_create(&list_line=0x%p)",
                rv, &list_line);
        return rv;
    }
//    init line
    rv = line_init(list_line, pathname);
    if (rv) {
        logging("e", "%d = line_init("
                     "list_line=0x%p, pathname=0x%p)",
                rv, list_line, pathname);
        goto e;
    }
//    parse map
    rv = map_parse(list_map, list_line);
    if (rv) {
        logging("e", "%d = map_parse("
                     "list_map=0x%p, list_line=0x%p)",
                rv, list_map, list_line);
        goto e;
    }
    e:
//    free line
    rv = line_free(list_line);
    if (rv) {
        logging("e", "%d = line_free(list_line=0x%p)",
                rv, list_line);
        code = rv;
    }
//    free list
    rv = list_buffer_destroy(&list_line);
    if (rv) {
        logging("e", "%d = list_buffer_destroy(&list_line=0x%p)",
                rv, &list_line);
        code = rv;
    }
    return code;
}

static int line_free(List_buffer *list_line) {
    int rv, size, cnt;
    Buffer *data;
    rv = list_buffer_size(list_line, &size);
    if (rv) {
        logging("e", "%d = list_buffer_size("
                     "list_line=0x%p, &size=0x%p)",
                rv, list_line, &size);
        return rv;
    }
    for (cnt = 0; cnt < size; ++cnt) {
        rv = list_buffer_remove(list_line, 0, &data);
        if (rv) {
            logging("e", "%d = list_buffer_remove("
                         "list_line=0x%p, 0, &data=0x%p)",
                    rv, list_line, &data);
            return rv;
        }
        free(data);
    }
    return 0;
}

static int map_free(List_map *list_map) {
    int rv, size, cnt;
    Map *map;
    rv = list_map_size(list_map, &size);
    if (rv) {
        logging("e", "%d = list_map_size("
                     "list_map=0x%p, &size=0x%p)",
                rv, list_map, &size);
        return rv;
    }
    for (cnt = 0; cnt < size; ++cnt) {
        rv = list_map_remove(list_map, 0, &map);
        if (rv) {
            logging("e", "%d = list_map_remove("
                         "list_map=0x%p, 0, &map=0x%p)",
                    rv, list_map, &map);
            return rv;
        }
        map_free_item(map);
    }
    return 0;
}

static int line_init(List_buffer *list_line, const char *pathname) {
    int rv;
    FILE *ptr_file = NULL;
//    open file
    STREAM_OPEN_FILE(pathname, "rb", &ptr_file);
    if (NULL == ptr_file) {
        logging("e", "STREAM_OPEN_FILE("
                     "pathname=\"%s\", \"rb\", &ptr_file=0x%p)",
                pathname, &ptr_file);
        return CODE_ERROR_STREAM_FILE_OPEN;
    }
//    read file
    rv = line_read_file(list_line, ptr_file);
    if (rv) {
        logging("e", "%d = line_read_file(list_line=0x%p, ptr_file=0x%p)",
                rv, list_line, ptr_file);
//        return normal, but file should be close file
    }
//    close file
    if (EOF == fclose(ptr_file)) {
        logging("e", "fclose(ptr_file=0x%p)", ptr_file);
        rv = CODE_ERROR_STREAM_FILE_CLOSE;
    }
    return rv;
}

static int line_read_file(List_buffer *list_line, FILE *ptr_file) {
    int rv = 0, count;
    Buffer *ptr_line = NULL;
    const char *ptr_beg, *ptr_end;
    while (!feof(ptr_file)) {
        char buffer[READ_BUFFER_CACHE_SIZE] = {0};
        fread(buffer, 1, READ_BUFFER_CACHE_SIZE - 1, ptr_file);
//        reach end of file
        if ('\0' == *buffer) {
            break;
        }
//        read error
        if (ferror(ptr_file)) {//read error
            logging("e", "fread(buffer=0x%p, 1, %d, ptr_file=0x%p)",
                    buffer, READ_BUFFER_CACHE_SIZE, ptr_file);
            return CODE_ERROR_STREAM_FILE_READ;
        }
//        init beg
        ptr_beg = buffer;
        do {
            ptr_end = strchr(ptr_beg, '\n');
//            calc count of char
            if (ptr_end) {
                count = (int) (ptr_end - ptr_beg);
            } else {
                count = (int) strlen(ptr_beg);
            }
//            copy data
            rv = buffer_data_copy(ptr_line, ptr_beg, count, &ptr_line);
            if (rv) {
                logging("e", "%d = buffer_data_copy(ptr_line=0x%p, "
                             "ptr_beg=0x%p, count=%d, &ptr_line=0x%p)",
                        rv, ptr_line, ptr_beg, count, &ptr_line);
                goto e;
            }
//            is not end of line, continue read data
            if (NULL == ptr_end) {
                break;
            }
            rv = line_append(list_line, ptr_line);
            if (rv) {
                logging("e", "%d = line_append("
                             "list_line=0x%p, ptr_line=0x%p)",
                        rv, list_line, ptr_line);
                goto e;
            }
//            prepare new line
            ptr_line = NULL;
//            skip char of '\n'
            ptr_beg = ptr_end + 1;
        } while (*ptr_beg);
    }

//   last has not char of '\n', append a new line
    if (ptr_line) {
        rv = line_append(list_line, ptr_line);
        if (rv) {
            logging("e", "%d = line_append("
                         "list_line=0x%p, ptr_line=0x%p)",
                    rv, list_line, ptr_line);
            goto e;
        }
        ptr_line = NULL;
    }
    e:
    if (rv && ptr_line) {
        free(ptr_line);
    }
    return rv;
}

static int line_append(List_buffer *list_line, Buffer *line) {
    int val = line->size;
//            trim '\r'
    if (0 > val) {
        --val;
        if ('\r' == line->buffer[val]) {
            line->size = val;
            line->buffer[val] = '\0';
        }
    }
//            add line data
    val = list_buffer_insert(list_line, -1, line, NULL);
    if (val) {
        logging("e", "%d = list_buffer_insert("
                     "list_line=0x%p, -1, line=0x%p, NULL)",
                val, list_line, line);
        return val;
    }
    return 0;
}

static int buffer_data_copy(Buffer *in_line,
                            const char *str,
                            int count,
                            Buffer **out_line) {
    Buffer *line = in_line;
    char *buffer;
    int size = (int) sizeof(Buffer) + count + 1;
    if (NULL == line) {
        line = (Buffer *) malloc(size);
        if (NULL == line) {
            logging("e", "NULL = (Buffer *) malloc(size=%d)", size);
            return CODE_ERROR_LIB_STD;
        }
        line->size = 0;
    } else {
        size += line->size;
        line = (Buffer *) realloc(in_line, size);
        if (NULL == line) {
            logging("e", "NULL = (Buffer *) realloc("
                         "in_line=0x%p, size=%d)",
                    in_line, size);
            return CODE_ERROR_LIB_STD;
        }
    }
//    reset point of buffer
    buffer = (char *) (line + 1);
    line->buffer = buffer;
//    set buffer position of start
    buffer += line->size;
//    copy data
    memcpy(buffer, str, count);
    buffer[count] = '\0';
//    reset buffer count of char
    line->size += count;
    *out_line = line;
    return 0;
}

static int map_parse(List_map *list_map, List_buffer *list_line) {
    int rv, size, index;
    Buffer *line;
    Map *map = NULL;
    rv = list_buffer_size(list_line, &size);
    if (rv) {
        logging("e", "%d = list_buffer_size(list_line=0x%p, &size=0x%p)",
                rv, list_line, &size);
        return rv;
    }
    for (index = 0; index < size; ++index) {
//        peek each line
        rv = list_buffer_peek(list_line, index, &line);
        if (rv) {
            logging("e", "%d = list_buffer_peek("
                         "list_line=0x%p, index=%d, &line=0x%p)",
                    rv, list_line, index, &line);
            return rv;
        }
//        retrieve item
        rv = map_retrieve_item(line, &map);
        if (rv) {
            logging("e", "%d = map_retrieve_item(line=0x%p, &map=0x%p)",
                    rv, line, &map);
            return rv;
        }
//        line is invalid
        if (NULL == map) {
            continue;
        }
//        insert map
        rv = list_map_insert(list_map, -1, map, NULL);
        if (rv) {
            map_free_item(map);
            logging("e", "%d = list_map_insert("
                         "list_map=0x%p, -1, map=0x%p, NULL)",
                    rv, list_map, map);
            return rv;
        }
    }
    return 0;
}

static void map_free_item(Map *map) {
    free(map->key);
    free(map->value);
    free(map);
}


static int map_retrieve_item(const Buffer *line, Map **item) {
    int size;
    int rv = 0;
    Map *map = NULL;
    Buffer *key = NULL, *value = NULL;
    char *beg, *end;
//    empty line
    if (0 == line->size) {
        goto e;
    }
    beg = line->buffer;
//    not found '='
    end = strchr(beg, '=');
    if (NULL == end) {
        goto e;
    }
//    retrieve key
    rv = buffer_retrieve_with_trim(beg, end, &key);
    if (rv) {
        logging("e", "%d = buffer_retrieve_with_trim("
                     "beg=0x%p, end=0x%p, &key=0x%p)",
                rv, beg, end, &key);
        goto e;
    }
//    key is empty
    if (NULL == key) {
        goto e;
    }
//    retrieve value
//    skip '='
    beg = end + 1;
    end = line->buffer + line->size;
    rv = buffer_retrieve_with_trim(beg, end, &value);
    if (rv) {
        logging("e", "%d = buffer_retrieve_with_trim("
                     "beg=0x%p, end=0x%p, &value=0x%p)",
                rv, beg, end, &value);
        goto e;
    }
//    value is empty
    if (NULL == value) {
        goto e;
    }
//   init map item
    size = (int) sizeof(Map);
    map = malloc(size);
    if (NULL == map) {
        logging("f", "NULL = malloc(size=%d)", size);
        rv = CODE_ERROR_LIB_STD;
        goto e;
    }
    map->key = key;
    map->value = value;
    e:
    if (rv) {
        if (key) {
            free(key);
        }
        if (value) {
            free(value);
        }
    } else {
        *item = map;
    }
    return rv;
}


static int buffer_retrieve_with_trim(const char *beg,
                                     const char *end,
                                     Buffer **out_buffer) {
    int count;
    int rv;
    Buffer *buf = NULL;
//    left space trim
    while (beg < end) {
        if (!isspace((*beg))) {
            break;
        }
        ++beg;
    }
//    right space trim
    while (beg < --end) {
        if (!isspace((*end))) {
            break;
        }
    }
    if (beg > end) {
        *out_buffer = NULL;
        return 0;
    }
//    buffer init
    count = (int) (end - beg) + 1;
    rv = buffer_data_copy(NULL, beg, count, &buf);
    if (rv) {
        logging("e", "%d = buffer_data_copy("
                     "NULL, beg=0x%p, count=%d, &buf=0x%p)",
                rv, NULL, beg, count, &buf);
        return rv;
    }
    *out_buffer = buf;
    return 0;
}

static int map_update_item(List_map *list_map, int index,
                           const char *key, const char *value) {
    int rv, len;
    Map *map = NULL;
    Buffer *key_buf = NULL, *value_buf = NULL;
//    init value
    len = (int) strlen(value);
    rv = buffer_data_copy(NULL, value, len, &value_buf);
    if (rv) {
        logging("e", "%d = buffer_data_copy("
                     "NULL, key=0x%p, len=%d, &value_buf=0x%p)",
                rv, key, len, &value_buf);
        goto e;
    }
    if (index < 0) {
//        init key
        len = (int) strlen(key);
        rv = buffer_data_copy(NULL, key, len, &key_buf);
        if (rv) {
            logging("e", "%d = buffer_data_copy("
                         "NULL, key=0x%p, len=%d, &key_buf=0x%p)",
                    rv, key, len, &key_buf);
            goto e;
        }
//        init map
        len = sizeof(Map);
        map = (Map *) malloc(len);
        if (NULL == map) {
            logging("e", "NULL = (Map *) malloc(len=%d)", len);
            rv = CODE_ERROR_LIB_STD;
            goto e;
        }
        map->key = key_buf;
    } else {
        rv = list_map_remove(list_map, index, &map);
        if (rv) {
            logging("e", "%d = list_map_remove("
                         "list_map=0x%p, index=%d, &map=0x%p)",
                    rv, list_map, index, &map);
            goto e;
        }
        key_buf = map->key;
        free(map->value);
    }
//    append map
    map->value = value_buf;
    rv = list_map_insert(list_map, -1, map, NULL);
    if (rv) {
        logging("e", "%d = list_map_insert("
                     "list_map=0x%p, -1, map=0x%p, NULL)",
                rv, list_map, map);
        goto e;
    }
    return 0;
    e:
    if (key_buf) {
        free(key_buf);
    }
    if (value_buf) {
        free(value_buf);
    }
    if (map) {
        free(map);
    }
    return rv;
}

static int map_logging_persistent(const char *pathname, List_map *list_map) {
    int rv, index, size;
    Map *map;
    Buffer *tmp;
    int code = 0;
    FILE *ptr_file = NULL;
    const char *split = " = ";
    const int sz_split = (int) strlen(split);
//    open file
    STREAM_OPEN_FILE(pathname, "wb", &ptr_file);
    if (NULL == ptr_file) {
        logging("e", "STREAM_OPEN_FILE("
                     "pathname=\"%s\", \"wb\", &ptr_file=0x%p)",
                pathname, &ptr_file);
        return CODE_ERROR_STREAM_FILE_OPEN;
    }
    rv = list_map_size(list_map, &size);
    if (rv) {
        logging("e", "%d = list_map_size("
                     "list_map=0x%p, &size=0x%p)",
                rv, list_map, &size);
        code = rv;
//        skip loop
        size = 0;
    }
    for (index = 0; index < size; ++index) {
        rv = list_map_peek(list_map, index, &map);
        if (rv) {
            logging("e", "%d = list_map_peek("
                         "list_map=0x%p, index=%d, &map=0x%p)",
                    rv, list_map, index, &map);
            code = rv;
            break;
        }
//    write key
        tmp = map->key;
        fwrite(tmp->buffer, 1, tmp->size, ptr_file);
//    write split
        fwrite(split, 1, sz_split, ptr_file);
//    write value
        tmp = map->value;
        fwrite(tmp->buffer, 1, tmp->size, ptr_file);
//    write new line
        fwrite("\n", 1, 1, ptr_file);
    }
    if (ferror(ptr_file)) {
        logging("e", "write error: \"%s\"   \"wb\"", pathname);
        code = CODE_ERROR_STREAM_FILE_WRITE;
//    return normal, but file should be close file
    }
//    close file
    if (EOF == fclose(ptr_file)) {
        logging("e", "fclose(ptr_file=0x%p)", ptr_file);
        code = CODE_ERROR_STREAM_FILE_CLOSE;
    }
    return code;
}
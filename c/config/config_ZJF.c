//
// Created by 12131 on 2021/7/15.
//

#include "config_ZJF.h"

typedef struct {
    size_t size;
    size_t nKey;
    size_t nValue;
    char *key;
    char *value;
} Pair;

typedef struct {
    size_t count;
    size_t size;
    Pair **list;
} Map;

typedef struct {
    size_t size;
    size_t len;
    char *buffer;
} Serialize;

typedef struct {
    void *ini;
    Pair *pair;
    Map *map;
    Serialize *file;
} Config;

//read file
static int load_map_ZJF(void *ini, Map **map);

//**********************pair**********************
//init map
static int map_init_ZJF(size_t count, Map **map);

//resize map (size * 2)
static int pair_list_resize_ZJF(Map **map);

//append pair
static int pair_append_ZJF(Map **map, Pair *pair);

//key search index
static int pair_search_index_ZJF(Map *map, const char *key, int *index);

//parse line
static int pair_parse_ZJF(const char *str, const char *delimiter, Pair **pair);

//word info
static int pair_word_info_ZJF(const char *beg, const char *end, const char **pos, size_t *count);

static int pair_init_ZJF(const char *key, size_t nKey, const char *value, size_t nValue, Pair **pair);


static int config_serialize(Map *map, Serialize **ptr, const char *delimiter);


int config_init_ZJF(void **handle, void *ini) {
    Config *config = NULL;
    int rv = -111;
    if (!handle) {
        log_ZJF("e", "handle == NULL");
        goto End;
    }
    if (!ini) {
        log_ZJF("e", "ini == NULL");
        goto End;
    }
    rv = 0;
    size_t size = sizeof(Config);
    config = (Config *) malloc(size);
    if (!config) {
        rv = -333;
        log_ZJF("f", "malloc %llu error", size);
        goto End;
    }
//    ini
    memset(config, 0, size);
    config->ini = ini;
    rv = load_map_ZJF(ini, &config->map);
    if (rv) {
        log_ZJF("e", "load pair error");
        goto End;
    }
    *handle = config;
    End:
    if (rv && config) {
        free(config);
    }
    return rv;
}

int config_free_ZJF(void **handle) {
    int rv = -111;
    if (!handle) {
        log_ZJF("e", "handle == NULL");
        goto End;
    }
    rv = 0;
    Config *config = *handle;
    if (!config) {
        goto End;
    }
    if (config->pair) {
        free(config->pair);
    }
    if (config->map) {
        Pair **list = config->map->list;
        size_t count = config->map->count;
        Pair *ptr;
        for (int i = 0; i < count; ++i) {
            ptr = list[i];
            if (ptr) {
                free(ptr);
            }
        }
        free(config->map);
    }
    if(config->file){
        free(config->file);
    }
    free(config);
    *handle = NULL;
    End:
    return rv;
}

int config_read_ZJF(void *handle, const char *key, char **value) {
    int rv = -111;
    char *ptr = NULL;
    if (!handle) {
        log_ZJF("e", "handle == NULL");
        goto End;
    }
    if (!key) {
        log_ZJF("e", "key == NULL");
        goto End;
    }
    if (!*key) {
        log_ZJF("e", "key == \"\", do nothing");
        goto End;
    }
    if (!value) {
        log_ZJF("e", "value == NULL");
        goto End;
    }
    int index = -1;
    Config *config = (Config *) handle;
    rv = pair_search_index_ZJF(config->map, key, &index);
    if (rv) {
        log_ZJF("e", "search key index error: %s", key);
        goto End;
    }
    if (index < 0) {
        goto End;
    }
//    copy data
    Pair *pair = config->map->list[index];
    rv = pair_init_ZJF(pair->key, pair->nKey, pair->value, pair->nValue, &config->pair);
    ptr = config->pair->value;
    End:
    *value = ptr;
    return rv;
}

int config_write_ZJF(void *handle, const char *key, const char *value) {
    int rv = -111;
    Pair *ptr = NULL;
    void *stream = NULL;
    if (!handle) {
        log_ZJF("e", "handle == NULL");
        goto End;
    }
    if (!key) {
        log_ZJF("e", "key == NULL");
        goto End;
    }
    if (!*key) {
        log_ZJF("e", "key == \"\", do nothing");
        goto End;
    }
    if (!value) {
        log_ZJF("e", "value == NULL");
        goto End;
    }
    if (!*value) {
        log_ZJF("e", "value == \"\", do nothing");
        goto End;
    }
    Config *config = (Config *) handle;
    rv = pair_init_ZJF(key, strlen(key), value, strlen(value), &ptr);
    if (rv) {
        log_ZJF("e", "init pair error: %s=%s", key, value);
        goto End;
    }
    rv = pair_append_ZJF(&config->map, ptr);
    if (rv) {
        free(ptr);
        log_ZJF("e", "append pair error: %s=%s", key, value);
        goto End;
    }
    rv = config_serialize(config->map, &config->file, "=");
    if (rv) {
        log_ZJF("e", "serialize config error");
        goto End;
    }
    rv = stream_init_ZJF(&stream, config->ini);
    if(rv){
        log_ZJF("e", "init stream error");
        goto End;
    }
    rv = stream_write_ZJF(stream, config->file->buffer, NULL);
    if(rv){
        log_ZJF("e", "write stream error");
        goto End;
    }
    End:
    if(stream && stream_free_ZJF(&stream)){
        log_ZJF("e", "free stream error");
    }
    return rv;
}

int load_map_ZJF(void *ini, Map **map) {
    int rv = -111;
    void *stream = NULL;
    if (!ini) {
        log_ZJF("e", "ini == NULL");
        goto End;
    }
    if (!map) {
        log_ZJF("e", "map == NULL");
        goto End;
    }
    rv = map_init_ZJF(2, map);
    if (rv) {
        log_ZJF("e", "init map error");
        goto End;
    }
    rv = stream_init_ZJF(&stream, ini);
    if (rv) {
        log_ZJF("e", "init stream error");
        goto End;
    }
    char *line;
    Pair *pair = NULL;
    int cnt = 0;
    while (stream) {
        ++cnt;
        rv = stream_read_ZJF(stream, &line);
        if (rv) {
            log_ZJF("e", "init stream error");
            goto End;
        }
        if (!line) {
            break;
        }
        rv = pair_parse_ZJF(line, "=", &pair);
        if (rv) {
            log_ZJF("e", "parse pair error: %s", line);
            goto End;
        }
        if (!pair) {
            continue;
        }
//        printf("l: %d\n", cnt);
//        printf("k: %s\n", pair->key);
//        printf("v: %s\n", pair->value);
        rv = pair_append_ZJF(map, pair);
        if (rv) {
            free(pair);
            log_ZJF("e", "append pair error");
            goto End;
        }
        pair = NULL;
    }
    End:
    if (rv) {
        if (stream && stream_init_ZJF(&stream, ini)) {
            log_ZJF("e", "free stream error");
        }
    }
    return rv;
}

int map_init_ZJF(size_t count, Map **map) {
    int rv = -111;
    if (count < 1) {
        log_ZJF("e", "count == %llu", count);
        goto End;
    }
    if (!map) {
        log_ZJF("e", "map == NULL");
        goto End;
    }
    rv = 0;
    size_t sz = count * sizeof(Pair **) + sizeof(Map);
    Map *ptr = (Map *) malloc(sz);
    if (!ptr) {
        rv = -333;
        log_ZJF("f", "malloc %llu error", sz);
        goto End;
    }
    memset(ptr, 0, sz);
    ptr->count = count;
    ptr->size = sz;
    ptr->list = (Pair **) (ptr + 1);
    *map = ptr;
    End:
    return rv;
}

//parse line
int pair_parse_ZJF(const char *str, const char *delimiter, Pair **pair) {
    int rv = -111;
    if (!str) {
        log_ZJF("e", "str == NULL");
        goto End;
    }
    if (!delimiter) {
        log_ZJF("e", "delimiter == NULL");
        goto End;
    }
    if (!*delimiter) {
        log_ZJF("e", "delimiter == \"\"");
        goto End;
    }
    if (!pair) {
        log_ZJF("e", "pair == NULL");
        goto End;
    }
    rv = 0;
    char *pos = strstr(str, delimiter);
//    dot limit is not exist
    if (!pos) {
        goto End;
    }
    const char *key, *value;
    size_t nKey, nValue;
//    key info
    rv = pair_word_info_ZJF(str, pos, &key, &nKey);
    if (rv) {
        log_ZJF("e", "key info error");
        goto End;
    }
    if (!key) {
        goto End;
    }
//    value info
    ++pos;
    rv = pair_word_info_ZJF(pos, pos + strlen(pos), &value, &nValue);
    if (rv) {
        log_ZJF("e", "value info error");
        goto End;
    }
    if (!value) {
        goto End;
    }
    rv = pair_init_ZJF(key, nKey, value, nValue, pair);
    if (rv) {
        log_ZJF("e", "init pair error: %s = %s", key, value);
        goto End;
    }
    End:
    return rv;
}

//word info
int pair_word_info_ZJF(const char *beg, const char *end, const char **pos, size_t *count) {
    int rv = -111;
    const char *ptr = NULL;
    size_t sz = 0;
    if (!beg) {
        log_ZJF("e", "beg == NULL");
        goto End;
    }
    if (!end) {
        log_ZJF("e", "end == NULL");
        goto End;
    }
    if (!pos) {
        log_ZJF("e", "pos == NULL");
        goto End;
    }
    if (!count) {
        log_ZJF("e", "count == NULL");
        goto End;
    }
    rv = 0;
//    trim space of left
    while (beg < end) {
        if (!isspace(*beg)) {
            break;
        }
        ++beg;
    }
//    word == ""
    if (beg >= end) {
        goto End;
    }
//    trim space of right
    while (beg <= --end) {
        if (!isspace(*end)) {
            break;
        }
    }
//    set info
    ptr = beg;
    sz = end - beg + 1;
    End:
//        return
    *pos = ptr;
    *count = sz;
    return rv;
}

int pair_init_ZJF(const char *key, size_t nKey, const char *value, size_t nValue, Pair **pair) {
    int rv = -111;
    if (!key) {
        log_ZJF("e", "key == NULL");
        goto End;
    }
    if (nKey < 1) {
        log_ZJF("e", "nKey == %llu", nKey);
        goto End;
    }
    if (!value) {
        log_ZJF("e", "value == NULL");
        goto End;
    }
    if (nValue < 1) {
        log_ZJF("e", "nValue == %llu", nValue);
        goto End;
    }
    if (!pair) {
        log_ZJF("e", "pair == NULL");
        goto End;
    }
    rv = 0;

    Pair *ptr = *pair;
    size_t size = nKey + nValue + 2 + sizeof(Pair);
    if (!ptr || ptr->size < size) {
        if (ptr) {
            free(ptr);
            *pair = NULL;
        }
        ptr = (Pair *) malloc(size);
        if (!ptr) {
            log_ZJF("f", "malloc %llu error", size);
            rv = -333;
            goto End;
        }
        ptr->size = size;
        *pair = ptr;
    }
//    set key
    char *buf = (char *) (ptr + 1);
    memcpy(buf, key, nKey);
    buf[nKey] = '\0';
    ptr->key = buf;
    ptr->nKey = nKey;

//    set value
    buf = buf + nKey + 1;
    memcpy(buf, value, nValue);
    buf[nValue] = '\0';
    ptr->value = buf;
    ptr->nValue = nValue;
    End:
    return rv;
}

int pair_append_ZJF(Map **map, Pair *pair) {
    int rv = -111;
    if (!map) {
        log_ZJF("e", "map == NULL");
        goto End;
    }
    if (!*map) {
        log_ZJF("e", "*map == NULL, do nothing");
        goto End;
    }
    if (!pair) {
        log_ZJF("e", "pair == NULL");
        goto End;
    }
    int index = -1;
    Map *pMap = *map;
    Pair **list = pMap->list;
    const size_t num = pMap->count;
//    use new value
    rv = pair_search_index_ZJF(pMap, pair->key, &index);
    if (rv) {
        log_ZJF("e", "*search index of pair error");
        goto End;
    }
//    key exist
    if (index >= 0) {
        free(list[index]);
        list[index] = pair;
        goto End;
    }
//    empty space
    index = -1;
    for (int i = 0; i < num; ++i) {
        if (!list[i]) {
            index = i;
            break;
        }
    }
//    direct save
    if (index >= 0) {
        list[index] = pair;
        goto End;
    }
    rv = pair_list_resize_ZJF(map);
    if (rv) {
        log_ZJF("e", "resize pair list error");
        goto End;
    }
    (*map)->list[num] = pair;
    End:
    return rv;
}

//key search index
int pair_search_index_ZJF(Map *map, const char *key, int *index) {
    int ix = -1;
    int rv = -111;
    if (!map) {
        log_ZJF("e", "map == NULL");
        goto End;
    }
    if (!key) {
        log_ZJF("e", "key == NULL");
        goto End;
    }
    if (!index) {
        log_ZJF("e", "index == NULL");
        goto End;
    }
    rv = 0;
    const size_t num = map->count;
    Pair **list = map->list;
    Pair *ptr;
    for (int i = 0; i < num; ++i) {
        ptr = list[i];
        if (ptr && !strcmp(ptr->key, key)) {
            ix = i;
            break;
        }
    }
    End:
    *index = ix;
    return rv;
}

int pair_list_resize_ZJF(Map **map) {
    int rv = -111;
    if (!map) {
        log_ZJF("e", "map == NULL");
        goto End;
    }
    if (!*map) {
        log_ZJF("e", "*map == NULL, do nothing");
        goto End;
    }
    Map *tmp = *map, *ptr;
    size_t num = tmp->count * 2;
    rv = map_init_ZJF(num, &ptr);
    if (rv) {
        log_ZJF("e", "*reset map list error");
        goto End;
    }
    memcpy(ptr->list, tmp->list, tmp->count * sizeof(tmp->list));
    *map = ptr;
    free(tmp);
    End:
    return rv;
}

int config_serialize(Map *map, Serialize **ptr, const char *delimiter) {
    int rv = -111;
    if (!map) {
        log_ZJF("e", "map == NULL");
        goto End;
    }
    if (!ptr) {
        log_ZJF("e", "ptr == NULL");
        goto End;
    }
    rv = 0;
    Serialize *file = *ptr;
    size_t size = 0;
    Pair *it;
//    calc size of string
//    delimiter and \n
    const size_t len = strlen(delimiter);
    for (int i = 0; i < map->count; ++i) {
        it = map->list[i];
        if (it) {
            size += it->nKey;
            size += it->nValue;
            size += len;
            ++size;
        }
    }
//    prepare buffer
    if (!file || file->size < size) {
        if (file) {
            free(file);
            *ptr = NULL;
        }
        const size_t sz = size + 1 + sizeof(Serialize);
        file = (Serialize *) malloc(sz);
        if (!file) {
            rv = -333;
            log_ZJF("f", "malloc error: %llu", sz);
            goto End;
        }
        *ptr = file;
        file->size = size + 1;
        file->buffer = (char *) (file + 1);
    }
    file->len = size;
    char *buffer = file->buffer;
    buffer[size] = 0;
    const char *line = "\n";
    size = strlen(line);
    for (int i = 0; i < map->count; ++i) {
        it = map->list[i];
        if (it) {
//            copy key
            memcpy(buffer, it->key, it->nKey);
            buffer += it->nKey;
//            copy delimiter
            memcpy(buffer, delimiter, len);
            buffer += len;
//            copy value
            memcpy(buffer, it->value, it->nValue);
            buffer += it->nValue;
//            copy line
            memcpy(buffer, line, size);
            buffer += size;
        }
    }
    End:
    return rv;
}
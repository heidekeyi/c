//
// Created by 12131 on 2021/7/31.
//

#ifndef LIB_LIST_IMPL_PROTO_H
#define LIB_LIST_IMPL_PROTO_H


#include <stdio.h>
#include "../../../log/logging.h"
#include "../list_proto_ui.h"


static void test_insert(List *list);

static void test_remove(List *list);

static void test_search(List *list);

static void test_reverse(List *list);

static void test_replace(List *list);

static void show(List *list);

static void test_list_proto();

static void show(List *list) {
    int code, size = 0, index = 0, data;
    printf("********%s********\n", LOG_FUNC_NAME);
    code = list_DDTYPE_NAMEDD_size(list, &size);
    if (code) {
        printf("%d:::%s:::list_DDTYPE_NAMEDD_size(list, &size)\n",
               code, LOG_FUNC_NAME);
    }
    printf("size: %d\n", size);
    while (index < size) {
        code = list_DDTYPE_NAMEDD_peek(list, index, &data);
        if (code) {
            printf("%d:::%s:::list_DDTYPE_NAMEDD_peek(list, index, &data)\n",
                   code, LOG_FUNC_NAME);
            break;
        }
        printf("%2d::%-6d", index, data);
        ++index;
    }
    if (size) {
        puts("");
    }
    printf("********%s********\n", LOG_FUNC_NAME);
}


static void test_insert(List *list) {
    int code, index, pos;
    int ia[] = {3, 0, -1, 2, -5, 8, 3};
    int size = (int) (sizeof(ia) / sizeof(*ia));
    printf("********%s********", LOG_FUNC_NAME);
    show(list);
    for (index = 0; index < size; ++index) {
        code = list_DDTYPE_NAMEDD_insert(list, ia[index], ia[index], &pos);
        if (code) {
            printf("%d:::%s:::list_DDTYPE_NAMEDD_insert("
                   "list, ia[index], ia[index], &pos)\n",
                   code, LOG_FUNC_NAME);
            break;
        }
        printf("insert pos: %d\n", pos);
        show(list);
    }
    code = list_DDTYPE_NAMEDD_insert(list, 3, 5, NULL);
    if (code) {
        printf("%d:::%s:::list_DDTYPE_NAMEDD_insert("
               "list, ia[index], ia[index], &pos)\n",
               code, LOG_FUNC_NAME);
    } else {
        show(list);
    }
    printf("********%s********\n", LOG_FUNC_NAME);
}

static int is_match(int a, int b) {
    return a == b;
}

static void test_search(List *list) {
    int code, index, data;
    int ia[] = {1, -1, 2, 3, 5, 8};
    int size = (int) (sizeof(ia) / sizeof(*ia));
    printf("********%s********\n", LOG_FUNC_NAME);
    show(list);
    for (index = 0; index < size; ++index) {
        for (index = 0; index < size; ++index) {
            code = list_DDTYPE_NAMEDD_search(list, ia[index], &data, is_match);
            if (code) {
                printf("%d:::%s:::list_DDTYPE_NAMEDD_search("
                       "list, ia[index], &index, is_match)\n",
                       code, LOG_FUNC_NAME);
                break;
            }
            printf("%2d::%-6d", data, ia[index]);
        }
    }
    puts("");
    printf("********%s********\n", LOG_FUNC_NAME);
}


static void test_reverse(List *list) {
    int code;
    printf("********%s********\n", LOG_FUNC_NAME);
    show(list);
    code = list_DDTYPE_NAMEDD_reverse(list);
    if (code) {
        printf("%d:::%s:::list_DDTYPE_NAMEDD_reverse(list)\n",
               code, LOG_FUNC_NAME);
    } else {
        show(list);
    }
    printf("********%s********\n", LOG_FUNC_NAME);
}


static void test_remove(List *list) {
    int code, index, data;
    int ia[] = {6, 0, 1};
    int size = (int) (sizeof(ia) / sizeof(*ia));
    printf("********%s********\n", LOG_FUNC_NAME);
    show(list);
    for (index = 0; index < size; ++index) {
        code = list_DDTYPE_NAMEDD_remove(list, ia[index], &data);
        if (code) {
            printf("%d:::%s:::list_DDTYPE_NAMEDD_remove("
                   "list, ia[index], &data)\n",
                   code, LOG_FUNC_NAME);
            break;
        }
        printf("remove data: %d\n", data);
        show(list);
    }
    code = list_DDTYPE_NAMEDD_remove(list, 0, NULL);
    if (code) {
        printf("%d:::%s:::list_DDTYPE_NAMEDD_remove("
               "list, ia[index], &data)\n",
               code, LOG_FUNC_NAME);
    } else {
        show(list);
    }
    printf("********%s********\n", LOG_FUNC_NAME);
}


static void test_replace(List *list) {
    int code, index, data;
    int ia[] = {11, 33, 0, 55, 77};
    int ii[] = {1, 3, 0, 5, 77};
    int size = (int) (sizeof(ia) / sizeof(*ia));
    printf("********%s********\n", LOG_FUNC_NAME);
    show(list);
    for (index = 0; index < size; ++index) {
        code = list_DDTYPE_NAMEDD_replace(list, ii[index], ia[index], &data);
        if (code) {
            printf("%d:::%s:::list_DDTYPE_NAMEDD_replace("
                   "list, ii[index], ia[index], &data)\n",
                   code, LOG_FUNC_NAME);
            break;
        }
        printf("replace data: %d\n", data);
        show(list);
    }
    code = list_DDTYPE_NAMEDD_replace(list, ii[0], ia[1], NULL);
    if (code) {
        printf("%d:::%s:::list_DDTYPE_NAMEDD_replace("
               "list, ii[index], ia[index], &data)\n",
               code, LOG_FUNC_NAME);
    } else {
        show(list);
    }
    printf("********%s********\n", LOG_FUNC_NAME);
}

static void test_list_proto() {
    List *list = NULL;
    int code;
    printf("********%s********\n", LOG_FUNC_NAME);
    code = list_DDTYPE_NAMEDD_create(&list);
    if (code) {
        printf("%d:::%s:::list_DDTYPE_NAMEDD_create(&list)\n",
               code, LOG_FUNC_NAME);
    }
    test_insert(list);
    test_replace(list);
    test_search(list);
    test_reverse(list);
    test_remove(list);
    test_remove(list);
    test_remove(list);
    code = list_DDTYPE_NAMEDD_destroy(&list);
    if (code) {
        printf("%d:::%s:::list_DDTYPE_NAMEDD_destroy(&list)\n",
               code, LOG_FUNC_NAME);
    }
    printf("********%s********\n", LOG_FUNC_NAME);
}


#endif //LIB_LIST_IMPL_PROTO_H

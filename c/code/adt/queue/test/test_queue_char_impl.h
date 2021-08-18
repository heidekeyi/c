//
// Created by 12131 on 2021/8/1.
//

#ifndef LIB_TEST_QUEUE_CHAR_IMPL_H
#define LIB_TEST_QUEUE_CHAR_IMPL_H

#include <stdio.h>
#include "../../../log/logging.h"
#include "../queue_generic_ui.h"
#include "../queue_generic_impl.h"

QUEUE_INTERFACE_GENERIC(char, char)

QUEUE_IMPLEMENT_GENERIC(char, char)

static void test_push(Queue_char *queue);

static void test_pop(Queue_char *queue);

static void test_queue_char();

static void test_push(Queue_char *queue) {
    int code, size, index;
    char carr[] = {'3', '6', '9', 'a', 'z', 'A', 'Z'};
    int len = (int) (sizeof(carr) / sizeof(*carr));
    printf("********%s********\n", LOG_FUNC_NAME);
    code = queue_char_size(queue, &size);
    if (code) {
        printf("%d:::%s:::queue_char_size(queue, &size)\n",
               code, LOG_FUNC_NAME);
    } else {
        printf("size: %d\n", size);
    }
    for (index = 0; index < len; ++index) {
        code = queue_char_push(queue, carr[index]);
        if (code) {
            printf("%d:::%s:::queue_char_push(queue, carr[index])\n",
                   code, LOG_FUNC_NAME);
            break;
        } else {
            printf("%c\t", carr[index]);
        }
    }
    puts("");
    code = queue_char_size(queue, &size);
    if (code) {
        printf("%d:::%s:::queue_char_size(queue, &size)\n",
               code, LOG_FUNC_NAME);
    } else {
        printf("size: %d\n", size);
    }
    printf("********%s********\n", LOG_FUNC_NAME);
}

static void test_pop(Queue_char *queue) {
    int code, size, index;
    char ch;
    printf("********%s********\n", LOG_FUNC_NAME);
    code = queue_char_size(queue, &size);
    if (code) {
        printf("%d:::%s:::queue_char_size(queue, &size)\n",
               code, LOG_FUNC_NAME);
    } else {
        printf("size: %d\n", size);
    }
    for (index = 0; index < size; ++index) {
        code = queue_char_pop(queue, &ch);
        if (code) {
            printf("%d:::%s:::queue_char_pop(queue, &ch)\n",
                   code, LOG_FUNC_NAME);
            break;
        } else {
            printf("%c\t", ch);
        }
    }
    puts("");
    code = queue_char_size(queue, &size);
    if (code) {
        printf("%d:::%s:::queue_char_size(queue, &size)\n",
               code, LOG_FUNC_NAME);
    } else {
        printf("size: %d\n", size);
    }
    code = queue_char_pop(queue, &ch);
    if (code) {
        printf("%d:::%s:::queue_char_pop(queue, &ch)\n",
               code, LOG_FUNC_NAME);
    } else {
        printf("%c\t", ch);
    }
    printf("********%s********\n", LOG_FUNC_NAME);
}

static void test_queue_char() {
    Queue_char *queue = NULL;
    int code;
    printf("********%s********\n", LOG_FUNC_NAME);
    code = queue_char_create(&queue);
    if (code) {
        printf("%d:::%s:::queue_char_create(&queue)\n",
               code, LOG_FUNC_NAME);
    }
    test_push(queue);
    test_pop(queue);
    code = queue_char_destroy(&queue);
    if (code) {
        printf("%d:::%s:::queue_char_destroy(&queue)\n",
               code, LOG_FUNC_NAME);
    }
    printf("********%s********\n", LOG_FUNC_NAME);
}

#endif //LIB_TEST_QUEUE_CHAR_IMPL_H

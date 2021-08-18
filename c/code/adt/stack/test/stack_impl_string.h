//
// Created by 12131 on 2021/8/1.
//

#ifndef LIB_STACK_IMPL_STRING_H
#define LIB_STACK_IMPL_STRING_H

#include <stdio.h>
#include "../../../log/logging.h"
#include "../stack_generic_ui.h"
#include "../stack_generic_impl.h"

STACK_INTERFACE_GENERIC(char *, string)

STACK_IMPLEMENT_GENERIC(char *, string)


static void test_push(Stack_string *stack);

static void test_pop(Stack_string *stack);

static void test_stack_string();

static void test_push(Stack_string *stack) {
    int code, size, index;
    char *str[] = {"apple", "orange", "man", "php"};
    int len = (int) (sizeof(str) / sizeof(*str));
    printf("********%s********\n", LOG_FUNC_NAME);
    code = stack_string_size(stack, &size);
    if (code) {
        printf("%d:::%s:::stack_string_size(stack, &size)\n",
               code, LOG_FUNC_NAME);
    } else {
        printf("size: %d\n", size);
    }
    for (index = 0; index < len; ++index) {
        code = stack_string_push(stack, str[index]);
        if (code) {
            printf("%d:::%s:::stack_string_push(stack, str[index])\n",
                   code, LOG_FUNC_NAME);
            break;
        } else {
            printf("%s\t", str[index]);
        }
    }
    puts("");
    code = stack_string_size(stack, &size);
    if (code) {
        printf("%d:::%s:::stack_string_size(stack, &size)\n",
               code, LOG_FUNC_NAME);
    } else {
        printf("size: %d\n", size);
    }
    printf("********%s********\n", LOG_FUNC_NAME);
}

static void test_pop(Stack_string *stack) {
    int code, size, index;
    char *str;
    printf("********%s********\n", LOG_FUNC_NAME);
    code = stack_string_size(stack, &size);
    if (code) {
        printf("%d:::%s:::stack_string_size(stack, &size)\n",
               code, LOG_FUNC_NAME);
        return;
    } else {
        printf("size: %d\n", size);
    }
    for (index = 0; index < size; ++index) {
        code = stack_string_pop(stack, &str);
        if (code) {
            printf("%d:::%s:::stack_string_push(stack, str[index])\n",
                   code, LOG_FUNC_NAME);
            break;
        } else {
            printf("%s\t", str);
        }
    }
    puts("");
    code = stack_string_size(stack, &size);
    if (code) {
        printf("%d:::%s:::stack_string_size(stack, &size)\n",
               code, LOG_FUNC_NAME);
    } else {
        printf("size: %d\n", size);
    }
    code = stack_string_pop(stack, &str);
    if (code) {
        printf("%d:::%s:::stack_string_push(stack, str[index])\n",
               code, LOG_FUNC_NAME);
    } else {
        printf("%s\t", str);
    }
    printf("********%s********\n", LOG_FUNC_NAME);
}

static void test_stack_string() {
    Stack_string *stack = NULL;
    int code;
    printf("********%s********\n", LOG_FUNC_NAME);
    code = stack_string_create(&stack);
    if (code) {
        printf("%d:::%s:::stack_string_create(&stack)\n",
               code, LOG_FUNC_NAME);
    }
    test_push(stack);
    test_pop(stack);
    code = stack_string_destroy(&stack);
    if (code) {
        printf("%d:::%s:::stack_string_destroy(&stack)\n",
               code, LOG_FUNC_NAME);
    }
    printf("********%s********\n", LOG_FUNC_NAME);
}

#endif //LIB_STACK_IMPL_STRING_H

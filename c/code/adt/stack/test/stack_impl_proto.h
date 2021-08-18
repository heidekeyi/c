//
// Created by 12131 on 2021/7/31.
//

#ifndef LIB_STACK_IMPL_PROTO_H
#define LIB_STACK_IMPL_PROTO_H

#include <stdio.h>
#include "../../../log/logging.h"
#include "../stack_proto_ui.h"
#include "../stack_proto_impl.h"

static void test_push(Stack *stack);

static void test_pop(Stack *stack);

static void test_stack_proto_static();

static void test_push(Stack *stack) {
    int code, index;
    int ia[] = {21, 18, 15, 12, 9, 6, 3};
    int size = (int) (sizeof(ia) / sizeof(*ia));
    for (index = 0; index < size; ++index) {
        code = stack_DDTYPE_NAMEDD_push(stack, ia[index]);
        if (code) {
            printf("%d:::%s:::stack_DDTYPE_NAMEDD_push(stack, ia[index])\n",
                   code, LOG_FUNC_NAME);
            break;
        }
        printf("%-2d\t", ia[index]);
    }
    puts("");
}

static void test_pop(Stack *stack) {
    int code, index, data, size;
    code = stack_DDTYPE_NAMEDD_size(stack, &size);
    if (code) {
        printf("%d:::%s:::stack_DDTYPE_NAMEDD_size(stack, &size)\n",
               code, LOG_FUNC_NAME);
        return;
    }
    for (index = 0; index < size; ++index) {
        code = stack_DDTYPE_NAMEDD_pop(stack, &data);
        if (code) {
            printf("%d:::%s:::stack_DDTYPE_NAMEDD_push(stack, ia[index])\n",
                   code, LOG_FUNC_NAME);
            break;
        }
        printf("%-2d\t", data);
    }
    puts("");
}

static void test_stack_proto_static() {
    int code;
    Stack *stack = NULL;
    printf("********%s********\n", LOG_FUNC_NAME);
    code = stack_DDTYPE_NAMEDD_create(&stack);
    if (code) {
        printf("%d:::%s:::stack_DDTYPE_NAMEDD_create(&stack)\n",
               code, LOG_FUNC_NAME);
    }
    test_push(stack);
    test_pop(stack);
    code = stack_DDTYPE_NAMEDD_destroy(&stack);
    if (code) {
        printf("%d:::%s:::stack_DDTYPE_NAMEDD_destroy(&stack)\n",
               code, LOG_FUNC_NAME);
    }
    printf("********%s********\n", LOG_FUNC_NAME);
}


#endif //LIB_STACK_IMPL_PROTO_H

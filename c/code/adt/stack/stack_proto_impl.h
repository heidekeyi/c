//
// Created by 12131 on 2021/7/31.
//

#ifndef LIB_STACK_PROTO_IMPL_H
#define LIB_STACK_PROTO_IMPL_H

#include "stdlib.h"
#include "stack_proto_ui.h"
#include "../../log/logging.h"
#include "../../code_error/code_error.h"


#include "../list/list_proto_ui.h"
#include "../list/list_proto_impl.h"

ADT_STACK_LINK int stack_DDTYPE_NAMEDD_create(Stack **stack) {
    int rv;
    List *handle = NULL;
    if (NULL == stack) {
        logging("e", "NULL == stack");
        return CODE_ERROR_ARG_NULL;
    }
    rv = list_DDTYPE_NAMEDD_create(&handle);
    if (rv) {
        logging("e", "%d = list_DDTYPE_NAMEDD_create(&handle=0x%p)",
                rv, &handle);
        return rv;
    }
    *stack = (Stack *) handle;
    return rv;
}

ADT_STACK_LINK int stack_DDTYPE_NAMEDD_destroy(Stack **stack) {
    int rv;
    List **handle = (List **) stack;
    if (NULL == stack) {
        logging("e", "NULL == stack");
        return CODE_ERROR_ARG_NULL;
    }
    if (NULL == *stack) {
        logging("e", "NULL == *stack");
        return CODE_ERROR_ARG_NULL;
    }
    rv = list_DDTYPE_NAMEDD_destroy(handle);
    if (rv) {
        logging("e", "%d = list_DDTYPE_NAMEDD_destroy(handle=0x%p)",
                rv, handle);
        return rv;
    }
    *stack = NULL;
    return rv;
}

ADT_STACK_LINK int stack_DDTYPE_NAMEDD_size(Stack *stack,
                                            int *out_size) {
    int rv;
    int size = 0;
    List *handle = (List *) stack;
    if (NULL == stack) {
        logging("e", "NULL == stack");
        return CODE_ERROR_ARG_NULL;
    }
    if (NULL == out_size) {
        logging("e", "out_size == NULL");
        return CODE_ERROR_ARG_NULL;
    }
    rv = list_DDTYPE_NAMEDD_size(handle, &size);
    if (rv) {
        logging("e",
                "%d = list_DDTYPE_NAMEDD_size(handle=0x%p, &size=0x%p)",
                rv, handle, &size);
        return rv;
    }
    *out_size = size;
    return rv;
}

ADT_STACK_LINK int stack_DDTYPE_NAMEDD_push(Stack *stack,
                                            DATA_TYPE data) {
    int rv;
    List *handle = (List *) stack;
    if (NULL == stack) {
        logging("e", "NULL == stack");
        return CODE_ERROR_ARG_NULL;
    }
    rv = list_DDTYPE_NAMEDD_insert(handle, 0, data, NULL);
    if (rv) {
        logging("e", "%d = list_DDTYPE_NAMEDD_insert("
                     "handle=0x%p, 0, data, NULL)",
                rv, handle);
        return rv;
    }
    return rv;
}

ADT_STACK_LINK int stack_DDTYPE_NAMEDD_pop(Stack *stack,
                                           DATA_TYPE *out_data) {
    int rv, size = 0;
    DATA_TYPE data;
    List *handle = (List *) stack;
    if (NULL == stack) {
        logging("e", "NULL == stack");
        return CODE_ERROR_ARG_NULL;
    }
    if (NULL == out_data) {
        logging("e", "NULL == out_data");
        return CODE_ERROR_ARG_NULL;
    }
    rv = list_DDTYPE_NAMEDD_size(handle, &size);
    if (rv) {
        logging("e",
                "%d = list_DDTYPE_NAMEDD_size(handle=0x%p, &size=0x%p)",
                rv, handle, &size);
        return rv;
    }
    if (size <= 0) {
        logging("e", "stack is empty");
        return CODE_ERROR_ARG_EMPTY_ADT;
    }
    rv = list_DDTYPE_NAMEDD_remove(handle, 0, &data);
    if (rv) {
        logging("e", "%d = list_DDTYPE_NAMEDD_remove("
                     "handle=0x%p, 0, &data=0x%p)",
                rv, handle, &data);
        return rv;
    }
    *out_data = data;
    return rv;
}


#endif //LIB_STACK_PROTO_IMPL_H

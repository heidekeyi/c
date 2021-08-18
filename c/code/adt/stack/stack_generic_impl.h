//
// Created by 12131 on 2021/7/31.
//

#ifndef LIB_STACK_GENERIC_IMPL_H
#define LIB_STACK_GENERIC_IMPL_H

#include "stack_generic_ui.h"
#include "../../log/logging.h"
#include "../../code_error/code_error.h"
#include "../list/list_generic_ui.h"
#include "../list/list_generic_impl.h"

#define STACK_IMPLEMENT_GENERIC(TYPE, NAME)\
\
LIST_INTERFACE_GENERIC(TYPE, NAME)\
\
LIST_IMPLEMENT_GENERIC(TYPE, NAME)\
\
ADT_STACK_LINK int stack_##NAME##_create(Stack_##NAME **stack) {\
    int rv;\
    List_##NAME *handle = NULL;\
    if (NULL == stack) {\
        logging("e", "NULL == stack");\
        return CODE_ERROR_ARG_NULL;\
    }\
    rv = list_##NAME##_create(&handle);\
    if (rv) {\
        logging("e", "%d = list_"#NAME"_create(&handle=0x%p)",\
                rv, &handle);\
        return rv;\
    }\
    *stack = (Stack_##NAME *) handle;\
    return 0;\
}\
\
ADT_STACK_LINK int stack_##NAME##_destroy(Stack_##NAME **stack) {\
    int rv;\
    List_##NAME **handle = (List_##NAME **) stack;\
    if (NULL == stack) {\
        logging("e", "NULL == stack");\
        return CODE_ERROR_ARG_NULL;\
    }\
    if (NULL == *stack) {\
        logging("e", "NULL == *stack");\
        return CODE_ERROR_ARG_NULL;\
    }\
    rv = list_##NAME##_destroy(handle);\
    if (rv) {\
        logging("e", "%d = list_"#NAME"_destroy(handle=0x%p)",\
                rv, handle);\
        return rv;\
    }\
    *stack = NULL;\
    return 0;\
}\
\
ADT_STACK_LINK int stack_##NAME##_size(Stack_##NAME *stack, int *out_size) {\
    int rv;\
    int size = 0;\
    List_##NAME *handle = (List_##NAME *) stack;\
    if (NULL == stack) {\
        logging("e", "NULL == stack");\
        return CODE_ERROR_ARG_NULL;\
    }\
    if (NULL == out_size) {\
        logging("e", "out_size == NULL");\
        return CODE_ERROR_ARG_NULL;\
    }\
    rv = list_##NAME##_size(handle, &size);\
    if (rv) {\
        logging("e",\
                "%d = list_"#NAME"_size(handle=0x%p, &size=0x%p)",\
                rv, handle, &size);\
        return rv;\
    }\
    *out_size = size;\
    return 0;\
}\
\
ADT_STACK_LINK int stack_##NAME##_push(Stack_##NAME *stack, TYPE data) {\
    int rv;\
    List_##NAME *handle = (List_##NAME *) stack;\
    if (NULL == stack) {\
        logging("e", "NULL == stack");\
        return CODE_ERROR_ARG_NULL;\
    }\
    rv = list_##NAME##_insert(handle, 0, data, NULL);\
    if (rv) {\
        logging("e", "%d = list_"#NAME"_insert("\
                     "handle=0x%p, 0, data, NULL)",\
                rv, handle);\
        return rv;\
    }\
    return 0;\
}\
\
ADT_STACK_LINK int stack_##NAME##_pop(Stack_##NAME *stack, TYPE *out_data) {\
    int rv, size = 0;\
    TYPE data;\
    List_##NAME *handle = (List_##NAME *) stack;\
    if (NULL == stack) {\
        logging("e", "NULL == stack");\
        return CODE_ERROR_ARG_NULL;\
    }\
    if (NULL == out_data) {\
        logging("e", "NULL == out_data");\
        return CODE_ERROR_ARG_NULL;\
    }\
    rv = list_##NAME##_size(handle, &size);\
    if (rv) {\
        logging("e",\
                "%d = list_"#NAME"_size(handle=0x%p, &size=0x%p)",\
                rv, handle, &size);\
        return rv;\
    }\
    if (size <= 0) {\
        logging("e", "stack is empty");\
        return CODE_ERROR_ARG_EMPTY_ADT;\
    }\
    rv = list_##NAME##_remove(handle, 0, &data);\
    if (rv) {\
        logging("e", "%d = list_"#NAME"_remove("\
                     "handle=0x%p, 0, &data=0x%p)",\
                rv, handle, &data);\
        return rv;\
    }\
    *out_data = data;\
    return 0;\
}\

#endif //LIB_STACK_GENERIC_IMPL_H

//
// Created by 12131 on 2021/8/1.
//

#ifndef LIB_QUEUE_GENERIC_IMPL_H
#define LIB_QUEUE_GENERIC_IMPL_H


#include "queue_generic_ui.h"
#include "../../log/logging.h"
#include "../../code_error/code_error.h"
#include "../list/list_generic_ui.h"
#include "../list/list_generic_impl.h"

#define QUEUE_IMPLEMENT_GENERIC(TYPE, NAME)\
\
LIST_INTERFACE_GENERIC(TYPE, NAME)\
\
LIST_IMPLEMENT_GENERIC(TYPE, NAME)\
\
ADT_QUEUE_LINK int queue_##NAME##_create(Queue_##NAME **queue) {\
    int rv;\
    List_##NAME *handle = NULL;\
    if (NULL == queue) {\
        logging("e", "NULL == queue");\
        return CODE_ERROR_ARG_NULL;\
    }\
    rv = list_##NAME##_create(&handle);\
    if (rv) {\
        logging("e", "%d = list_"#NAME"_create(&handle=0x%p)",\
                rv, &handle);\
        return rv;\
    }\
    *queue = (Queue_##NAME *) handle;\
    return 0;\
}\
\
ADT_QUEUE_LINK int queue_##NAME##_destroy(Queue_##NAME **queue) {\
    int rv;\
    List_##NAME **handle = (List_##NAME **) queue;\
    if (NULL == queue) {\
        logging("e", "NULL == queue");\
        return CODE_ERROR_ARG_NULL;\
    }\
    if (NULL == *queue) {\
        logging("e", "NULL == *queue");\
        return CODE_ERROR_ARG_NULL;\
    }\
    rv = list_##NAME##_destroy(handle);\
    if (rv) {\
        logging("e", "%d = list_"#NAME"_destroy(handle=0x%p)",\
                rv, handle);\
        return rv;\
    }\
    *queue = NULL;\
    return 0;\
}\
\
ADT_QUEUE_LINK int queue_##NAME##_size(Queue_##NAME *queue, int *out_size) {\
    int rv;\
    int size = 0;\
    List_##NAME *handle = (List_##NAME *) queue;\
    if (NULL == queue) {\
        logging("e", "NULL == queue");\
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
ADT_QUEUE_LINK int queue_##NAME##_push(Queue_##NAME *queue, TYPE data) {\
    int rv;\
    List_##NAME *handle = (List_##NAME *) queue;\
    if (NULL == queue) {\
        logging("e", "NULL == queue");\
        return CODE_ERROR_ARG_NULL;\
    }\
    rv = list_##NAME##_insert(handle, -1, data, NULL);\
    if (rv) {\
        logging("e", "%d = list_"#NAME"_insert("\
                     "handle=0x%p, -1, data, NULL)",\
                rv, handle);\
        return rv;\
    }\
    return 0;\
}\
\
ADT_QUEUE_LINK int queue_##NAME##_pop(Queue_##NAME *queue, TYPE *out_data) {\
    int rv, size = 0;\
    TYPE data;\
    List_##NAME *handle = (List_##NAME *) queue;\
    if (NULL == queue) {\
        logging("e", "NULL == queue");\
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
        logging("e", "queue is empty");\
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


#endif //LIB_QUEUE_GENERIC_IMPL_H

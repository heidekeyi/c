//
// Created by 12131 on 2021/7/31.
//

#ifndef LIB_LIST_GENERIC_LINK_H
#define LIB_LIST_GENERIC_LINK_H


#include <stdlib.h>
#include "list_generic_ui.h"
#include "../../log/logging.h"
#include "../../code_error/code_error.h"


#define LIST_IMPLEMENT_GENERIC(TYPE, NAME)\
\
typedef struct IN_Node_##NAME {\
    TYPE data;\
    struct IN_Node_##NAME *next;\
} Node_##NAME;\
\
typedef struct {\
    void *_;\
    int size;\
    Node_##NAME *node;\
} Handle_##NAME;\
\
/*	pre_node is option	*/\
/*	check index is valid when in_flag_check is not zero	*/\
static int list_##NAME##_node_retrieve(Handle_##NAME *handle,\
                                       int in_index,\
                                       int in_flag_check,\
                                       Node_##NAME **out_node,\
                                       Node_##NAME **out_prev_node);\
\
static int list_##NAME##_node_create(Node_##NAME **out_node);\
\
ADT_LIST_LINK int list_##NAME##_create(List_##NAME **list) {\
    int size = (int) sizeof(Handle_##NAME);\
    Handle_##NAME *handle = NULL;\
    if (NULL == list) {\
        logging("e", "NULL == list");\
        return CODE_ERROR_ARG_NULL;\
    }\
    handle = (Handle_##NAME *) calloc(1, size);\
    if (NULL == handle) {\
        logging("f",\
                "NULL = (Handle_##NAME *) calloc(1, size=%d)",\
                size);\
        return CODE_ERROR_LIB_STD;\
    }\
    *list = (List_##NAME *) handle;\
    return 0;\
}\
\
ADT_LIST_LINK int list_##NAME##_destroy(List_##NAME **list) {\
    Handle_##NAME *handle = NULL;\
    Node_##NAME *node = NULL, *tmp = NULL;\
    if (NULL == list) {\
        logging("e", "NULL == list");\
        return CODE_ERROR_ARG_NULL;\
    }\
    if (NULL == *list) {\
        logging("e", "NULL == *list");\
        return CODE_ERROR_ARG_NULL;\
    }\
    handle = (Handle_##NAME * ) * list;\
    node = handle->node;\
    while (node) {\
        tmp = node->next;\
        free(node);\
        node = tmp;\
    }\
    free(handle);\
    *list = NULL;\
    return 0;\
}\
\
ADT_LIST_LINK int list_##NAME##_size(List_##NAME *list, int *out_size) {\
    Handle_##NAME *handle = (Handle_##NAME *) list;\
    if (NULL == list) {\
        logging("e", "NULL == list");\
        return CODE_ERROR_ARG_NULL;\
    }\
    if (NULL == out_size) {\
        logging("e", "NULL == out_size");\
        return CODE_ERROR_ARG_NULL;\
    }\
    *out_size = handle->size;\
    return 0;\
}\
\
ADT_LIST_LINK int list_##NAME##_search(List_##NAME *list,\
                                       TYPE in_data,\
                                       int *out_index,\
                                       int (*is_match)(\
                                               TYPE data,\
                                               TYPE elem)) {\
    int index = -1, cnt = 0;\
    Handle_##NAME *handle = (Handle_##NAME *) list;\
    Node_##NAME *node = NULL;\
    if (NULL == list) {\
        logging("e", "NULL == list");\
        return CODE_ERROR_ARG_NULL;\
    }\
    if (NULL == out_index) {\
        logging("e", "NULL == out_index");\
        return CODE_ERROR_ARG_NULL;\
    }\
    if (NULL == is_match) {\
        logging("e", "NULL == is_match_int");\
        return CODE_ERROR_ARG_NULL;\
    }\
\
    node = handle->node;\
    while (node) {\
        if ((*is_match)(in_data, node->data)) {\
            index = cnt;\
            break;\
        }\
        node = node->next;\
        ++cnt;\
    }\
    *out_index = index;\
    return 0;\
}\
\
/*	out_index is option	*/\
ADT_LIST_LINK int list_##NAME##_insert(List_##NAME *list,\
                                       int in_index,\
                                       TYPE in_data,\
                                       int *out_index) {\
    int rv;\
    Node_##NAME *new_node = NULL, *node = NULL, *prev_node = NULL;\
    Handle_##NAME *handle = (Handle_##NAME *) list;\
    if (NULL == list) {\
        logging("e", "NULL == list");\
        return CODE_ERROR_ARG_NULL;\
    }\
    if (in_index < 0 || in_index >= handle->size) {\
        in_index = handle->size;\
    }\
\
/*	retrieve relative node	*/\
    rv = list_##NAME##_node_retrieve(handle, in_index, 0,\
                                     &node, &prev_node);\
    if (rv) {\
        logging("e", "%d = list_"#NAME"_node_retrieve("\
                     "handle=0x%p, in_index=%d, 0, "\
                     "&node=0x%p, &prev_node=0x%p)",\
                rv, handle, in_index, &node, &prev_node);\
        return rv;\
    }\
/*	create node	*/\
    rv = list_##NAME##_node_create(&new_node);\
    if (rv) {\
        logging("e",\
                "%d = list_"#NAME"_node_create(&new_node=0x%p)",\
                rv, &new_node);\
        return rv;\
    }\
/*	init node data	*/\
    new_node->data = in_data;\
    new_node->next = node;\
\
/*	add node	*/\
    if (prev_node) {/*	normal node	*/\
        prev_node->next = new_node;\
    } else {/*	head node	*/\
        handle->node = new_node;\
    }\
    ++handle->size;\
\
    if (out_index) {\
        *out_index = in_index;\
    }\
    return 0;\
}\
\
/*	out_data is option	*/\
ADT_LIST_LINK int list_##NAME##_remove(List_##NAME *list,\
                                       int in_index,\
                                       TYPE *out_data) {\
    int rv;\
    Node_##NAME *node = NULL, *prev_node = NULL;\
    Handle_##NAME *handle = (Handle_##NAME *) list;\
/*	retrieve node	*/\
    rv = list_##NAME##_node_retrieve(handle, in_index, 1,\
                                     &node, &prev_node);\
    if (rv) {\
        logging("e", "%d = list_"#NAME"_node_retrieve("\
                     "handle=0x%p, in_index=%d, 1, "\
                     "&node=0x%p, &prev_node=0x%p)",\
                rv, handle, in_index, &node, &prev_node);\
        return rv;\
    }\
\
/*	remove node	*/\
    if (prev_node) {/*	normal node	*/\
        prev_node->next = node->next;\
    } else {/*	head node	*/\
        handle->node = node->next;\
    }\
\
    if (out_data) {\
        *out_data = node->data;\
    }\
    --handle->size;\
    free(node);\
    return 0;\
}\
\
ADT_LIST_LINK int list_##NAME##_peek(List_##NAME *list,\
                                     int in_index,\
                                     TYPE *out_data) {\
    int rv;\
    Handle_##NAME *handle = (Handle_##NAME *) list;\
    Node_##NAME *node = NULL;\
    if (NULL == out_data) {\
        logging("e", "NULL == out_data");\
        return CODE_ERROR_ARG_NULL;\
    }\
/*	retrieve node	*/\
    rv = list_##NAME##_node_retrieve(handle, in_index, 1,\
                                     &node, NULL);\
    if (rv) {\
        logging("e", "%d = list_"#NAME"_node_retrieve("\
                     "handle=0x%p, in_index=%d, 1,"\
                     " &node=0x%p, NULL)",\
                rv, handle, in_index, &node);\
        return rv;\
    }\
    *out_data = node->data;\
    return 0;\
}\
\
/*	out_data is option	*/\
ADT_LIST_LINK int list_##NAME##_replace(List_##NAME *list,\
                                        int in_index,\
                                        TYPE in_data,\
                                        TYPE *out_data) {\
    int rv;\
    Handle_##NAME *handle = (Handle_##NAME *) list;\
    Node_##NAME *node = NULL;\
    rv = list_##NAME##_node_retrieve(handle, in_index, 1,\
                                     &node, NULL);\
    if (rv) {\
        logging("e", "%d = list_"#NAME"_node_retrieve("\
                     "handle=0x%p, in_index=%d, 1, "\
                     "&node=0x%p, NULL)",\
                rv, handle, in_index, &node);\
        return rv;\
    }\
    if (out_data) {\
        *out_data = node->data;\
    }\
    node->data = in_data;\
    return 0;\
}\
\
ADT_LIST_LINK int list_##NAME##_reverse(List_##NAME *list) {\
    Node_##NAME *node = NULL, *tmp = NULL, *head = NULL;\
    Handle_##NAME *handle = (Handle_##NAME *) list;\
    if (NULL == list) {\
        logging("e", "NULL == list");\
        return CODE_ERROR_ARG_NULL;\
    }\
\
    node = handle->node;\
    while (node) {\
        tmp = node->next;\
        if (head) {\
            node->next = head;\
        } else {\
            node->next = NULL;\
        }\
        head = node;\
        node = tmp;\
    }\
    handle->node = head;\
    return 0;\
}\
\
static int list_##NAME##_node_create(Node_##NAME **out_node) {\
    Node_##NAME *node = NULL;\
    int size = (int) sizeof(Node_##NAME);\
    if (NULL == out_node) {\
        logging("e", "NULL == out_node");\
        return CODE_ERROR_ARG_NULL;\
    }\
\
    node = calloc(1, size);\
    if (NULL == node) {\
        logging("f", "NULL = calloc(1, size=%d)", size);\
        return CODE_ERROR_LIB_STD;\
    }\
    *out_node = node;\
    return 0;\
}\
\
\
/*	pre_node is option	*/\
/*	check index is valid when in_flag_check is not zero	*/\
static int list_##NAME##_node_retrieve(Handle_##NAME *handle,\
                                       int in_index,\
                                       int in_flag_check,\
                                       Node_##NAME **out_node,\
                                       Node_##NAME **out_prev_node) {\
    int cnt = 0;\
    Node_##NAME *next_node = NULL, *prev_node = NULL;\
    if (NULL == handle) {\
        logging("e", "NULL == handle");\
        return CODE_ERROR_ARG_NULL;\
    }\
    if (NULL == out_node) {\
        logging("e", "NULL == out_node");\
        return CODE_ERROR_ARG_NULL;\
    }\
\
    if (in_flag_check) {/*	check range of index	*/\
        if (0 == handle->size) {\
            logging("e", "list is empty");\
            return CODE_ERROR_ARG_EMPTY_ADT;\
        }\
        if (in_index < 0 || in_index >= handle->size) {\
            logging("e", "in_index == %d not in [0, %d)",\
                    in_index, handle->size);\
            return CODE_ERROR_ARG_INDEX_OUT_RANGE;\
        }\
    }\
\
/*	position to in_index	*/\
    next_node = handle->node;\
    while (cnt < in_index) {\
        prev_node = next_node;\
        next_node = next_node->next;\
        ++cnt;\
    }\
    *out_node = next_node;\
    if (out_prev_node) {\
        *out_prev_node = prev_node;\
    }\
    return 0;\
}\


#endif //LIB_LIST_GENERIC_IMPLEMENT_LINK_H

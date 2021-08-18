//
// Created by 12131 on 2021/7/31.
//

#ifndef LIB_LIST_GENERIC_UI_H
#define LIB_LIST_GENERIC_UI_H

//#define ADT_LIST_LINK extern

#ifndef ADT_LIST_LINK
#define ADT_LIST_LINK static
#endif


#define LIST_INTERFACE_GENERIC(TYPE, NAME)\
\
typedef struct {\
    TYPE *_;\
} List_##NAME;\
\
ADT_LIST_LINK int list_##NAME##_create(List_##NAME **list);\
\
ADT_LIST_LINK int list_##NAME##_destroy(List_##NAME **list);\
\
ADT_LIST_LINK int list_##NAME##_size(List_##NAME *list, int *out_size);\
\
ADT_LIST_LINK int list_##NAME##_search(List_##NAME *list,\
                                       TYPE in_data,\
                                       int *out_index,\
                                       int (*is_match)(TYPE data,\
                                                       TYPE elem));\
\
/*	in_index: -1 and larger than len is append	*/\
/*	out_index is option	*/\
ADT_LIST_LINK int list_##NAME##_insert(List_##NAME *list,\
                                       int in_index,\
                                       TYPE in_data,\
                                       int *out_index);\
\
/*	out_data is option	*/\
ADT_LIST_LINK int list_##NAME##_remove(List_##NAME *list,\
                                       int in_index,\
                                       TYPE *out_data);\
\
ADT_LIST_LINK int list_##NAME##_peek(List_##NAME *list,\
                                     int in_index,\
                                     TYPE *out_data);\
\
/*	out_data is option	*/\
ADT_LIST_LINK int list_##NAME##_replace(List_##NAME *list,\
                                        int in_index,\
                                        TYPE in_data,\
                                        TYPE *out_data);\
\
ADT_LIST_LINK int list_##NAME##_reverse(List_##NAME *list);

#endif //LIB_LIST_GENERIC_INTERFACE_H

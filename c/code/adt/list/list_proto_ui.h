//
// Created by 12131 on 2021/7/31.
//

#ifndef LIB_LIST_PROTO_UI_H
#define LIB_LIST_PROTO_UI_H

#include "list_type.h"

typedef int DATA_TYPE;


ADT_LIST_LINK int list_DDTYPE_NAMEDD_create(List **list);

ADT_LIST_LINK int list_DDTYPE_NAMEDD_destroy(List **list);

ADT_LIST_LINK int list_DDTYPE_NAMEDD_size(List *list, int *out_size);

ADT_LIST_LINK int list_DDTYPE_NAMEDD_search(List *list,
                                            DATA_TYPE in_data,
                                            int *out_index,
                                            int (*is_match)(DATA_TYPE in_data,
                                                            DATA_TYPE ele_data));

/*	in_index: -1 and larger than len is append	*/
/*	out_index is option	*/
ADT_LIST_LINK int list_DDTYPE_NAMEDD_insert(List *list,
                                            int in_index,
                                            DATA_TYPE in_data,
                                            int *out_index);

/*	out_data is option	*/
ADT_LIST_LINK int list_DDTYPE_NAMEDD_remove(List *list,
                                            int in_index,
                                            DATA_TYPE *out_data);

ADT_LIST_LINK int list_DDTYPE_NAMEDD_peek(List *list,
                                          int in_index,
                                          DATA_TYPE *out_data);

/*	out_data is option	*/
ADT_LIST_LINK int list_DDTYPE_NAMEDD_replace(List *list,
                                             int in_index,
                                             DATA_TYPE in_data,
                                             DATA_TYPE *out_data);

ADT_LIST_LINK int list_DDTYPE_NAMEDD_reverse(List *list);


#endif //LIB_LIST_PROTO_UI_H

//
// Created by 12131 on 2021/7/31.
//

#ifndef LIB_STACK_PROTO_UI_H
#define LIB_STACK_PROTO_UI_H

typedef int DATA_TYPE;

#include "stack_type.h"

ADT_STACK_LINK int stack_DDTYPE_NAMEDD_create(Stack **stack);

ADT_STACK_LINK int stack_DDTYPE_NAMEDD_destroy(Stack **stack);

ADT_STACK_LINK int stack_DDTYPE_NAMEDD_size(Stack *stack,
                                            int *out_size);

ADT_STACK_LINK int stack_DDTYPE_NAMEDD_push(Stack *stack,
                                            DATA_TYPE in_data);

ADT_STACK_LINK int stack_DDTYPE_NAMEDD_pop(Stack *stack,
                                           DATA_TYPE *out_data);


#endif //LIB_STACK_PROTO_UI_H

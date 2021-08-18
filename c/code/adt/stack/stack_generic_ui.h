//
// Created by 12131 on 2021/7/31.
//

#ifndef LIB_STACK_GENERIC_UI_H
#define LIB_STACK_GENERIC_UI_H

//#define ADT_STACK_LINK extern

#ifndef ADT_STACK_LINK
#define ADT_STACK_LINK static
#endif


#define STACK_INTERFACE_GENERIC(TYPE, NAME)\
\
typedef struct {\
    TYPE *_;\
} Stack_##NAME;\
\
ADT_STACK_LINK int stack_##NAME##_create(Stack_##NAME **stack);\
\
ADT_STACK_LINK int stack_##NAME##_destroy(Stack_##NAME **stack);\
\
ADT_STACK_LINK int stack_##NAME##_size(Stack_##NAME *stack, int *out_size);\
\
ADT_STACK_LINK int stack_##NAME##_push(Stack_##NAME *stack, TYPE in_data);\
\
ADT_STACK_LINK int stack_##NAME##_pop(Stack_##NAME *stack, TYPE *out_data);\


#endif //LIB_STACK_GENERIC_UI_H

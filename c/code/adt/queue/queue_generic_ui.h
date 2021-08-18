//
// Created by 12131 on 2021/8/1.
//

#ifndef LIB_QUEUE_GENERIC_UI_H
#define LIB_QUEUE_GENERIC_UI_H


//#define ADT_QUEUE_LINK extern

#ifndef ADT_QUEUE_LINK
#define ADT_QUEUE_LINK static
#endif


#define QUEUE_INTERFACE_GENERIC(TYPE, NAME)\
\
typedef struct {\
    TYPE *_;\
} Queue_##NAME;\
\
ADT_QUEUE_LINK int queue_##NAME##_create(Queue_##NAME **queue);\
\
ADT_QUEUE_LINK int queue_##NAME##_destroy(Queue_##NAME **queue);\
\
ADT_QUEUE_LINK int queue_##NAME##_size(Queue_##NAME *queue, int *out_size);\
\
ADT_QUEUE_LINK int queue_##NAME##_push(Queue_##NAME *queue, TYPE in_data);\
\
ADT_QUEUE_LINK int queue_##NAME##_pop(Queue_##NAME *queue, TYPE *out_data);\


#endif //LIB_QUEUE_GENERIC_UI_H

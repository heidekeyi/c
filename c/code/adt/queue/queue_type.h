//
// Created by 12131 on 2021/8/1.
//

#ifndef LIB_QUEUE_TYPE_H
#define LIB_QUEUE_TYPE_H

#ifndef QUEUE_TYPE_HANDLE_INSTANCE
#define QUEUE_TYPE_HANDLE_INSTANCE
typedef struct {
    void *_;
} Queue;
#endif

#ifdef ADT_QUEUE_LINK_EXTERN
#define ADT_QUEUE_LINK extern
#else
#define ADT_QUEUE_LINK static
#endif

#endif //LIB_QUEUE_TYPE_H

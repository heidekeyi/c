//
// Created by 12131 on 2021/7/31.
//

#ifndef LIB_STACK_TYPE_H
#define LIB_STACK_TYPE_H

#ifndef STACK_TYPE_HANDLE_INSTANCE
#define STACK_TYPE_HANDLE_INSTANCE
typedef struct {
    void *_;
} Stack;
#endif

#ifdef ADT_STACK_LINK_EXTERN
#define ADT_STACK_LINK extern
#else
#define ADT_STACK_LINK static
#endif

#endif //LIB_STACK_TYPE_H

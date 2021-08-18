//
// Created by 12131 on 2021/7/31.
//

#ifndef LIB_LIST_TYPE_H
#define LIB_LIST_TYPE_H

#ifndef LIST_TYPE_HANDLE_INSTANCE
#define LIST_TYPE_HANDLE_INSTANCE
typedef struct {
    void *_;
} List;
#endif

#ifdef ADT_LIST_LINK_EXTERN
#define ADT_LIST_LINK extern
#else
#define ADT_LIST_LINK static
#endif

#endif //LIB_LIST_TYPE_H

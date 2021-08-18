//
// Created by 12131 on 2021/7/31.
//

#ifndef LIB_LIST_GENERIC_IMPL_H
#define LIB_LIST_GENERIC_IMPL_H

//#define ADT_LIST_TYPE_LINK_EXTERN_EXIST_INSTANCE
#ifndef ADT_LIST_TYPE_LINK_EXTERN_EXIST_INSTANCE
//#define ADT_USE_SEQUENCE_STORAGE
#ifndef ADT_USE_SEQUENCE_STORAGE

#include "list_generic_link.h"

#else

#include "list_generic_seq.h"

#endif //ADT_USE_SEQUENCE_STORAGE

#else
#define LIST_IMPLEMENT_GENERIC(TYPE, NAME)
#endif

#endif //LIB_LIST_GENERIC_IMPLEMENT_H


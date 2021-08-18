//
// Created by 12131 on 2021/7/31.
//

#ifndef LIB_LIST_PROTO_IMPL_H
#define LIB_LIST_PROTO_IMPL_H

#include "list_proto_ui.h"


//#define ADT_LIST_USE_SEQUENCE
#ifdef ADT_LIST_USE_SEQUENCE

#include "list_proto_seq.h"

#else

#include "list_proto_link.h"

#endif

#endif


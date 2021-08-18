//
// Created by 12131 on 2021/8/1.
//

#include "test_stack_string_static.h"

#define ADT_LIST_TYPE_LINK_EXTERN_EXIST_INSTANCE
#define ADT_LIST_LINK extern
//#define ADT_STACK_LINK extern

#include "stack_impl_string.h"


void test_stack_string_static(void) {
    test_stack_string();
}


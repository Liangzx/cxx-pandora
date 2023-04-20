#include <stdio.h>

#include "st.h"

void set_static_val(int v) {
    static_val = v;
    printf("set_static_val: val=%d ptr=%p\n", static_val, &static_val);
}

int get_static_val() {
    printf("set_static_val: val=%d ptr=%p\n", static_val, &static_val);
    return static_val;
}

void set_global_val(int v) {
    global_val = v;
    printf("set_global_val: val=%d ptr=%p\n", global_val, &global_val);
}

int get_global_val() {
    printf("global_val: val=%d ptr=%p\n", global_val, &global_val);
    return global_val;
}

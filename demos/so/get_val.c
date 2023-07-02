#include <unistd.h>
#include <stdio.h>

#include "st.h"

int main() {
    for (int i = 0; i < 100; i++) {

        printf("---------------------------\n");
        printf("static_val=%d global_val=%d\n", get_static_val(), get_global_val());

        sleep(2);
    }
}
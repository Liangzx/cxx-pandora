#include <unistd.h>
#include "st.h"

int main()
{
    for(int i = 0; i <100; i++)
    {
        set_static_val(2 * i);
        set_global_val(i);

        sleep(2);
    }
}
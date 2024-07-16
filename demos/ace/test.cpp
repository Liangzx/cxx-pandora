#include <ace/Log_Msg.h>

int main() {
    int a = 1;
    ACE_ERROR((LM_DEBUG, "hello,%@", &a));

    return 0;
}

#include "ptest.h"

void
empty_test(void){
    return;
}

int
main(int argv , char * args[])
{
    if(argv > 1){
        void (*fun[])(void) = {
            empty_test
        };
        putestInitWithFuncs(fun , *args[1]);
    }
    return 0;
}












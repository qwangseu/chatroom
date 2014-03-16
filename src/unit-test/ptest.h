#ifndef __PTEST_H__
#define __PTEST_H__

#include "../app.h"

void
appTestInit(){
    initServerConfig();
    return;
}

void
putestInitWithFuncs(void (**_fun)(void) , int index){
    appTestInit();
    
    _fun[index - 48](); 

    return ;
}

#endif


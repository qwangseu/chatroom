#include "config.h"
#include "../app.h"

/*
 * 退出程序，清理app_server里面的内存
 */
static void
exitHandleFromInitServerConfig(void){
    return;
}

/*
 * 初始化 server 结构
 */
void 
initServerConfig(void) {
    
    app_server.verbosity = APP_WARNING;
  
    app_server.bug_report_start = 0;

    atexit(exitHandleFromInitServerConfig);
}



















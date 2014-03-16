#ifndef __LOG_H__
#define __LOG_H__

#include <ucontext.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>


/* Log levels */
#define APP_DEBUG     0
#define APP_VERBOSE   1
#define APP_NOTICE    2
#define APP_WARNING   3
#define APP_LOG_RAW   (1<<10) /* Modifier to log without timestamp */


#define APP_MAX_LOGMSG_LEN    1024 /* Default maximum length of syslog messages */

void appLog(int level , const char *fmt , ...);
void appLogRaw(int level, const char *msg);



#endif



















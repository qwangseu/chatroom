#ifndef __APP_H__
#define __APP_H__

#include "common.h"

#include "rapp/config.h"
#include "rapp/log.h"

struct appServer{
   
    int verbosity;           /* Loglevel in redis.conf */

    /* Logging */
    char * logfile;          /* Path of log file */

    /* Assert &bug reportign*/
    int bug_report_start;    /* True if bug report header was already logged.*/
};

struct appServer app_server;

/* debug */
void logStackTrace(ucontext_t *uc);
void _appPanic(char *msg , char *file , int line);
void bugReportStart(void);

#endif


















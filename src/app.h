#ifndef __APP_H__
#define __APP_H__

#include "common.h"

#include "rapp/config.h"
#include "rapp/log.h"
#include "rapp/socket_server.h"

#define MIN_CLIENT_FDSET 1024

#define likely(x)   (__builtin_expect(!!(x) , 1))
#define unlikely(x) (__builtin_expect(!!(x) , 0))

struct appServer{
   
    int verbosity;           /* Loglevel in redis.conf */

    /* Logging */
    char * logfile;          /* Path of log file */

    /* Assert &bug reportign*/
    int bug_report_start;    /* True if bug report header was already logged.*/
};

struct fdSet{
    int size; //set[] size
    int used;
    int set[0];
};

struct appServer app_server;
struct fdSet * client_fdset;
struct socket_server * global_ss;

/* debug */
void logStackTrace(ucontext_t *uc);
void _appPanic(char *msg , char *file , int line);
void bugReportStart(void);

#endif


















#include "log.h"
#include "../app.h"

/*============================ Utility functions ============================ */

/* Low level logging. To use only for very big messages, otherwise
 * redisLog() is to prefer. */
void 
appLogRaw(int level, const char *msg) {
    const char *c = ".-*#";
    FILE *fp;
    char buf[64];
    int rawmode = (level & APP_LOG_RAW);

    level &= 0xff; /* clear flags */
    if (level < app_server.verbosity) return;

    fp = (app_server.logfile == NULL) ? stdout : fopen(app_server.logfile,"a");
    if (!fp) return;

    if (rawmode) {
        fprintf(fp,"%s\n",msg);
    } else {
        int off;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        off = strftime(buf,sizeof(buf),"%d %b %H:%M:%S.",localtime(&tv.tv_sec));
        snprintf(buf+off,sizeof(buf)-off,"%03d",(int)tv.tv_usec/1000);
        fprintf(fp,"[%d] %s %c %s\n",(int)getpid(),buf,c[level],msg);
    }
    fflush(fp);

    if (app_server.logfile) fclose(fp);

}


void
appLog(int level , const char *fmt , ...){
    va_list ap;
    char msg[APP_MAX_LOGMSG_LEN];
    if((level&0xff) < app_server.verbosity) return;

    va_start(ap , fmt);
    vsnprintf(msg , sizeof(msg) , fmt ,ap);
    va_end(ap);
    
    appLogRaw(level , msg);
}























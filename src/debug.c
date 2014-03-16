#include "app.h"
#include <execinfo.h>

#ifdef HAVE_BACKTRACE
static void *
getMcontextEip(ucontext_t *uc) {
#if defined(__APPLE__) && !defined(MAC_OS_X_VERSION_10_6)
    /* OSX < 10.6 */
#if defined(__x86_64__)
    return (void*) uc->uc_mcontext->__ss.__rip;
#elif defined(__i386__)
    return (void*) uc->uc_mcontext->__ss.__eip;
    #else
    return (void*) uc->uc_mcontext->__ss.__srr0;
    #endif
#elif defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6)
    /* OSX >= 10.6 */
#if defined(_STRUCT_X86_THREAD_STATE64) && !defined(__i386__)
    return (void*) uc->uc_mcontext->__ss.__rip;
    #else
    return (void*) uc->uc_mcontext->__ss.__eip;
    #endif
#elif defined(__linux__)
    /* Linux */
#if defined(__i386__)
    return (void*) uc->uc_mcontext.gregs[14]; /* Linux 32 */
#elif defined(__X86_64__) || defined(__x86_64__)
    return (void*) uc->uc_mcontext.gregs[16]; /* Linux 64 */
#elif defined(__ia64__) /* Linux IA64 */
    return (void*) uc->uc_mcontext.sc_ip;
    #endif
#else
    return NULL;
#endif
}


/* Logs the stack trace using the backtrace() call. This function is designed
 * to be called from signal handlers safely. */
void 
logStackTrace(ucontext_t *uc) {
    void *trace[100];
    int trace_size = 0, fd;

    /* Open the log file in append mode. */
    fd = app_server.logfile ?
        open(app_server.logfile, O_APPEND|O_CREAT|O_WRONLY, 0644) :
        STDOUT_FILENO;
    if (fd == -1) return;

    /* Generate the stack trace */
    trace_size = backtrace(trace, 100);

    /* overwrite sigaction with caller's address */
    if (getMcontextEip(uc) != NULL)
        trace[1] = getMcontextEip(uc);

    /* Write symbols to log file */
    backtrace_symbols_fd(trace, trace_size, fd);

    /* Cleanup */
    if (app_server.logfile) close(fd);
}

#endif /* HAVE_BACKTRACE */

void
_appPanic(char *msg , char *file , int line){
    bugReportStart();
    
    appLog(APP_WARNING , "------------------------------------------");
    appLog(APP_WARNING , "!!! Software Failure.");
    appLog(APP_WARNING , "Guru Meditation: %s # %s:%d" , msg,file,line);
#ifdef HAVE_BACKTRACE
    appLog(APP_WARNING , "forcing SIGSEGV in order to print the stack trace");
#endif
    appLog(APP_WARNING , "------------------------------------------");
    
    *((char *)-1) = 'x';
}

void 
bugReportStart(void){
    if(app_server.bug_report_start == 0){
       appLog(APP_WARNING ,
               "\n\n=== APP BUG REPORT START:Cut & paste starting from here ===");
        app_server.bug_report_start = 1;
    }
}

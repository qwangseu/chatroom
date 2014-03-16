#ifndef __CONFIG_H__
#define __CONFIG_H__

/* For proc filesystem */
#ifdef __linux__
#define HAVE_PROC_STAT 1
#define HAVE_PROC_MAPS 1
#define HAVE_PROC_SMAPS 1
#endif

/* For polling API */
#ifdef __linux__
#define HAVE_EPOLL 1
#endif

#if defined(__APPLE__) || defined(__linux__) || defined(__sun)
#define HAVE_BACKTRACE 1
#endif

#ifndef strnicmp
#define strnicmp strncasecmp
#endif

#define NOT_COMMENT(c)  (c!=';' && c!='#')  /* not comment line */

void initServerConfig(void);

#endif



















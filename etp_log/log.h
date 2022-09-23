#ifndef LOG_H
#define LOG_H
#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#ifdef __cplusplus
extern "C"{
#endif
int mkdirs(const char * path, const mode_t mode, const int fail_on_exist);
void logInit(const char *logPath);
void extrapidLog(const int logLevel, const char *moduleName, const char *fmt, ...);
#ifdef __cplusplus
}
#endif
#endif

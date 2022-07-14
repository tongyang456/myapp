/*
 * writelog.h
*/
#include "../frame/pub.h"

#ifndef WRITE_LOG_H_
#define WRITE_LOG_H_

void writelog(const char *func, const char *file, const int line, const char *type, const char *format, ...);

#endif

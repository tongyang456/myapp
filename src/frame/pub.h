/*
 * pub.h 提供了程序运行基本结构
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include <dirent.h>
#include <ctype.h>
#include <assert.h>

#ifndef PUB_H_
#define PUB_H_

#define BUFSIZE 8192
#define CONFIG_FILENAME "../config/config.ini"
#define WRITELOG(type, format, ...) writelog(__func__, __FILE__, __LINE__, type, format, ##__VA_ARGS__)
#include "../frame/base.h"
#endif 

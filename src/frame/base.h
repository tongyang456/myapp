/*
 * base.h
 */
#ifndef BASE_H_
#define BASE_H_

#include "pub.h"

char g_dbhost[12+1];
char g_dbuser[4+1];
char g_dbpassword[32+1];
char g_dbname[12+1];
char g_dbport[5+1];
char g_root_path[128+1];

int getGetMysqlConf(void);
int getBaseConf(void);
void setdaemon();
void catch_Signal(int Sign); //对捕捉到的信号做处理
int Signal(int signo, void (*func)(int));//信号捕捉
#endif 

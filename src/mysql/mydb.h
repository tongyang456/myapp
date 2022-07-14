#ifndef MYDB_H_
#define MYDB_H_

#include "../frame/pub.h"
#include <mysql/mysql.h>
#include "db_ap_index_info.h"
#include "db_sys_user.h"
 
#define SOFTAC_AP_INDEX_INFO_TABLE "ap_index_info"
#define TIMESTAMP_LENGTH 40
#define SUCCESS 0
#define FAILURE -1 
#define SIMPLE_STR_LEN 64 
#define FORMAT_TIME_STR "%Y-%m-%d %H:%M:%S"

/* 格式化时间转换函数 */
int get_db_format_time_str_from_tm(struct tm* timeinfo,char* format_time);
int get_db_tm_from_format_time(char* format_time,struct tm* timeinfo);

/* 数据库接口，包含了添加，删除，插入等操作 */
int db_do_query_2(MYSQL* mysql,const char* query,int(*call_back)(void*,MYSQL_RES*,int,int),void* result);
MYSQL* db_get_database_connection(MYSQL* mysql,const char* host,const char* user, const char* passwd,const char* db_name, const char *db_port);
int db_put_database_connection(MYSQL* mysql);
MYSQL* db_connect_to_database(MYSQL* mysql,const char* host,const char* user, const char* passwd,const char* db_name, const char *db_port);
int db_disconnect_to_database(MYSQL* mysql);
char *delLastcomma(char *tmpSetsql);
#endif /* MYDB_H_ */

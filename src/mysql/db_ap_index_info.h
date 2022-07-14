#ifndef DB_AP_INDEX_INFO_H_
#define DB_AP_INDEX_INFO_H_

#include "mydb.h"
#define SOFTAC_AP_INDEX_INFO_TABLE "ap_index_info"
#define TIMESTAMP_LENGTH 40

//ap_index_info
typedef struct ap_index_info{
    int signal_intensity;
    int down_up_BW;
    int time_delay;
    char collect_timestamp[TIMESTAMP_LENGTH];
}ap_info;

typedef struct query_ap_index_info{
    int num_columns;
    int num_rows;
    int cur_row;
    ap_info* apInfo;
}query_ap_info;

int db_get_ap_index_info(query_ap_info* query_info);
int db_add_ap_index_info(ap_info* ap);
int db_delete_ap_index_info_before_time(char* timestamp);
int db_delete_ap_index_info_after_time(char* timestamp);
int db_update_ap_index_info_by_id(char* orgId, char *newId);
int call_back_get_ap_index_info(void* arg,MYSQL_RES* resultSet,int num_fields,int num_rows);
#endif

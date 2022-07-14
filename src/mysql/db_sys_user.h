#ifndef DB_SYS_USER_H_
#define DB_SYS_USER_H_

#include "mydb.h"
#define SOFT_SYS_USER_TABLE "sys_user"
#define TIMESTAMP_LENGTH 40

//sys_user
typedef struct sys_user{
    int id;
    char user[64+1];
    char password[64+1];
    char phone[12+1];
    char createtimestamp[TIMESTAMP_LENGTH];
}sys_user_t;

typedef struct query_sys_user{
    int num_columns;
    int num_rows;
    int cur_row;
    sys_user_t* tsysUser;
}query_sys_user_t;

int db_get_sys_user(query_sys_user_t* t_query_sys_user, sys_user_t* t_sys_user);
int db_add_sys_user(sys_user_t* t_sys_user);
int db_delete_sys_user(sys_user_t* t_sys_user);
int db_update_sys_user(sys_user_t* t_sys_user_org, sys_user_t* t_sys_user_new);
int call_back_get_sys_user(void* arg,MYSQL_RES* resultSet,int num_fields,int num_rows);
#endif

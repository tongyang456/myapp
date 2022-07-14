#include "db_ap_index_info.h"

/*
 * 查询数据库表ap_index_info的接口，即用户调用的入口。
 *param:query_info,类型为结构体query_ap_info，输出对应的结果集 
 *return:FAILURE -1,0 means SUCCESS
 */
int db_get_ap_index_info(query_ap_info* query_info)
{
    MYSQL mysql;
    char* prepStmt = "select * from %s;";
    char queryStmt[256] = {0};
    int ret = 0;

    db_get_database_connection(&mysql,g_dbhost,g_dbuser,g_dbpassword,g_dbname,g_dbport);
    sprintf(queryStmt,prepStmt,SOFTAC_AP_INDEX_INFO_TABLE);
    WRITELOG("LOG_DEBUG", "queryStmt = [%s]", queryStmt);
    ret = db_do_query_2(&mysql,queryStmt,call_back_get_ap_index_info,query_info);

    db_put_database_connection(&mysql);
    if(ret > 0)
    {
        return SUCCESS;
    }

    return FAILURE;
}

/*
 *数据库表ap_index_info中增加一条记录
 *param:ap,类型为结构体ap_info,表示输入表对应的结构体类型数据
 *return:0 means success,-1 means failure
 */
int db_add_ap_index_info(ap_info* ap)
{
    MYSQL mysql;
    char* prepStmt = "insert into %s values(NULL,%d,%d,%d,'%s');";
    char* queryStmt = (char*)malloc(2048);
    int ret = 0;

    if( NULL == ap || NULL == queryStmt )
    {
        if(queryStmt)
        {
            free(queryStmt);
        }
        return FAILURE;
    }
    //连接数据库
    db_get_database_connection(&mysql,g_dbhost,g_dbuser,g_dbpassword,g_dbname,g_dbport);

    //填充数据库插入语句
    sprintf(queryStmt,prepStmt,SOFTAC_AP_INDEX_INFO_TABLE,ap->signal_intensity,ap->down_up_BW,ap->time_delay,ap->collect_timestamp);
    WRITELOG("LOG_DEBUG", "queryStmt = [%s]", queryStmt);

    //执行操作
    ret = db_do_query_2(&mysql,queryStmt,NULL,NULL);
    db_put_database_connection(&mysql);
    free(queryStmt);
    if(ret > 0)
    {
        return SUCCESS;
    }
    return FAILURE;
}

/*
 *删除表格ap_index_info中的时间点timestamp之前的记录
 *param:timestamp格式化的时间字符串，输入参数，也是过滤条件
 *return:0 means success,-1 means failure
 */
int db_delete_ap_index_info_before_time(char* timestamp)
{

    MYSQL mysql;
    char* prepStmt = "delete from %s where collect_timestamp < '%s';";
    char queryStmt[256] = {0};
    int ret = 0;

    db_get_database_connection(&mysql,g_dbhost,g_dbuser,g_dbpassword,g_dbname,g_dbport);
    sprintf(queryStmt,prepStmt,SOFTAC_AP_INDEX_INFO_TABLE,timestamp);
    WRITELOG("LOG_DEBUG", "queryStmt = [%s]", queryStmt);
    ret = db_do_query_2(&mysql,queryStmt,NULL,NULL);

    db_put_database_connection(&mysql);
    if( ret > 0 )
    {
        return SUCCESS;
    }
    return FAILURE;
}

/*
 *删除表格ap_index_info中的时间点timestamp之后的记录
 *param:timestamp格式化的时间字符串，输入参数，也是过滤条件
 *return:0 means success,-1 means failure
 */
int db_delete_ap_index_info_after_time(char* timestamp)
{
    MYSQL mysql;
    char* prepStmt = "delete from %s where collect_timestamp > '%s';";
    char queryStmt[256] = {0};
    int ret = 0;

    db_get_database_connection(&mysql,g_dbhost,g_dbuser,g_dbpassword,g_dbname,g_dbport);
    sprintf(queryStmt,prepStmt,SOFTAC_AP_INDEX_INFO_TABLE,timestamp);
    ret = db_do_query_2(&mysql,queryStmt,NULL,NULL);

    db_put_database_connection(&mysql);
    if( ret > 0 )
    {
        return SUCCESS;
    }
    return FAILURE;
}

/*
 * 更新byId
 */
int db_update_ap_index_info_by_id(char* orgId, char *newId)
{
    MYSQL mysql;
    char* prepStmt = "update %s set _id='%s' where _id='%s';";
    char queryStmt[256] = {0};
    int ret = 0;

    db_get_database_connection(&mysql,g_dbhost,g_dbuser,g_dbpassword,g_dbname,g_dbport);
    sprintf(queryStmt,prepStmt,SOFTAC_AP_INDEX_INFO_TABLE,orgId,newId);
    ret = db_do_query_2(&mysql,queryStmt,NULL,NULL);

    db_put_database_connection(&mysql);
    if( ret > 0 )
    {
        return ret;
    }
    return FAILURE;
}

/*
 * 处理数据库表ap_index_info的回调函数
 * param:arg,对应的返回类型，是函数的输出变量
 * param:resultSet,对应查询表的结果集
 * param:num_fields,对应查询表的字段数
 * param:num_rows,查询结果的数据库记录行数
 * return:返回值，0为成功，-1为失败
 */
int call_back_get_ap_index_info(void* arg,MYSQL_RES* resultSet,int num_fields,int num_rows)
{
    query_ap_info* result = (query_ap_info*)arg;
    MYSQL_ROW row;
    if( result == NULL )
    {
        WRITELOG("LOG_DEBUG","%s function %d bad input parameter.\n",__func__,__LINE__);
        return FAILURE;
    }
    if(result && (result->apInfo == NULL) && num_fields > 0 && num_rows > 0)
    {
        result->apInfo = (ap_info*)malloc(num_rows*sizeof(ap_info));
        if( NULL == result->apInfo )
        {
            WRITELOG("LOG_ERR","%s function %d memory malloc failed.\n",__func__,__LINE__);
            return FAILURE;
        }

        memset(result->apInfo,0,num_rows*sizeof(ap_info));
        result->num_columns = num_fields;
        result->num_rows = num_rows;
        result->cur_row = 0;
    }

    if(result->apInfo != NULL && result->cur_row < result->num_rows )
    {
        while(( row = mysql_fetch_row(resultSet) ) != NULL)
        {
            //printf("%d\n",result->cur_row);
            if(row[1])
            {
                result->apInfo[result->cur_row].signal_intensity = atoi(row[1]);
            }
            else
            {
                result->apInfo[result->cur_row].signal_intensity = 0;
            }
            printf("Field[1]:%d\t",result->apInfo[result->cur_row].signal_intensity);
            if(row[2])
            {
                result->apInfo[result->cur_row].down_up_BW = atoi(row[2]);
            }
            else
            {
                result->apInfo[result->cur_row].down_up_BW = 0;
            }
            printf("Field[2]:%d\t",result->apInfo[result->cur_row].down_up_BW);
            if(row[3])
            {
                result->apInfo[result->cur_row].time_delay = atoi(row[3]);
            }
            else
            {
                result->apInfo[result->cur_row].time_delay = 0;
            }
            printf("Field[3]:%d\t",result->apInfo[result->cur_row].time_delay);
            if(row[4])
            {
                strcpy(result->apInfo[result->cur_row].collect_timestamp,row[4]);
            }
            else
            {
                strcpy(result->apInfo[result->cur_row].collect_timestamp,"");
            }
            printf("Field[4]:%s\n",row[4]);
            result->cur_row++;
        }
        return SUCCESS;
    }
   // result = (void*)ap;
   // free(ap);
    return FAILURE;
}


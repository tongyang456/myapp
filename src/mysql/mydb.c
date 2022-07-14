#include "mydb.h"
 
/*
 *从结构体struct tm中转换为格式化的时间字符串
 *param:timeinfo,输入参数。
 *param:format_tiem_str,输出参数，格式如:"%Y-%m-%d %H:%M:%S"
 *return:返回值0为成功，-1为失败
 */
int get_db_format_time_str_from_tm(struct tm* timeinfo,char*format_time_str)
{
    if( timeinfo == NULL || format_time_str == NULL )
    {
      WRITELOG("LOG_DEBUG","%s bad input parameters timeinfo %p and format_time %p\n",__func__,timeinfo,format_time_str);
      return FAILURE;
    }
    strftime(format_time_str,SIMPLE_STR_LEN,FORMAT_TIME_STR,timeinfo);
    return SUCCESS;
}

/*
 *it's  from format_time_str to struct tm or time_t
 *input parameter:format_time_str
 *output parameter:struct tm* timeinfo
 *return: 0 means success ,-1 means failure
 */
int get_db_tm_from_format_time(char* format_time_str,struct tm* timeinfo)
{
    if( timeinfo == NULL || format_time_str == NULL )
    {
       WRITELOG("LOG_DEBUG","%s bad input parameters timeinfo %p and format_time_str %p\n",__func__,timeinfo,format_time_str);
       return FAILURE;
    }

    if(strptime(format_time_str,FORMAT_TIME_STR,timeinfo))
    {
        return SUCCESS;
    }
    return FAILURE;
}

/*
 *To get format time function,eg:2014-12-15 22:43:55
 *parameter:
 *parameter:
 *returned: format time string
 */
int get_db_format_time_from_tm(struct tm* timeinfo,char* format_time)
{
    if( timeinfo == NULL || format_time == NULL )
    {
      WRITELOG("LOG_ERR","%s bad input parameters timeinfo %p and format_time %p\n",__func__,timeinfo,format_time);
      return FAILURE;
    }

    snprintf(format_time,SIMPLE_STR_LEN-1,"%4d-%02d-%02d %02d:%02d:%02d",1900+timeinfo->tm_year,timeinfo->tm_mon+1,timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
    return SUCCESS;
}

/*a common interface for mysql's operation
 * param:mysql is a handle for mysql
 * param:query, a SQL sentence ,for select ,update ,insert and so on.
 * param: call_back , a function to handle with data
 */
int db_do_query_2(MYSQL* mysql,const char* query,int(*call_back)(void*,MYSQL_RES*,int,int),void* result)
{
    int affected_rows = 0;
    if( mysql_query(mysql,query) != 0)
    {
       WRITELOG("LOG_DEBUG","Database query failed:%s [%s]\n",mysql_error(mysql),query);
       return FAILURE;
    }

    if( mysql_field_count(mysql) > 0 )
    {
       //执行有返回数据集的SQL语句
       MYSQL_RES* res = NULL; // to store result set
       int num_fields = 0;//the number of the filed
       int num_rows = 0;// the number of the row
       if( !(res = mysql_store_result(mysql)))
       {
           WRITELOG("LOG_DEBUG","mysql_store_result() happened error:%s",mysql_error(mysql));
           return FAILURE;
       }
       num_fields = mysql_num_fields(res);
       num_rows = mysql_num_rows(res);
       WRITELOG("LOG_DEBUG","num_fields = [%d] num_rows=[%d]",num_fields ,num_rows);
       if(call_back)
       {
            call_back(result,res,num_fields,num_rows);
       }

       mysql_free_result(res);
       return num_rows;
    }
    else
    {
        //执行没有返回数据集的SQL语句
        affected_rows = mysql_affected_rows(mysql);
        WRITELOG("LOG_DEBUG"," Operation %s affected rows %d\n.",query,affected_rows);
        return affected_rows;
    }
}

MYSQL* db_get_database_connection(MYSQL* mysql,const char* host,const char* user, const char* passwd,const char* db_name, const char *db_port)
{
    return db_connect_to_database(mysql,host,user,passwd,db_name,db_port);
}

int db_put_database_connection(MYSQL* mysql)
{
   return db_disconnect_to_database(mysql);
}

/*
 * close mysql
 */
int db_disconnect_to_database(MYSQL* mysql)
{
   if(mysql)
   {
       mysql_close(mysql);
       return SUCCESS;
   }
   WRITELOG("LOG_ERR","Disconnected mysql database. Mysql maybe null\n");
   return FAILURE;
}

/*
 *just to connect to dabase
 *param:mysql, a handle of mysql
 *param:host,means mysqlserver's hostname.default localhost
 *param:user,mysql's username
 *param:passwd
 *param:db_name,means database's name you will access.
 *return: a global mysql descriptor
 *
 */
MYSQL* db_connect_to_database(MYSQL* mysql,const char* host,const char* user,
                            const char* passwd,const char* db_name, const char* db_port)
{
    MYSQL* mysql_ptr = NULL;
    if( !db_name )
    {
        WRITELOG("LOG_ERR","DB NAME  error\n");
        return NULL;
    }

    mysql_ptr = mysql_init(mysql);
    if(mysql_ptr == NULL)
    {
        WRITELOG("LOG_ERR","Initlize MySQL failed\n");
        return NULL;
    }

    if(!mysql_real_connect(mysql_ptr,host,user,passwd,db_name,db_port,NULL,0))
    {
        WRITELOG("LOG_ERR","Cannot connect to database\n");
        mysql_error(mysql_ptr);
        return NULL;
    }
    return mysql_ptr;

}

char *delLastcomma(char *tmpSetsql){
    int len = strlen(tmpSetsql);
    while(len > 0){
       if(tmpSetsql[len] == ','){
           tmpSetsql[len] = ' ';
           break;
       }else{
           len--;
       }
    }
    return tmpSetsql;
}

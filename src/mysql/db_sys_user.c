#include "db_sys_user.h"

int db_get_sys_user(query_sys_user_t* t_query_sys_user, sys_user_t* t_sys_user)
{
    MYSQL mysql;
    char* prepStmt = "select * from %s where %s;";
    char queryStmt[256] = {0};
    char tmpsql[128]={0};
    int ret = 0;

    memset( tmpsql, 0x00, sizeof(tmpsql) );
    strcpy( tmpsql,"1=1" );

    if(t_sys_user->id){
        sprintf(tmpsql, "%s and id = '%d'",tmpsql, t_sys_user->id);
    }

    if(t_sys_user->password != NULL){
        sprintf(tmpsql, "%s and password = '%s'",tmpsql, t_sys_user->password);
    }

    if(t_sys_user->phone != NULL){
        sprintf(tmpsql, "%s and phone = '%s'",tmpsql, t_sys_user->phone);
    }

    if(t_sys_user->createtimestamp != NULL){
        sprintf(tmpsql, "%s and createtimestamp = '%s'",tmpsql, t_sys_user->createtimestamp);
    }

    db_get_database_connection( &mysql,g_dbhost,g_dbuser,g_dbpassword,g_dbname,g_dbport );
    sprintf(queryStmt,prepStmt,SOFT_SYS_USER_TABLE, tmpsql);
    WRITELOG("LOG_DEBUG", "queryStmt = [%s]", queryStmt);
    ret = db_do_query_2(&mysql,queryStmt,call_back_get_sys_user,t_query_sys_user);
    db_put_database_connection(&mysql);
    if(ret > 0)
    {
        return SUCCESS;
    }

    return 0;
}

int db_add_sys_user(sys_user_t* t_sys_user)
{
    MYSQL mysql;
    char* prepStmt = "insert into %s values(%d,'%s','%s','%s','%s');";
    char* queryStmt = (char*)malloc(2048);
    int ret = 0;
    
    if( NULL == t_sys_user || NULL == queryStmt )
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
    sprintf(queryStmt,prepStmt,SOFT_SYS_USER_TABLE,t_sys_user->id,t_sys_user->user,t_sys_user->password,t_sys_user->phone,t_sys_user->createtimestamp);
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

int db_delete_sys_user(sys_user_t* t_sys_user)
{
    MYSQL mysql;
    char* prepStmt = "delete from %s where %s;";
    char queryStmt[512] = {0};
    char tmpsql[128]={0};
    int ret = 0;

    memset( tmpsql, 0x00, sizeof(tmpsql) );
    strcpy( tmpsql,"1=1" );

    if(t_sys_user->id){
        sprintf(tmpsql, "%s and id = '%d'",tmpsql, t_sys_user->id);
    }

    if(t_sys_user->password != NULL){
        sprintf(tmpsql, "%s and password = '%s'",tmpsql, t_sys_user->password);
    }

    if(t_sys_user->phone != NULL){
        sprintf(tmpsql, "%s and phone = '%s'",tmpsql, t_sys_user->phone);
    }

    if(t_sys_user->createtimestamp != NULL){
        sprintf(tmpsql, "%s and createtimestamp = '%s'",tmpsql, t_sys_user->createtimestamp);
    }

    db_get_database_connection(&mysql,g_dbhost,g_dbuser,g_dbpassword,g_dbname,g_dbport);
    sprintf(queryStmt,prepStmt,SOFT_SYS_USER_TABLE, tmpsql);
    WRITELOG("LOG_DEBUG", "queryStmt = [%s]", queryStmt);
    ret = db_do_query_2(&mysql,queryStmt,NULL,NULL);

    db_put_database_connection(&mysql);
    if( ret > 0 )
    {
        return SUCCESS;
    }
    return FAILURE;
}

int db_update_sys_user(sys_user_t* t_sys_user_org, sys_user_t* t_sys_user_new)
{
    MYSQL mysql;
    char* prepStmt = "update %s set %s where %s;";
    char queryStmt[512] = {0};
    char tmpSetsql[128]={0};
    char tmpWheresql[128]={0};
    int ret = 0;

    memset( tmpSetsql, 0x00, sizeof(tmpSetsql) );
    memset( tmpWheresql, 0x00, sizeof(tmpWheresql) );
    strcpy( tmpWheresql,"1=1" );

    if(t_sys_user_org->id){
        sprintf(tmpSetsql, "%s id = '%d', ",tmpSetsql, t_sys_user_org->id);
    }

    if(t_sys_user_org->user){
        sprintf(tmpSetsql, "%s user = '%s', ",tmpSetsql, t_sys_user_org->user);
    }

    if(t_sys_user_org->password){
        sprintf(tmpSetsql, "%s password = '%s', ",tmpSetsql, t_sys_user_org->password);
    }

    if(t_sys_user_org->phone){
        sprintf(tmpSetsql, "%s phone = '%s', ",tmpSetsql, t_sys_user_org->phone);
    }

    if(t_sys_user_org->createtimestamp){
        sprintf(tmpSetsql, "%s createtimestamp = '%s', ",tmpSetsql, t_sys_user_org->createtimestamp);
    }

    delLastcomma(tmpSetsql);

    if(t_sys_user_new->id){
        sprintf(tmpWheresql, "%s and id = '%d'",tmpWheresql, t_sys_user_new->id);
    }

    if(t_sys_user_new->user){
        sprintf(tmpWheresql, "%s and user = '%s'",tmpWheresql, t_sys_user_new->user);
    }

    if(t_sys_user_new->password){
        sprintf(tmpWheresql, "%s and password = '%s'",tmpWheresql, t_sys_user_new->password);
    }

    if(t_sys_user_new->phone){
        sprintf(tmpWheresql, "%s and phone = '%s'",tmpWheresql, t_sys_user_new->phone);
    }

    if(t_sys_user_new->createtimestamp){
        sprintf(tmpWheresql, "%s and createtimestamp = '%s'",tmpWheresql, t_sys_user_new->createtimestamp);
    }

    db_get_database_connection(&mysql,g_dbhost,g_dbuser,g_dbpassword,g_dbname,g_dbport);
    sprintf(queryStmt,prepStmt,SOFT_SYS_USER_TABLE,tmpSetsql,tmpWheresql);

    ret = db_do_query_2(&mysql,queryStmt,NULL,NULL);
    WRITELOG("LOG_DEBUG", "queryStmt = [%s]", queryStmt);
    db_put_database_connection(&mysql);
    if( ret > 0 )
    {
        return ret;
    }
    return FAILURE;
}

int call_back_get_sys_user(void* arg,MYSQL_RES* resultSet,int num_fields,int num_rows)
{
    query_sys_user_t* result = (query_sys_user_t*)arg;
    MYSQL_ROW row;
    if( result == NULL )
    {
        WRITELOG("LOG_DEBUG","%s function %d bad input parameter.\n",__func__,__LINE__);
        return FAILURE;
    }
    if(result && (result->tsysUser == NULL) && num_fields > 0 && num_rows > 0)
    {
        result->tsysUser = (sys_user_t*)malloc(num_rows*sizeof(sys_user_t));
        if( NULL == result->tsysUser )
        {
            WRITELOG("LOG_ERR","%s function %d memory malloc failed.\n",__func__,__LINE__);
            return FAILURE;
        }

        memset(result->tsysUser,0,num_rows*sizeof(sys_user_t));
        result->num_columns = num_fields;
        result->num_rows = num_rows;
        result->cur_row = 0;
    }

    if(result->tsysUser != NULL && result->cur_row < result->num_rows )
    {
        while(( row = mysql_fetch_row(resultSet) ) != NULL)
        {
            printf("%d\n",result->cur_row);
            if(row[0])
            {
                result->tsysUser[result->cur_row].id = atoi(row[0]);
            }
            else
            {
                result->tsysUser[result->cur_row].id = 0;
            }
            printf("Field[0]:%d\t",result->tsysUser[result->cur_row].id);

            if(row[1])
            {
                strcpy(result->tsysUser[result->cur_row].user ,row[1]);
            }
            else
            {
                strcpy(result->tsysUser[result->cur_row].user ,"");
            }
            printf("Field[1]:%s\t",result->tsysUser[result->cur_row].user);

            if(row[2])
            {
                strcpy(result->tsysUser[result->cur_row].password ,row[2]);
            }
            else
            {
                strcpy(result->tsysUser[result->cur_row].password ,"");
            }
            printf("Field[2]:%s\t",result->tsysUser[result->cur_row].password);

            if(row[3])
            {
                strcpy(result->tsysUser[result->cur_row].phone, row[3]);
            }
            else
            {
                strcpy(result->tsysUser[result->cur_row].phone, "");
            }
            printf("Field[3]:%s\n",result->tsysUser[result->cur_row].phone);

            if(row[4])
            {
                strcpy(result->tsysUser[result->cur_row].createtimestamp, row[4]);
            }
            else
            {
                strcpy(result->tsysUser[result->cur_row].createtimestamp, "");
            }
            printf("Field[4]:%s\n",result->tsysUser[result->cur_row].createtimestamp);
            result->cur_row++;
        }
        return SUCCESS;
    }
    return FAILURE;
}


#include "httptrans.h"

int Register(struct httpRequest httpreq,struct httpReponse *httpreponse){
    cJSON *json= cJSON_Parse(httpreq.content);
    char *json_data = NULL;
    json_data = cJSON_Print(json);

    cJSON *phone = cJSON_GetObjectItem(json, "phone");
    if (!phone) {
        printf("no phone!\n");
	packErrMsg(httpreponse,"no phone!", "ERROR");
        return -1;
    } else{
        WRITELOG("LOG_DEBUG","phone=====================,%s\n",phone->valuestring);
    }

    cJSON *user= cJSON_GetObjectItem(json, "user");
    if (!user) {
        printf("no user!\n");
	packErrMsg(httpreponse,"no user!", "ERROR");
        return -1;
    } else{
        WRITELOG("LOG_DEBUG","user=====================,%s\n",user->valuestring);
    }

    //终端注册
    time_t timep;
    struct tm* p;
    time(&timep);
    p = localtime(&timep);

    //struct tm tmpTime;
    //get_db_tm_from_format_time("2014-12-24 12:00:00",&tmpTime);
    //printf("from format_time_str to tm_month is %d\n",tmpTime.tm_mon);
    
    query_sys_user_t querySysUser;
    memset(&querySysUser, 0, sizeof(querySysUser));

    sys_user_t user_t;
    strcpy(user_t.user, user->valuestring);
    strcpy(user_t.phone, phone->valuestring);
    get_db_format_time_str_from_tm(p,user_t.createtimestamp);

    //SELECT
    db_get_sys_user(&querySysUser,&user_t);
    if(querySysUser.tsysUser!= NULL){
        free(querySysUser.tsysUser);
    }
    WRITELOG("LOG_DEBUG", "num_rows =[%d]", querySysUser.num_rows);

    if(querySysUser.num_rows == 0){
        db_add_sys_user(&user_t);
    }

    free(json_data);
    cJSON_Delete(json);

    httpreponse->headcontent = (char*)malloc(sizeof(char)*4);
    httpreponse->bodycontent = (char*)malloc(sizeof(char)*4);
    httpreponse->contentlength = (char*)malloc(sizeof(char)*4);

    strcpy(httpreponse->errorCode,"SUCCESS");
    strcpy(httpreponse->errorMsg,"成功");
    strcpy(httpreponse->bodycontent,"SUCCESS");
    strcpy(httpreponse->headcontent,"success.html");
    strcpy(httpreponse->contentlength,"7");
    return 0;
}

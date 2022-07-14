#include "writelog.h"

//向my.log文件中写日志
void writelog(const char *func, const char *file, const int line, const char *type, const char *format, ...) 
{
    FILE *file_fp;
    time_t loacl_time;
    char time_str[128];

    time(&loacl_time); // 获取本地时间
    strftime(time_str, sizeof(time_str), "[%Y.%m.%d %X]", localtime(&loacl_time));
    
    va_list ap;        // 日志内容格式转换
    va_start(ap, format);
    char fmt_str[4096];
    vsnprintf(fmt_str, sizeof(fmt_str), format, ap);
    va_end(ap);

    char logfile[30]={0};
    //if(!strcmp(type,"LOG_DEBUG")) {
        sprintf(logfile, "my.log");
    //}else{
    //    sprintf(logfile, "%s", type);
    //}

    file_fp = fopen(logfile, "a"); // 打开日志文件
    // 写入到日志文件中
    if (file_fp != NULL)
    {
        //fprintf(file_fp, "[%s]%s[%s@%s:%d] %s\n", type, time_str, func, file, line, fmt_str);
        fprintf(file_fp, "[%lu]%s[%s@%s:%d] %s\n", pthread_self(), time_str, func, file, line, fmt_str);
        fclose(file_fp);
    }
    else
    {
        //fprintf(stderr, "[%s]%s[%s@%s:%d] %s\n", type, time_str, func, file, line, fmt_str);
        fprintf(stderr, "[%lu]%s[%s@%s:%d] %s\n", pthread_self(), time_str, func, file, line, fmt_str);
    }
}

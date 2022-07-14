#include "../frame/pub.h"
#include <mcheck.h>

int main(int arg, char *args[])
{    
    setenv("MALLOC_TRACE", "mtrace.out", 1);
    mtrace();
    if (arg < 2) {                       //如果没有参数，main函数返回
        printf("usage:%s port\n", args[0]);
        return EXIT_FAILURE;
    }

    int iport = atoi(args[1]);          //将第一个参数转化为整数
    if (iport == 0) {
        printf("port %d is invalid\n", iport);
        return EXIT_FAILURE;
    }

    int st = socket_create(iport);      //建立listen socket
    if (st == 0)
        return EXIT_FAILURE;

    WRITELOG("LOG_DEBUG","-------------myhttp begin");

    getGetMysqlConf();
    getBaseConf();
    setdaemon();                        //设置进程为daemon状态

    Signal(SIGINT, catch_Signal);        //捕捉SIGINT信号    
    Signal(SIGPIPE, catch_Signal);
    Signal(SIGALRM, catch_Signal);
    Signal(SIGTERM, catch_Signal);

    socket_accept(st);
    close(st);

    WRITELOG("LOG_DEBUG","--------------myhttp end");
    return EXIT_SUCCESS;
}


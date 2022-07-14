/*
 * base.c
 */
#include "base.h"

/*创建守护进程*/
void setdaemon(void)
{
    pid_t pid, sid;
    pid = fork();
    if (pid < 0) {
        WRITELOG("LOG_DEBUG", "fork failed %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if ((sid = setsid()) < 0) {
        WRITELOG("LOG_DEBUG","setsid failed %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    /*
     if (chdir("/") < 0)
     {
        printf("chdir failed %s\n", strerror(errno));
        exit(EXIT_FAILURE);
     }
     umask(0);  / umask(022);
     close(STDIN_FILENO);       //dup2  /dev/null
     close(STDOUT_FILENO);
     close(STDERR_FILENO);
    */
}

/*信号捕捉回调函数*/
void catch_Signal(int Sign)
{
    switch (Sign)
    {
    case SIGINT: /*ctrl+c*/
        WRITELOG("LOG_DEBUG", "signal SIGINT");
        break;
    case SIGPIPE:
        WRITELOG("LOG_DEBUG", "signal SIGPIPE");
    case SIGALRM:
        WRITELOG("LOG_DEBUG", "signal SIGALRM");
        break;    
    case SIGTERM:
        WRITELOG("LOG_DEBUG", "signal SIGTERM");
        exit(0);
        break;
    default:
        break;
    }
}

/* 注册信号捕捉函数封装 */
int Signal(int signo, void (*func)(int))
{
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    return sigaction(signo, &act, &oact);
}

int getGetMysqlConf(void){
    memset(&g_dbuser, 0, sizeof(g_dbuser));
    memset(&g_dbpassword, 0, sizeof(g_dbpassword));
    memset(&g_dbport, 0, sizeof(g_dbport));
    memset(&g_dbname, 0, sizeof(g_dbname));
    memset(&g_dbhost, 0, sizeof(g_dbhost));

    GetIniKeyString("mysql","dbuser",CONFIG_FILENAME,g_dbuser);
    GetIniKeyString("mysql","dbpassword",CONFIG_FILENAME,g_dbpassword);
    GetIniKeyString("mysql","dbport",CONFIG_FILENAME,g_dbport);
    GetIniKeyString("mysql","dbname",CONFIG_FILENAME,g_dbname);
    GetIniKeyString("mysql","dbhost",CONFIG_FILENAME,g_dbhost);
}

int getBaseConf(void){
    memset(&g_root_path, 0, sizeof(g_root_path));
    GetIniKeyString("basic","root_path",CONFIG_FILENAME,g_root_path);
}

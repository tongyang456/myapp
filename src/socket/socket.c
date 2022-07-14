/*
 * socket.c
 */
#include "socket.h"

void *socket_contr(void *arg)                               //线程入口函数
{
    long tid;
    tid = (long)arg;
    WRITELOG("LOG_DEBUG","thread begin %ld, pthread ID - %lu\n", tid, pthread_self());

    int st = *(int *)arg;                                   //得到来自client端的socket
    if((int *)arg != NULL){
        free((int *)arg);
    }

    char *buf = (char *)malloc(sizeof(char)*(BUFSIZE));
    char *resp = NULL;
    int respsize = 0;
    memset(buf, 0x00, BUFSIZE);
    memset(&resp, 0X00, sizeof(resp));

    ssize_t rc = recv(st, buf, BUFSIZE, 0);                 //接收来自client端socket的消息
    if (rc <= 0) {
        WRITELOG("LOG_DEBUG", "recv failed %s", strerror(errno));
    } else {
        WRITELOG("LOG_DEBUG", "req=[%s] ", buf);
        dealHttpContent(buf, sizeof(buf), &resp, &respsize);
        WRITELOG("LOG_DEBUG", "resp=[%s] respsize=[%d]", resp, respsize);
        send_msg(st, resp, respsize);
        if(resp != NULL){
            free(resp);
        }
    }

    free(buf);
    close(st);                                                      //关闭client端socket
    WRITELOG("LOG_DEBUG","thread end %ld, pthread ID - %lu\n", tid, pthread_self());

    return NULL;
}

void send_msg(int sock, char *content, size_t contentsize)
{
    int rc = 0;
    while (1) {
        rc = send(sock, &content[rc], contentsize, 0);
        if (rc >= contentsize)
            break;
        if (rc <= 0)
            break;
        contentsize -= rc;
    }
}

/* 根据参数port，建立server端socket */
int socket_create(int port)                                     
{
    int st = socket(AF_INET, SOCK_STREAM, 0);                   //建立TCP的socket描述符
    if (st == -1) {
        WRITELOG("LOG_DEBUG", "socket failed %s", strerror(errno));
        return 0;
    }

    int on = 1;
    if (setsockopt(st, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
        WRITELOG("LOG_DEBUG", "setsockopt failed %s", strerror(errno));
        return 0;
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(st, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        WRITELOG("LOG_DEBUG", "bind failed %s", strerror(errno));
        return 0;
    }
    if (listen(st, 128) == -1) {
        WRITELOG("LOG_DEBUG", "listen failed %s", strerror(errno));
        return 0;
    }
    WRITELOG("LOG_DEBUG", "listen %d success", port);

    return st;  //返回socket描述符
}

/* 将struct sockaddr_in转化为IP地址 字符串形式 */
void sockaddr_toa(const struct sockaddr_in *addr, char *IPAddr)     
{
    unsigned char *p = (unsigned char *) &(addr->sin_addr.s_addr);
    sprintf(IPAddr, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
}

/* 阻塞等待客户端请求到达,启新线程处理请求 */
void socket_accept(int st)
{
    pthread_t thr_d;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);    //修改过线程属性, 设为分离态

    int client_st = 0;
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    while (1) {                                                      //循环执行accept
        memset(&client_addr, 0, sizeof(client_addr));
        client_st = accept(st, (struct sockaddr *) &client_addr, &len);

        if (client_st == -1 && errno != 4) {
            WRITELOG("LOG_DEBUG", "accept failed %d: %s", errno, strerror(errno));
            break; 
        } else {
            char sIP[32], text[128] = {0};

            memset(sIP, 0, sizeof(sIP));
            sockaddr_toa(&client_addr, sIP);                        //将客户端的IP转换为字符串类型

            sprintf(text, "accept by %s", sIP);
            WRITELOG("IP_DEBUG", "ip=[ %s ]", text);               //将该客户端链接事件及其IP写入日志

            int *tmp = malloc(sizeof(int));                         //nei cun rong yu fa
            *tmp = client_st;
            pthread_create(&thr_d, &attr, socket_contr, tmp);       //将client的socket做为参数，启动一个可分离线程，socket_contr作为回调函数
        }
    }
    pthread_attr_destroy(&attr);                                    //释放线程属性
}

void socket_close(int st){
    close(st);
}

/*
 * socket.h
 */
#include "../frame/pub.h"

#ifndef SOCKET_H_
#define SOCKET_H_

int socket_create(int port);
void socket_accept(int st);
void send_msg(int sock, char *content, size_t contentsize);
void socket_close(int st);
void *socket_contr(void *arg);
#endif 

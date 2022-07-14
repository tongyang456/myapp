/*
 * httptrans.h 业务处理
*/

#ifndef HTTPTRANS_H_
#define HTTPTRANS_H_
#include "../frame/pub.h"
#include "../http/http.h"
#include "../xml2jsonConverter/xml2jsonConv.h"
#include "../mysql/mydb.h"

typedef struct
{
    struct httpRequest httpreq;
    struct httpReponse *httpreponse;
    int (*handler)(struct httpRequest, struct httpReponse*);
}httpFrame;

int Register(struct httpRequest httpreq,struct httpReponse* httpreponse);
int Query(struct httpRequest httpreq,struct httpReponse* httpreponse);
int Index(struct httpRequest httpreq,struct httpReponse* httpreponse);
int Error(struct httpRequest httpreq,struct httpReponse* httpreponse);

int *getHttpFrameMethodByHttpUrl(struct httpRequest httpreq);
int packErrMsg(struct httpReponse *httpreponse,char *errmsg, char *errcode );

extern size_t getfilecontent(const char *filename, char **buf);
extern int dealhtpptrans(const struct httpRequest httpreq, struct httpReponse *httpresp);

#endif

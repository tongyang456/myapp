#include "httptrans.h"

int dealhtpptrans(struct httpRequest httpreq, struct httpReponse *httpresp){
    httpFrame httpframe;
    memset(&httpframe, 0x00, sizeof(httpFrame));
    httpframe.httpreq= httpreq;
    httpframe.httpreponse = httpresp;
    httpframe.handler = getHttpFrameMethodByHttpUrl(httpreq);
    int result = httpframe.handler(httpreq, httpresp);
    return 0;
}

int *getHttpFrameMethodByHttpUrl(struct httpRequest httpreq){
    if(!strncmp(httpreq.url,"/root/query", 11) || !strcmp(httpreq.url,"/") || !strcmp(httpreq.url,"/favicon.ico")){
        return (int *)Query;
    }else if(!strcmp(httpreq.url,"/root/register")){
        return (int *)Register;
    }else{
        return (int *)Error;
    }
}

int packErrMsg(struct httpReponse *httpreponse,char *errmsg, char *errcode ){

    httpreponse->headcontent = (char*)malloc(sizeof(char)*128);
    httpreponse->bodycontent = (char*)malloc(sizeof(char)*(strlen(errmsg)));
    httpreponse->contentlength = (char*)malloc(sizeof(char)*10);

    memset(httpreponse->headcontent, 0x00, sizeof(128));
    memset(httpreponse->bodycontent, 0x00, sizeof(strlen(errmsg)));
    memset(httpreponse->contentlength, 0x00, sizeof(10));

    strcpy(httpreponse->errorCode,errcode);
    strcpy(httpreponse->errorMsg,errmsg);
    strcpy(httpreponse->bodycontent,errmsg);
    strcpy(httpreponse->headcontent,"error.html");
    sprintf(httpreponse->contentlength, "%d", strlen(errmsg));
}

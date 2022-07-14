#include "httptrans.h"

int Query(struct httpRequest httpreq,struct httpReponse *httpreponse){
    char path[2048] = { 0 };
    int icontentlen = 0;
    char *contentbuf = NULL;

    if(!strcmp(httpreq.url,"/")){
        sprintf(path,"%s/index.html",g_root_path);
    }else{
        sprintf(path,"%s%s",g_root_path,httpreq.url);
    }

    icontentlen = getfilecontent(path, &contentbuf);

    if(icontentlen > 0 ){
        httpreponse->headcontent = (char*)malloc(sizeof(char)*(strlen(httpreq.url)));
        httpreponse->bodycontent = (char*)malloc(sizeof(char)*icontentlen);
        httpreponse->contentlength = (char*)malloc(sizeof(char)*((int)log10(icontentlen)+1));

        memset(httpreponse->headcontent, 0x00, sizeof(httpreponse->headcontent));
        memset(httpreponse->bodycontent, 0x00, sizeof(httpreponse->bodycontent));
        memset(httpreponse->contentlength, 0x00, sizeof(httpreponse->contentlength));

        memcpy(httpreponse->bodycontent, contentbuf, icontentlen);
        strcpy(httpreponse->headcontent, httpreq.url);
        strcpy(httpreponse->errorMsg, "SUCCESS");
        strcpy(httpreponse->errorCode, "SUCCESS");
        sprintf(httpreponse->contentlength, "%d", icontentlen);
    }else{
        httpreponse->headcontent = (char*)malloc(sizeof(char)*128);
        httpreponse->bodycontent = (char*)malloc(sizeof(char)*128);
        httpreponse->contentlength = (char*)malloc(sizeof(char)*10);

        memset(httpreponse->headcontent, 0x00, sizeof(128));
        memset(httpreponse->bodycontent, 0x00, sizeof(128));
        memset(httpreponse->contentlength, 0x00, sizeof(10));

        strcpy(httpreponse->errorCode,"FAIL");
        strcpy(httpreponse->errorMsg,"失败");
        strcpy(httpreponse->bodycontent,"error,访问出错!!");
        strcpy(httpreponse->headcontent,"error.html");
        strcpy(httpreponse->contentlength,"20");
    }

    if(contentbuf != NULL){
        free(contentbuf);
    }
    return 0;
}

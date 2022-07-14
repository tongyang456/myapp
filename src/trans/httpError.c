#include "httptrans.h"

int Error(struct httpRequest httpreq,struct httpReponse *httpreponse){

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

    return 0;
}

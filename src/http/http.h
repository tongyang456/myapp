/*http.h*/
#ifndef HTTP_H
#define HTTP_H

#include "../frame/pub.h"

#define METHOD_SPLIT "?"
#define ARG_SPLIT "&"
#define TEMP_SIZE 128
#define POST_LEN "Content-Length:"
#define POST_LEN_S "content-length:"
#define POST_METHOD "POST"
#define GET_METHOD "GET"
#define HEAD "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length:%u\r\n\r\n"

struct httpRequest{
    char *url;
    char *method;
    char **argarray;
    char *content;
    int argcnt;
    char **headarray;
    int headcnt;
};

struct httpReponse{
    char *headcontent;
    char *contentlength;
    char *bodycontent;
    char errorMsg[128];
    char errorCode[8];
};

/*编码*/
void strencode(char* to, size_t tosize, const char* from);
/*解码*/
void strdecode(char *to, const char *from);
/*16进制数转化为10进制*/
int hexit(char c);
/*http处理主函数*/
int dealHttpContent(const char *reqcontent, size_t reqcontentsize, char **respcontent, int* respilen);
/*将请求报文转换成httpRequest结构体*/
struct httpRequest init_http_request(const char *content, size_t contentsize);
/*将处理后的httpReponse，拼接成http响应报文*/
void feedbackHttpContent(struct httpReponse httpreponse, char **buf, int *len);
const char *getfiletype(const char *filename);
void get_file_ext(const char *filename, char *extname);  //得到文件扩展名
void getMethodName(const char *content, char **method);
void getUrlName(const char *content, char **url);
void getArgArray(const char *content, char ***array_p, int *count);
int getContent(const char *sHTTPMsg, char **content);
void getHeadArray(const char *content, char ***head_p, int *count);

/*将获取到的键值对存储在数组里面*/
char *getArgValueByName(const char *key, char ***array, int count);
/*获取http的body里面的有效内容*/
void printhttpreq(struct httpRequest httpreq);
void freeMem(struct httpRequest httpreq, struct httpReponse httpreponse);
void free_mem(char *str);
void free_mem_array(char ***array_p, int count);
extern int dealhtpptrans(const struct httpRequest httpreq, struct httpReponse *httpresp);
struct httpReponse init_http_response();
void printhttpresp(struct httpReponse httpresp);
size_t getfilecontent(const char *filename, char **buf);
void file_infos(const char *dir, char *name, char *linebuf, int *len);

#endif


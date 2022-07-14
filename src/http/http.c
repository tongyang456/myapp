#include "http.h"

//"编码"，用作回写浏览器的时候，将除字母数字及/_.-~以外的字符转义后回写。
void strencode(char* to, size_t tosize, const char* from)
{
    int tolen;
    for (tolen = 0; *from != '\0' && tolen + 4 < tosize; ++from) {
        if (isalnum(*from) || strchr("/_.-~", *from) != (char*)0) {
            *to = *from;
            ++to;
            ++tolen;
        } else {
            sprintf(to, "%%%02x", (int)*from & 0xff);   
            to += 3;
            tolen += 3;
        } 
    } 
    *to = '\0';
}

/*
 * 这里的内容是处理%20之类的东西！是"解码"过程。
 * %20 URL编码中的‘ ’(space)
 * %21 '!' %22 '"' %23 '#' %24 '$'
 * %25 '%' %26 '&' %27 ''' %28 '('......
 * 相关知识html中的‘ ’(space)是&nbsp
 */
void strdecode(char *to, const char *from)
{
    for ( ; *from != '\0'; ++to, ++from) {
        if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2])) { //依次判断from中 %20 三个字符
            *to = hexit(from[1])*16 + hexit(from[2]);
            from += 2;                      //移过已经处理的两个字符(%21指针指向1),表达式3的++from还会再向后移一个字符
        } else
            *to = *from;
    }
    *to = '\0';
}

int hexit(char c) //16进制数转化为10进制
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    return 0;
}

/*http请求主函数*/
int dealHttpContent(const char *reqcontent, size_t reqcontentsize, char **respcontent, int* respilen){
    struct httpReponse httpreponse = init_http_response();
    struct httpRequest httprequest = init_http_request(reqcontent, sizeof(reqcontentsize));
    printhttpreq(httprequest);
    dealhtpptrans(httprequest, &httpreponse);
    printhttpresp(httpreponse);
    feedbackHttpContent(httpreponse, respcontent, respilen);
    freeMem(httprequest, httpreponse);
    return 0;
}

struct httpReponse init_http_response(){
    struct httpReponse httpresp;
    memset(&httpresp, 0, sizeof(struct httpReponse));
    return httpresp;
}

void freeMem(struct httpRequest httpreq, struct httpReponse httpresp){
    free_mem(httpreq.method);
    free_mem(httpreq.content);
    free_mem(httpreq.url);
    free_mem_array(&httpreq.argarray, httpreq.argcnt);
    free_mem_array(&httpreq.headarray, httpreq.headcnt);

    free_mem(httpresp.bodycontent);
    free_mem(httpresp.headcontent);
    free_mem(httpresp.contentlength);
}

void free_mem_array(char ***array_p, int count)
{
    char **array = *array_p;
    int i = 0;

    if (array_p == NULL) {
        return;
    }

    void free(void*);

    if (array != NULL) {
        for (i = 0; i < count; i++) {
            if (array[i] != NULL) {
                free(array[i]);
                array[i] = NULL;
            }
        }

        if(array != NULL){
            free(array);
        }
        *array_p = NULL;
    }
}

void free_mem(char *str){
    if(str!=NULL){
        free(str);
    }
}

void printhttpreq(struct httpRequest httpreq){
    int i = 0;
    if(httpreq.method != NULL){
        WRITELOG("LOG_DEBUG", "httpreq.method = [%s][%d]", httpreq.method, strlen(httpreq.method));
    }
    WRITELOG("LOG_DEBUG", "httpreq.argcnt = [%d]", httpreq.argcnt);
    if(httpreq.content != NULL){
        WRITELOG("LOG_DEBUG", "httpreq.content = [%s][%d]", httpreq.content, strlen(httpreq.content));
    }
    if(httpreq.url != NULL){
        WRITELOG("LOG_DEBUG", "httpreq.url = [%s][%d]", httpreq.url,strlen(httpreq.url));
    }
    for(i = 0; i < httpreq.argcnt; i++){
        WRITELOG("LOG_DEBUG", "httpreq.argarray= [%s],[%d]", httpreq.argarray[i],strlen(httpreq.argarray[i]));
    }
    WRITELOG("LOG_DEBUG", "httpreq.headcnt = [%d]", httpreq.headcnt);
    for(i = 0; i < httpreq.headcnt; i++){
        WRITELOG("LOG_DEBUG", "httpreq.headarray = [%s],[%d]", httpreq.headarray[i], strlen(httpreq.headarray[i]));
    }
}

void printhttpresp(struct httpReponse httpresp){
    if(httpresp.bodycontent != NULL){
        WRITELOG("LOG_DEBUG", "httpresp.bodycontent= [%s],[%d]", httpresp.bodycontent, strlen(httpresp.bodycontent));
    }
    if(httpresp.contentlength!= NULL){
        WRITELOG("LOG_DEBUG", "httpresp.contentlength= [%s],[%d]", httpresp.contentlength, strlen(httpresp.contentlength));
    }
    if(httpresp.headcontent != NULL){
        WRITELOG("LOG_DEBUG", "httpresp.headcontent= [%s],[%d]", httpresp.headcontent, strlen(httpresp.headcontent));
    }
    if(httpresp.errorMsg != NULL){
        WRITELOG("LOG_DEBUG", "httpresp.errorMsg= [%s],[%d]", httpresp.errorMsg, strlen(httpresp.errorMsg));
    }
    if(httpresp.errorCode != NULL){
        WRITELOG("LOG_DEBUG", "httpresp.errorCode= [%s],[%d]", httpresp.errorCode, strlen(httpresp.errorCode));
    }
}

/*组装httpRequest*/
struct httpRequest init_http_request(const char *content, size_t contentsize){
    struct httpRequest httpreq;
    memset(&httpreq, 0x00, sizeof(struct httpRequest));
    httpreq.method = NULL;

    getMethodName(content, &httpreq.method);
    getUrlName(content, &httpreq.url);
    getArgArray(content, &httpreq.argarray, &httpreq.argcnt);
    getHeadArray(content, &httpreq.headarray, &httpreq.headcnt);
    getContent(content, &httpreq.content);
    return httpreq;
}

//根据get提供的文件名，生成静态 http 代替消息内容
void feedbackHttpContent(struct httpReponse httpreponse, char **buf, int *len) 
{
    char headbuf[1024];
    memset(headbuf, 0, sizeof(headbuf));
    sprintf(headbuf, HEAD, getfiletype(httpreponse.headcontent), atol(httpreponse.contentlength));  //设置消息头
    size_t iheadlen = strlen(headbuf);                              //得到消息头长度
    size_t isumlen = iheadlen + atol(httpreponse.contentlength);    //得到消息总长度
    *buf = (char *)malloc(sizeof(char)*(isumlen));                  //根据消息总长度，动态分配内存
    memset(*buf, 0x00, sizeof(char)*(isumlen));
    char *tmp = *buf;
    memcpy(tmp, headbuf, iheadlen);                                 //安装消息头
    memcpy(&tmp[iheadlen], httpreponse.bodycontent, atol(httpreponse.contentlength));   //安装消息体，重点
    *len = isumlen;                                                 //返回消息总长度
    return;
}

const char *getfiletype(const char *filename)          //根据扩展名返回文件类型描述
{
    char sExt[1024] = { 0 };
    get_file_ext(filename, sExt);       //得文件扩展名

    if (strncmp(sExt, "bmp", 3) == 0)
        return "image/bmp";

    if (strncmp(sExt, "gif", 3) == 0)
        return "image/gif";

    if (strncmp(sExt, "ico", 3) == 0)
        return "image/x-icon";

    if (strncmp(sExt, "jpg", 3) == 0)
        return "image/jpeg";

    if (strncmp(sExt, "png", 3) == 0)
        return "image/png";

    if (strncmp(sExt, "mp3", 3) == 0)
        return "audio/mp3";

    if (strncmp(sExt, "avi", 3) == 0)
        return "video/avi";

    if (strncmp(sExt, "mpg", 3) == 0)
        return "video/mpg";

    if (strncmp(sExt, "mp4", 3) == 0)
        return "video/mpeg4";

    if (strncmp(sExt, "wma", 3) == 0)
        return "audio/x-ms-wma";

    if (strncmp(sExt, "wmv", 3) == 0)
        return "video/x-ms-wmv";

    if (strncmp(sExt, "rm", 2) == 0)
        return "video/x-ms-rm";

    if (strncmp(sExt, "rmvb", 4) == 0)
        return "video/x-ms-rmvb";

    if (strncmp(sExt, "rar", 3) == 0)
        return "application/x-rar-compressed";

    if (strncmp(sExt, "pdf", 3) == 0)
        return "application/pdf";

    if (strncmp(sExt, "zip", 3) == 0)
        return "application/zip";

    //if (strncmp(sExt, "ppt", 3) == 0)
    //  return "application/vnd.ms-powerpoint"; 

    if (strncmp(sExt, "ppt", 3) == 0 || strncmp(sExt, "pptx", 4) == 0)
        return "application/x-ppt";

    if (strncmp(sExt, "xls", 3) == 0 || strncmp(sExt, "xlsx", 4) == 0)
        return "application/ms-excel";

    if (strncmp(sExt, "doc", 3) == 0 || strncmp(sExt, "docx", 4) == 0)
        return "application/msword";

    if (strncmp(sExt, "dll", 3) == 0)
        return "application/x-msdownload";

    if (strncmp(sExt, "exe", 3) == 0)
        return "application/x-msdownload";

    if (strncmp(sExt, "css", 3) == 0)
        return "text/css";

    if (strncmp(sExt, "dtd", 3) == 0)
        return "text/xml";

    if (strncmp(sExt, "xml", 3) == 0)
        return "text/xml";

    if (strncmp(sExt, "js", 2) == 0)
        return "text/javascript";

    if (strncmp(sExt, "txt", 3) == 0)
        return "text/html;charset=utf-8";

    if (strncmp(sExt, "shtml", 4) == 0)
        return "text/html;charset=utf-8";

    if (strncmp(sExt, "xhtml", 4) == 0)
        return "text/html;charset=utf-8";

    if (strncmp(sExt, "htm", 3) == 0)
        return "text/html;charset=utf-8";

    if (strncmp(sExt, "html", 4) == 0)
        return "text/html;charset=utf-8";

    if (strncmp(sExt, "cgi", 3) == 0)
        return "text/html;charset=utf-8";

    if (sExt[0] == 0 || sExt[0] == ' ')
        return "text/html;charset=utf-8";

    return "application/x-msdownload";
}

void get_file_ext(const char *filename, char *extname)  //得到文件扩展名
{
    int len = strlen(filename);
    int i;
    for (i = len-1; i >= 0; i--) {
        if (filename[i] == '.') {
            strncpy(extname, &filename[i + 1], 1023);
            break;
        }
    }
}

//获取方法名称,暂时支持get和post
void getMethodName(const char *content, char **method){
    int i = 0;

    for(i = 0; i < 10; i++){
        if(content[i]==' '){
            break;
        }
    }

    *method = (char *)malloc(sizeof(char)*(i+1));
    memset(*method, 0x00, sizeof(char)*(i+1));
    memcpy(*method, content, i);
    method[i]='\0';
}

//获取完整的url
void getUrlName(const char *content, char **url )
{
    char *command = NULL;
    int i = 0,i1 = 0, istart = 0, iend = 0;
    int msg_len = strlen(content);

    for (i = 0; i < msg_len; i++) {
        if ((content[i] == ' ') && (istart == 0)) {
            //istart = i + 2;
            istart = i + 1;
        } else {
            if (content[i] == ' ') {
                iend = i;
                break;
            }
        }
    }

    int len = iend - istart;
    if (istart > msg_len || len <= 0 || len >= 2048)
        return;

    command = (char *)malloc(sizeof(char)*(len+1));
    memset(command, 0x00, sizeof(char)*(len+1));
    memcpy(command, &content[istart], len);
    command[len]='\0';

    for(i1 = 0; i1 < strlen(command); i1++){
        if(command[i1] == '?'){
            break;
        }
    }
    if(i1 > 0 ){
        *url = (char *)malloc(sizeof(char)*(i1+1));
        memset(*url, 0, sizeof(char)*(i1+1));
        memcpy(*url, &command[0], i1);
        url[i1]='\0';
    }
    if(command!=NULL){
        free(command);
    }
    return;
}

//获取参数数组
void getArgArray(const char *content, char ***array_p, int *count){
    char *command = NULL;
    char *argname = NULL;
    int i = 0,i1 = 0, istart = 0, iend = 0;
    int msg_len = strlen(content);

    for (i = 0; i < msg_len; i++) {
        if ((content[i] == ' ') && (istart == 0)) {
            istart = i + 2;
        } else {
            if (content[i] == ' ') {
                iend = i;
                break;
            }
        }
    }

    int len = iend - istart;
    if (istart > msg_len || len <= 0 || len >= 2048)
        return;

    command = (char *)malloc(sizeof(char)*(len+1));
    memset(command,0x00,sizeof(char)*(len+1));
    memcpy(command, &content[istart], len);
    command[len]='\0';

    for(i1 = 0; i1 < strlen(command); i1++){
        if(command[i1] == '?'){
            break;
        }
    }

    if( (len-i1-1) > 0 ){
        argname = (char *)malloc(sizeof(char)*(len-i1));
        memset(argname,0x00,sizeof(char)*(len-i1));
        memcpy(argname, &command[i1+1], len-i1-1);
        argname[len-i1]='\0';
        spitString(argname, '&', array_p, count);
    }
    if(command!=NULL){
        free(command);
    }
    if(argname!=NULL){
        free(argname);
    }
    return;
}

//获取头属性
void getHeadArray(const char *content, char ***head_p, int *count){
    const char *s = content;
    int end = 0,start = 0;
    while(s[start]!='\0'){
        if((s[start-1] == '\r') && (s[start] == '\n')){
            break;
        }else
            start++;
    }

    while(s[end]!='\0'){
        if((s[end-3] == '\r') && (s[end-2] == '\n') && (s[end-1] == '\r') && (s[end] == '\n')){
            break;
        }else
            end++;
    }
    char *content1 = (char *)malloc(sizeof(char)*(end-start));
    memset(content1,0x00, sizeof(char)*(end-start));
    memcpy(content1, &content[start+1], end-start-1);
    content1[end-start]='\0';
    spitStringByString(content1,"\r\n", end-start-1, head_p, count);

    if(content1 != NULL){
        free(content1);
    }
}

char *getArgValueByName(const char *key, char ***array, int count){
    int i = 0, retn = 0, value_len = 0;
    char *value = NULL;
    char *value1 = NULL;

    for (i = 0; i < count; i++) {
        if(!getKeyByValue(array[i], key, &value, &value_len)) {
            value1 = malloc(value_len);
            memcpy(value1, &value, value_len);
            return value1;
        }
    }
    return NULL;
}

//得到实际的内容
int getContent(const char *sHTTPMsg, char **content)
{
    const char *s = strstr(sHTTPMsg, POST_LEN);
    if (s) {
        int end = 0;
        while (s[end] != '\n' && s[end] != '\0') {
            end++;
        }
        char len[100] = { 0 };
        strncpy(len, &s[strlen(POST_LEN)], end);
        int i_len = atoi(len);
        s = strstr(s, "\r\n\r\n");
        if (s) {
            *content = (char *)malloc(sizeof(char)*(i_len+1));
            memset(*content, 0, sizeof(char)*(i_len+1));
            memcpy(*content, &s[4], i_len);
            content[i_len]='\0';
            return i_len;
        }
    }else{
        const char *s = strstr(sHTTPMsg, POST_LEN_S);
        if (s) {
            int end = 0;
            while (s[end] != '\n' && s[end] != '\0') {
                end++;
            }
            char len[100] = { 0 };
            strncpy(len, &s[strlen(POST_LEN_S)], end);
            int i_len = atoi(len);
            s = strstr(s, "\r\n\r\n");
            if (s) {
                *content = (char *)malloc(sizeof(char)*(i_len+1));
                memset(*content, 0, sizeof(char)*(i_len+1));
                memcpy(*content, &s[4], i_len);
                content[i_len]='\0';
                return i_len;
            }
        }
    }

    return 0;
}

size_t getfilecontent(const char *filename, char **buf)         //得到文件内容
{
    int ishtml = 0;                                             //标志位----是否是html
    int n;

    if (strcmp(getfiletype(filename), "text/html") == 0)        //filename是一个html文件
        ishtml = 1;

    struct stat t;
    memset(&t, 0, sizeof(t));

    FILE *fd = fopen(filename, "rb");                           //从只读方式打开参数filename指定的文件
    if (fd != NULL) {
        stat(filename, &t);
        int rc = t.st_size;
        if (ishtml) {                                           //是一个html文件
            rc += 4;
            *buf = malloc(rc);                                  //根据文件大小，动态分配内存buf
            fread(*buf, t.st_size, 1, fd);                      //将文件读取到buf, 注意参3, 其单位是参2
            (*buf)[rc - 4] = '\r';                              //如果是html文件，将缓冲区最后4个字节设置为\r\n\r\n
            (*buf)[rc - 3] = '\n';
            (*buf)[rc - 2] = '\r';
            (*buf)[rc - 1] = '\n';
        } else if (S_ISDIR(t.st_mode) ){                        //是一个目录
            struct dirent **dl;
            n = scandir(filename, &dl, NULL, alphasort);
            if (n < 0){
                perror("scandir");
                return 0;
            }
            else {
                char *tmp_buf = NULL;//辅助指针变量
                char linebuf[1024]={0};
                int len = 0, sumlen = 0;

                //文件头
                memset(linebuf, 0, sizeof(linebuf));
                len = sprintf(linebuf,"%s%s%s%s%s",
                        "<html>\n<head>\n<title>Index of /</title>" \
                        "<link href=\"/favicon.ico\" rel=\"icon\" type=\"image/x-icon\" /></head>\n", \
                        "<body bgcolor=\"white\">\n", "<h1>Index of ", \
                        filename, "</h1><hr><pre><a href=\"../\">../</a>\n" );

                tmp_buf = (char *)realloc(tmp_buf, (sumlen+len)*sizeof(char *));
                memcpy(&tmp_buf[sumlen], linebuf, (len)*sizeof(char *));
                sumlen += len;

                //文件体
                int i = 0;
                for(i = 0; i < n; ++i) {
                    memset(linebuf, 0, sizeof(linebuf));
                    file_infos(filename, dl[i]->d_name, linebuf, &len);
                    tmp_buf = (char *)realloc(tmp_buf, (sumlen+len)*sizeof(char *));
                    memcpy(&tmp_buf[sumlen], linebuf, len*sizeof(char *));
                    sumlen += len;
                }

                //文件尾
                memset(linebuf, 0, sizeof(linebuf));
                len = sprintf(linebuf,"%s%s", "</pre><hr></body>\n","</html>");
                tmp_buf = (char *)realloc(tmp_buf, (sumlen+len)*sizeof(char *));
                memcpy(&tmp_buf[sumlen], linebuf, (len)*sizeof(char *));
                sumlen += len;

                *buf = tmp_buf;
                rc = sumlen;
            }
        } else {                                                //不是html文件
            *buf = malloc(t.st_size);                           //根据文件大小，动态分配内存buf
            fread(*buf, t.st_size, 1, fd);                      //将文件读取到buf
        }
        fclose(fd);
        return rc;
    } else {                                                    //失败
        WRITELOG("LOG_DEBUG", "open %s failed %s\n", filename, strerror(errno));
        return 0;
    }
}

//获取目录下所以文件的详细数据,包括日期和时间
void file_infos(const char *dir, char *name, char *linebuf, int *len)
{
    struct stat sb;
    char encoded_name[1024];
    char value[TEMP_SIZE] = { 0 };
    char path[4096];
    char timestr[16];
    int value_len = 0;
    int ret = 0;
    *len = 0;

    strencode(encoded_name, sizeof(encoded_name), name); //解码

    //. ..这种文件都隐藏掉
    if( strcmp(name,".") && strcmp(name, "..") )
    {
        snprintf(path, sizeof(path), "%s/%s", dir, name);
        if (lstat(path, &sb) < 0)
            *len = sprintf(linebuf,"<a href=\"./%s/\">%-32.32s</a>\n", encoded_name, name);
        else {
            strftime(timestr, sizeof(timestr), "%d%b%Y %H:%M", localtime(&sb.st_mtime));
            if(S_ISDIR(sb.st_mode))
            {
                *len = sprintf(linebuf,"<a href=\"./%s/\">%-32.32s</a>  %15s  %14ld-\n", encoded_name,  \
                        name, timestr, (int64_t)sb.st_size);
            }else {
                *len = sprintf(linebuf,"<a href=\"./%s\">%-32.32s</a>  %15s  %14ld-\n", encoded_name,  \
                        name, timestr, (int64_t)sb.st_size);
            }
        }
    }
}


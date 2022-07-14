#include "strings.h"

/*用字符分割字符串，将分割后的字符串存储到指定的数组内*/
//int spitString(char *str, char ch, char array[][128], int *count)
//{
//    char *p = str;
//    char *q = p;
//    int temp_count = 0;
//    int len = 0;
//
//    if (str == NULL || array == NULL || count == NULL) {
//        fprintf(stderr, "str == NULL || array == NULL || count == NULL\n");
//        return -1;
//    }
//
//    //在一个字符串中 找到一个字符  找到了 返回第一个字符的地址， 失败返回NULL
//    //strchr(母串， 字符)
//    while ((p = strchr(p, ch)) != NULL) {
//        //找到了
//        strncpy(array[temp_count], q, p - q);
//        array[temp_count][p - q] = '\0';
//
//        temp_count++;
//        p++;
//        q = p;
//        if (*p == '\0') {
//            break;
//        }
//    }
//
//    if (*q != '\0') {
//        len = (str + strlen(str)) - q;
//        strncpy(array[temp_count], q, len);
//        array[temp_count][len] = '\0';
//        temp_count++;
//    }
//
//    *count = temp_count;
//
//    return 0;
//}

    //char *tmp = "aaaaa...abbbbb...fwqfqeeqfeq...aqwfqwfq...qw1221...asd13112...1241413";
    //int cnt1 = strlen(tmp);
    //char **array = NULL;
    //int *count = 0;
    //int retn = 0;
    //int i = 0;

    //retn = spitStringByString(tmp, "...", cnt1,&array, &count);
    //for (i = 0; i < count; i++) {
    //    WRITELOG("LOG_DEBUG", "22222222222222222array[%d]: [%s]", i, array[i]);
    //}
    //free_mem_array(&array, count);

    //char *value = getArgValueByName("name", httpreq.argarray, httpreq.argcnt);
    //WRITELOG("LOG_DEBUG", "value = [%s]", value);
    //if(value!= NULL){
    //    free(value);
    //}


/*
   有一个字符串开头或结尾含有n个空格
   （” abcdefgdddd ”），
   欲去掉前后空格，返回一个新字符串。
   要求1：请自己定义一个接口（函数），并实现功
   能；
   要求2：编写测试用例。
   int trimSpace(char *inbuf, char *outbuf);
   */
int trimSpace(char *inbuf, char *outbuf)
{
    int i = 0;
    int j = 0;
    int len = 0;

    if (inbuf == NULL || outbuf == NULL) {
        fprintf(stderr, " (inbuf == NULL || outbuf == NULL)\n  ");
        return -1;
    }

    j = strlen(inbuf) - 1;

    //左边开始遍历
    while (isspace(inbuf[i]) && inbuf[i] != '\0') {
        i++;
    }

    //右边
    while (isspace(inbuf[j]) && j > i) {
        j--;
    }

    len = j - i + 1;

    strncpy(outbuf, inbuf + i, len);
    outbuf[len] = '\0';

    return 0;
}


/*
   2、键值对（“key = value”）字符串，在开发中经常使用.
   要求1：请自己定义一个接口，实现根据key获取.
   要求2：编写测试用例。
   要求3：键值对中间可能有n多空格，请去除空格
   注意：键值对字符串格式可能如下：
   "key1    =    value1"
   "key2    =                            value2"
   "key3        =    hello word      "
   "key4                                =    value4"
   "key5            =            "
   "key6            ="
   int    getKeyByValue(char    *keyvaluebuf,        char    *keybuf,        char    *valuebuf,    int    *
   valuebuflen);
   int    main(void)
   {
//...
getKeyByValude("key1    =    valude1",    "key1",    buf,    &len);
//...
return 0;
}
*/


/*
   此函数功能 解析 类似以下字符串
   "key1    =    value1"
   " key2       =                            value2            "
//             ↑
"key3        =    hello word      "
"key4                                =    value4"
"key5            =            "
"key6            ="
通过key值 找到value

keyvaluebuf  in  : 传入原字符串
keybuf          in  : 要找到的key值
valuebuf      out: 得到value值
valuebuflen  out: 得到的value长度


return   0 成功 -1 失败

*/
int getKeyByValue(char *keyvaluebuf, char *keybuf, char *valuebuf, int *valuebuflen)
{
    char *p = keyvaluebuf; //原字符串
    char *key_p = keybuf; //key : "key1"
    char *value_p = valuebuf;// 得到的 目的内存首地址
    int ret = 0;
    int len = 0;

    if (keyvaluebuf == NULL || keybuf == NULL || valuebuf == NULL || valuebuflen == NULL) {
        fprintf(stderr, "[%s:%d]keyvaluebuf == NULL || keybuf == NULL || valuebuf == NULL || valuebuflen == NULL\n",
                __FUNCTION__, __LINE__);
        return -1;
    }

    //查找keyvaluebuf 是否有 keybuf  
    p = strstr(p, key_p);
    if (p == NULL) {
        //没找到 key 值
        fprintf(stderr, "未找到 key%s", key_p);
        return -1;
    }
    p += strlen(key_p);

    //查找keyvaluebuf 是否有 =
    p = strstr(p, "=");
    if (p == NULL) {
        //没找到=
        fprintf(stderr, "未找到=\n");
        return -1;
    }
    p += strlen("=");

    //找到=号右边的value 去掉value两端的空白字符
    ret = trimSpace(p, value_p);
    if (ret < 0) {
        fprintf(stderr, "trimSpace err\n");
        return -1;
    }

    len = strlen(value_p);
    *valuebuflen = len;

    return 0;
}

int spitString(char *str, char ch, char ***array_p, int *count)
{
    char * p = str;
    char * q = p;
    int temp_count = 0;
    char **array = NULL;
    int str_len = 0;
    int retn = 0;

    if (str == NULL || array_p == NULL || count == NULL) {
        fprintf(stderr, " (str == NULL || array_p == NULL || count == NULL)\n");
        return -1;
    }

    //1 求出 字符串中 拆分的个数
    while ((p = strchr(p, ch)) != NULL) {
        temp_count++;
        p++;
        q = p;
        if (*q == '\0') {
            break;
        }
    }
    if (*q != '\0') {
        temp_count++;
    }
    //此时temp_count 就是 子字符串的个数
    
    //2 根据个数开辟指针数组 在堆上
    array = (char**)malloc(sizeof(char*)* temp_count);
    if (array == NULL) {
        fprintf(stderr, "malloc  char **array error\n");
        retn = -1;
        goto END;
    }
    memset(array, 0, sizeof(char*)*temp_count);

    //3 拆分字符串， 为每一个指针开辟堆空间 拷贝字符串
    p = str;
    q = p;
    temp_count = 0;

    while ((p = strchr(p, ch)) != NULL) {
        //找到了
        str_len = p - q;
        array[temp_count] = (char*)malloc(sizeof(char)* (str_len+1));
        if (array[temp_count] == NULL) {
            fprintf(stderr, "malloc array[%d] error\n", temp_count);
            retn = -1;
            goto END;
        }
        strncpy(array[temp_count], q, str_len);
        array[temp_count][str_len] = '\0';

        temp_count++;
        p++;
        q = p;
        if (*p == '\0') {
            break;
        }
    }

    if (*q != '\0' ) {
        str_len = (str + strlen(str)) - q;
        array[temp_count] = (char*)malloc(sizeof(char)*(str_len + 1));
        if (array[temp_count] == NULL) {
            fprintf(stderr, "malloc array[%d] error\n", temp_count);
            retn = -1;
            goto END;
        }

        strncpy(array[temp_count], q, str_len);
        array[temp_count][str_len] = '\0';

        temp_count++;
    }

    if (array != NULL) {
        *array_p = array;
        *count = temp_count;
    }

    //释放内存的步骤
END:
    if (retn != 0) {
        //已经出现错误了
        free_mem(&array, temp_count);
    }
    return 0;
}

int spitStringByString(char *str, char *ch, int strLen, char ***array_p, int *count)
{
    char * p = str;
    char * q = p;
    int temp_count = 0;
    char **array = NULL;
    int str_len = 0;
    int retn = 0;

    if (str == NULL || array_p == NULL || count == NULL) {
        fprintf(stderr, " (str == NULL || array_p == NULL || count == NULL)\n");
        return -1; 
    }   

    //1 求出 字符串中 拆分的个数
    while ((p = strstr(p, ch)) != NULL) {
        temp_count++;
        p++;
        q = p;
        if (*q == '\0') {
            break;
        }
    }
    if (*q != '\0') {
        temp_count++;
    }
    //此时temp_count 就是 子字符串的个数


    //2 根据个数开辟指针数组 在堆上
    array = (char**)malloc(sizeof(char*)* temp_count);
    if (array == NULL) {
        fprintf(stderr, "malloc  char **array error\n");
        retn = -1;
        goto END;
    }
    memset(array, 0, sizeof(char*)*temp_count);

    //3 拆分字符串， 为每一个指针开辟堆空间 拷贝字符串
    p = str;
    q = p;
    temp_count = 0;

    while ((p = strstr(p, ch)) != NULL) {
        if(temp_count == 0){
            str_len = p - q ;
            array[temp_count] = (char*)malloc(sizeof(char)* (str_len+1));
            if (array[temp_count] == NULL) {
                fprintf(stderr, "malloc array[%d] error\n", temp_count);
                retn = -1;
                goto END;
            }
            strncpy(array[temp_count], q, str_len);
            array[temp_count][str_len] = '\0';
            temp_count++;
            p++;
            q = p;
            if (*p == '\0') {
                break;
            }

        }else{
            str_len = p - q ;
            array[temp_count] = (char*)malloc(sizeof(char)* (str_len-strlen(ch)+1+1));
            if (array[temp_count] == NULL) {
                fprintf(stderr, "malloc array[%d] error\n", temp_count);
                retn = -1;
                goto END;
            }
            strncpy(array[temp_count], q+strlen(ch)-1, str_len-strlen(ch)+1);
            array[temp_count][str_len-strlen(ch)+1+1] = '\0';

            temp_count++;
            p++;
            q = p;
            if (*p == '\0') {
                break;
            }
        } 
    }

    if (*q != '\0') {
        if(*q == 0X0A){
            ;
        }else{
            str_len = (str + strlen(str)) - q;
            array[temp_count] = (char*)malloc(sizeof(char)*(str_len+1));
            if (array[temp_count] == NULL) {
                fprintf(stderr, "malloc array[%d] error\n", temp_count);
                retn = -1;
                goto END;
            }

            strncpy(array[temp_count], q+strlen(ch)-1, str_len);
            array[temp_count][str_len] = '\0';
            temp_count++;
        }
    }

    if (array != NULL) {
        *array_p = array;
        *count = temp_count;
    }

    //释放内存的步骤
END:
    if (retn != 0) {
        //已经出现错误了
        free_mem(&array, temp_count);
    }
    return 0;
}

#ifdef XML2JSONCONV_TEST // test harness
int main(int argc, char **argv){
    char *tmp = "aaaaa...abbbbb...fwqfqeeqfeq...aqwfqwfq...qw1221...asd13112...1241413";
    int cnt1 = strlen(tmp);
    char **array = NULL;
    int *count = 0;
    int retn = 0;
    int i = 0;

    retn = spitStringByString(tmp, "...", cnt1,&array, &count);
    for (i = 0; i < *count; i++) {
        WRITELOG("LOG_DEBUG", "array[%d]: [%s]", i, array[i]);
    }
}
#endif

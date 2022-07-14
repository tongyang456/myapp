/*base64.h*/
#ifndef _BASE64_H
#define _BASE64_H
 
#include "../frame/pub.h"
unsigned char *base64_encode(unsigned char *str,int strlen);
unsigned char *base64_decode(unsigned char *code,int *codelen);
#endif

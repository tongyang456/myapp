/*http.h*/
#ifndef STRINGS_H
#define STRINGS_H

#include "../frame/pub.h"
//int spitString(char *str, char ch, char array[][128], int *count);
int spitString(char *str, char ch, char ***array_p, int *count);
int spitStringByString(char *str, char *ch, int strLen, char ***array_p, int *count);
int trimSpace(char *inbuf, char *outbuf);
int getKeyByValue(char *keyvaluebuf, char *keybuf, char *valuebuf, int *valuebuflen);

#endif

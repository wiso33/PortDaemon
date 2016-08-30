#include "UrlForm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "logfile.h"

UrlForm::UrlForm(const char *url){
    int len;
    strcpy(buf, url);

    char *p = buf;
    scheme = p;
    while(*p && *p != ':')p++;
    if(*p == ':'){
        *p = 0;
        p += 3;
    }else{
        scheme = 0;
        return;
    }

    host = p;
    while(*p && *p != ':' && *p != '/')p++;

    if(*p == ':'){
        *p++ = 0;
        port = 0;
        while(*p >= '0' && *p <= '9'){
            port =  (port * 10) + (*p++ - '0');
        }
        len = p - buf;
    }else{
//        memmove(p + 1, p, strlen(p) + 1);
//        port = 80;
//        *p++ = 0;
        port = 80;
        len = p - buf;
        *p++ = 0;
    }
    urlEncode(p, url + len);
    path = p;
//    logfile::aaa(path);
}


void UrlForm::urlEncode(char *dst, const char *src){
    unsigned char a, c;
    while(*src){
        c = *src++;
        if(isalnum(c) || strchr("_-.~!#$&'()*+,/:;=?@[]", c)){
            *dst++ = c;
        }else{
            *dst++ = '%';
            a = (c >> 4);
            *dst++ = a > 9? a + 55 : a + '0';
            a = (c & 0x0f);
            *dst++ = a > 9? a + 55 : a + '0';
        }
    }
    *dst = 0;
}

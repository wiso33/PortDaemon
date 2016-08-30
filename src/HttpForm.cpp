#include "HttpForm.h"

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "logfile.h"

const char Content_Length[] = "Content-Length:";

int headLength(char *head, int len);
int contentLength(char *head, int);
int statusCode(char *);


HttpForm::HttpForm(){
    head = 0;
    status_code = 0;
    head_length = 0;
    content_length = 0;
    content = 0;
}

HttpForm::~HttpForm(){
//    char buf[256];
//    sprintf(buf, "~HttpForm: %lx\n", (long)head);
//    printf(buf);
    free(head);
}

void HttpForm::parse(char *str, int len){
    head_length = headLength(str, len);
    if(head_length){
        content_length = contentLength(str, len - head_length);
        head = (char *)malloc(head_length + content_length + 1);
        status_code = statusCode(str);
        if(head){
            memcpy(head, str, len);
            content = head + head_length;
            head[head_length + content_length] = 0;
        }
    }
}

void HttpForm::clean(){
    free(head);
    head = 0;
    status_code = 0;
    head_length = 0;
    content_length = 0;
    content = 0;
}

int headLength(char *head, int len){
    for(char *p = head; p < head + len; p++){
        if(*p == 0x0d && *(p + 1) == 0x0a && *(p + 2) == 0x0d && *(p + 3) == 0x0a){
            return (p - head) + 4;
        }
    }
    return 0;
}

int contentLength(char *head, int def_len){
    char *p = strstr(head, Content_Length);
    if(p){
        int code = 0;
        p += sizeof(Content_Length);
        while(*p != 0x0d && (*p < '0' || *p > '9'))p++;
        while(*p >= '0' && *p <= '9'){
            code = code * 10 + (*p++ - '0');
        }
        return code;
    }
    return def_len;
}

int statusCode(char *p){
    int code = 0;
    while(*p != ' ')p++;
    while(*p == ' ')p++;
    while(*p >= '0' && *p <= '9'){
        code = code * 10 + (*p++ - '0');
    }
    return code;
}

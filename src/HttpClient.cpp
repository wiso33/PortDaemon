#include "HttpClient.h"
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include "UrlForm.h"
#include "logfile.h"

const char HTTP_GET_HEAD[] = "GET %s HTTP/1.1\r\n"
                    "Accept: */*\r\n"
                    "Accept-Language: cn\r\n"
                    "User-Agent: Mozilla/4.0\r\n"
                    "Host: %s\r\n"
                    "Connection: Keep-Alive\r\n"
                    "\r\n\r\n";

const char HTTP_POST_HEAD[] = "POST %s HTTP/1.1\r\n"
                "Host: %s\r\n"
                "Content-Type: application/x-www-form-urlencoded\r\n"
                "Content-Length: %d\r\n"
                "\r\n%s";

int time_out = 30;

int receive(int sockfd, char *buf, int bufsize);

void setTimeout(int time){
    time_out = time;
}

int httpGet(HttpForm &httpForm, const char *url){
    int sockfd, ret;
    struct sockaddr_in servaddr;
    char str[4096], *p;

    httpForm.clean();
    UrlForm urlForm(url);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        logfile::aaa("socket error!");
        return HTTP_ERR_SOCKET;
    };

    bzero(&servaddr, sizeof(sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(urlForm.port);
//    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (inet_pton(AF_INET, urlForm.host, &servaddr.sin_addr) <= 0) {
        logfile::aaa("inet_pton error!");
        return HTTP_ERR_INET;
    }

    if (connect(sockfd, (sockaddr *)&servaddr, sizeof(sockaddr_in)) < 0) {
        logfile::aaa("connect error!");
        return HTTP_ERR_CONNECT;
    }

    sprintf(str, HTTP_GET_HEAD, urlForm.path, urlForm.host);
//    logfile::aaa(str);

    ret = send(sockfd, (void *)str, strlen(str), 0);//same as: int write(int fp, void *buf, int buflen);
    if (ret < 0) {
        sprintf(str, "send error %d£¬Error messaqe'%s'\n", errno, strerror(errno));
        logfile::aaa(str);
        return HTTP_ERR_REQUEST;
    }else{
//        logfile::aaa("send success ,total send ", ret);
    }

    ret = receive(sockfd, str, sizeof(str));
    if(ret < 0){
        close(sockfd);
        return ret;
    }

    httpForm.parse(str, ret);
    p = httpForm.head + ret;
    int len = httpForm.head_length + httpForm.content_length - ret;
    while(len){
        ret = receive(sockfd, p, len);
        if(ret > 0){
            p += ret;
            len -= ret;
        }else break;
    }

    close(sockfd);
    return HTTP_OK;
}

int httpPost(HttpForm &httpForm, const char *url, const char *content){
    int sockfd, ret;
    struct sockaddr_in servaddr;
    char str[4096], *p;

    httpForm.clean();
    UrlForm urlForm(url);
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        logfile::aaa("socket error!");
        return HTTP_ERR_SOCKET;
    };

    bzero(&servaddr, sizeof(sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(urlForm.port);

    if (inet_pton(AF_INET, urlForm.host, &servaddr.sin_addr) <= 0) {
        logfile::aaa("inet_pton error!");
        return HTTP_ERR_INET;
    }

    if (connect(sockfd, (sockaddr *)&servaddr, sizeof(sockaddr_in)) < 0) {
        logfile::aaa("connect error!");
        return HTTP_ERR_CONNECT;
    }

    sprintf(str, HTTP_POST_HEAD, urlForm.path, urlForm.host, strlen(content), content);
//    logfile::aaa(str);

    ret = send(sockfd, (void *)str, strlen(str), 0);
    if (ret < 0) {
        sprintf(str, "send error %d£¬Error messaqe'%s'\n", errno, strerror(errno));
        logfile::aaa(str);
        return HTTP_ERR_REQUEST;
    }else{
//        logfile::aaa("send success ,total send ", ret);
    }

    ret = receive(sockfd, str, sizeof(str));
    if(ret < 0){
        close(sockfd);
        return ret;
    }

    httpForm.parse(str, ret);
    p = httpForm.head + ret;
    int len = httpForm.head_length + httpForm.content_length - ret;
    while(len){
        ret = receive(sockfd, p, len);
        if(ret > 0){
            p += ret;
            len -= ret;
        }else break;
    }

    close(sockfd);
    return HTTP_OK;
}

int receive(int sockfd, char *buf, int bufsize){
    fd_set t_set1;
    struct timeval tv;

//  usleep(1000);
    tv.tv_sec = time_out;
    tv.tv_usec = 0;
    FD_ZERO(&t_set1);
    FD_SET(sockfd, &t_set1);
    int h = select(sockfd + 1, &t_set1, NULL, NULL, &tv);
//    logfile::aaa("select:", h);
    if(h > 0){
        int len = recv(sockfd, (void *)buf, bufsize, 0);
//        logfile::bbb(buf, len);
        return len;
    }else
    if(h == 0){
        logfile::aaa("HTTP timeout");
        return HTTP_ERR_TIMEOUT;
    }else{
        logfile::aaa("HTTP receive error");
        return HTTP_ERR_RECEIVE;
    }
}

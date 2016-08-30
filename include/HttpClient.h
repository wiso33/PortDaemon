#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "HttpForm.h"

#define HTTP_OK             0
#define HTTP_ERR_SOCKET     -1
#define HTTP_ERR_INET       -2
#define HTTP_ERR_CONNECT    -3
#define HTTP_ERR_REQUEST    -4
#define HTTP_ERR_RECEIVE    -5
#define HTTP_ERR_TIMEOUT    -6

int httpGet(HttpForm &form, const char *url);

int httpPost(HttpForm &form, const char *url, const char *content);

void setTimeout(int time);

#endif // HTTPCLIENT_H

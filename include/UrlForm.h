#ifndef URLFORM_H
#define URLFORM_H


class UrlForm
{
    char buf[1024];
    public:
        UrlForm(const char *url);
        char *scheme;
        char *host;
        int port;
        char *path;

        static void urlEncode(char *dst, const char *src);
};

#endif // URLFORM_H

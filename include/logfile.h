#ifndef LOGFILE_H
#define LOGFILE_H

class logfile
{
    public:
        static void init();
        static void checkfile();
        static char * getfile();
//        static void aaa(int a,...);
        static void aaa(const char *str);
        static void aaa(const char *str, char *text);
        static void bbb(const char *str, int len);
        static void aaa(const char *str, int a);
        static void aaa(const char *str, int a, int b);
        static void printf(const char *str,...);
};

#endif // LOGFILE_H

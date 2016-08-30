#ifndef CINIFILE_H
#define CINIFILE_H


class CIniFile
{
    char *iBuf, *iEnd;
    char *getPos(const char *key);

    public:
        CIniFile();
        virtual ~CIniFile();
        char * load(const char *pathfile);

        bool getItem(const char *key, char *var);
        bool getItem(const char *key, int &var);

};

#endif // CINIFILE_H

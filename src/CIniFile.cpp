#include "CIniFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

CIniFile::CIniFile(){
    iBuf = 0;
}

char * CIniFile::load(const char *pathfile){
    FILE *fp;
    fp = fopen(pathfile, "rb");
    if(fp > 0){
        fseek(fp, 0, SEEK_END);
        int len = ftell(fp);
        free(iBuf);
        iBuf = (char *)malloc(len);
        if(iBuf != 0){
            rewind(fp);
            fread(iBuf, len, 1, fp);
            iEnd = iBuf + len;
        }
        fclose(fp);
    }
//    int handle = open(pathfile, O_RDONLY);
//    int len = lseek(handle, 0, SEEK_END);
//    printf("flen: %d\n", len);
//    close(handle);
    return iBuf;
}

CIniFile::~CIniFile()
{
    free(iBuf);
//    printf("CIniFile::~CIniFile()\n");
}

bool CIniFile::getItem(const char *key, char *str_var){
    char *p = getPos(key);
    char *q = str_var;
    if(p){
        while(p < iEnd && *p != 0x0d && *p != 0x0a)*q++ = *p++;
        if(q != str_var){
            *q = 0;
            return true;
        }
    }
    return false;
}

bool CIniFile::getItem(const char *key, int &var){
    char *p = getPos(key);
    char *q = p;
    if(p){
        int i = 0;
        while(p < iEnd && *p >= '0' && *p <= '9')i = (i * 10) + (*p++ - '0');
        if(p != q){
            var = i;
            return true;
        }
    }
    return false;
}

char * CIniFile::getPos(const char *key){
    if(iBuf){
        key = strstr(iBuf, key);
        if(key){
            char *p = (char *)strchr(key, '=');
            if(p){
                p++;
                while(*p == ' ' || *p == 0x09)p++;
                return p;
            }
        }
    }
    return 0;
}

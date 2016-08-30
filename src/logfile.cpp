#include "logfile.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "main.h"

const char CR[] = "\r\n";
//const char log_file[] = "/home/yn/log.txt";
//char *log_file;
char log_file[256];
char today[10] = "";
void logfile::init(){
    sprintf(log_file, "log_v%s.txt", VERSION);
//    remove(log_file);
}

void logfile::checkfile(){
    char tmp[10], name[64];
    time_t t;

    time(&t);
    struct tm *p = localtime(&t);
    sprintf(tmp, "%d%02d%02d", 1900 + p->tm_year, 1+p->tm_mon, p->tm_mday);

    if(strcmp(tmp, today)){
        if(strlen(today)){
            sprintf(name, "log_v%s_%s.txt", VERSION, today);
            rename(log_file, name);
        }
        strcpy(today, tmp);
    }
}

char * logfile::getfile(){
    return log_file;
}

void logTime(FILE *fp){

    time_t t;
    time(&t);
    struct tm *p = localtime(&t);
    fprintf(fp, "%02d:%02d:%02d ", p->tm_hour, p->tm_min, p->tm_sec);
}

//C的不定参数不成熟,参数个数不好确定
//void logfile::aaa(int a,...){
//    FILE *fp = fopen(log_file, "ab");
//    if(fp){
//        va_list valist;
//        va_start(valist, a);
//        int i;
//        while(1){
//            i = va_arg(valist, int);
//            if(i == 0)break;
//            fprintf(fp, "%d ", i);
//        }
//        fclose(fp);
//        va_end(valist);
//    }
//}

void logfile::aaa(const char *str){
    FILE *fp = fopen(log_file, "ab");
    if(fp){
        logTime(fp);
        if(str)fputs(str, fp);
        else fputs("null", fp);
        fwrite(CR, 2, 1, fp);
        fclose(fp);
    }
}

void logfile::aaa(const char *str, char *text){
    FILE *fp = fopen(log_file, "ab");
    if(fp){
        logTime(fp);
        if(str)fputs(str, fp);
        else fputs("null", fp);

        if(text)fputs(text, fp);
        else fputs("null", fp);

        fwrite(CR, 2, 1, fp);
        fclose(fp);
    }
}

void logfile::aaa(const char *str, int a){
    FILE *fp = fopen(log_file, "ab");
    if(fp){
        logTime(fp);
        fprintf(fp, "%s%d\r\n", str, a);
        fclose(fp);
    }
}

void logfile::bbb(const char *str, int len){
    FILE *fp = fopen(log_file, "ab");
    if(fp){
        logTime(fp);
        if(str){
            if(len > 0){
                fwrite(str, len, 1, fp);
            }else{
                fprintf(fp, "len:%d", len);
            }
        }else{
            fputs("null", fp);
        }
        fputs(CR, fp);
        fclose(fp);
    }
}

void logfile::aaa(const char *str, int a, int b){
    FILE *fp = fopen(log_file, "ab");
    if(fp){
        logTime(fp);
        fprintf(fp, "%s%d_%d\r\n", str, a, b);
        fclose(fp);
    }
}

void logfile::printf(const char *str,...){
    FILE *fp = fopen(log_file, "ab");
    if(fp){
        logTime(fp);
        fprintf(fp, str);
        fclose(fp);
    }
}

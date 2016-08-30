
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "main.h"
#include "HttpClient.h"
#include "CIniFile.h"
#include "logfile.h"
#include "UrlForm.h"

using namespace std;

const char *INI_FILE = "config.ini";
const char *INI_CHECK_URL = "check_url";
const char *INI_CHECK_PERIOD = "check_period";
const char *INI_ERROR_NUM = "error_num";
const char *INI_ERROR_PERIOD = "error_period";
const char *INI_NOTIFY_URL = "notify_url";
const char *INI_CHKVER_URL = "chkver_url";
const char *INI_CHKVER_PERIOD = "chkver_period";
const char *INI_TIMEOUT = "http_timeout";
const char *INI_SERVER_SHUTDOWN = "server_shutdown";
const char *INI_SERVER_START = "server_start";

extern int time_out;

#define SIZE 1024

bool tomcatExists(char *basePath);
int checkVersion(char *verchk_url);

int main (int argc, char *argv[])
{
    int i, check_period, error_num, error_period, chkver_period, cnt_u, cnt_v, try_num = 0;
    char check_url[SIZE], startup[SIZE], shutdown[SIZE], notify_url[SIZE], chkver_url[SIZE];
    pid_t fpid;
    HttpForm form;
    CIniFile ini;


//    FILE *fp = popen("pgrep PortDaemon", "r");//ps -ef|grep PortDaemon
//    if(fp){
//        const char kill[] = "kill ";
//        char tmp[2048];
//        strcpy(tmp, kill);
//        fgets(tmp + sizeof(kill) - 1, sizeof(tmp) - sizeof(kill), fp);
//        fclose(fp);
//        tmp[strlen(tmp) - 1] = 0;
//        printf(tmp);
//        system(kill);
//        return 0;
//    }
    logfile::init();

    if(ini.load(INI_FILE) == 0){
        printf("Fail to load config.ini");
        return 1;
    }

    if(ini.getItem(INI_CHECK_URL, check_url) == false)return 2;
    if(ini.getItem(INI_CHECK_PERIOD, check_period) == false)return 3;
    if(ini.getItem(INI_ERROR_NUM, error_num) == false)return 4;
    if(ini.getItem(INI_ERROR_PERIOD, error_period) == false)return 5;
    if(ini.getItem(INI_NOTIFY_URL, notify_url) == false)return 6;
    if(ini.getItem(INI_CHKVER_URL, chkver_url) == false)return 7;
    if(ini.getItem(INI_CHKVER_PERIOD, chkver_period) == false)return 8;
    if(ini.getItem(INI_TIMEOUT, time_out) == false)return 9;
    if(ini.getItem(INI_SERVER_SHUTDOWN, shutdown) == false)return 10;
    if(ini.getItem(INI_SERVER_START, startup) == false)return 11;
    cnt_u = check_period;
    cnt_v = 0;

//    httpGet(form, notify_url);
//    return 0;

    fpid = fork();
    if (fpid < 0){
        printf("error in fork!");
        exit(1);
    }else if (fpid > 0) {
        logfile::aaa("parent process id: ", getpid());
    }else{
        logfile::aaa("child process id: ", setsid());
        while( 1 )
        {
            logfile::checkfile();
            if(cnt_u-- == 0){
                if(ini.load(INI_FILE)){
                    ini.getItem(INI_CHECK_URL, check_url);
                    ini.getItem(INI_CHECK_PERIOD, check_period);
                    ini.getItem(INI_ERROR_NUM, error_num);
                    ini.getItem(INI_ERROR_PERIOD, error_period);
                    ini.getItem(INI_NOTIFY_URL, notify_url);
                    ini.getItem(INI_TIMEOUT, time_out);
                    ini.getItem(INI_SERVER_SHUTDOWN, shutdown);
                    ini.getItem(INI_SERVER_START, startup);
                }

                httpGet(form, check_url);

                logfile::aaa("http_status_code: ", form.status_code);
                cnt_u = check_period;
                if(form.status_code == 200)
                {
                    try_num = 0;
                }else{
                    if(++try_num < error_num){
                        cnt_u = error_period;
                    }else{
                        for(i = 0; i < error_num; i++){
                            httpGet(form, notify_url);
                            if(form.status_code == 200)break;
                        }
                        if(i < error_num){
                            logfile::aaa("Success to notify.");
                        }else{
                            logfile::aaa("Fail to notify.");
                        }

                        logfile::aaa("server shutdown......");
                        system(shutdown);
                        sleep(10);

                        logfile::aaa("server start......");
        //                char * argv[]={};
        //                execv(startup, 0);
                        system(startup);
                        try_num = 0;
                    }
                }
            }
            if(cnt_v-- == 0){
                if(ini.load(INI_FILE)){
                    ini.getItem(INI_CHKVER_URL, chkver_url);
                    ini.getItem(INI_CHKVER_PERIOD, chkver_period);
                }
                cnt_v = chkver_period;
                i = checkVersion(chkver_url);
                logfile::aaa("check version:", i);

                if(i == 0){
                    break;
                }
            }
            sleep(1);
        }
    }
    return 0;
}

const char EXE_NAME[] = "PortDaemon";
const char CMD1[] = "chmod u+x PortDaemon";
const char CMD2[] = "./PortDaemon";
const char VER_FILE[] = "version.txt";

int checkVersion(char *verchk_url){
    HttpForm form;
    char *p = verchk_url;
    while(*p)p++;
    strcpy(p, VER_FILE);

    httpGet(form, verchk_url);
    if(form.status_code != 200)return 1;
    if(memcmp(VERSION, form.content, strlen(VERSION)) >= 0)return 2;

    strcpy(p, EXE_NAME);
    httpGet(form, verchk_url);

    if(form.status_code != 200)return 3;

    remove(EXE_NAME);
    usleep(500000);

    FILE *fp = fopen(EXE_NAME, "wb");
    if(fp == 0){
        return 4;
    }
    fwrite(form.content, form.content_length, 1, fp);
    fclose(fp);

    system(CMD1);
    system(CMD2);
    return 0;
}
/*
bool tomcatExists(char *basePath){
    char buf[2048], path[256];
    bool rv = true;
    FILE *fp = popen("ps aux|grep tomcat", "r");
    if(fp){
        fgets(buf, sizeof(buf), fp);
        fclose(fp);
        logfile::aaa(buf);
        strcpy(path, basePath);
        strcat(path, "bin");//filter process of tail -f tomcat/logs/catalina.out
        if(strstr(buf, path) == 0)rv = false;
    }else{
        logfile::aaa("popen fail");
    }
    return rv;
}
*/


/*
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include "logfile.h"

using namespace std;

#define MAXFILE 65535

void sigterm_handler(int arg);
volatile sig_atomic_t _running = 1;


int main()
{
    pid_t pc,pid;
//    int i,fd,len,flag = 1;
    int len, i;
    char buf[]="this is a Dameon\n";
    len = strlen(buf);
    logfile::clean();
    pc = fork(); //第一步
    if(pc<0){
        printf("error fork\n");
        exit(1);
    }else if(pc>0){
        exit(0);
    }
    pid = setsid(); //第二步
    if (pid < 0){
        perror("setsid error");
    }
    chdir("/"); //第三步
    umask(0); //第四步
    for(i=0;i<MAXFILE;i++) //第五步
    close(i);
    signal(SIGTERM, sigterm_handler);
    while( _running )
    {
//        if( flag ==1 &&(fd=open("/tmp/daemon.log",O_CREAT|O_WRONLY|O_APPEND,0600))<0)
//        {
//            perror("open");
//            flag=0;
//            exit(1);
//        }
//        write(fd,buf,len);
//        close(fd);
        logfile::aaa("connect fail");
        usleep(10*1000000); //10毫秒
    }
}
void sigterm_handler(int arg)
{
    _running = 0;
}
*/

#pragma once

// CUtils 命令目标
//////////////////////////////////////////////////////////////////////////
// 类名: CUtils
// 工具包类
//////////////////////////////////////////////////////////////////////////
#ifndef _UTILS_H
#define _UTILS_H

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#if !defined(__linux__)
#include <WINSOCK2.H>

#else
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <syslog.h>
#include<netinet/in.h>
#include <iconv.h>


// 类型转换
typedef unsigned int    UINT;
typedef void*           LPVOID;
typedef unsigned long   DWORD;
typedef unsigned long   ULONG;
typedef unsigned short  WORD;
typedef int             SOCKET;
typedef const char*     LPCSTR;
typedef const char*     LPCTSTR;
typedef char            LPSTR;
typedef long            BOOL;
typedef unsigned int    UINT_PTR;
typedef sockaddr        SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR    (-1)
#define MAX_PATH		260
#define IN
#define OUT

// 函数转换
extern int AfxBeginThread(UINT(*start)(void *), void *arg);
extern DWORD GetTickCount();
extern void Sleep(DWORD dwMilliseconds);
#endif	// #if !defined(__linux__)

# ifndef __socklen_t_defined
typedef int socklen_t;
#endif // __socklen_t_defined
using namespace std;


////////////////////////////////define//////////////////////////////////////////
#define MAX_THREADS     1							// 线程数
#define MAX_BUF			1024*10						// 通用缓存大小
#define VERSION         "Program Version: v2.28"	// 版本号
#define PRINTF			CUtils::WriteLogPrint
#define LOG				CUtils::WriteLog
#define DEBUGLOG		CUtils::WriteDebugLog


#if defined(__linux__)
// 转移为UTF8编码
#define _LOCAL(a)					CUtils::Gb2312ToUtf8(a).c_str()
#define CREATEDIRECTORY(pDirName)	mkdir(pDirName, 0755)
#else
// 转移为GB2312编码
#define _LOCAL(a)					CUtils::Utf8ToGb2312(a).c_str()
#define CREATEDIRECTORY(pDirName)	CreateDirectory(pDirName, NULL)
#endif
////////////////////////////////define//////////////////////////////////////////


struct DataBase
{
	// 数据库配置信息
	string  strHost;
	string	strDb;
	string	strUser;
	string	strPass;
};

class CUtils
{
public:

	//-----------------------------------
	// 说明: 读取Ini配置文件的工具函数
	static int	 	LoadIni(LPCSTR lpSection, LPCTSTR lpKey, LPCTSTR lpFileName, int nDefault);
	static float 	LoadIni(LPCSTR lpSection, LPCTSTR lpKey, LPCTSTR lpFileName, float fDefault);
	static string 	LoadIni(LPCSTR lpSection, LPCTSTR lpKey, LPCTSTR lpFile, LPCTSTR lpDefault = "");
	static void		SaveIni(LPCSTR lpSection, LPCTSTR lpKey, int nValue, LPCTSTR lpFileName );
	static void		SaveIni(LPCSTR lpSection, LPCTSTR lpKey, float& fValue, LPCTSTR lpFileName );
	static void		SaveIni(LPCSTR lpSection, LPCTSTR lpKey, LPCTSTR pValue, LPCTSTR lpFileName );
	//-----------------------------------

	// 获取主模块文件全路径文件名
	static char* GetModuleFileName(BOOL bPath=false);

	// 判断一个本地文件是否存在
	static BOOL IsExistFile( LPCTSTR lpFile );

	// 写日志
	static DWORD WriteLog(const char* Format,...);
	static DWORD WriteLogPrint(const char* Format,...);
	// 可自定义LOG前缀的日志
	static DWORD WriteLogPrefix(LPCSTR lpPrefix, const char* Format,...);
	// 写日志(带DEBUG开关)
	static DWORD WriteDebugLog(const char* Format,...);
	// 写日志
	static DWORD WriteLogFile(LPCSTR lpData, int nLen);

    // 替换子字符串
    static void string_replace(string&s1,const string&s2,const string&s3);

	// 分割字符串；返回分割数量
	static int Split(const string strSrc, const string strDelimiter, OUT string* strArray, int nArraySize);

	// 判断是否是UTF8编码
	static bool IsTextUTF8(const char* str,long length);

	// 把UTF-8格式字符串转换为Gb2312格式
	static string Utf8ToGb2312(const char* pText);
	// 把Gb2312字符串转换为UTF-8格式
	static string Gb2312ToUtf8(const char* pText);


private:
	static int Utf8ToGb2312(char *szInbuf, int nInlen, char *szOutbuf, int nOutlen);
	static int Gb2312ToUtf8(char *szInbuf, int nInlen, char *szOutbuf, int nOutlen);
};

// 颜色定义
#define COLOR_BLACK		0
#define COLOR_GREEN		1
#define COLOR_BLUE		2
#define COLOR_RED		3


#endif // _UTILS_H

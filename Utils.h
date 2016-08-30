#pragma once

// CUtils ����Ŀ��
//////////////////////////////////////////////////////////////////////////
// ����: CUtils
// ���߰���
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


// ����ת��
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

// ����ת��
extern int AfxBeginThread(UINT(*start)(void *), void *arg);
extern DWORD GetTickCount();
extern void Sleep(DWORD dwMilliseconds);
#endif	// #if !defined(__linux__)

# ifndef __socklen_t_defined
typedef int socklen_t;
#endif // __socklen_t_defined
using namespace std;


////////////////////////////////define//////////////////////////////////////////
#define MAX_THREADS     1							// �߳���
#define MAX_BUF			1024*10						// ͨ�û����С
#define VERSION         "Program Version: v2.28"	// �汾��
#define PRINTF			CUtils::WriteLogPrint
#define LOG				CUtils::WriteLog
#define DEBUGLOG		CUtils::WriteDebugLog


#if defined(__linux__)
// ת��ΪUTF8����
#define _LOCAL(a)					CUtils::Gb2312ToUtf8(a).c_str()
#define CREATEDIRECTORY(pDirName)	mkdir(pDirName, 0755)
#else
// ת��ΪGB2312����
#define _LOCAL(a)					CUtils::Utf8ToGb2312(a).c_str()
#define CREATEDIRECTORY(pDirName)	CreateDirectory(pDirName, NULL)
#endif
////////////////////////////////define//////////////////////////////////////////


struct DataBase
{
	// ���ݿ�������Ϣ
	string  strHost;
	string	strDb;
	string	strUser;
	string	strPass;
};

class CUtils
{
public:

	//-----------------------------------
	// ˵��: ��ȡIni�����ļ��Ĺ��ߺ���
	static int	 	LoadIni(LPCSTR lpSection, LPCTSTR lpKey, LPCTSTR lpFileName, int nDefault);
	static float 	LoadIni(LPCSTR lpSection, LPCTSTR lpKey, LPCTSTR lpFileName, float fDefault);
	static string 	LoadIni(LPCSTR lpSection, LPCTSTR lpKey, LPCTSTR lpFile, LPCTSTR lpDefault = "");
	static void		SaveIni(LPCSTR lpSection, LPCTSTR lpKey, int nValue, LPCTSTR lpFileName );
	static void		SaveIni(LPCSTR lpSection, LPCTSTR lpKey, float& fValue, LPCTSTR lpFileName );
	static void		SaveIni(LPCSTR lpSection, LPCTSTR lpKey, LPCTSTR pValue, LPCTSTR lpFileName );
	//-----------------------------------

	// ��ȡ��ģ���ļ�ȫ·���ļ���
	static char* GetModuleFileName(BOOL bPath=false);

	// �ж�һ�������ļ��Ƿ����
	static BOOL IsExistFile( LPCTSTR lpFile );

	// д��־
	static DWORD WriteLog(const char* Format,...);
	static DWORD WriteLogPrint(const char* Format,...);
	// ���Զ���LOGǰ׺����־
	static DWORD WriteLogPrefix(LPCSTR lpPrefix, const char* Format,...);
	// д��־(��DEBUG����)
	static DWORD WriteDebugLog(const char* Format,...);
	// д��־
	static DWORD WriteLogFile(LPCSTR lpData, int nLen);

    // �滻���ַ���
    static void string_replace(string&s1,const string&s2,const string&s3);

	// �ָ��ַ��������طָ�����
	static int Split(const string strSrc, const string strDelimiter, OUT string* strArray, int nArraySize);

	// �ж��Ƿ���UTF8����
	static bool IsTextUTF8(const char* str,long length);

	// ��UTF-8��ʽ�ַ���ת��ΪGb2312��ʽ
	static string Utf8ToGb2312(const char* pText);
	// ��Gb2312�ַ���ת��ΪUTF-8��ʽ
	static string Gb2312ToUtf8(const char* pText);


private:
	static int Utf8ToGb2312(char *szInbuf, int nInlen, char *szOutbuf, int nOutlen);
	static int Gb2312ToUtf8(char *szInbuf, int nInlen, char *szOutbuf, int nOutlen);
};

// ��ɫ����
#define COLOR_BLACK		0
#define COLOR_GREEN		1
#define COLOR_BLUE		2
#define COLOR_RED		3


#endif // _UTILS_H

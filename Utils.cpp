// CUtils.cpp : 实现文件

#include "stdafx.h"
#include "Utils.h"
//#pragma comment (lib,"version.lib")

extern bool g_bDebug;


#if defined(__linux__)
#define	VSNPRINTF vsnprintf
#else
#define	VSNPRINTF _vsnprintf
#endif

#if defined(__linux__)
#include <glib.h>
#include "string.h"

// 创建线程
int AfxBeginThread(UINT(*start)(void *), void *arg)
{
    pthread_t ntid;
    return pthread_create(&ntid, NULL, (void *(*)(void *))start, arg);
}

// 毫秒级时间计数
DWORD GetTickCount()
{
    struct timeval current;
    gettimeofday(&current, NULL);
    return current.tv_sec * 1000 + current.tv_usec/1000;
}

// 线程睡眠指定时间
void Sleep(DWORD dwMilliseconds)
{
    usleep(1000*dwMilliseconds);
}
#endif


// CCUtils 成员函数
// 说明: 读取Ini配置文件的函数
int CUtils::LoadIni(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szFileName, int nDefault)
{
	char szDefault[16]={0};
	sprintf(szDefault, "%d", nDefault);
	string strValue = LoadIni(szSection, szKey, szFileName, szDefault);
	return atoi(strValue.c_str());
}

float CUtils::LoadIni(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szFileName, float fDefault)
{
	char szDefault[16]={0};
	sprintf(szDefault, "%f", fDefault);
	string strValue = LoadIni(szSection, szKey, szFileName, szDefault);
	return (float)atof(strValue.c_str());
}

string CUtils::LoadIni(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR lpFile, LPCTSTR szDefault)
{
	char szValue[MAX_PATH]={0};
#if defined(__linux__)
	GKeyFile* keyfile = g_key_file_new();
    if (!g_key_file_load_from_file(keyfile, lpFile, G_KEY_FILE_NONE, NULL))
    {
        fprintf(stderr, "LoadIni Open file fail!\n");
        return szValue;
    }
    char* pUrl = g_key_file_get_string(keyfile, szSection, szKey, NULL);
    if(pUrl)
    	memcpy(szValue, pUrl, sizeof(szValue));
    g_key_file_free(keyfile);
#else
	GetPrivateProfileStringA( szSection, szKey, szDefault, szValue, MAX_PATH, lpFile );
#endif
	return szValue;
}

// 说明: 写入Ini配置文件的函数
void CUtils::SaveIni( LPCTSTR szSection, LPCTSTR szKey, int nValue, LPCTSTR szFileName )
{
	char szValue[32]={0};
	sprintf(szValue, "%d", nValue);

	SaveIni( szSection, szKey, szValue, szFileName );
}

void CUtils::SaveIni( LPCTSTR szSection, LPCTSTR szKey, float& fValue, LPCTSTR szFileName )
{
	char szValue[32]={0};
	sprintf(szValue, "%f", fValue);
	SaveIni( szSection, szKey, szValue, szFileName );
}

void CUtils::SaveIni(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR lpValue, LPCTSTR szFile)
{
#if defined(__linux__)
	GKeyFile* keyfile = g_key_file_new();
    if (!g_key_file_load_from_file(keyfile, szFile, G_KEY_FILE_NONE, NULL))
    {
        fprintf(stderr, "SaveIni Open file fail!\n");
        return;
    }
    g_key_file_set_string(keyfile, szSection, szKey, lpValue);
    g_key_file_free(keyfile);
#else
	WritePrivateProfileString( szSection, szKey, lpValue, szFile );
#endif
}


// 获取主模块文件全路径
char* CUtils::GetModuleFileName(BOOL bPath)
{
	static char szPath[MAX_PATH];
#if defined(__linux__)
    readlink ("/proc/self/exe", szPath, BUFSIZ);
#else
	::GetModuleFileName(NULL, szPath, MAX_PATH);
#endif

	if (bPath)
	{
		char* index = strrchr(szPath, '\\');
		if(index==NULL)
            index = strrchr(szPath, '/');
        if(index)
            szPath[strlen(szPath) - strlen(index)] = 0;
	}
	return szPath;
}

// 判断一个本地文件是否存在
BOOL CUtils::IsExistFile(LPCTSTR szFile)
{
	BOOL bRet = false;
	FILE* pf = fopen(szFile, "r");
	if (pf)
	{
		bRet = true;
		fclose(pf);
	}
	return bRet;
}



// 写日志
DWORD CUtils::WriteLog(const char* Format,...)
{
	DWORD dwRel=0;
	time_t t = time(0);
	char szFile[MAX_PATH]={0};
	char szTime[32]={0};
	char szBuffer[2048]={0};
	strftime(szBuffer, sizeof(szBuffer), "%X ", localtime(&t));

	va_list va;
	va_start(va,Format);
	VSNPRINTF(szBuffer+strlen(szBuffer), sizeof(szBuffer)-strlen(szBuffer), Format, va);
	va_end(va);
	strcat(szBuffer, "\n");

    strcpy(szFile, GetModuleFileName(true));
	strftime(szTime, sizeof(szTime), "/Log/%Y%m%d.log", localtime(&t));
	strcat(szFile, szTime);
	FILE* pf = fopen(szFile, "a");
	if ( pf )
	{
		dwRel = (DWORD)fwrite(szBuffer, 1, strlen(szBuffer), pf);
		fclose(pf);
	}

	return dwRel;
}


// 写日志(带DEBUG开关)
DWORD CUtils::WriteDebugLog(const char* Format,...)
{
	if(!g_bDebug)
		return 0;

	DWORD dwRel=0;
	time_t t = time(0);
	char szFile[MAX_PATH]={0};
	char szTime[32]={0};
	char szBuffer[2048]={0};
	strftime(szBuffer, sizeof(szBuffer), "%X ", localtime(&t));

	va_list va;
	va_start(va,Format);
	VSNPRINTF(szBuffer+strlen(szBuffer), sizeof(szBuffer)-strlen(szBuffer), Format, va);
	va_end(va);
	strcat(szBuffer, "\n");

	strcpy(szFile, GetModuleFileName(true));
	strftime(szTime, sizeof(szTime), "/Log/%Y%m%d.log", localtime(&t));
	strcat(szFile, szTime);
	FILE* pf = fopen(szFile, "a");
	if ( pf )
	{
		dwRel = (DWORD)fwrite(szBuffer, 1, strlen(szBuffer), pf);
		fclose(pf);
	}

	return dwRel;
}

DWORD CUtils::WriteLogPrint(const char* Format,...)
{
	DWORD dwRel=0;
	time_t t = time(0);
	char szFile[MAX_PATH]={0};
	char szTime[32]={0};
	char szBuffer[2048]={0};
	strftime(szBuffer, sizeof(szBuffer), "%X ", localtime(&t));

	va_list va;
	va_start(va,Format);
	VSNPRINTF(szBuffer+strlen(szBuffer), sizeof(szBuffer)-strlen(szBuffer), Format, va);
	va_end(va);
	strcat(szBuffer, "\n");

    strcpy(szFile, GetModuleFileName(true));
	strftime(szTime, sizeof(szTime), "/Log/%Y%m%d.log", localtime(&t));
	strcat(szFile, szTime);
	FILE* pf = fopen(szFile, "a");
	if ( pf )
	{
		dwRel = (DWORD)fwrite(szBuffer, 1, strlen(szBuffer), pf);
		fclose(pf);
	}

	printf(szBuffer);
	return dwRel;
}

// 可自定义LOG前缀的日志
DWORD CUtils::WriteLogPrefix(LPCSTR lpPrefix, const char* Format,...)
{
	DWORD dwRel=0;
	time_t t = time(0);
	char szFile[MAX_PATH]={0};
	char szTime[32]={0};
	char szBuffer[2048]={0};
	//strftime(szBuffer, sizeof(szBuffer), "%X ", localtime(&t));

	va_list va;
	va_start(va,Format);
	VSNPRINTF(szBuffer+strlen(szBuffer), sizeof(szBuffer)-strlen(szBuffer), Format, va);
	va_end(va);
	strcat(szBuffer, "\n");

	strftime(szTime, sizeof(szTime), "%Y%m%d", localtime(&t));
	sprintf(szFile, "%s/Log/%s%s.log", GetModuleFileName(true), lpPrefix, szTime);
	FILE* pf = fopen(szFile, "w");
	if ( pf )
	{
		dwRel = (DWORD)fwrite(szBuffer, 1, strlen(szBuffer), pf);
		fclose(pf);
	}

	return dwRel;
}

DWORD CUtils::WriteLogFile(LPCSTR lpData, int nLen)
{
	DWORD dwRel=0;
	time_t t = time(0);
	char szFile[MAX_PATH]={0};
	char szTime[32]={0};
    strcpy(szFile, GetModuleFileName(true));
	strftime(szTime, sizeof(szTime), "/Log/%Y%m%d.log", localtime(&t));
	strcat(szFile, szTime);
	strftime(szTime, sizeof(szTime), "%X ", localtime(&t));
	FILE* pf = fopen(szFile, "a");
	if ( pf )
	{
		dwRel = (DWORD)fwrite(szTime, 1, strlen(szTime), pf);
		dwRel += (DWORD)fwrite(lpData, 1, nLen, pf);
		fclose(pf);
	}

	return dwRel;
}

#if defined(__linux__)
int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
    iconv_t cd;
    char **pin = &inbuf;
    char **pout = &outbuf;

    cd = iconv_open(to_charset,from_charset);
    if (cd==0) return -1;
    memset(outbuf,0,outlen);
    size_t in_buf=inlen;
    size_t out_buf=outlen;
    if (iconv(cd,pin,&in_buf,pout,&out_buf)==(size_t)-1) return -1;
        iconv_close(cd);
    return 0;
}

int CUtils::Utf8ToGb2312(char *szInbuf, int nInlen, char *szOutbuf, int nOutlen)
{
    return code_convert((char *)"utf-8",(char *)"gb2312",szInbuf,nInlen,szOutbuf,nOutlen);
}
int CUtils::Gb2312ToUtf8(char *szInbuf, int nInlen, char *szOutbuf, int nOutlen)
{
    return code_convert((char *)"gb2312",(char *)"utf-8",szInbuf,nInlen,szOutbuf,nOutlen);
}

string CUtils::Utf8ToGb2312(const char* pText)
{
	if( !pText )
		return "";
	int nLen = (int)strlen(pText);
	if (!IsTextUTF8(pText, nLen))
		return pText;

    char szBuf[512]={0};
    code_convert((char *)"utf-8",(char *)"gb2312",(char*)pText,nLen,szBuf,sizeof(szBuf));
    return szBuf;
}

string CUtils::Gb2312ToUtf8(const char* pText)
{
	if( !pText )
		return "";
	int nLen = (int)strlen(pText);
	if (IsTextUTF8(pText, nLen))
		return pText;

    char szBuf[512]={0};
    code_convert((char *)"gb2312",(char *)"utf-8",(char*)pText,strlen(pText),szBuf,sizeof(szBuf));
    return szBuf;
}
#else
// 把UTF-8格式字符串转换为ANSI格式
string CUtils::Utf8ToGb2312(const char* pText)
{
	if( !pText )
		return "";
	int nLen = (int)strlen(pText);
	if( nLen == 0 )
		return "";
	if (!IsTextUTF8(pText, nLen))
		return pText;

	// 先把UTF-8转换为Unicode
	WCHAR szWBuff[MAX_PATH];
	ZeroMemory( szWBuff, sizeof(szWBuff) );
	::MultiByteToWideChar( CP_UTF8, 0, pText, nLen, szWBuff, MAX_PATH );
	nLen = (int)wcslen( szWBuff );
	if( nLen == 0 )
		return NULL;

	// 再把Unicode转化为ANSI
	TCHAR szTBuff[MAX_PATH];
	ZeroMemory( szTBuff, sizeof(szTBuff) );
	::WideCharToMultiByte( CP_ACP, NULL, szWBuff, nLen, szTBuff, MAX_PATH, NULL, NULL );
	return szTBuff;
}
string CUtils::Gb2312ToUtf8(const char* pText)
{
	return "";
}
#endif


// 分割字符串；返回分割数量
int CUtils::Split(const string strSrc, const string strDelimiter, OUT string* strArray, int nArraySize)
{
	int i,nLen = 0;
	string strTemp = strSrc;
	nLen = (int)strSrc.find(strDelimiter);
	for (i=0; i<nArraySize && nLen>0; i++)
	{
		strArray[i] = strTemp.substr(0, nLen);
		strTemp = strTemp.substr(nLen+strDelimiter.size());
		nLen = (int)strTemp.find(strDelimiter);
		printf("%s\n", strArray[i].c_str());
	}
	if (strTemp.size()>0 && i<nArraySize)
	{
		strArray[i] = strTemp;
		printf("%s\n", strArray[i].c_str());
	}

	return i;
}

void CUtils::string_replace(string&s1,const string&s2,const string&s3)
{
	string::size_type pos=0;
	string::size_type a=s2.size();
	string::size_type b=s3.size();
	while((pos=s1.find(s2,pos))!=string::npos)
	{
		s1.replace(pos,a,s3);
		pos+=b;
	}
}

// 判断是否是UTF8编码
bool CUtils::IsTextUTF8(const char* str,long length)
{
	int i;
	int nBytes=0;//UFT8可用1-6个字节编码,ASCII用一个字节
	unsigned char chr;
	bool bAllAscii=true; //如果全部都是ASCII, 说明不是UTF-8
	for(i=0;i<length;i++)
	{
		chr= *(str+i);
		if( (chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
			bAllAscii= false;
		if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数
		{
			if(chr>=0x80)
			{
				if(chr>=0xFC&&chr<=0xFD)
					nBytes=6;
				else if(chr>=0xF8)
					nBytes=5;
				else if(chr>=0xF0)
					nBytes=4;
				else if(chr>=0xE0)
					nBytes=3;
				else if(chr>=0xC0)
					nBytes=2;
				else
				{
					return false;
				}
				nBytes--;
			}
		}
		else //多字节符的非首字节,应为 10xxxxxx
		{
			if( (chr&0xC0) != 0x80 )
			{
				return false;
			}
			nBytes--;
		}
	}

	if( nBytes > 0 ) //违返规则
	{
		return false;
	}

	if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
	{
		return false;
	}
	return true;
}
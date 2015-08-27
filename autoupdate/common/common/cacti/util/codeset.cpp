#include "stdafx.h"
#include "codeset.h"
#include <windows.h>

string GBKToUTF8(const char* strGBK)
{
    int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len+1];
    memset(wstr, 0, len+1);
    MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len+1];
    memset(str, 0, len+1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    string strTemp = str;
    if(wstr) delete[] wstr;
    if(str) delete[] str;
    return strTemp;
}
string UTF8ToGBK(const char* strUTF)   //转码，从utf-8转码成GBK
{
	#ifdef WIN32
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_UTF8, 0, strUTF, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	string strTemp = str;
	if(wstr) delete[] wstr;
	if(str) delete[] str;
	return strTemp;
	#else
	#endif
}


// 进行url编码
bool UrlEncode(const char* szSrc, char* pBuf, int cbBufLen, bool bUpperCase,int flag)
{
    if(szSrc == NULL || pBuf == NULL || cbBufLen <= 0)
        return false;
	
    size_t len_ascii = strlen(szSrc);
    if(len_ascii == 0)
    {
        pBuf[0] = 0;
        return true;
    }
	else if (1 == len_ascii)		// 特殊处理长度为1的源串 
	{
		pBuf[0] = szSrc[0];
		pBuf[1] = 0;
		return true;
	}
    
    //先转换到UTF-8
    char baseChar = bUpperCase ? 'A' : 'a';
    int cchWideChar = MultiByteToWideChar(CP_ACP, 0, szSrc, len_ascii, NULL, 0);
    LPWSTR pUnicode = (LPWSTR)malloc((cchWideChar + 1) * sizeof(WCHAR));
    if(pUnicode == NULL)
        return false;
    MultiByteToWideChar(CP_ACP, 0, szSrc, len_ascii, pUnicode, cchWideChar + 1);
	
    int cbUTF8 = WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, NULL, 0, NULL, NULL);
    LPSTR pUTF8 = (LPSTR)malloc((cbUTF8 + 1) * sizeof(CHAR));
    if(pUTF8 == NULL)
    {
        free(pUnicode);
        return false;
    }
    WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, pUTF8, cbUTF8 + 1, NULL, NULL);
    pUTF8[cbUTF8] = '\0';
	
    unsigned char c;
    int cbDest = 0; //累加
    unsigned char *pSrc = (unsigned char*)pUTF8;
    unsigned char *pDest = (unsigned char*)pBuf;
    while(*pSrc && cbDest < cbBufLen - 1)
    {
        c = *pSrc;
        if(isalpha(c) || isdigit(c) || c == '-' || c == '.' || c == '~')
        {
            *pDest = c;
            ++pDest;
            ++cbDest;
        }
        else if(c == ' ' && flag == 0)
        {
            *pDest = '+';
            ++pDest;
            ++cbDest;
        }
        else
        {
            //检查缓冲区大小是否够用？
            if(cbDest + 3 > cbBufLen - 1)
                break;
            pDest[0] = '%';
            pDest[1] = (c >= 0xA0) ? ((c >> 4) - 10 + baseChar) : ((c >> 4) + '0');
            pDest[2] = ((c & 0xF) >= 0xA)? ((c & 0xF) - 10 + baseChar) : ((c & 0xF) + '0');
            pDest += 3;
            cbDest += 3;
        }
        ++pSrc;
    }
    //null-terminator
    *pDest = '\0';
    free(pUnicode);
    free(pUTF8);
    return true;
}
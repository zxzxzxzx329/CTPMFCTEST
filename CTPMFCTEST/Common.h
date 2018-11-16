#pragma warning(disable : 4996)
#ifndef _COMMON_H_
#define _COMMON_H_

#include "ThostFtdcTraderApi.h"
#include "ThostFtdcMdApi.h"
#include "stdafx.h"
#include <string>
using namespace std;

const int WM_SHOW_QUOTE = WM_USER + 0x200;							//显示商品行情信息

// char转WCAHR  
void C2W(const char* szSrc, WCHAR* wszDst, int nMaxLen/*wszDst的字符长度*/);

// WCHAR转char 
void W2C(const WCHAR* wszSrc, char* szDst, int nMaxLen/*szDst的字符长度*/);

CString ANSIToUnicode(char * szChar);
string UnicodeToANSI(CString & szStr);

static int iRequestID = 0;											//请求编号

#endif
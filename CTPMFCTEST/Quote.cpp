#include "stdafx.h"
#include "Quote.h"
#include "CTPMFCTEST.h"

map<CString,CString> CQuote::m_mapTimeMsg;
Mutex CQuote::m_mutex;
CThostFtdcReqUserLoginField CQuote::m_req;


CQuote::CQuote()
{
}

CQuote::~CQuote()
{
}

void CQuote::OnRspError(CThostFtdcRspInfoField *pRspInfo,
						int nRequestID, bool bIsLast)
{
    IsErrorRspInfo(pRspInfo);
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void CQuote::OnFrontDisconnected(int nReason)
{
	Locker locker(m_mutex);
	CString m_strTime = _T(""); 
	CString m_strMsg = _T("");
	GetTime(m_strTime);
	m_strMsg.Format(_T("%d"),nReason);

	m_mapTimeMsg.insert(make_pair(m_strTime, m_strMsg));
}

///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void CQuote::OnHeartBeatWarning(int nTimeLapse)
{
	Locker locker(m_mutex);
	CString m_strTime = _T(""); 
	CString m_strMsg = _T("");
	GetTime(m_strTime);
	m_strMsg.Format(_T("%d"),nTimeLapse);

	m_mapTimeMsg.insert(make_pair(m_strTime, m_strMsg));
}

///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CQuote::OnFrontConnected()
{
	CString m_strMsg = _T("行情登录中.....");

	Info(m_strMsg);

	ReqUserLogin();
}

void CQuote::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));

	req = m_req;
	int iResult = pMdApi->ReqUserLogin(&req, ++iRequestID);

	CString m_strMsg = _T("发送行情用户登录请求:");
	Info(iResult, m_strMsg);
}

///登录请求响应
void CQuote::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///获取当前交易日
		CString strMsg = _T("获取当前交易日:");
		CString strTradingDay = _T("");

		char *pTradingDay = const_cast<char*>(pMdApi->GetTradingDay());
		strTradingDay = ANSIToUnicode(pTradingDay);
		strMsg = strMsg + strTradingDay;

		Info(strMsg);
	}
}

void CQuote::OnSubscribeMarketData(char *ppInstrumentID[], int nCount)
{
	int iResult = pMdApi->SubscribeMarketData(ppInstrumentID, nCount);

	CString m_strTime = _T(""); 
	CString m_strMsg = _T("发送行情订阅请求:");

	Info(iResult, m_strMsg);
}

///订阅行情应答
void CQuote::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CString m_strMsg = _T("订阅行情品种：");
	m_strMsg.Append(_T("%d::::"), pRspInfo ->ErrorID);
	CString strTmp = _T("");
	CString strInstrumentID = _T("");
	strTmp = ANSIToUnicode(pRspInfo ->ErrorMsg);
	m_strMsg.Append(strTmp);
	strInstrumentID = ANSIToUnicode(pSpecificInstrument->InstrumentID);
	m_strMsg = m_strMsg + strInstrumentID;

	Info(m_strMsg);
}

///取消订阅行情应答
void CQuote::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

///深度行情通知
void CQuote::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//CThostFtdcDepthMarketDataField *tmpData = new CThostFtdcDepthMarketDataField;
	//tmpData = pDepthMarketData;
	//strncpy(tmpData->InstrumentID , pDepthMarketData->InstrumentID, 31);
	//strncpy(tmpData->UpdateTime , pDepthMarketData->UpdateTime, 9);
	//strncpy(tmpData->TradingDay , pDepthMarketData->TradingDay, 9);

	//CString strTmp = _T("");
	//CString strTradingDay = _T("");
	//strTmp.Format(_T("%s"),ANSIToUnicode(pDepthMarketData ->UpdateTime));   // 更新时间
	//strTradingDay.Format(_T("%s"),ANSIToUnicode(pDepthMarketData ->TradingDay));
	//tmpData->AskPrice1 = pDepthMarketData->AskPrice1;
	//tmpData->BidPrice1 = pDepthMarketData->BidPrice1;
	//tmpData->AskVolume1 = pDepthMarketData->AskVolume1;
	//tmpData->BidVolume1 = pDepthMarketData->BidVolume1;
	//tmpData->LastPrice = pDepthMarketData->LastPrice;
	//tmpData->Volume = pDepthMarketData->Volume;

	theApp.m_pMainWnd ->PostMessage(WM_SHOW_QUOTE, (WPARAM)pDepthMarketData);

}

// 
bool CQuote::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	CString m_strTime = _T(""); 
	CString m_strMsg = _T("");
	m_strMsg.Format(_T("%d::::"), pRspInfo ->ErrorID);
	CString strTmp = _T("");
	strTmp.Format(_T("%s"), ANSIToUnicode(pRspInfo ->ErrorMsg));
	m_strMsg.Append(strTmp);

	GetTime(m_strTime);
	//if(bResult)
	{
        Locker locker(m_mutex);

        m_mapTimeMsg.insert(make_pair(m_strTime, m_strMsg));
	}

	return bResult;
}

void CQuote::GetTime(CString& strTime)
{
	CTime tm = CTime::GetCurrentTime();
	strTime = tm.Format("%Y-%m-%d %H:%M:%S-->"); 
}

void CQuote::Info(const CString& strMsg)
{
	Locker locker(m_mutex);
	CString m_strTime = _T(""); 
	GetTime(m_strTime);
    m_mapTimeMsg.insert(make_pair(m_strTime, strMsg));
}

void CQuote::Info(const int iResult, CString& strMsg)
{
	Locker locker(m_mutex);
	CString m_strTime = _T(""); 

	GetTime(m_strTime);
	if(iResult == 0)
	{
		strMsg.Append(_T("成功"));
	}
	else
	{
		strMsg.Append(_T("失败"));
	}
	m_mapTimeMsg.insert(make_pair(m_strTime, strMsg));
}


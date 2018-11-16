#pragma once
#include "stdafx.h"
#include "ThostFtdcTraderApi.h"
#include "Quote.h"

class CTraderSpi : public CThostFtdcTraderSpi
{
public:
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///投资者结算结果确认响应
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询资金账户响应
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者持仓响应
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单录入请求响应
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约保证金率响应
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///请求查询合约手续费率响应
	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单操作请求响应
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	virtual void OnFrontDisconnected(int nReason);
		
	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	virtual void OnHeartBeatWarning(int nTimeLapse);
	
	///报单通知
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///成交通知
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);


private:
	///用户登录请求
	void ReqUserLogin();
	///投资者结算结果确认
	void ReqSettlementInfoConfirm();
	///请求查询合约
	void ReqQryInstrument(const TThostFtdcInstrumentIDType InstrumentID);
	///请求查询资金账户
	void ReqQryTradingAccount();
	///请求查询投资者持仓
	void ReqQryInvestorPosition();
	///报单录入请求
	void ReqOrderInsert();
	///报单操作请求
	void ReqOrderAction(CThostFtdcOrderField *pOrder);
	///请求查询合约保证金率
	void ReqQryInstrumentMarginRate();
	///请求查询合约手续费率
	void ReqQryInstrumentCommissionRate();
	// 是否收到成功的响应
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	// 是否我的报单回报
	bool IsMyOrder(CThostFtdcOrderField *pOrder);
	// 是否正在交易的报单
	bool IsTradingOrder(CThostFtdcOrderField *pOrder);

public:
	CQuote m_QuoteTrade;
	// 会话参数
	TThostFtdcFrontIDType	FRONT_ID;		//前置编号
	TThostFtdcSessionIDType	SESSION_ID;		//会话编号
	TThostFtdcOrderRefType	ORDER_REF;		//报单引用
	CString m_szInstrumentID;               //合约代码
};
/////////////////////////////////////////////////////////////////////////

#include "ThostFtdcTraderApi.h"
#include "ThostFtdcMdApi.h"
#include "TraderSpi.h"

#pragma warning(disable : 4996)

// USER_API参数
extern CThostFtdcTraderApi* pUserApi;

void CTraderSpi::OnFrontConnected()
{
	CString strMsg = _T("交易登录中Trade Connected");
    m_QuoteTrade.Info(strMsg);

	// 用户登录请求
	ReqUserLogin();
}

void CTraderSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));

	req = m_QuoteTrade.m_req;
	memcpy(req.Password, "910406", 41);
	memcpy(req.TradingDay, "20160728", 9);
	//req.Password = "910406";
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);

	CString m_strMsg = _T("发送交易用户登录请求:");;
	m_QuoteTrade.Info(iResult, m_strMsg);
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(IsErrorRspInfo(pRspInfo))
	{
		
	}
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// 保存会话参数
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(ORDER_REF, "%d", iNextOrderRef);


		// test
		// 端登成功,发出报单录入请求
		CThostFtdcInputOrderField ord;
		memset(&ord, 0, sizeof(ord));
		//经纪公司代码
		strcpy(ord.BrokerID, "2030");
		//投资者代码
		strcpy(ord.InvestorID, "88888888");
		// 合约代码
		strcpy(ord.InstrumentID, "IC1608");
		///报单引用
		strcpy(ord.OrderRef, "000000000001");
		// 用户代码
		strcpy(ord.UserID, "88888888");
		// 报单价格条件
		ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		// 买卖方向
		ord.Direction = THOST_FTDC_D_Buy;
		// 组合开平标志
		strcpy(ord.CombOffsetFlag, "0");
		// 组合投机套保标志
		strcpy(ord.CombHedgeFlag, "1");
		// 价格
		ord.LimitPrice = 6160;
		// 数量
		ord.VolumeTotalOriginal = 10;
		// 有效期类型
		ord.TimeCondition = THOST_FTDC_TC_GFD;
		// GTD日期
		strcpy(ord.GTDDate, "");
		// 成交量类型
		ord.VolumeCondition = THOST_FTDC_VC_AV;
		// 最小成交量
		ord.MinVolume = 0;
		// 触发条件
		ord.ContingentCondition = THOST_FTDC_CC_Immediately;
		// 止损价
		ord.StopPrice = 0;
		// 强平原因
		ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
		// 自动挂起标志
		ord.IsAutoSuspend = 0;
		int iResult = pUserApi->ReqOrderInsert(&ord, 1);

		///投资者结算结果确认
		//ReqSettlementInfoConfirm();
	}
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));

	//req.BrokerID = m_QuoteTrade.m_req.BrokerID;
	memcpy(req.BrokerID, m_QuoteTrade.m_req.BrokerID,11);
	memcpy(req.InvestorID,m_QuoteTrade.m_req.UserID,13);
	//req.InvestorID = m_QuoteTrade.m_req.UserID;
	//请求查询结算信息确认
	int iResult = pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);

	CString m_strMsg = _T("投资者结算结果确认:");

	m_QuoteTrade.Info(iResult, m_strMsg);
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///请求查询合约
		TThostFtdcInstrumentIDType InstrumentID;
		memcpy(InstrumentID, "IC1608",31);
		ReqQryInstrument(InstrumentID);
	}
}


void CTraderSpi::ReqQryInstrument(const TThostFtdcInstrumentIDType InstrumentID)
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));

	//req.InstrumentID = InstrumentID;
	memcpy(req.InstrumentID, InstrumentID, 31);
	int iResult = pUserApi->ReqQryInstrument(&req, ++iRequestID);

	CString m_strMsg = _T("请求查询合约:");
	CString strInstrumentID = ANSIToUnicode(req.InstrumentID);
	m_strMsg = m_strMsg + strInstrumentID;

	m_QuoteTrade.Info(iResult, m_strMsg);
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///请求查询合约

		ReqQryTradingAccount();
	}
}

void CTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));

	//req.BrokerID = m_QuoteTrade.m_req.BrokerID;
	memcpy(req.BrokerID, m_QuoteTrade.m_req.BrokerID,11);
	//req.InvestorID = m_QuoteTrade.m_req.UserID;
	memcpy(req.InvestorID,m_QuoteTrade.m_req.UserID,13);

	int iResult = pUserApi->ReqQryTradingAccount(&req, ++iRequestID);

	CString m_strMsg = _T("请求查询资金账户:");

	m_QuoteTrade.Info(iResult, m_strMsg);
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		//cerr << "--->>> 可取资金: " << pTradingAccount->WithdrawQuota  << endl;
		//静态权益=上日结算-出金金额+入金金额
		double preBalance = pTradingAccount->PreBalance - pTradingAccount->Withdraw + pTradingAccount->Deposit;

		//动态权益=静态权益+ 平仓盈亏+ 持仓盈亏- 手续费
		double current = preBalance+ pTradingAccount->CloseProfit + pTradingAccount->PositionProfit - pTradingAccount->Commission;
	

		////检查交易日志文件是否存在，是否需要新建文本文件
		//if (LogFilePaths[0] == '\0')
		//{
		//	strcpy(LogFilePaths, "./AutoTrade/");
		//	strcat_s(LogFilePaths, pTradingAccount->TradingDay);
		//	strcat_s(LogFilePaths, ".txt");

		//	//检查文件是否存在，是否需要新建文本文件
		//	ifstream inf;
		//	ofstream ouf;
		//	inf.open(LogFilePaths, ios::out);
		//}

		///请求查询投资者持仓

		ReqQryInvestorPosition();
	}
}

void CTraderSpi::ReqQryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));

	//req.BrokerID = m_QuoteTrade.m_req.BrokerID;
	memcpy(req.BrokerID, m_QuoteTrade.m_req.BrokerID,11);
	//req.InvestorID = m_QuoteTrade.m_req.UserID;
	memcpy(req.InvestorID,m_QuoteTrade.m_req.UserID,13);
	//req.InstrumentID = m_QuoteTrade.m_req.
	int iResult = pUserApi->ReqQryInvestorPosition(&req, ++iRequestID);

	CString m_strMsg = _T("请求查询投资者持仓:");

	m_QuoteTrade.Info(iResult, m_strMsg);
}

///请求查询投资者持仓响应
void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		CString strMsg = _T("合约保证金以及手续费查询请求");
		m_QuoteTrade.Info(strMsg);
		m_szInstrumentID.Format(_T("%s"),ANSIToUnicode(pInvestorPosition ->InstrumentID));	
		ReqQryInstrumentMarginRate();
		ReqQryInstrumentCommissionRate();
		Sleep(1000);
	}
}

///请求查询合约保证金率
void CTraderSpi::ReqQryInstrumentMarginRate()
{
	CThostFtdcQryInstrumentMarginRateField req;
	memset(&req, 0, sizeof(req));

	///经纪公司代码
	//req.BrokerID = m_QuoteTrade.m_req.BrokerID;
	memcpy(req.BrokerID, m_QuoteTrade.m_req.BrokerID,11);
	///投资者代码
	//req.InvestorID = m_QuoteTrade.m_req.UserID;
	memcpy(req.InvestorID,m_QuoteTrade.m_req.UserID,13);
	///合约代码
	memcpy(req.InstrumentID,m_szInstrumentID,m_szInstrumentID.GetLength()); 
	//////投机套保标志
	req.HedgeFlag = THOST_FTDC_HF_Speculation;	//投机

	int iResult = pUserApi->ReqQryInstrumentMarginRate(&req, ++iRequestID);

	CString m_strMsg = _T("查询合约保证金率: ");
	m_QuoteTrade.Info(iResult, m_strMsg);
}

///请求查询合约手续费率
void CTraderSpi::ReqQryInstrumentCommissionRate()
{
	CThostFtdcQryInstrumentCommissionRateField req;
	memset(&req, 0, sizeof(req));

	///经纪公司代码
	//req.BrokerID = m_QuoteTrade.m_req.BrokerID;
	memcpy(req.BrokerID, m_QuoteTrade.m_req.BrokerID,11);
	///投资者代码
	//req.InvestorID = m_QuoteTrade.m_req.UserID;
	memcpy(req.InvestorID,m_QuoteTrade.m_req.UserID,13);
	///合约代码
	memcpy(req.InstrumentID,m_szInstrumentID,m_szInstrumentID.GetLength());

	int iResult = pUserApi->ReqQryInstrumentCommissionRate(&req, ++iRequestID);

	CString m_strMsg = _T("查询合约手续费率: ");
	m_QuoteTrade.Info(iResult, m_strMsg);
}

//报单-限价,暂时未使用
void CTraderSpi::ReqOrderInsert()
{

}

// 报单后，如不能通过THOST校验，THOST拒绝报单，返回OnRspOrderInsert（含错误编码）。如果校验通过，THOST接收报单，转发给交易所
// 交易所收到报单后，通过校验。用户会收到OnRtnOrder、OnRtnTrade。
// 如果交易所认为报单错误，用户就会收到OnErrRtnOrder
void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{

	}
}

//报单-修改，撤单
void CTraderSpi::ReqOrderAction(CThostFtdcOrderField *pOrder)
{
	static bool ORDER_ACTION_SENT = false;		//是否发送了报单
	if (ORDER_ACTION_SENT)
		return;

	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, pOrder->BrokerID);
	///投资者代码
	strcpy(req.InvestorID, pOrder->InvestorID);
	///报单操作引用
	//	TThostFtdcOrderActionRefType	OrderActionRef;
	///报单引用
	strcpy(req.OrderRef, pOrder->OrderRef);
	///请求编号
	//	TThostFtdcRequestIDType	RequestID;
	///前置编号
	req.FrontID = FRONT_ID;
	///会话编号
	req.SessionID = SESSION_ID;
	///交易所代码
	//	TThostFtdcExchangeIDType	ExchangeID;
	///报单编号
	//	TThostFtdcOrderSysIDType	OrderSysID;
	///操作标志
	req.ActionFlag = THOST_FTDC_AF_Delete;
	///价格
	//	TThostFtdcPriceType	LimitPrice;
	///数量变化
	//	TThostFtdcVolumeType	VolumeChange;
	///用户代码
	//	TThostFtdcUserIDType	UserID;
	///合约代码
	strcpy(req.InstrumentID, pOrder->InstrumentID);
	//cerr << "--->>> 报单操作请求: "<< endl;
	int iResult = pUserApi->ReqOrderAction(&req, ++iRequestID);

	CString m_strMsg = _T("报单撤单请求: ");
	if(pOrder->Direction == '0')
	{
		m_strMsg.Append(_T("方向：买 -->"));
	}
	else
	{
		m_strMsg.Append(_T("方向：卖 -->"));
	}

	if(pOrder->CombOffsetFlag[0] == '0')
	{
		m_strMsg.Append(_T("开平方向：开仓 -->"));
	}
	else
	{
		m_strMsg.Append(_T("开平方向：平仓 -->"));
	}

	m_QuoteTrade.Info(iResult, m_strMsg);
	ORDER_ACTION_SENT = true;
}

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::OnFrontDisconnected(int nReason)
{
    CString m_strMsg = _T("交易断开: ");
	m_QuoteTrade.Info(nReason, m_strMsg);
}

void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	CString m_strMsg = _T("心跳中断时间: ");
	m_QuoteTrade.Info(nTimeLapse, m_strMsg);
}

///报单通知
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	CString m_strMsg = _T("报单状态：");
	if('0' == pOrder->OrderSubmitStatus)
	{
		m_strMsg.Append(_T("已经提交::"));
	}
	else if('3' == pOrder->OrderSubmitStatus)
	{
		m_strMsg.Append(_T("已经接受::"));
	}

	if (IsMyOrder(pOrder))
	{
		if (IsTradingOrder(pOrder))
		{
			m_strMsg.Append(_T("正在执行！"));
		}
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
		{
		    m_strMsg.Append(_T("撤单成功！"));
		}
	}

	m_QuoteTrade.Info(m_strMsg);
}

void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	CString m_strMsg = _T("--->>> 成交通知:::");

	CString strInstrumentID = _T("");
	strInstrumentID = ANSIToUnicode(pTrade->InstrumentID);

	m_strMsg = m_strMsg + strInstrumentID;

	if('0' == pTrade->Direction)
	{
		m_strMsg.Append(_T("买:::"));
	}
	else
	{
		m_strMsg.Append(_T("卖:::"));
	}

	if('0' == pTrade->OffsetFlag)
	{
		m_strMsg.Append(_T("开仓:::"));
	}
	else
	{
		m_strMsg.Append(_T("平仓:::"));
	}

	m_strMsg.Append(_T("数量：%d"),pTrade->Volume);
	m_strMsg.Append(_T("价格：%lf"),pTrade->Price);

	m_QuoteTrade.Info(m_strMsg);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	CString m_strTime = _T(""); 
	CString m_strMsg = _T("");
	m_strMsg.Format(_T("%d::::"), pRspInfo ->ErrorID);
	CString strTmp = _T("");
	strTmp.Format(_T("%s"), ANSIToUnicode(pRspInfo ->ErrorMsg));
	m_strMsg.Append(strTmp);

	m_QuoteTrade.GetTime(m_strTime);
	//if(bResult)
	{
		Locker locker(m_QuoteTrade.m_mutex);

		m_QuoteTrade.m_mapTimeMsg.insert(make_pair(m_strTime, m_strMsg));
	}

	return bResult;
}

bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == FRONT_ID) &&
		(pOrder->SessionID == SESSION_ID) &&
		(strcmp(pOrder->OrderRef, ORDER_REF) == 0));
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

///请求查询合约保证金率响应
void CTraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	double bzj_b=100*(pInstrumentMarginRate->LongMarginRatioByMoney);

	CString m_strMsg = _T("合约保证金率：");

	CString strInstrumentID = _T("");
	strInstrumentID = ANSIToUnicode(pInstrumentMarginRate->InstrumentID);
	m_strMsg = m_strMsg + strInstrumentID;
	m_strMsg.Append(_T("%lf"),bzj_b);

	m_QuoteTrade.Info(m_strMsg);
}
///请求查询合约手续费率响应
void CTraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	double sxf_w=100*100*(pInstrumentCommissionRate->OpenRatioByMoney);

	CString m_strMsg = _T("合约手续费率：");

	CString strInstrumentID = _T("");
	strInstrumentID = ANSIToUnicode(pInstrumentCommissionRate->InstrumentID);
	m_strMsg = m_strMsg + strInstrumentID;
	m_strMsg.Append(_T("%lf"),sxf_w);
	m_strMsg.Append(_T("%lf"),pInstrumentCommissionRate->CloseTodayRatioByVolume);

	m_QuoteTrade.Info(m_strMsg);
}

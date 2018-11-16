// OrderDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "CTPMFCTEST.h"
#include "OrderDialog.h"


// COrderDialog 对话框

IMPLEMENT_DYNAMIC(COrderDialog, CDialog)

COrderDialog::COrderDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COrderDialog::IDD, pParent)
{

}

COrderDialog::~COrderDialog()
{
}

void COrderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COrderDialog, CDialog)
END_MESSAGE_MAP()


// COrderDialog 消息处理程序

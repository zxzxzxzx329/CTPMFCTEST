#pragma once


// COrderDialog 对话框

class COrderDialog : public CDialog
{
	DECLARE_DYNAMIC(COrderDialog)

public:
	COrderDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COrderDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_ORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

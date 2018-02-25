
// PrintInCloudDlg.h : 头文件
//

#pragma once

#include "json/json.h"
// CPrintInCloudDlg 对话框
class CPrintInCloudDlg : public CDialogEx
{
// 构造
public:
	CPrintInCloudDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRINTINCLOUD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	Json::Value task;

	CPrintDialog _pd;
	bool setprint = false;

	HICON m_hIcon;
	CStatic* CON;
	CWnd* PIC;
	CListCtrl* MODELLIST;
	CWnd* PREVIEW;

	int state;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void OnTimer(UINT_PTR);
	void loadList();
	int printList(Json::Value,int,CDC *,int,int,int,int);


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSetprinter();
	afx_msg void OnBnClickedPrintAll();
	afx_msg void OnBnClickedPrintList();
	afx_msg void OnBnClickedPrintOne();
	afx_msg void OnBnClickedList(NMHDR *, LRESULT *);

	afx_msg void OnBnClickedPrintchoose();
	afx_msg void OnBnClickedAuto();
};

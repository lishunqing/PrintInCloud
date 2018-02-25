
// PrintInCloudDlg.h : ͷ�ļ�
//

#pragma once

#include "json/json.h"
// CPrintInCloudDlg �Ի���
class CPrintInCloudDlg : public CDialogEx
{
// ����
public:
	CPrintInCloudDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRINTINCLOUD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
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

	// ���ɵ���Ϣӳ�亯��
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


// PrintInCloudDlg.h : ͷ�ļ�
//

#pragma once


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
	HICON m_hIcon;
	CStatic* CON;
	CWnd* PIC;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void OnTimer(UINT_PTR);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

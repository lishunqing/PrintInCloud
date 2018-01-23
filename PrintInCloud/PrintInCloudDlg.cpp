
// PrintInCloudDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PrintInCloud.h"
#include "PrintInCloudDlg.h"
#include "afxdialogex.h"
#include <iostream>  
#include "server.h"
#include "json/json.h"
#include "print.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPrintInCloudDlg �Ի���



CPrintInCloudDlg::CPrintInCloudDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PRINTINCLOUD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPrintInCloudDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPrintInCloudDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
//	ON_BN_CLICKED(IDOK, &CPrintInCloudDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPrintInCloudDlg ��Ϣ�������

BOOL CPrintInCloudDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CON = (CStatic*)GetDlgItem(IDC_CONNECTION);
	PIC = GetDlgItem(IDC_PICTURE);

	SetTimer(1, 0, NULL);//5��ˢ��һ��
	//_CrtSetBreakAlloc(521);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPrintInCloudDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPrintInCloudDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPrintInCloudDlg::OnTimer(UINT_PTR x)
{
	KillTimer(1);

	//��ȡע����е�ID
	const LPCTSTR lpConfig = L"Software\\Store\\Config";
	HKEY hKey;
	TCHAR buff[1024] = { 0 };
	long lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpConfig, NULL, KEY_QUERY_VALUE, &hKey);
	if (lRet == ERROR_SUCCESS) {
		DWORD l = sizeof(buff);
		lRet = RegQueryValueEx(hKey, L"id", NULL, NULL,(BYTE*) buff, &l);
	}
	//	return false;
	//
	RegCloseKey(hKey);

	DWORD e = 0;
	CString ret = getTaks(buff,e);
	SetTimer(1, 5000, NULL);//5��ˢ��һ��

	Json::Value root;
	Json::Reader reader;

	if (e) {
		CString m;
		m.Format(L"���ӷ�����ʧ�ܡ������룺%d",e);
		CON->SetWindowText(m);
		return;
	}
	std::string json = CT2CA(ret.GetBuffer());
	reader.parse(json, root, false);

	auto id = root["id"];
	if (id.isString()) {
		const char* ID = id.asCString();
		CString m;
		m.Format(L"���ӷ������ɹ����豸��ţ�%s", CA2CT(ID));
		CON->SetWindowText(m);

		CRect rect;
		PIC->GetWindowRect(&rect);
		ScreenToClient(rect);
		CDC *dc = GetDC();
		print::drawQRCode(dc, rect, ID);

		HKEY hKey;
		long lRet = RegCreateKey(HKEY_CURRENT_USER, lpConfig, &hKey);
		if (lRet == ERROR_SUCCESS) {
			lRet = RegSetValueExA(hKey, "id", 0, REG_SZ, (const BYTE*)ID,strlen(ID));
		}
		RegCloseKey(hKey);
	}
	auto name = root["name"];


}

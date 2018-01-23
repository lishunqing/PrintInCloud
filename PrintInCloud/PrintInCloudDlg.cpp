
// PrintInCloudDlg.cpp : 实现文件
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


// CPrintInCloudDlg 对话框



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


// CPrintInCloudDlg 消息处理程序

BOOL CPrintInCloudDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CON = (CStatic*)GetDlgItem(IDC_CONNECTION);
	PIC = GetDlgItem(IDC_PICTURE);

	SetTimer(1, 0, NULL);//5秒刷新一次
	//_CrtSetBreakAlloc(521);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPrintInCloudDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPrintInCloudDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPrintInCloudDlg::OnTimer(UINT_PTR x)
{
	KillTimer(1);

	//获取注册表中的ID
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
	SetTimer(1, 5000, NULL);//5秒刷新一次

	Json::Value root;
	Json::Reader reader;

	if (e) {
		CString m;
		m.Format(L"连接服务器失败。错误码：%d",e);
		CON->SetWindowText(m);
		return;
	}
	std::string json = CT2CA(ret.GetBuffer());
	reader.parse(json, root, false);

	auto id = root["id"];
	if (id.isString()) {
		const char* ID = id.asCString();
		CString m;
		m.Format(L"连接服务器成功。设备编号：%s", CA2CT(ID));
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

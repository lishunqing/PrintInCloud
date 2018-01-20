
// PrintInCloudDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PrintInCloud.h"
#include "PrintInCloudDlg.h"
#include "afxdialogex.h"
#include <iostream>  
#include "server.h" 
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
	ON_BN_CLICKED(IDOK, &CPrintInCloudDlg::OnBnClickedOk)
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



void CPrintInCloudDlg::OnBnClickedOk()
{
	CRect rect;
	PIC->GetWindowRect(&rect);
	ScreenToClient(rect);

	HDC hdcScreen = ::GetDC(NULL);
	CDC *dc = GetDC();


	DWORD e = 0;

	CString ret = getTaks(NULL,e);
	print::drawQRCode(dc, rect, ret);

	CON->SetWindowText(L"���ӳɹ�");

	MessageBox(ret);
}

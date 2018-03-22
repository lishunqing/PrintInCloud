
// PrintInCloudDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PrintInCloud.h"
#include "PrintInCloudDlg.h"
#include "afxdialogex.h"
#include "server.h"
#include "print.h"
#include <WinSpool.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPrintInCloudDlg �Ի���



CPrintInCloudDlg::CPrintInCloudDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PRINTINCLOUD_DIALOG, pParent), _pd(FALSE, PD_ALLPAGES | PD_COLLATE | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE)
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
ON_NOTIFY(NM_CLICK, IDC_LIST, &CPrintInCloudDlg::OnBnClickedList)
ON_BN_CLICKED(IDC_SETPRINTER, &CPrintInCloudDlg::OnBnClickedSetprinter)
ON_BN_CLICKED(IDC_PRINTALL, &CPrintInCloudDlg::OnBnClickedPrintAll)
ON_BN_CLICKED(IDC_PRINTLIST, &CPrintInCloudDlg::OnBnClickedPrintList)
ON_BN_CLICKED(IDC_PRINTONE, &CPrintInCloudDlg::OnBnClickedPrintOne)
ON_BN_CLICKED(IDC_PRINTCHOOSE, &CPrintInCloudDlg::OnBnClickedPrintchoose)
ON_BN_CLICKED(IDC_AUTO, &CPrintInCloudDlg::OnBnClickedAuto)
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
	PIC = GetDlgItem(IDC_QRCODE);
	PREVIEW = GetDlgItem(IDC_PRINTPREVIEW);
	MODELLIST = (CListCtrl*)GetDlgItem(IDC_LIST);
	DWORD dwStyle = MODELLIST->GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	MODELLIST->SetExtendedStyle(dwStyle);

	((CButton*)GetDlgItem(IDC_AUTO))->SetCheck(1);

	srand((unsigned)time(NULL));

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
	OnTimer(1);
	OnBnClickedAuto();
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

	Json::Value root;
	Json::Reader reader;

	if (e) {
		CString m;
		m.Format(L"���ӷ�����ʧ�ܡ������룺%d",e);
		CON->SetWindowText(m);
		SetTimer(1, 5000, NULL);//5��ˢ��һ��
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
		ReleaseDC(dc);

		HKEY hKey;
		long lRet = RegCreateKey(HKEY_CURRENT_USER, lpConfig, &hKey);
		if (lRet == ERROR_SUCCESS) {
			lRet = RegSetValueExA(hKey, "id", 0, REG_SZ, (const BYTE*)ID,strlen(ID));
		}
		RegCloseKey(hKey);
	}
	Json::Value t = root["task"];
	if (t.isNull())
	{
		SetTimer(1, 5000, NULL);//5��ˢ��һ��
		return;
	}
	task = t;
	loadList();
	int type = t["type"].asInt();

	if (state)
	{
		if (type == 1)
		{
			OnBnClickedPrintAll();
		}
		else if (type == 2)
		{
			OnBnClickedPrintList();
		}
	}
	SetTimer(1, 5000, NULL);//5��ˢ��һ��
}


void CPrintInCloudDlg::OnBnClickedSetprinter()
{
	if (_pd.DoModal() == IDOK)
		setprint = true;
}
void CPrintInCloudDlg::loadList()
{
	auto t = task;
	auto style = t["style"];
	auto model = t["model"];

	if (style.isNull() || model.isNull())
		return;

	MODELLIST->DeleteAllItems();
	while(MODELLIST->DeleteColumn(0));
	MODELLIST->InsertColumn(0, _T("���"), LVCFMT_CENTER, 100);//������

	MODELLIST->InsertColumn(1, _T("����"), LVCFMT_CENTER, 80);//������
	MODELLIST->InsertColumn(1, _T("�۸�"), LVCFMT_CENTER, 80);//������

	if (style.isArray())
	{
		int c = style.size();
		if (c > 2) c = 2;
		for (int i = c - 1; i >= 0; --i)
		{
			auto s = style[i];
			auto name = s["stylename"].asCString();
			MODELLIST->InsertColumn(1, CA2CT(name), LVCFMT_CENTER, 60);//������
		}
	}

	MODELLIST->InsertColumn(1, _T("����"), LVCFMT_CENTER, 80);//������

	if (model.isArray())
	{
		int s = model.size();
		for (int i = 0; i < s; ++i)
		{
			auto m = model[i];
			auto str = m.toStyledString();
			auto text = m["modelcode"].asCString();
			int nRow = MODELLIST->InsertItem(i, CA2CT(text));
			text = m["name"].asCString();
			MODELLIST->SetItemText(nRow, 1, CA2CT(text));
			text = m["style1"].asCString();
			MODELLIST->SetItemText(nRow, 2, CA2CT(text));
			text = m["style2"].asCString();
			MODELLIST->SetItemText(nRow, 3, CA2CT(text));
			double price = m["price"].asDouble();
			CString n;
			n.Format(_T("%.02f"),price);
			MODELLIST->SetItemText(nRow, 4, n.GetBuffer());
			int amount = m["amount"].asInt();
			n.Format(_T("%d"), amount);
			MODELLIST->SetItemText(nRow, 5, n.GetBuffer());
		}
	}
}

void CPrintInCloudDlg::OnBnClickedPrintAll()
{
	auto t = task;
	auto style = t["style"];
	auto model = t["model"];

	if (style.isNull() || model.isNull())
		return;

	CPrintDialog dpd(FALSE, PD_ALLPAGES | PD_COLLATE | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE);
	dpd.GetDefaults();
	CPrintDialog *pd = NULL;
	if (setprint)
	{
		pd = &_pd;
	}
	else
	{
		pd = &dpd;
	}
	//���¶���ֽ�Ŵ�С
	HDC   hDC = pd->CreatePrinterDC();
	DEVMODE* lpDevMode = (DEVMODE*)pd->GetDevMode();
	lpDevMode->dmFields = DM_ORIENTATION | DM_PAPERSIZE | DM_PAPERLENGTH | DM_PAPERWIDTH;
	lpDevMode->dmPaperSize = DMPAPER_USER;
	lpDevMode->dmPaperWidth = 800;
	lpDevMode->dmPaperLength = 400;
	lpDevMode->dmOrientation = DMORIENT_LANDSCAPE;
	ResetDC(hDC, lpDevMode); //ʹ���õĲ�����������
	CDC   dc;
	dc.Attach(hDC);

	const int mmperinch = 254;

	int dpix = dc.GetDeviceCaps(LOGPIXELSX);
	int dpiy = dc.GetDeviceCaps(LOGPIXELSY);
	int xsize = 400 * dpix / mmperinch;
	int ysize = 800 * dpiy / mmperinch;

	DOCINFO   di;
	memset(&di, 0, sizeof(DOCINFO));
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = _T("TAG Print");
	di.lpszOutput = (LPTSTR)NULL;
	di.fwType = 0;

	dc.StartDoc(&di);

	int s = model.size();
	for (int i = 0; i < s; ++i)
	{
		auto m = model[i];
		int amount = m["amount"].asInt();
		for (int j = 0; j < amount; ++j)
		{
			dc.StartPage();
			print::printTag(task, i, &dc, 0, 0, xsize, ysize);
			dc.EndPage();
		}
	}
	dc.EndDoc();
	dc.DeleteDC();
}


void CPrintInCloudDlg::OnBnClickedPrintList()
{
	CPrintDialog dpd(FALSE, PD_ALLPAGES | PD_COLLATE | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE);
	dpd.GetDefaults();
	CPrintDialog *pd = NULL;
	if (setprint)
	{
		pd = &_pd;
	}
	else
	{
		pd = &dpd;
	}
	//���¶���ֽ�Ŵ�С
	HDC   hDC = pd->CreatePrinterDC();
	DEVMODE* lpDevMode = (DEVMODE*)pd->GetDevMode();
	lpDevMode->dmFields = DM_ORIENTATION | DM_PAPERSIZE | DM_PAPERLENGTH | DM_PAPERWIDTH;
	lpDevMode->dmPaperSize = DMPAPER_USER;
	lpDevMode->dmPaperWidth = 800;
	lpDevMode->dmPaperLength = 400;
	lpDevMode->dmOrientation = DMORIENT_LANDSCAPE;
	ResetDC(hDC, lpDevMode); //ʹ���õĲ�����������
	CDC   dc;
	dc.Attach(hDC);

	const int mmperinch = 254;

	int dpix = dc.GetDeviceCaps(LOGPIXELSX);
	int dpiy = dc.GetDeviceCaps(LOGPIXELSY);
	int xsize = 400 * dpix / mmperinch;
	int ysize = 800 * dpiy / mmperinch;

	DOCINFO   di;
	memset(&di, 0, sizeof(DOCINFO));
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = _T("TAG Print");
	di.lpszOutput = (LPTSTR)NULL;
	di.fwType = 0;

	dc.StartDoc(&di);

	//todo,��ҳ
	int start = 0;
	do
	{
		dc.StartPage();
		start = print::printList(task, start, &dc, 0, 0, xsize, ysize);
		dc.EndPage();
	} while (start >= 0);
	

	dc.EndDoc();
	dc.DeleteDC();

}


void CPrintInCloudDlg::OnBnClickedPrintOne()
{
	POSITION pos = MODELLIST->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox( L"������ѡ��һ��", L"��ʾ", MB_OK);
		return;
	}
	int idx = MODELLIST->GetNextSelectedItem(pos);
	CPrintDialog dpd(FALSE, PD_ALLPAGES | PD_COLLATE | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE);
	dpd.GetDefaults();
	CPrintDialog *pd = NULL;
	if (setprint)
	{
		pd = &_pd;
	}
	else
	{
		pd = &dpd;
	}
	//���¶���ֽ�Ŵ�С
	HDC   hDC = pd->CreatePrinterDC();
	DEVMODE* lpDevMode = (DEVMODE*)pd->GetDevMode();
	lpDevMode->dmFields = DM_ORIENTATION | DM_PAPERSIZE | DM_PAPERLENGTH | DM_PAPERWIDTH;
	lpDevMode->dmPaperSize = DMPAPER_USER;
	lpDevMode->dmPaperWidth = 800;
	lpDevMode->dmPaperLength = 400;
	lpDevMode->dmOrientation = DMORIENT_LANDSCAPE;
	ResetDC(hDC, lpDevMode); //ʹ���õĲ�����������
	CDC   dc;
	dc.Attach(hDC);

	const int mmperinch = 254;

	int dpix = dc.GetDeviceCaps(LOGPIXELSX);
	int dpiy = dc.GetDeviceCaps(LOGPIXELSY);
	int xsize = 400 * dpix / mmperinch;
	int ysize = 800 * dpiy / mmperinch;

	DOCINFO   di;
	memset(&di, 0, sizeof(DOCINFO));
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = _T("TAG Print");
	di.lpszOutput = (LPTSTR)NULL;
	di.fwType = 0;

	dc.StartDoc(&di);
	dc.StartPage();

	print::printTag(task, idx, &dc, 0, 0, xsize, ysize);

	dc.EndPage();
	dc.EndDoc();
	dc.DeleteDC();
}

void CPrintInCloudDlg::OnBnClickedPrintchoose()
{
	POSITION pos = MODELLIST->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox(L"������ѡ��һ��", L"��ʾ", MB_OK);
		return;
	}
	auto t = task;
	auto style = t["style"];
	auto model = t["model"];

	if (style.isNull() || model.isNull())
		return;
	int idx = MODELLIST->GetNextSelectedItem(pos);
	CPrintDialog dpd(FALSE, PD_ALLPAGES | PD_COLLATE | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE);
	dpd.GetDefaults();
	CPrintDialog *pd = NULL;
	if (setprint)
	{
		pd = &_pd;
	}
	else
	{
		pd = &dpd;
	}
	//���¶���ֽ�Ŵ�С
	HDC   hDC = pd->CreatePrinterDC();
	DEVMODE* lpDevMode = (DEVMODE*)pd->GetDevMode();
	lpDevMode->dmFields = DM_ORIENTATION | DM_PAPERSIZE | DM_PAPERLENGTH | DM_PAPERWIDTH;
	lpDevMode->dmPaperSize = DMPAPER_USER;
	lpDevMode->dmPaperWidth = 800;
	lpDevMode->dmPaperLength = 400;
	lpDevMode->dmOrientation = DMORIENT_LANDSCAPE;
	ResetDC(hDC, lpDevMode); //ʹ���õĲ�����������
	CDC   dc;
	dc.Attach(hDC);

	const int mmperinch = 254;

	int dpix = dc.GetDeviceCaps(LOGPIXELSX);
	int dpiy = dc.GetDeviceCaps(LOGPIXELSY);
	int xsize = 400 * dpix / mmperinch;
	int ysize = 800 * dpiy / mmperinch;

	DOCINFO   di;
	memset(&di, 0, sizeof(DOCINFO));
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = _T("TAG Print");
	di.lpszOutput = (LPTSTR)NULL;
	di.fwType = 0;

	dc.StartDoc(&di);
	auto m = model[idx];
	int amount = m["amount"].asInt();
	for (int j = 0; j < amount; ++j)
	{
		dc.StartPage();
		print::printTag(task, idx, &dc, 0, 0, xsize, ysize);
		dc.EndPage();
	}
	dc.EndDoc();
	dc.DeleteDC();
}

void CPrintInCloudDlg::OnBnClickedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	//�б����¼���Ԥ����ǩ
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem < 0)
	{
		*pResult = -1;
		return;
	}
	int i = pNMItemActivate->iItem;

	CRect rect;
	PREVIEW->GetWindowRect(&rect);
	ScreenToClient(rect);
	rect.bottom = rect.top + rect.Width() * 2;
	CDC *dc = GetDC();
	FillRect(dc->GetSafeHdc(), rect, CreateSolidBrush(RGB(255, 255, 255)));
	print::printTag(task, i, dc, rect.left, rect.top, rect.Width(), rect.Height());
	ReleaseDC(dc);

}



void CPrintInCloudDlg::OnBnClickedAuto()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_AUTO);
	state = pBtn->GetCheck();
	CRect clint;
	GetWindowRect(&clint);
	if (state)
	{
		CRect rect;
		PIC->GetWindowRect(&rect);
		int b = rect.left - clint.left;
		clint.right = rect.right + b;
		clint.bottom = rect.bottom + 3 * b;
		MoveWindow(&clint);
	}
	else
	{
		CRect rect;
		PIC->GetWindowRect(&rect);
		int b = rect.left - clint.left;
		PREVIEW->GetWindowRect(&rect);

		clint.right = rect.right + b;
		clint.bottom = rect.bottom + 3 * b;
		MoveWindow(&clint);
	}
}

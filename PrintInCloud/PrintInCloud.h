
// PrintInCloud.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPrintInCloudApp: 
// �йش����ʵ�֣������ PrintInCloud.cpp
//

class CPrintInCloudApp : public CWinApp
{
public:
	CPrintInCloudApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPrintInCloudApp theApp;
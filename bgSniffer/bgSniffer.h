
// bgSniffer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CbgSnifferApp:
// �йش����ʵ�֣������ bgSniffer.cpp
//

class CbgSnifferApp : public CWinApp
{
public:
	CbgSnifferApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CbgSnifferApp theApp;
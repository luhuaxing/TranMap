
// TranMap.h : TranMap Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CTranMapApp:
// �йش����ʵ�֣������ TranMap.cpp
//

class CTranMapApp : public CWinApp
{
public:
	CTranMapApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTranMapApp theApp;

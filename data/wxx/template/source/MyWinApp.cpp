/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"
#include "Application.hpp"



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// Definitions for the CDockContainerApp class
CMyWinApp::CMyWinApp()
{
}

CMyWinApp::~CMyWinApp()
{
}

//===========================================================================
BOOL CMyWinApp::InitInstance()
{
	//Create the Frame Window
	m_Mainframe.Create(); // throws a CWinException on failure    

	return TRUE;
}



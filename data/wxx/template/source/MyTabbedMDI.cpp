/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"
#include "Application.hpp"



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CMyTabbedMDI::CMyTabbedMDI()
{
}

CMyTabbedMDI::~CMyTabbedMDI()
{
}

//===========================================================================
CWnd* CMyTabbedMDI::NewMDIChildFromID(int mdiChild)
{
	CWnd* pView = NULL;

	
	switch(mdiChild)
	{
	case ID_MDI_VIEW_MY:
		pView = new CMyView();
		break;

	default:
		TRACE("Unknown TabbedMDI id\n");
		break;
	}

	return pView;
}

CWnd* CMyTabbedMDI::AddMDIChild(CWnd* pView, LPCTSTR pTabText, int mdiChildID /*= 0*/)
{
	assert(pView); // Cannot add Null CWnd*
	assert(lstrlen(pTabText) < MAX_MENU_STRING);

	GetTab().AddTabPage(pView, pTabText, IDI_VIEW, mdiChildID);

	// Fake a WM_MOUSEACTIVATE to propagate focus change to dockers
	if (IsWindow())
		GetParent().SendMessage(WM_MOUSEACTIVATE, (WPARAM)(GetAncestor().GetHwnd()), MAKELPARAM(HTCLIENT,WM_LBUTTONDOWN));

	return pView;
}

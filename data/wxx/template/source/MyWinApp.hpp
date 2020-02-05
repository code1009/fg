#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "Mainframe.hpp"



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyWinApp : public CWinApp
{
public:
	CMyWinApp();
	virtual ~CMyWinApp();
	virtual BOOL InitInstance();
	CMainFrame& GetMainFrame() { return m_Mainframe; }

private:
	CMainFrame m_Mainframe;
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// returns a reference to the CMyWinApp object
inline CMyWinApp* GetMyWinApp() 
{
	return static_cast<CMyWinApp*>(GetApp()); 
}

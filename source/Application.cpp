/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"
#include "Application.hpp"



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma comment (lib, "comctl32.lib")
#pragma comment (lib, "cx_static_library.lib")



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
inline BOOL MyInitCommonControls(DWORD dwFlags)
{
	INITCOMMONCONTROLSEX iccx = { sizeof(INITCOMMONCONTROLSEX), dwFlags };
	BOOL bRet = ::InitCommonControlsEx(&iccx);

	return bRet;
}

//===========================================================================
int APIENTRY _tWinMain
(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPTSTR    lpCmdLine,
	_In_     int       nCmdShow
)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	try
	{
		// Start Win32++
		CMyWinApp theApp;

//		GetComCtlVersion();
//		MyInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);

		// Run the application
		return theApp.Run();
	}

	// catch all unhandled CException types
	catch (const CException &e)
	{
		// Display the exception and quit
		MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);

		return -1;
	}

	return 0u;
}

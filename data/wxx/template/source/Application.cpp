/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"

#if defined(_DEBUG)
#include <Visual Leak Detector/2.5.1/include/vld.h>
#endif

//===========================================================================
#include <objidl.h>

//===========================================================================
#ifdef _MSC_VER
#pragma warning (disable : 4458) // disable warning: declaration hides class member
#endif
#include <gdiplus.h>
#ifdef _MSC_VER
#pragma warning (default : 4458) // return warning to default
#endif

//===========================================================================
#include "Application.hpp"




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma comment (lib, "comctl32.lib")
#pragma comment (lib, "gdiplus.lib")
#pragma comment (lib, "cx_static_library.lib")
//#if defined(_DEBUG)
//#pragma comment(lib, "vld.lib")
//#endif



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined(_DEBUG)
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using namespace Gdiplus;




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
	//-----------------------------------------------------------------------
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);


	//-----------------------------------------------------------------------
	//cx::debug::print_system();
	if (false == my_debug_initialize())
	{
		return 0;
	}

	CX_DEBUG_TRACEF(CX_TWA_NORMAL, "Start!");


	//-----------------------------------------------------------------------
	int nResult;


	//-----------------------------------------------------------------------
	HRESULT hResult;

	
	hResult = ::OleInitialize(NULL);
	CX_DEBUG_ASSERT(SUCCEEDED(hResult));

//	hResult = ::CoInitialize(NULL);
//	CX_DEBUG_ASSERT(SUCCEEDED(hResult));

//	hResult = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
//	hResult = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
//	CX_DEBUG_ASSERT(SUCCEEDED(hResult));
	

	//-----------------------------------------------------------------------
	ULONG_PTR gdiplus_token;
	GdiplusStartupInput gdiplusStartupInput;


	Status status;
	
	
	status = ::GdiplusStartup(&gdiplus_token, &gdiplusStartupInput, NULL);
	CX_DEBUG_ASSERT(Ok==status);


	//-----------------------------------------------------------------------
	try
	{
		// Start Win32++
		CMyWinApp theApp;

//		GetComCtlVersion();
//		MyInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);

		// Run the application
		nResult = theApp.Run();
	}

	// catch all unhandled CException types
	catch (const CException &e)
	{
		// Display the exception and quit
		MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);

		nResult = -1;
	}

	//-----------------------------------------------------------------------
	my_debug_uninitialize();


	return nResult;
}

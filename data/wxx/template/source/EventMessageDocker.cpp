/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"
#include "Application.hpp"



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CEventMessageDockerWnd::CEventMessageDockerWnd()
{
}

CEventMessageDockerWnd::~CEventMessageDockerWnd()
{
//    if (IsWindow()) DeleteAllItems();
}

//===========================================================================
void CEventMessageDockerWnd::PreCreate(CREATESTRUCT& cs)
{
	CListView::PreCreate(cs);

	cs.dwExStyle |= WS_EX_CLIENTEDGE;

	cs.style |= (LVS_OWNERDATA);
//	cs.style |= (LVS_REPORT | LVS_SHOWSELALWAYS | LVS_OWNERDATA);
}

void CEventMessageDockerWnd::OnAttach()
{
	//-----------------------------------------------------------------------
	m_SmallImageList.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);

//	SetImageList(m_SmallImageList, LVSIL_SMALL);


	//-----------------------------------------------------------------------
	DWORD dwStyle;
	DWORD dwExtendedStyle;


	dwStyle = 
		WS_CHILD          | 
		WS_VISIBLE        | 
		WS_CLIPSIBLINGS   | 
		WS_CLIPCHILDREN   | 
		LVS_REPORT        | 
		LVS_SHOWSELALWAYS | 
		LVS_OWNERDATA     |
		0u;

	dwExtendedStyle = 
		LVS_EX_FULLROWSELECT    |
		LVS_EX_GRIDLINES        |
		LVS_EX_DOUBLEBUFFER     |
//		LVS_EX_LABELTIP 	    |
//		LVS_EX_SUBITEMIMAGES    |
//		LVS_EX_TRANSPARENTBKGND |
		0u;

	SetStyle         (dwStyle);
	SetExtendedStyle (dwExtendedStyle);


	//--------------------------------------------------------------------------
	LVCOLUMN LvColumn;


	memset (&LvColumn, 0, sizeof(LvColumn));
	LvColumn.mask      = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	LvColumn.fmt       = LVCFMT_RIGHT;
	LvColumn.pszText   = "번호";
	LvColumn.cx        = 50;
	LvColumn.iSubItem  = 0;
	InsertColumn(0, LvColumn);

	memset (&LvColumn, 0, sizeof(LvColumn));
	LvColumn.mask      = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	LvColumn.fmt       = LVCFMT_LEFT;
	LvColumn.pszText   = "시간";
	LvColumn.cx        = 180;
	LvColumn.iSubItem  = 1;
	InsertColumn(1, LvColumn);

	memset (&LvColumn, 0, sizeof(LvColumn));
	LvColumn.mask      = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	LvColumn.fmt       = LVCFMT_CENTER;
	LvColumn.pszText   = "항목";
	LvColumn.cx        = 80;
	LvColumn.iSubItem  = 2;
	InsertColumn(2, LvColumn);

	memset (&LvColumn, 0, sizeof(LvColumn));
	LvColumn.mask      = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	LvColumn.fmt       = LVCFMT_LEFT;
	LvColumn.pszText   = "내용";
	LvColumn.cx        = 800;
	LvColumn.cchTextMax= 1024*3; // GetItem올때 가져올수있는 크기 (시하는 텍스트는 259+1[MAX_PATH]널문자로 제한됨)
	LvColumn.iSubItem  = 3;
	InsertColumn(3, LvColumn);
	

	SetItemCountEx (100, LVSICF_NOSCROLL);
}

void CEventMessageDockerWnd::OnDestroy()
{
	SetImageList(NULL, LVSIL_SMALL);
}

LRESULT CEventMessageDockerWnd::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
// Respond to a mouse click on the window
{
	SetFocus();
	return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CEventMessageDockerWnd::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_MOUSEACTIVATE:  return OnMouseActivate(msg, wparam, lparam);
	}

	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CEventMessageDockerWnd::OnNotifyReflect(WPARAM wparam, LPARAM lparam)
{
	LPNMHDR NmHdr = reinterpret_cast<LPNMHDR>(lparam);


	switch (NmHdr->code)
	{
	case LVN_GETDISPINFO:
		return OnLvnGetDispInfo(wparam, lparam);
		break;

	case LVN_BEGINLABELEDIT:
		break;

	case NM_CUSTOMDRAW:
		return OnNmCustomDraw(wparam, lparam);
		break;

	default:
		break;
	}


	return CListView::OnNotifyReflect(wparam, lparam);
}

LRESULT CEventMessageDockerWnd::OnLvnGetDispInfo (WPARAM wparam, LPARAM lparam)
{
	NMLVDISPINFO *NmLvDispInfo = reinterpret_cast<NMLVDISPINFO*>(lparam);
	LVITEM*       LvItem       = &NmLvDispInfo->item;


	if (LvItem->mask & LVIF_TEXT)
	{
		strncpy (LvItem->pszText, "text", LvItem->cchTextMax);
	}

	return 0;
}

LRESULT CEventMessageDockerWnd::OnLvnBeginLabelEdit (WPARAM wparam, LPARAM lparam)
{
	return CListView::OnNotify(wparam, lparam);
}

LRESULT CEventMessageDockerWnd::OnNmCustomDraw (WPARAM wparam, LPARAM lparam)
{
	LPNMCUSTOMDRAW NmCustomDraw = reinterpret_cast<LPNMCUSTOMDRAW>(lparam);
	LRESULT        Result       = 0;


	switch(NmCustomDraw->dwDrawStage)
	{
	case  CDDS_PREPAINT       : return OnPrePaint       (wparam, lparam); break;
	case  CDDS_POSTPAINT      : return OnPostPaint      (wparam, lparam); break;
	case  CDDS_PREERASE       : return OnPreErase       (wparam, lparam); break;
	case  CDDS_POSTERASE      : return OnPostErase      (wparam, lparam); break;
	case  CDDS_ITEMPREPAINT   : return OnItemPrePaint   (wparam, lparam); break;
	case  CDDS_ITEMPOSTPAINT  : return OnItemPostPaint  (wparam, lparam); break;
	case  CDDS_ITEMPREERASE   : return OnItemPreErase   (wparam, lparam); break;
	case  CDDS_ITEMPOSTERASE  : return OnItemPostErase  (wparam, lparam); break;
	case (CDDS_ITEMPREPAINT |
	      CDDS_SUBITEM      ) : return OnSubItemPrePaint(wparam, lparam); break;

	default:
		return CListView::OnNotify(wparam, lparam);
		break;
	}
	
	return Result;
}

LRESULT CEventMessageDockerWnd::OnPrePaint(WPARAM wparam, LPARAM lparam)
{
	return CDRF_NOTIFYITEMDRAW;
}

LRESULT CEventMessageDockerWnd::OnPostPaint(WPARAM wparam, LPARAM lparam)
{
	return CDRF_DODEFAULT;
}

LRESULT CEventMessageDockerWnd::OnPreErase(WPARAM wparam, LPARAM lparam)
{
	return CDRF_DODEFAULT;
}

LRESULT CEventMessageDockerWnd::OnPostErase(WPARAM wparam, LPARAM lparam)
{
	return CDRF_DODEFAULT;
}

LRESULT CEventMessageDockerWnd::OnItemPrePaint(WPARAM wparam, LPARAM lparam)
{
	NMLVCUSTOMDRAW* NmLvCustomDraw = reinterpret_cast<LPNMLVCUSTOMDRAW>(lparam);


	NmLvCustomDraw->clrText = RGB(0x00u, 0x00u, 0xFFu);

	if (0)
	{
//		:: SelectObject(NmLvCustomDraw->nmcd.hdc, m_BoldFont.m_hFont);
	
		return CDRF_NEWFONT; 
	}

	return CDRF_DODEFAULT;
}

LRESULT CEventMessageDockerWnd::OnItemPostPaint(WPARAM wparam, LPARAM lparam)
{
	return CDRF_DODEFAULT;
}

LRESULT CEventMessageDockerWnd::OnItemPreErase(WPARAM wparam, LPARAM lparam)
{
	return CDRF_DODEFAULT;
}

LRESULT CEventMessageDockerWnd::OnItemPostErase(WPARAM wparam, LPARAM lparam)
{
	return CDRF_DODEFAULT;
}

LRESULT CEventMessageDockerWnd::OnSubItemPrePaint(WPARAM wparam, LPARAM lparam)
{
	return CDRF_DODEFAULT;
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CEventMessageDockContainer::CEventMessageDockContainer()
{
	SetView        (m_Wnd); 
	SetDockCaption (_T("EventMessage - Docking container"));
	SetTabText     (_T("EventMessage"));
	SetTabIcon     (IDI_DOCKER);
} 



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CEventMessageDocker::CEventMessageDocker()
{
	SetView     (m_DockContainer);
//	SetBarWidth (4);
}



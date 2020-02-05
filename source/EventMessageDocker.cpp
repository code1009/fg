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

//	cs.style |= (LVS_OWNERDRAWFIXED);
	cs.style |= (LVS_OWNERDATA|LVS_OWNERDRAWFIXED);
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
		LVS_OWNERDRAWFIXED|
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

	case WM_DRAWITEM:    return OnDrawItem(wparam, lparam);
	case WM_MEASUREITEM: return OnMeasureItem(wparam, lparam);
	case WM_COMPAREITEM: return OnCompareItem(wparam, lparam);
	case WM_DELETEITEM:  return OnDeleteItem(wparam, lparam);
/*
	case OCM_DRAWITEM:    return OnDrawItem(wparam, lparam);
	case OCM_MEASUREITEM: return OnMeasureItem(wparam, lparam);
	case OCM_COMPAREITEM: return OnCompareItem(wparam, lparam);
	case OCM_DELETEITEM:  return OnDeleteItem(wparam, lparam);
*/
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

//---------------------------------------------------------------------------
LRESULT CEventMessageDockerWnd::OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DRAWITEM:    return OnDrawItem(wparam, lparam);
	case WM_MEASUREITEM: return OnMeasureItem(wparam, lparam);
	case WM_COMPAREITEM: return OnCompareItem(wparam, lparam);
	case WM_DELETEITEM:  return OnDeleteItem(wparam, lparam);
/*
	case OCM_DRAWITEM:    return OnDrawItem(wparam, lparam);
	case OCM_MEASUREITEM: return OnMeasureItem(wparam, lparam);
	case OCM_COMPAREITEM: return OnCompareItem(wparam, lparam);
	case OCM_DELETEITEM:  return OnDeleteItem(wparam, lparam);
*/
	}

	return CListView::OnMessageReflect(msg, wparam, lparam);
}

LRESULT CEventMessageDockerWnd::OnDrawItem(WPARAM wparam, LPARAM lparam)
{
	DrawItem((LPDRAWITEMSTRUCT)lparam);

	return (LRESULT)TRUE;
}

LRESULT CEventMessageDockerWnd::OnMeasureItem(WPARAM wparam, LPARAM lparam)
{
	MeasureItem((LPMEASUREITEMSTRUCT)lparam);

	return (LRESULT)TRUE;
}

LRESULT CEventMessageDockerWnd::OnCompareItem(WPARAM wparam, LPARAM lparam)
{
	return CompareItem((LPCOMPAREITEMSTRUCT)lparam);
}

LRESULT CEventMessageDockerWnd::OnDeleteItem(WPARAM wparam, LPARAM lparam)
{
	DeleteItem((LPDELETEITEMSTRUCT)lparam);

	return (LRESULT)TRUE;
}

//---------------------------------------------------------------------------
void CEventMessageDockerWnd::GetCellRect(int header_column, const CRect& item_rect, CRect& cell_rect)
{
	CHeader header;
	
	header.Attach(GetHeader());

	CRect header_rect;
	header_rect = header.GetItemRect(header_column);

	// If we don't do this, when we scroll to the right, we will be 
	// drawing as if we weren't and your cells won't line up with the
	// columns.
	int x_offset = -GetScrollPos(SB_HORZ);

	cell_rect.left = x_offset + header_rect.left;
	cell_rect.right = x_offset + header_rect.right;
	cell_rect.top = item_rect.top;
	cell_rect.bottom = item_rect.bottom;

	header.Detach();
}

void CEventMessageDockerWnd::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType != ODT_LISTVIEW)
	{
		return;
	}

	if(lpDrawItemStruct->itemAction != ODA_DRAWENTIRE)
	{
		return;
	}


	// must be implemented
	HDC dc = lpDrawItemStruct->hDC;
	CRect item_rect = lpDrawItemStruct->rcItem;
	int item_id = lpDrawItemStruct->itemID;

	bool checkered_toggle = (item_id % 2) != 0;



	CHeader header;
	
	header.Attach(GetHeader());

	int cols = header.GetItemCount();

	header.Detach();


	for (int n = 0; n < cols; n++)
	{
		CRect cell_rect;


		GetCellRect(n, item_rect, cell_rect);



		COLORREF square_color;

		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			square_color = GetSysColor(COLOR_HIGHLIGHT);
			::SetTextColor(dc, GetSysColor(COLOR_HIGHLIGHTTEXT));
		}
		else
		{
			square_color = checkered_toggle ? RGB(230, 244, 255) : RGB(145, 200, 255);
			::SetTextColor(dc, GetSysColor(COLOR_BTNTEXT));
		}



		HPEN square_pen = CreatePen(PS_SOLID, 1, square_color);
		HBRUSH squares_brush = CreateSolidBrush(square_color);
		SelectObject(dc, square_pen);
		SelectObject(dc, squares_brush);
		Rectangle(dc, cell_rect.left, cell_rect.top,cell_rect.right, cell_rect.bottom);
		DeleteObject(square_pen);
		DeleteObject(squares_brush);



		std::stringstream ss;
		ss << item_id << _T(":") << n;

		DrawText(dc, ss.str().c_str(), ss.str().size(), cell_rect, DT_CENTER | DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE);



		checkered_toggle = !checkered_toggle;
	}
}

void CEventMessageDockerWnd::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if(lpMeasureItemStruct->CtlType != ODT_MENU)
	{
		// return default height for a system font
		HWND hWnd = GetDlgItem(lpMeasureItemStruct->CtlID);

		CClientDC dc(hWnd);
		TEXTMETRIC tm = { 0 };
		dc.GetTextMetrics(tm);

		lpMeasureItemStruct->itemHeight = tm.tmHeight + tm.tmExternalLeading + 10;
	}
	else
	{
		lpMeasureItemStruct->itemHeight = ::GetSystemMetrics(SM_CYMENU);
	}
}

int CEventMessageDockerWnd::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	// all items are equal
	return 0;
}

void CEventMessageDockerWnd::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	// default - nothing
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



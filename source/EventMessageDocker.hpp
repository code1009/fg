#pragma once



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CXListView;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CXListViewInplaceEdit : public CEdit
{
public:
	CXListView* m_ListView;
	cx::int_t   m_Item;
	cx::int_t   m_Column;
	std::string m_Text;

public:
	cx::bool_t  m_Cancel;

public:
	explicit CXListViewInplaceEdit(CXListView* listview, cx::int_t item, cx::int_t column, std::string text);
	virtual ~CXListViewInplaceEdit();

public:
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual int  OnCreate (CREATESTRUCT& cs);
	virtual void OnDestroy();
	virtual void OnAttach ();

public:
	virtual BOOL PreTranslateMessage(MSG& msg);

public:
	virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

public:
	LRESULT OnNcDestroy (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnKeyDown   (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnChar      (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnSetFocus  (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnKillFocus (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnGetDlgCode(UINT msg, WPARAM wparam, LPARAM lparam);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CXListView : public CListView
{
private:
	CImageList                          m_SmallImageList;
	std::vector<CXListViewInplaceEdit*> m_InplaceEditContainer;

	std::vector < std::vector<std::string> > m_TestContainer;

	CFont m_Font;

public:
	CXListView();
	virtual ~CXListView();

public:
	virtual void    PreCreate       (CREATESTRUCT& cs);
	virtual void    OnDestroy       ();
	virtual void    OnAttach        ();
	virtual LRESULT WndProc         (UINT msg, WPARAM wparam, LPARAM lparam);
	virtual LRESULT OnNotifyReflect (WPARAM wparam, LPARAM lparam);
	virtual LRESULT OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam);

public:
	LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnLButtonDown  (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnLButtonDbClk (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnHScroll      (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnVScroll      (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnMouseWheel   (UINT msg, WPARAM wparam, LPARAM lparam);

	LRESULT InplaceEdit_OnDelete   (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT InplaceEdit_OnUpdate   (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT InplaceEdit_OnNavigate (UINT msg, WPARAM wparam, LPARAM lparam);
	void    InplaceEdit_New        (int item, int column);

public:
	LRESULT OnLvnGetDispInfo   (WPARAM wparam, LPARAM lparam);
	LRESULT OnLvnBeginLabelEdit(WPARAM wparam, LPARAM lparam);

	LRESULT OnNmCustomDraw   (WPARAM wparam, LPARAM lparam);
	LRESULT OnPrePaint       (WPARAM wparam, LPARAM lparam);
	LRESULT OnPostPaint      (WPARAM wparam, LPARAM lparam);
	LRESULT OnPreErase       (WPARAM wparam, LPARAM lparam);
	LRESULT OnPostErase      (WPARAM wparam, LPARAM lparam);
	LRESULT OnItemPrePaint   (WPARAM wparam, LPARAM lparam);
	LRESULT OnItemPostPaint  (WPARAM wparam, LPARAM lparam);
	LRESULT OnItemPreErase   (WPARAM wparam, LPARAM lparam);
	LRESULT OnItemPostErase  (WPARAM wparam, LPARAM lparam);
	LRESULT OnSubItemPrePaint(WPARAM wparam, LPARAM lparam);

public:
	LRESULT OnMeasureItem(UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnDrawItem   (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnDeleteItem (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnCompareItem(UINT msg, WPARAM wparam, LPARAM lparam);

	void DrawItem   (LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	int  CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/);
	void DeleteItem (LPDELETEITEMSTRUCT /*lpDeleteItemStruct*/);

	void GetCellRect(int header_column, const CRect& item_rect, CRect& cell_rect);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CEventMessageDockContainer : public CDockContainer
{
public:
	CEventMessageDockContainer();
	~CEventMessageDockContainer() {}

private:
	CXListView m_Wnd;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CEventMessageDocker : public CDocker
{
public:
	CEventMessageDocker();
	virtual ~CEventMessageDocker() {}

private:
	CEventMessageDockContainer m_DockContainer;
};


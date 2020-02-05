#pragma once



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CEventMessageDockerWnd : public CListView
{
public:
	CEventMessageDockerWnd();
	virtual ~CEventMessageDockerWnd();

public:
	virtual void PreCreate(CREATESTRUCT& cs);

public:
	virtual void OnAttach();
	virtual void OnDestroy();
	virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
	virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
	virtual LRESULT OnNotifyReflect(WPARAM wparam, LPARAM lparam);

	virtual LRESULT OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam);

private:
	LRESULT OnLvnGetDispInfo   (WPARAM wparam, LPARAM lparam);
	LRESULT OnLvnBeginLabelEdit(WPARAM wparam, LPARAM lparam);
	LRESULT OnNmCustomDraw(WPARAM wparam, LPARAM lparam);
	LRESULT OnPrePaint(WPARAM wparam, LPARAM lparam);
	LRESULT OnPostPaint(WPARAM wparam, LPARAM lparam);
	LRESULT OnPreErase(WPARAM wparam, LPARAM lparam);
	LRESULT OnPostErase(WPARAM wparam, LPARAM lparam);
	LRESULT OnItemPrePaint(WPARAM wparam, LPARAM lparam);
	LRESULT OnItemPostPaint(WPARAM wparam, LPARAM lparam);
	LRESULT OnItemPreErase(WPARAM wparam, LPARAM lparam);
	LRESULT OnItemPostErase(WPARAM wparam, LPARAM lparam);
	LRESULT OnSubItemPrePaint(WPARAM wparam, LPARAM lparam);

private:
	LRESULT OnMeasureItem(WPARAM wparam, LPARAM lparam);
	LRESULT OnDrawItem(WPARAM wparam, LPARAM lparam);
	LRESULT OnDeleteItem(WPARAM wparam, LPARAM lparam);
	LRESULT OnCompareItem(WPARAM wparam, LPARAM lparam);

	void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/);
	void DeleteItem(LPDELETEITEMSTRUCT /*lpDeleteItemStruct*/);

	void GetCellRect(int header_column, const CRect& item_rect, CRect& cell_rect);

private:
	CImageList m_SmallImageList;
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CEventMessageDockContainer : public CDockContainer
{
public:
	CEventMessageDockContainer();
	~CEventMessageDockContainer() {}

private:
	CEventMessageDockerWnd m_Wnd;
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


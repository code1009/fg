#pragma once



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyDockerWnd : public CWnd
{
public:
	virtual void PreCreate(CREATESTRUCT& cs);

public:
	virtual int OnCreate(CREATESTRUCT& cs);
	virtual void OnDestroy();
	virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
	virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
	virtual LRESULT OnTimer(UINT msg, WPARAM wparam, LPARAM lparam);
	virtual LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam);
	virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

	virtual void OnDraw (CDC& dc);

	virtual void Draw (CDC& dc);

private:
	int m_cxClientMax;
	int m_cyClientMax;
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyDockContainer : public CDockContainer
{
public:
	CMyDockContainer(); 
	~CMyDockContainer() {}

private:
	CMyDockerWnd m_Wnd;
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyDocker : public CDocker
{
public:
	CMyDocker(); 
	virtual ~CMyDocker() {}

private:
	CMyDockContainer m_DockContainer;
};


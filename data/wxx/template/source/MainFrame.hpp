#pragma once



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMainFrame : public CDockFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();

public:
	void LoadDefaultDockers();
	void LoadDefaultMDIs();

	void SetContainerTabsAtTop(BOOL atTop);
	void HideSingleContainerTab(BOOL hideSingle);
	void SetMDITabsAtTop(BOOL atTop);

	BOOL OnDefaultLayout();
	BOOL OnCloseMDIs();
	BOOL OnCloseDockers();
	BOOL OnContainerTabsAtTop();
	BOOL OnHideSingleTab();
	BOOL OnMDITabsAtTop();

	BOOL OnFileNew();
	BOOL OnFileExit();

protected:
	virtual CDocker* NewDockerFromID(int dockID);

	virtual BOOL SaveRegistrySettings();

	virtual void    PreCreate(CREATESTRUCT& cs);
	virtual void    SetupMenuIcons();
	virtual void    SetupToolBar();
	virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

	virtual LRESULT OnDockActivated(UINT msg, WPARAM wparam, LPARAM lparam);
	virtual LRESULT OnDockDestroyed(UINT msg, WPARAM wparam, LPARAM lparam);

	virtual int     OnCreate(CREATESTRUCT& cs);
	virtual void    OnInitialUpdate();
	virtual LRESULT OnInitMenuPopup(UINT msg, WPARAM wparam, LPARAM lparam);
	virtual void    OnMenuUpdate(UINT id);
	virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);

private:
	BOOL m_isContainerTabsAtTop;
	BOOL m_isHideSingleTab     ;
	BOOL m_isMDITabsAtTop      ;

	CDocker* m_pActiveDocker;

	CMyTabbedMDI m_MyTabbedMDI;
};



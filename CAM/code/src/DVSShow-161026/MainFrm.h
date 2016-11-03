// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__51F369B6_58A3_4956_98AF_B3567A07A052__INCLUDED_)
#define AFX_MAINFRM_H__51F369B6_58A3_4956_98AF_B3567A07A052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "LeftBar.h"
#include "AW_CMultiViewSplitter/AW_CMultiViewSplitter.h"
#include "LeftCoolBar/CoolTabCtrl.h"
#include "DlgVideo1.h"
#include "SysSettings.h"
#include "NetModuleSet.h"
#include "CamLinkSet.h"
#include "Control.h"
#include "DlgVideoSettings.h"
#include "WorkspaceBar.h"


class CMainFrame : public CBCGPFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

	AW_CMultiViewSplitter	m_wndSplitter;
	CWorkspaceBar			m_wndWorkSpace;
	CBCGPMenuBar			m_wndMenuBar;
	int						m_nViewVideo1;
	int						m_nCurVideoNum;	
	BOOL					m_bShowContrl;
	BOOL					m_bIsCameraNode;

	CTreeCtrl				m_wndTreeCtrl;
	CImageList				m_ilTreeImage;
	
	CControl*				m_pControl;
	CBCGPToolBar			m_wndToolBar;
	NOTIFYICONDATA m_notifyIconData;	//����ϵͳ��Ҫ��������������������Ϣ����Ϣ

// Operations
public:
	BOOL				OnBarCheck(UINT nID);
	void				OnControl();
	void				OnStopVideo();
	void				InitialTray();
	void				AddTray();
	void				DeleteTray();
	void				ModifyTray();

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	afx_msg void OnViewAlarm();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CBCGPStatusBar  m_wndStatusBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnReceiveInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStartVideo();
	afx_msg void OnSetSysSettings();
	afx_msg void OnSetNetModul();
	afx_msg void OnSetCamLink();
	afx_msg void OnSysAbout();
	afx_msg void OnSysExit();
	afx_msg void OnCapturePic();
	afx_msg void OnViewLeft();
	afx_msg void OnUpdateViewLeft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCapPic(CCmdUI* pCmdUI);
	afx_msg void OnUpdateControl(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStartVideo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStopVideo(CCmdUI* pCmdUI);
	afx_msg void OnSetVideosettings();
	afx_msg void OnViewVideoplay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnTrayNotification(WPARAM wId,LPARAM lEvent);	//���������Ϣ
	afx_msg void ShowOrHideWnd();									//��ʾ-���ش���
	afx_msg void OnUpdateShowOrHideWnd(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();					//�˳�
	afx_msg void OnClose();					//��С��������
    virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateViewVideoplay(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__51F369B6_58A3_4956_98AF_B3567A07A052__INCLUDED_)

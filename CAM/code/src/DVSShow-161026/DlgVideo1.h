#if !defined(AFX_DLGVIDEO1_H__E1D587A0_874C_4D29_8E27_7143A8E10FD2__INCLUDED_)
#define AFX_DLGVIDEO1_H__E1D587A0_874C_4D29_8E27_7143A8E10FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "DVSShow.h"
#include "MainFrm.h"
#include "DlgPTZPreset.h"

class CDlgPTZPreset;
class CDlgOutputCtrl;


class CDlgVideo1 : public CBCGPFormView
{
protected:
	CDlgVideo1();           // protected constructor used by dynamic creation
	virtual ~CDlgVideo1();
	DECLARE_DYNCREATE(CDlgVideo1)
// Attributes
public:
	
	int					m_nCamNo[2][MAX_VIDEO_NUM];// 0-遥信索引号，1-摄像机编号
	int					m_nCamParam[5][MAX_VIDEO_NUM];//0-遥信索引号>=0的下标值，1-摄像机等级，2-摄像机编号，3-视频所在的显示窗口，4-定时器累加值，
	int					m_nYxIndex;
	int					m_nSubWndIndex;
	int					m_nCurChanNum;
	int					m_nTimeSGVideo;
	int					m_nTimeAskDIDOStatus;
	int					m_nVideoNum;
	CString				m_sCurCamIP;
	CRect				m_rcPreview, m_rcDVSView;
	CDlgPTZPreset*		m_pDlgPtzPreset;
	BOOL				m_bFullScreen;
	
	
// Operations
public:
	//initialize the SDK
	BOOL InitSDK();
	BOOL ExitSDk();
	int Connect(DEV_INFO *pDev, int nChannel, int nWndIndex = -1);
	void ShowVideoWindow(BYTE bStatus);
	void FullScreen(BYTE nWndStatus);
	void InitChildWindow();
	void StopRTVideoEx(int nWndIndex);
	
	int GetCamNoByYXIndex(int nYxIndex);
	int GetPtzPresetByYxIndex(int nYxIndex);
	int GetSpaceWndNum();
	UINT ArrageOutputWnds(UINT nWndNum);

	UINT StartRTVideoEx(VIDEOPARAM* video, int nYxIndex, BOOL bManual /*= TRUE*/, int nStyle /*= STYLE_FLASH*/);
	long DevLogin(DEV_INFO* pdev);

	BOOL CreateRecordDir();


// Form Data
public:
	//{{AFX_DATA(CDlgVideo1)
	enum { IDD = IDD_DLG_VIDEO1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVideo1)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDlgVideo1)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRmenuControl();
	afx_msg void OnVideoCappic();
	afx_msg void OnRmenuPreset();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVIDEO1_H__E1D587A0_874C_4D29_8E27_7143A8E10FD2__INCLUDED_)

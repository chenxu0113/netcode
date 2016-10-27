// VideoPlayerDlg.h : header file
//

#if !defined(AFX_VIDEOPLAYERDLG_H__F3E195CD_AC32_4244_B1A3_67C32C9E09AC__INCLUDED_)
#define AFX_VIDEOPLAYERDLG_H__F3E195CD_AC32_4244_B1A3_67C32C9E09AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVideoPlayerDlg dialog
typedef struct _PARAMS
{
	CString sPath;
	CTime ts;
	CTime te;
	BOOL bAllVideo;
} PARAMS;
void ThreadFindVideoFiles(LPVOID lpvoid);

class CVideoPlayerDlg : public CDialog
{
// Construction
public:
	CVideoPlayerDlg(CWnd* pParent = NULL);	// standard constructor
	~CVideoPlayerDlg();
// Attributes
public:
	CString				m_sIniPath;
	CString				m_sCurPlayRec;
	HICON				m_hSoundStartIcon, m_hSoundStopIcon;
	HICON				m_hPlayEnableIcon, m_hPlayDisableIcon;
	HICON				m_hPauseEnableIcon, m_hPauseDisableIcon;
	HICON				m_hStopEnableIcon, m_hStopDisableIcon;
	HICON				m_hFastDisableIcon, m_hFastEnableIcon;
	HICON				m_hStepFEnableIcon, m_hStepFDisableIcon;
	HICON				m_hSlowDisableIcon, m_hSlowEnableIcon;
	HICON				m_hStepBackEnableIcon, m_hStepBackDisableIcon;
	HICON				m_hCaptureIcon;
	BOOL				m_bSound, m_bPlay, m_bPause, m_bStep, m_bFind;
	BOOL				m_bFullScreen, m_bIsTheSameFile;
	HWND				m_hWndPlay1, m_hWndPlay2;
	int					m_iTotalSeconds, m_nTimerPlay, m_nTimerDateTime;
	WINDOWPLACEMENT		m_struOldWndpl;
	CRect				m_rcOldWinRect;
	PARAMS				m_params;
	HANDLE				m_hFindVideoFiles;
	DWORD				m_dwFindVideoFiles;

// Methods
public:
	void				SearchAllFiles(CString sPath, CTime tFrom, CTime tTo, BOOL bAllVideo=FALSE);
	void				SetStopState();
	void				StartPlayVideo();
	void				StopPlayVideo();
	void				SetPlaying();
	void				SetPlayState_Ctrls();
	void				SetPauseState_Ctrls();
	void				PausePlayVideo();
	void				SetPlaySoundState();
	void				SetStopSoundState();
	void				InitDateAndTime();
	void				SetFullScreenState(BOOL bFlag=FALSE);
	void				SetFullScreen(BOOL bFullScreen);
	void				StartPlaySound(BOOL bSound=FALSE);
	BOOL				CheckDate(CTime tFrom, CTime tTo);
	BOOL				IsInWnd(CPoint point);
	BOOL				CheckDateTime(SYSTEMTIME tt);
	BOOL				GetDateTimeByString(CString sDate, SYSTEMTIME& tt);
public:
// Dialog Data
	//{{AFX_DATA(CVideoPlayerDlg)
	enum { IDD = IDD_VIDEOPLAYER_DIALOG };
	CSliderCtrl	m_wndVolume;
	CSliderCtrl	m_wndPlayProgress;
	CListCtrl	m_wndVideoList;
	CDateTimeCtrl	m_wndEndTime;
	CDateTimeCtrl	m_wndStartTime;
	CTime	m_ctEndTime;
	CTime	m_ctStartTime;
	CString	m_sVideoPath;
	BOOL	m_bAllVideo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoPlayerDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVideoPlayerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnFindvideo();
	afx_msg void OnBtnVideopath();
	afx_msg void OnBtnDelone();
	afx_msg void OnBtnDelall();
	afx_msg void OnDblclkListVideofile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnStop();
	afx_msg void OnReleasedcaptureSliderPlayprogress(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPlay();
	afx_msg void OnBtnStepback();
	afx_msg void OnBtnStepforward();
	afx_msg void OnBtnSlow();
	afx_msg void OnBtnFast();
	afx_msg void OnBtnCappic();
	afx_msg void OnBtnSound();
	afx_msg void OnReleasedcaptureSliderVolume(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnCheckAllvideo();
	afx_msg void OnBtnStopfindvideo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOPLAYERDLG_H__F3E195CD_AC32_4244_B1A3_67C32C9E09AC__INCLUDED_)

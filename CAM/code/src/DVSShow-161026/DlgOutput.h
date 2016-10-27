#if !defined(AFX_DLGOUTPUT_H__4FFDDC62_9DE0_49DC_A263_EAACE813D389__INCLUDED_)
#define AFX_DLGOUTPUT_H__4FFDDC62_9DE0_49DC_A263_EAACE813D389__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOutput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOutput dialog

class CDlgOutput : public CBCGPDialog
{
// Construction
public:
	CDlgOutput(CWnd* pParent = NULL);   // standard constructor
	~CDlgOutput();
	// 当前单击的窗口
	int m_nCamNo;
	int m_nSubWndIndex;
	int m_nYxIndex;
	int m_nCameraPort;
	int m_nAlarmFlashCount;//告警边线闪烁次数
	BYTE m_nWndStatus;
	long m_lPlayHandle;
	long m_nPlaydecHandle;
	long m_lChannelNum;
	LONG m_lPort;
	BOOL m_bCapPic;
	BOOL m_bStartVideo;
	BOOL m_bStopVideo;
	BOOL m_bTempRecord;
	CString m_sCameraIP;
	int nSpeed;
	short nType;
	void DrawOutputBorder();		
	DWORD m_dwBrightValue,m_dwContrastValue, m_dwSaturationValue,m_dwHueValue;

	TSCameraDef m_TSCamDef;	
	
	int m_nPaintTimer;
	volatile BOOL m_bAlarmFlag;	//有告警信息的标志
	DWORD m_dwAlarmTick;	//告警信息产生时间戳
	void SetAlarmState();
	void CapturePic();
	void drawOSD(LONG nPort,HDC hDc);
	BOOL StartLocalRecord();
	BOOL StopLocalRecord();
	BOOL CreateDir(CString sPath);
private:
	void SoundChangeWnd(int pCurIndex, CDlgOutput* preVideoWnd);

	BOOL m_bRecord;	//is recording or not
	bool m_bSound;
public:
	
	long m_lLogin; //login handle
	int m_iChannel; //play channel

	int ConnectRealPlay(DEV_INFO *pDev, int nChannel);

	void GetColor(long *nBright, long *nContrast, long *nSaturation, long *nHue);
	void SetColor(long nBright, long nContrast, long nSaturation, long nHue);


	void  OnOpenSound();
	void  OnCloseSound();

	long GetLoginHandle();

	afx_msg void OnRmenuStopvideo();
	afx_msg void OnDisconnct();
	afx_msg void OnRmenuStartTemprecord();
	afx_msg void OnRmenuStopTemprecord();

	long SetDevChnColor(SDK_CONFIG_VIDEOCOLOR* pVideoColor);

	static void __stdcall videoInfoFramCallback(LONG nPort, LONG nType, LPCSTR pBuf,LONG nSize, LONG nUser);
	CString m_strInfoFrame[100];

// Dialog Data
	//{{AFX_DATA(CDlgOutput)
	enum { IDD = IDD_DLG_OUTPUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOutput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOutput)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
	virtual BOOL OnInitDialog();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateRmenuStartTemprecord(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRmenuStopTemprecord(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCancelalarm();
	afx_msg void OnUpdateCancelalarm(CCmdUI *pCmdUI);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOUTPUT_H__4FFDDC62_9DE0_49DC_A263_EAACE813D389__INCLUDED_)

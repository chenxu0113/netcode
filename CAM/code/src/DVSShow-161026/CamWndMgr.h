#pragma once

#define FIRST_WND	-1
#define LAST_WND	-2

class CCamWndMgr
{
public:
	CCamWndMgr(void);
	virtual ~CCamWndMgr(void);
	
	void AttachWnd(int nWndIndex, CDlgOutput *pDlg);
	void DetachWnd(int nWndIndex);
	CDlgOutput *GetAttachedWnd(int nWndIndex);
	int GetAttachedWndNum();

public:
	int m_nCameraNo;			//摄像机编号
	int m_nWndCount;			//窗口计数
	map<int, CDlgOutput *> m_WndMap;	//播放窗口队列<窗口索引, 窗口Dlg对象>
	CRITICAL_SECTION m_csWndMap;

};

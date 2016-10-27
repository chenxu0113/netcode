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
	int m_nCameraNo;			//��������
	int m_nWndCount;			//���ڼ���
	map<int, CDlgOutput *> m_WndMap;	//���Ŵ��ڶ���<��������, ����Dlg����>
	CRITICAL_SECTION m_csWndMap;

};

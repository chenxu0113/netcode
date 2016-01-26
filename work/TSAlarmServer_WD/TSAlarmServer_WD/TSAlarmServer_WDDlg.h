
// TSAlarmServer_WDDlg.h : ͷ�ļ�
//

#pragma once
#include "TrayIcon.h"
#include "AlarmTask.h"
#include "ReceiveAlarmData.h"
#include "MessageTask.h"

// CTSAlarmServer_WDDlg �Ի���
class CTSAlarmServer_WDDlg : public CDialog
{
// ����
public:
	CTSAlarmServer_WDDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TSALARMSERVER_WD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	CAlarmTask *m_pAlarmTask;
	CReceiveAlarmData *m_pReceiveAlarmData;
	CMessageTask *m_pMessageTask;
// ʵ��
protected:
	LRESULT OnTrayNotification(WPARAM wParam,LPARAM lParam);
	HICON m_hIcon;
	CTrayIcon m_TrayIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnClose();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAbout1();
	afx_msg void OnExit();
	afx_msg void OnShow();

};

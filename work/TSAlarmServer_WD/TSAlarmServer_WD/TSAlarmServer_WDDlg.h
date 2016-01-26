
// TSAlarmServer_WDDlg.h : 头文件
//

#pragma once
#include "TrayIcon.h"
#include "AlarmTask.h"
#include "ReceiveAlarmData.h"
#include "MessageTask.h"

// CTSAlarmServer_WDDlg 对话框
class CTSAlarmServer_WDDlg : public CDialog
{
// 构造
public:
	CTSAlarmServer_WDDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TSALARMSERVER_WD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	CAlarmTask *m_pAlarmTask;
	CReceiveAlarmData *m_pReceiveAlarmData;
	CMessageTask *m_pMessageTask;
// 实现
protected:
	LRESULT OnTrayNotification(WPARAM wParam,LPARAM lParam);
	HICON m_hIcon;
	CTrayIcon m_TrayIcon;
	// 生成的消息映射函数
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

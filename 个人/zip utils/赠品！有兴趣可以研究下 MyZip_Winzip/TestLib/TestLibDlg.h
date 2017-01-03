// TestLibDlg.h : header file
//

#if !defined(AFX_TESTLIBDLG_H__CBE24478_874E_11D5_B480_0050BA2122CF__INCLUDED_)
#define AFX_TESTLIBDLG_H__CBE24478_874E_11D5_B480_0050BA2122CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern "C" BOOL AddFiletoZip(CString FileName,CString ZipName,CString DicInf);
extern "C" BOOL UnzipFile(CString PathName,CString ZipName,BOOL UseDic);
extern "C" BOOL AddDirtoZip(CString DirName,CString ZipName,CString Parent_inf);
/////////////////////////////////////////////////////////////////////////////
// CTestLibDlg dialog

class CTestLibDlg : public CDialog
{
// Construction
public:
	CTestLibDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestLibDlg)
	enum { IDD = IDD_TESTLIB_DIALOG };
	CString	m_strDicinf;
	CString	m_strFileName;
	CString	m_strZipName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestLibDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestLibDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCompress();
	afx_msg void OnUnzip();
	afx_msg void OnDir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTLIBDLG_H__CBE24478_874E_11D5_B480_0050BA2122CF__INCLUDED_)

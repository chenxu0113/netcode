// TestLib.h : main header file for the TESTLIB application
//

#if !defined(AFX_TESTLIB_H__CBE24476_874E_11D5_B480_0050BA2122CF__INCLUDED_)
#define AFX_TESTLIB_H__CBE24476_874E_11D5_B480_0050BA2122CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestLibApp:
// See TestLib.cpp for the implementation of this class
//

class CTestLibApp : public CWinApp
{
public:
	CTestLibApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestLibApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestLibApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTLIB_H__CBE24476_874E_11D5_B480_0050BA2122CF__INCLUDED_)

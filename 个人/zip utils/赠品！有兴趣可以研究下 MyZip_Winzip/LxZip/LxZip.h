// LxZip.h : main header file for the LXZIP DLL
//

#if !defined(AFX_LXZIP_H__CBE24466_874E_11D5_B480_0050BA2122CF__INCLUDED_)
#define AFX_LXZIP_H__CBE24466_874E_11D5_B480_0050BA2122CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLxZipApp
// See LxZip.cpp for the implementation of this class
//
class FileEM
{
public:
	CString PathName;
	CString DirInf;
};


typedef CList<FileEM,FileEM&> FileListTml;


class CLxZipApp : public CWinApp
{
public:
	CLxZipApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLxZipApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CLxZipApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LXZIP_H__CBE24466_874E_11D5_B480_0050BA2122CF__INCLUDED_)

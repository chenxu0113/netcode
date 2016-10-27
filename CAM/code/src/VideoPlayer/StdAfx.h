// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0D89D142_8C93_458A_9B2D_4C7182ABF630__INCLUDED_)
#define AFX_STDAFX_H__0D89D142_8C93_458A_9B2D_4C7182ABF630__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#define USED_PORT 99		//card decode port
#define TIMER_PLAY			1
#define TIMER_DATETIME		2
#define TIMER_INTERVAL		1000

#include "plaympeg4.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0D89D142_8C93_458A_9B2D_4C7182ABF630__INCLUDED_)

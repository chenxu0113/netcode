//:Ignore
// XTVersion.h Xtreme Toolkit version definitions file
//
// This file is a part of the Xtreme Toolkit for MFC.
// ©1998-2003 Codejock Software, All Rights Reserved.
//
// This source code can only be used under the terms and conditions 
// outlined in the accompanying license agreement.
//
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#if !defined(__XTVERSION_H__)
#define __XTVERSION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// Xtreme Toolkit version definitions:
//////////////////////////////////////////////////////////////////////

// for determining version of XTLIB.DLL
#define _XTLIB_VERSION_         MAKELONG(0,2)
// Major Version Number
#define _XTLIB_VERSION_MAJOR    2
// Minor Version Number
#define _XTLIB_VERSION_MINOR    0

// Prefix for dlls/libs
#if _MSC_VER <= 1200
#define _XTLIB_FILE_PREFIX      "XT2000"	// VC 6.0 or earlier
#else
#define _XTLIB_FILE_PREFIX      "XTNET2000" // VC 7.0 or later
#endif //_MSC_VER > 1200

//////////////////////////////////////////////////////////////////////
// Evaluation version definitions:
//////////////////////////////////////////////////////////////////////

//#ifndef _XT_DEMOMODE
//#define _XT_DEMOMODE						// defined in the evaluation library
//#endif //_XT_DEMOMODE

//////////////////////////////////////////////////////////////////////
//:End Ignore

#endif  // __XTVERSION_H__

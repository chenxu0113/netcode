// ComDlgInc.h : main header file for the COMDLG DLL
//

#if !defined(AFX_COMDLGINC_H__9098874F_3506_4455_8C9E_977684323275__INCLUDED_)
#define AFX_COMDLGINC_H__9098874F_3506_4455_8C9E_977684323275__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define _COMDLG_LIBNAME1_	"ComDlg"
#ifdef _UNICODE
#define _COMDLG_LIBNAME2_	_COMDLG_LIBNAME1_"U"
#else
#define _COMDLG_LIBNAME2_	_COMDLG_LIBNAME1_
#endif

#ifdef _DEBUG
#define _COMDLG_LIBNAME3_	_COMDLG_LIBNAME2_"D"
#else
#define _COMDLG_LIBNAME3_	_COMDLG_LIBNAME2_
#endif

#if _MSC_VER < 1300
#define _COMDLG_LIBNAME4_	_COMDLG_LIBNAME3_
#elif _MSC_VER == 1300
#define _COMDLG_LIBNAME4_	_COMDLG_LIBNAME3_"70"
#elif _MSC_VER < 1400
#define _COMDLG_LIBNAME4_	_COMDLG_LIBNAME3_"71"
#elif _MSC_VER < 1500
#define _COMDLG_LIBNAME4_	_COMDLG_LIBNAME3_"80"
#elif _MSC_VER == 1500
#define _COMDLG_LIBNAME4_	_COMDLG_LIBNAME3_"90"
#elif _MSC_VER == 1600
#define _COMDLG_LIBNAME4_	_COMDLG_LIBNAME3_"100"
#else
#error "Unsupported VC++ version"
#endif

#define _COMDLG_DLLNAME_   _COMDLG_LIBNAME4_".dll"
#define _COMDLG_LIBNAME_   _COMDLG_LIBNAME4_".lib"

#ifndef _COMDLG_EXPORT_
#pragma comment(lib, _COMDLG_LIBNAME_)
#pragma message("Automatically linking with " _COMDLG_LIBNAME_)
#endif


#if defined _COMDLG_EXPORT_ && defined _WINDLL
#include "..\ComDlg\ResourceHandler.h"
#define SET_DLL_RESOURCE_HANDLE CResourceHandler res;
#else
#define SET_DLL_RESOURCE_HANDLE
#endif


#if defined WIN32 && defined _AFXDLL
	#ifdef _COMDLG_EXPORT_
		#define COMDLGEXPORT __declspec(dllexport)
	#else
		#define COMDLGEXPORT __declspec(dllimport)
	#endif
#else
	#define COMDLGEXPORT
#endif 

#ifndef _COMDLG_EXPORT_
//#include "..\ComDlg\Resource.h"
#define IDD_LOGON_DIALOG                102
#define IDD_MESSAGE_DIALOG              103
#define IDD_PROCESS_DIALOG              129
#define IDD_GRPTREE_DIALOG              130
#define IDD_VARIABLE_DIALOG             137
#define IDD_ABOUT_DIALOG                138
#include "..\src\Common\ComDlg\MessageBoxDlg.h"
//#include "..\src\Common\ComDlg\LogonDlg.h"
#include "..\src\Common\ComDlg\ProgressDlg.h"
#include "..\src\Common\ComDlg\AboutDlg.h"
#endif

//#include "Typedef.h"
//#include "Constdef.h"
// Exported functions
COMDLGEXPORT void WINAPI ComDlg_SetAppLook();//should edit soon
COMDLGEXPORT void WINAPI ComDlg_SetAppLanguage();//should edit soon
COMDLGEXPORT BOOL WINAPI ComDlg_CheckRegistered();//should edit soon
COMDLGEXPORT BOOL WINAPI ComDlg_IsDemoMode();
//should edit soon
#endif // !defined(AFX_COMDLGINC_H__9098874F_3506_4455_8C9E_977684323275__INCLUDED_)

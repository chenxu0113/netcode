#ifndef _RETRIEVEPRIVIEGE_H
#define _RETRIEVEPRIVIEGE_H



#define _PRI_LIBNAME1_	"Privilege"

#ifdef _UNICODE
	#define _PRI_LIBNAME2_	_PRI_LIBNAME1_"U"
#else
	#define _PRI_LIBNAME2_	_PRI_LIBNAME1_
#endif

#ifdef _DEBUG
	#define _PRI_LIBNAME3_	_PRI_LIBNAME2_"D"
#else
	#define _PRI_LIBNAME3_	_PRI_LIBNAME2_
#endif


#define _PRI_DLLNAME_A   _PRI_LIBNAME3_".dll"
#define _PRI_LIBNAME_A   _PRI_LIBNAME3_".lib"

#ifndef _AFXEXT

#pragma comment(lib, "..\\..\\..\\lib\\"_PRI_LIBNAME_A)
#pragma message("Automatically linking with " _PRI_LIBNAME_A)
#endif


#ifdef __cplusplus

extern "C" {
#endif
__declspec(dllexport)  int APIENTRY GP_GetEnvironCode(CString & Str);
__declspec(dllexport)  int APIENTRY GP_GetPrivilegeCodeFromEnviron(int EncryptFactor, int & nLen, BYTE * RetBuf);
__declspec(dllexport)  int APIENTRY GP_GetPrivilegeCodeFromEnvironCode(CString Str, int EncryptFactor, int & nLen, BYTE * RetBuf);
#ifdef __cplusplus
}
#endif

#endif
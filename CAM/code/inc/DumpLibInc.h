#ifndef _EXCEPTION_HANDLE_H_
#define _EXCEPTION_HANDLE_H_

extern "C" void Initialize();

#define _DUMPLIB_PREFIX_ "DumpLib"
#ifdef _DEBUG
#ifdef UNICODE
#define _DUMPLIB_NAME_ _DUMPLIB_PREFIX_##"UD"
#else
#define _DUMPLIB_NAME_ _DUMPLIB_PREFIX_##"D"
#endif
#else
#ifdef UNICODE
#define _DUMPLIB_NAME_ _DUMPLIB_PREFIX_##"U"
#else
#define _DUMPLIB_NAME_ _DUMPLIB_PREFIX_
#endif
#endif

#define _DUMPLIB_ _DUMPLIB_NAME_##".lib"

#pragma message("Auto linking with " _DUMPLIB_)
#pragma comment(lib, _DUMPLIB_)

#endif

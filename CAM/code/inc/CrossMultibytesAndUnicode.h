#ifndef _CROSS_MULTIBYTE_UNICODE_H
#define _CROSS_MULTIBYTE_UNICODE_H

#ifdef _UNICODE
#define strtoi(x) _wtoi(x)
#define strtol(x) _wtol(x)
#define strtof(x) _wtof(x)
#define strtoi64(x) _wtoi64(x)//MSW 130524 ADD
#define strtoken(x,y) wcstok(x,y)
#define strcompare(x,y) wcscmp(x,y)
#define stringcpy(x,y) wcscpy(x,y)
#define stringscanf(x,y,...) swscanf(x,y,__VA_ARGS__)
#define stringprintf(x,y,...) swprintf(x,y,__VA_ARGS__)
#define itostr(x,y,z) _itow(x,y,z)
#define ltostr(x,y,z) _ltow(x,y,z)
#define stringlen(x) wcslen(x)
#define stringcat(x,y) wcscat(x,y)
#define SQLTCHAR	wchar_t*
#define FileOpen _wfopen
#define FilePrintf fwprintf
#else
#define strtoi(x) atoi(x)
#define strtol(x) atol(x)
#define strtof(x) atof(x)
#define strtoken(x,y) strtok(x,y)
#define strcompare(x,y) strcmp(x,y)
#define stringcpy(x,y) strcpy(x,y)
#define stringscanf(x,y,...) sscanf(x,y,__VA_ARGS__)
#define stringprintf(x,y,...) sprintf(x,y,__VA_ARGS__)
#define itostr(x,y,z) itoa(x,y,z)
#define stringlen(x) strlen(x)
#define stringcat(x,y) strcat(x,y)
#define ltostr(x,y,z) _ltoa(x,y,z)
#define SQLTCHAR UCHAR FAR *
#define FileOpen fopen
#define FilePrintf fprintf
#endif

#endif

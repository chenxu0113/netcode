// LxZip.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "LxZip.h"
#include "F:\NTVcp\MyZip\LxZip\ZipFile.h"
#include "F:\NTVcp\MyZip\LxZip\UnZipFile.h"
#include "Shlwapi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BUF_SIZE 100

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CLxZipApp

BEGIN_MESSAGE_MAP(CLxZipApp, CWinApp)
	//{{AFX_MSG_MAP(CLxZipApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLxZipApp construction

CLxZipApp::CLxZipApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLxZipApp object

CLxZipApp theApp;

/*
AddFiletoZip() 将文件加到.zip文件中
FileName 文件名
ZipName .zip的文件名
dicinf   文件放入.zip文件后的目录位置（相对路径)
*/
extern "C" __declspec(dllexport)BOOL AddFiletoZip(CString FileName,CString ZipName,CString DicInf)
{

		char TmpPath[_MAX_PATH];
		char TmpFileName[_MAX_FNAME];
		char TmpExt[_MAX_EXT];
        CString TmpFullPath;
		
		CZipFile zf(ZipName, 1);

		char buf[BUF_SIZE];
			
		CFile f(FileName, CFile::modeRead);

		zip_fileinfo zi;

		zf.UpdateZipInfo(zi, f);

		_tsplitpath(FileName, NULL, NULL,TmpFileName, TmpExt);	

		strcpy(TmpPath,strcat(TmpFileName,TmpExt));
		TmpFullPath=TmpPath;
		
		zf.OpenNewFileInZip(DicInf+TmpFullPath, zi, Z_BEST_COMPRESSION);

		int size_read;
		do
		{
			size_read = f.Read(buf, BUF_SIZE);
			if (size_read)
			zf.WriteInFileInZip(buf, size_read);
		
		}
		while (size_read == BUF_SIZE);


	zf.Close();
	return TRUE;
}

/*
UnZipFile() 将.zip中的所有文件解压到一个目录中
PathName 目标目录名
ZipName .zip的文件名
UseDic  是否按.zip中的目录结构解压（FALSE 将所有文件解到一个目录中）
*/


extern "C" BOOL __declspec(dllexport)UnzipFile(CString PathName,CString ZipName,BOOL UseDic)
{
	CString TmpPathName;
	CString szPath;
	CString szTmpPath;
    CString str_pName;
	int pos;


	BOOL fileend;
	CUnzipFile uf(ZipName);
	
	uf.GoToFirstFile();
	
	unz_file_info ui;


	do
	{

	uf.GetCurrentFileInfo(&ui);

	int iNameSize = ui.size_filename + 1;
	char* pName = new char [iNameSize];


	uf.GetCurrentFileInfo(NULL, pName, iNameSize);
    
     str_pName=pName;
	if (PathName.Right(1)!="\\")
	      PathName=PathName+"\\";
	if (str_pName.Left(1)=="\\")
		  str_pName.Delete(0,1);

	TmpPathName=PathName+str_pName;

    TmpPathName.Replace ("/","\\");

	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];
 	TCHAR szFileName[_MAX_FNAME];   
	_tsplitpath(TmpPathName, szDrive, szDir,szFileName, NULL);

    if (szFileName[0]!=NULL)
	{
		if (UseDic)
		{

		szPath = CString(szDrive) + szDir;

		pos=szPath.Find("\\",0);	
		do
		{
		szTmpPath=szPath.Left(pos);
	
		if (!PathFileExists(szTmpPath))
		{
			CreateDirectory(szTmpPath,NULL);
		}
		pos=szPath.Find("\\",pos+1);
		}
		while(pos!=-1);
		}
		else
		{
			TmpPathName=PathName+szFileName;
		}

		CFile f(TmpPathName, CFile::modeWrite | CFile::modeCreate);

		delete[] pName;
		uf.OpenCurrentFile();
		char buf[BUF_SIZE];


		int size_read;
		do
		{
			size_read = uf.ReadCurrentFile(buf, BUF_SIZE);
			if (size_read > 0)
			f.Write(buf, size_read);
		}
		while (size_read == BUF_SIZE);
	

		uf.UpdateFileStatus(f, ui);
		}
		fileend=uf.GoToNextFile();
	}
	while(fileend);

	uf.Close();	
	return TRUE;
}

/*
AddDirZip() 将整个目录加入到.zip文件中
DirName 目录名
ZipName .zip文件名
Parent_inf  父目录名
*/

extern "C" BOOL __declspec(dllexport)AddDirtoZip(CString DirName,CString ZipName,CString Parent_inf)
{

	CFileFind tempFind;
    char tempFileFind[200];
    sprintf(tempFileFind,"%s\\*.*",DirName);
    BOOL IsFinded=(BOOL)tempFind.FindFile(tempFileFind);
    while(IsFinded)
   {
       IsFinded=(BOOL)tempFind.FindNextFile();
       if(!tempFind.IsDots())
       {
             char foundFileName[200];
             strcpy(foundFileName,tempFind.GetFileName().GetBuffer(200));
             if(tempFind.IsDirectory())
             {
                   char tempDir[200];
                   sprintf(tempDir,"%s\\%s",DirName,foundFileName);
                   AddDirtoZip(tempDir,ZipName,Parent_inf+"\\"+foundFileName);
             }
             else
             {
                   char tempFileName[200];
                   sprintf(tempFileName,"%s\\%s",DirName,foundFileName);
                   AddFiletoZip(tempFileName,ZipName,Parent_inf+"\\");
             }
        }
   }
   tempFind.Close();
   return TRUE;
}
/*
AddListtoZip() 将一个文件列表压缩到一个zip文件中
pi 指向文件列表的指针
ZipName zip文件的文件名
*/


extern "C" BOOL __declspec(dllexport)AddListtoZip(FileListTml& pi, CString ZipName)
{
	BOOL b_Result;
	CString PathName;
	CString DirInf;

	
	int i_Count=pi.GetCount();

	for (int i=0;i<i_Count;i++)
	{
		PathName=pi.GetAt(pi.FindIndex(i)).PathName;
		DirInf=pi.GetAt(pi.FindIndex(i)).DirInf;
		b_Result=AddFiletoZip(PathName,ZipName,DirInf);
	}
	return TRUE;
}

/*
GetListFromZip()从zip得到被压缩的文件的列表
pi 返回列表的指针
ZipName zip文件的名称
*/

extern "C" BOOL __declspec(dllexport)GetListFromZip(FileListTml& pi, CString ZipName)
{
	FileEM file_em;
	CString str_pName;
	CString szPath;

	pi.RemoveAll();

	BOOL fileend;
	CUnzipFile uf(ZipName);
	
	uf.GoToFirstFile();
	
	unz_file_info ui;

	do
	{

	uf.GetCurrentFileInfo(&ui);

	int iNameSize = ui.size_filename + 1;
	char* pName = new char [iNameSize];


	uf.GetCurrentFileInfo(NULL, pName, iNameSize);
    
    str_pName=pName;

    str_pName.Replace ("/","\\");

	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];
 	TCHAR szFileName[_MAX_FNAME];   
	_tsplitpath(pName, szDrive, szDir,szFileName, NULL);

 	szPath = CString(szDrive) + szDir;

	file_em.PathName=str_pName;
	file_em.DirInf=szPath;
	pi.AddTail(file_em);
	fileend=uf.GoToNextFile();

	}
	while(fileend);

	uf.Close();	
	return TRUE;

}

/*
UnZipFileFromZip()  从Zip文件中解出指定文件
FileNo 指定文件在Zip文件中的序号
ZipName Zip文件名
PathName 指定文件要被解压到的目录名
*/

extern "C" BOOL __declspec(dllexport)UnzipFileFromZip(int FileNo,CString ZipName,CString PathName)
{
	CString TmpPathName;
	CString szPath;
	CString szTmpPath;
    CString str_pName;
	int pos;


	BOOL fileend;
	CUnzipFile uf(ZipName);
	
	uf.GoToFirstFile();
	
	unz_file_info ui;
	

	while((fileend)&&((--FileNo)>0))
	{		
		
		fileend=uf.GoToNextFile();
	}
	


	if (fileend)
	{

	uf.GetCurrentFileInfo(&ui);

	int iNameSize = ui.size_filename + 1;
	char* pName = new char [iNameSize];


	uf.GetCurrentFileInfo(NULL, pName, iNameSize);
    
     str_pName=pName;
	if (PathName.Right(1)!="\\")
	      PathName=PathName+"\\";
	if (str_pName.Left(1)=="\\")
		  str_pName.Delete(0,1);

	TmpPathName=PathName+str_pName;

    TmpPathName.Replace ("/","\\");

	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];
 	TCHAR szFileName[_MAX_FNAME];   
	_tsplitpath(TmpPathName, szDrive, szDir,szFileName, NULL);

    if (szFileName[0]!=NULL)
	{


		szPath = CString(szDrive) + szDir;

		pos=szPath.Find("\\",0);	
		do
		{
		szTmpPath=szPath.Left(pos);
	
		if (!PathFileExists(szTmpPath))
		{
			CreateDirectory(szTmpPath,NULL);
		}
		pos=szPath.Find("\\",pos+1);
		}
		while(pos!=-1);
		}
		else
		{
			TmpPathName=PathName+szFileName;
		}

		CFile f(TmpPathName, CFile::modeWrite | CFile::modeCreate);

		delete[] pName;
		uf.OpenCurrentFile();
		char buf[BUF_SIZE];


		int size_read;
		do
		{
			size_read = uf.ReadCurrentFile(buf, BUF_SIZE);
			if (size_read > 0)
			f.Write(buf, size_read);
		}
		while (size_read == BUF_SIZE);
	

		uf.UpdateFileStatus(f, ui);
	
	}

	uf.Close();	
	return TRUE;	

}


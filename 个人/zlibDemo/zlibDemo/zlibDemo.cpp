// zlibDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "zlib.h"
#include <assert.h>
#include <Windows.h>
#include <iostream>

#include <string>
#include <math.h>
#include <fstream>  
#include <sstream>  
#include <iostream>
using std::cout;
using std::endl;
//静态加载如下：
//#pragma comment(lib,"zlib1")
const char* pFileCom="D:\OTHER_SVN\平江万达\trunk\code\NTS-9000\bin\send_data\37000020161229";
const char* pFileSave="D:\OTHER_SVN\平江万达\trunk\code\NTS-9000\bin\send_data\370000201612291.rar";//压缩后的文件
#define MAX_LEN 1024*100//本例只适用于小于100KB的文件
int _tmain(int argc, _TCHAR* argv[])
{
	string sPathSendData = "D:\\OTHER_SVN\\平江万达\\trunk\\code\\NTS-9000\\bin\\send_data\\37000020161229\\";
	int len =WideCharToMultiByte(CP_ACP,0,sPathSendData,-1,NULL,0,NULL,NULL);  
	char *pFileCom =new char[len +1];  
	WideCharToMultiByte(CP_ACP,0,sPathSendData,-1,pFileCom,len,NULL,NULL );

	//FILE* fp = fopen(pFileCom, "rb");
	//BYTE buf[MAX_LEN]={0};
	//int resultLen = fread(buf, 1, MAX_LEN, fp);
	//if(resultLen <= 0) 
	//{
	//	return FALSE;
	//}

	ifstream ifile(pFileCom,std::ifstream::binary);  
	//将文件读入到ostringstream对象buf中  
	ostringstream buf;  
	char ch;  
	while(buf&&ifile.get(ch))  
	{
		buf.put(ch);
	}

	//压缩文件
	FILE* fp1=NULL;
	fp1=fopen(pFileCom,"rb");
	assert(fp1);
	//申请缓冲区
	unsigned char* pBufferRes=new unsigned char[MAX_LEN];
	unsigned char* pBufferDes=new unsigned char[MAX_LEN];
	//置零
	memset(pBufferRes,0,MAX_LEN);
	memset(pBufferDes,0,MAX_LEN);
	//读取整个文件到缓冲区
	fread(pBufferRes,sizeof(unsigned char),MAX_LEN-1,fp1);
	uLongf lSize=strlen((const char*)pBufferRes);
	//动态加载DLL
	HINSTANCE hIns=::LoadLibrary(_T("..\\Debug\\zlib1.dll"));
	uLongf desLen;
	if(hIns)
	{
		typedef int (*fun)(Bytef*,uLongf*,const Bytef*,uLongf);
		fun f=NULL;
		//指向函数地址的指针
		f=(fun)GetProcAddress(hIns,"compress");
		if(f==NULL)
		{
			cout<<"获取模块函数地址失败!"<<endl;
			::FreeLibrary(hIns);
			return 1;
		}
		int nError=f(pBufferDes,&desLen,pBufferRes,lSize);
		//desLen必须大于lsize，详情请参照zlib文档
		if(nError!=Z_OK)
		{
			cout<<"压缩失败!"<<endl;
			::FreeLibrary(hIns);
			fclose(fp1);
			return 1;
		}
		cout<<"压缩成功,压缩率为: %"<<desLen/(float)lSize*100<<endl;
	}
	else
	{
		cout<<"加载库文件失败!"<<endl;
		return 1;
	}
	fclose(fp1);
	FILE* fp2=NULL;
	fp2=fopen(pFileSave,"wb");
	//将压缩后的信息写入文件
	fwrite(pBufferDes,sizeof(unsigned char),desLen,fp2);
	fclose(fp2);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//解压文件
	memset(pBufferDes,0,MAX_LEN);
	memset(pBufferRes,0,MAX_LEN);
	fp1=fopen("..\\Debug\\1.zip","rb");
	desLen=fread(pBufferRes,1,desLen,fp1);
	typedef int (*unFun)(Bytef*,uLongf*,const Bytef*,uLongf);
	unFun uf=NULL;
	uf=(unFun)GetProcAddress(hIns,"uncompress");
	uLongf unSize;
	if(uf)
	{
		int nRet=uf(pBufferDes,&unSize,pBufferRes,desLen);
		::FreeLibrary(hIns);
		if(nRet!=Z_OK)
		{
			cout<<"解压缩失败"<<endl;
			fclose(fp1);
			return 1;
		}
	}
	fclose(fp1);
	fp2=fopen("..\\Debug\\uncompress.xml","wb");
	fwrite(pBufferDes,1,unSize,fp2);
	fclose(fp2);
	cout<<"成功解压文件"<<pFileSave<<"到uncompress.xml"<<endl;
	//释放申请的内存空间
	delete[] pBufferDes;
	pBufferDes=NULL;
	delete[] pBufferRes;
	pBufferRes=NULL;
	return 0;
}

/*zlib中两个主要函数的声明:
int compress OF((Bytef *dest,   uLongf *destLen,const Bytef *source, uLong sourceLen));
int uncompress OF((Bytef *dest,   uLongf *destLen,const Bytef *source, uLong sourceLen));*/
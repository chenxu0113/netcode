// zlibDemo.cpp : �������̨Ӧ�ó������ڵ㡣
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
//��̬�������£�
//#pragma comment(lib,"zlib1")
const char* pFileCom="D:\OTHER_SVN\ƽ�����\trunk\code\NTS-9000\bin\send_data\37000020161229";
const char* pFileSave="D:\OTHER_SVN\ƽ�����\trunk\code\NTS-9000\bin\send_data\370000201612291.rar";//ѹ������ļ�
#define MAX_LEN 1024*100//����ֻ������С��100KB���ļ�
int _tmain(int argc, _TCHAR* argv[])
{
	string sPathSendData = "D:\\OTHER_SVN\\ƽ�����\\trunk\\code\\NTS-9000\\bin\\send_data\\37000020161229\\";
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
	//���ļ����뵽ostringstream����buf��  
	ostringstream buf;  
	char ch;  
	while(buf&&ifile.get(ch))  
	{
		buf.put(ch);
	}

	//ѹ���ļ�
	FILE* fp1=NULL;
	fp1=fopen(pFileCom,"rb");
	assert(fp1);
	//���뻺����
	unsigned char* pBufferRes=new unsigned char[MAX_LEN];
	unsigned char* pBufferDes=new unsigned char[MAX_LEN];
	//����
	memset(pBufferRes,0,MAX_LEN);
	memset(pBufferDes,0,MAX_LEN);
	//��ȡ�����ļ���������
	fread(pBufferRes,sizeof(unsigned char),MAX_LEN-1,fp1);
	uLongf lSize=strlen((const char*)pBufferRes);
	//��̬����DLL
	HINSTANCE hIns=::LoadLibrary(_T("..\\Debug\\zlib1.dll"));
	uLongf desLen;
	if(hIns)
	{
		typedef int (*fun)(Bytef*,uLongf*,const Bytef*,uLongf);
		fun f=NULL;
		//ָ������ַ��ָ��
		f=(fun)GetProcAddress(hIns,"compress");
		if(f==NULL)
		{
			cout<<"��ȡģ�麯����ַʧ��!"<<endl;
			::FreeLibrary(hIns);
			return 1;
		}
		int nError=f(pBufferDes,&desLen,pBufferRes,lSize);
		//desLen�������lsize�����������zlib�ĵ�
		if(nError!=Z_OK)
		{
			cout<<"ѹ��ʧ��!"<<endl;
			::FreeLibrary(hIns);
			fclose(fp1);
			return 1;
		}
		cout<<"ѹ���ɹ�,ѹ����Ϊ: %"<<desLen/(float)lSize*100<<endl;
	}
	else
	{
		cout<<"���ؿ��ļ�ʧ��!"<<endl;
		return 1;
	}
	fclose(fp1);
	FILE* fp2=NULL;
	fp2=fopen(pFileSave,"wb");
	//��ѹ�������Ϣд���ļ�
	fwrite(pBufferDes,sizeof(unsigned char),desLen,fp2);
	fclose(fp2);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//��ѹ�ļ�
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
			cout<<"��ѹ��ʧ��"<<endl;
			fclose(fp1);
			return 1;
		}
	}
	fclose(fp1);
	fp2=fopen("..\\Debug\\uncompress.xml","wb");
	fwrite(pBufferDes,1,unSize,fp2);
	fclose(fp2);
	cout<<"�ɹ���ѹ�ļ�"<<pFileSave<<"��uncompress.xml"<<endl;
	//�ͷ�������ڴ�ռ�
	delete[] pBufferDes;
	pBufferDes=NULL;
	delete[] pBufferRes;
	pBufferRes=NULL;
	return 0;
}

/*zlib��������Ҫ����������:
int compress OF((Bytef *dest,   uLongf *destLen,const Bytef *source, uLong sourceLen));
int uncompress OF((Bytef *dest,   uLongf *destLen,const Bytef *source, uLong sourceLen));*/
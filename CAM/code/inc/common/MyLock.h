/**
*
* �� �� ��	: MyLock.h
* ��������	: 2012��10��3��
* ��    ��  : ½�ι�
* �޸�����	: 2012-11-26
* ��ǰ�汾	: $Revision: 1.0 $
* ��������  : ϵͳ����������Linux/Windows
* �޸ļ�¼  : 2012-11-26 ������UNICODE֧�� ½�ι�
*
**/

#if !defined(AFX_MYLOCK_H_LUZHIGUO_INCLUDED_)
#define AFX_MYLOCK_H_LUZHIGUO_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN32
#include <string.h>
#include <time.h>		/* timespec{} for pselect() */
#include <stdio.h>
#include <winsock2.h> 
#else/* ifdef __linux*/
#include "localmutex.h"
#endif // !_WIN32

#define MUTEX_TIMEOUT 60000	//�ź����ĳ�ʱ������

class MyLock  
{
public:
	int cnt;

	MyLock()
	{
#ifdef _WIN32
		cnt=0;	// Windowsƽ̨������ʶ��cnt=1ʱ�����ɹ�
		/*
		m_hMutex = NULL;
		memset(m_cName,0,sizeof(m_cName));
		sprintf(m_cName,"MUTEX_%ld_%d",(long)this,rand()%10000);
		*/
#endif // !_WIN32
	}

	virtual ~MyLock()
	{
#ifdef _WIN32
		/*
		if(m_hMutex != NULL)
		{
			try
			{
				ReleaseMutex(m_hMutex);
				CloseHandle(m_hMutex);
			}
			catch(...)
			{
			}
		}
		*/
#endif // !_WIN32
	}

	// �������������ʧ�ܽ���������ֱ����ʱ
	void lock()
	{
#ifdef _WIN32
		int i=0;
		while(1)
		{
			if(cnt<=0)
			{
				cnt = 1;
				break;
			}
			Sleep(1);
			continue;
			if(i++>600)
			{
				cnt = 1;
#ifdef UNICODE
				wprintf(L"@@@@@@@@@@@@@@@@ ERROR: Wait for mutex timeout!!!! @@@@@@@@@@@@@\n");
#else
				printf("@@@@@@@@@@@@@@@@ ERROR: Wait for mutex timeout!!!! @@@@@@@@@@@@@\n");
#endif // !UNICODE
				break;
			}
		}
		/*
		if(strlen(m_cName) == 0)
			return;//δʵ��
		int i=0;
		while(1)
		{
			HANDLE h = CreateMutex(NULL, FALSE, m_cName);
			if (GetLastError() == ERROR_ALREADY_EXISTS) 
			{
				Sleep(10);
				if(i++>600)
				{
					printf("@@@@@@@@@@@@@@@@ ERROR: Wait for mutex timeout!!!! @@@@@@@@@@@@@\n");
					break;
				}
				continue;
			}
			cnt++;
			m_hMutex = h;
			break;
		}
		*/
#else
		m_Mutex.lock();
#endif // !_WIN32
	}

	// ���Լ������������ʧ�����ڼ���
	bool trylock()
	{
#ifdef _WIN32
		if(cnt==0)
		{
			cnt ++;
			return true;
		}
		else
			return false;
		/*
		HANDLE h = CreateMutex(NULL, FALSE, m_cName);
		if (GetLastError() == ERROR_ALREADY_EXISTS) 
		{
			return false;
		}
		cnt++;
		m_hMutex = h;
		return true;
		*/
#else
		return m_Mutex.trylock();
#endif // !_WIN32
	}

	//������
	void unlock()
	{
#ifdef _WIN32
		cnt=0;
		/*
		HANDLE h = m_hMutex;
		m_hMutex = NULL;
		ReleaseMutex(h);
		CloseHandle(h);
		*/
#else
		m_Mutex.unlock();
#endif // !_WIN32
	}

private:

#ifdef _WIN32 
	HANDLE m_hMutex;//������
#ifdef UNICODE
	wchar_t m_cName[50];//����������
#else
	char m_cName[50];//����������
#endif // !UNICODE
#else
	CLocalMutex m_Mutex;
#endif // !_WIN32
};

#endif // !defined(AFX_MYLOCK_H_LUZHIGUO_INCLUDED_)

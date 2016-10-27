/**
*
* �� �� ��	: localmutex.h
* ��������	: 2012-10-3
* ��    ��  : ½�ι�
* �޸�����	: 2013-4-2
* ��ǰ�汾	: $Revision: 1.0 $
* ��������  : �Զ����linux��ר�õĻ������࣬�������Posix�źŵƻ���,�ڴ��źŵ�
* �޸ļ�¼  : 
*
**/
#ifndef LOCALMUTEX_H
#define LOCALMUTEX_H

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>

class CLocalMutex 
{
public:
	CLocalMutex(int isLocal=0,unsigned int value=1)
	{
		Open_Semaphore(isLocal,value);
	}

	CLocalMutex(bool isManul)
	{
		if(!isManul)
		{
			Open_Semaphore();
		}
	}

	~CLocalMutex()
	{
		Destroy_Semaphore();
	}

public:
	sem_t	m_sem;
public:
	bool Open_Semaphore(int isLocal=0,unsigned int value=0)
	{
		int rc=sem_init(&m_sem,isLocal,value);
		if(rc<0)
		{
			printf("sem_init error! ret=%d \n",rc);
			return false;
		}
		return true;
	}

	bool Destroy_Semaphore()
	{
		int rc=sem_destroy(&m_sem);
		return (!(rc<0));
	}

	bool lock()
	{
		int rc = sem_wait(&m_sem);
		if(rc != 0)
		{
			printf(" sem_wait! ret=%d, errno=%d \n",rc,errno);
			abort();
		}
		return true;
	}

	bool trylock()
	{
		int rc=sem_trywait(&m_sem);
		return rc==0;
	}

	bool unlock()
	{
		sem_post(&m_sem);
		return true;
	}
	int get_value()
	{
		int i=0;

		int rc = sem_getvalue(&m_sem,&i);
		if(rc != 0)
		{
			printf("sem_getvalue error! ret=%d \n",rc);
			return -1;
		}
		return i;
	}
};

class CLMutexLocker 
{
public:
	CLMutexLocker(CLocalMutex *LocalMutex,bool bWait=false)
	{
		m_LocalMutex=LocalMutex;
		if(!bWait)
			m_LocalMutex->trylock();
		else
			m_LocalMutex->lock();
	}

	~CLMutexLocker()
	{
		m_LocalMutex->unlock();
	}

public:
	CLocalMutex* GetMutex()
	{
		return m_LocalMutex;
	}

private:
	CLocalMutex* m_LocalMutex;
};

#endif

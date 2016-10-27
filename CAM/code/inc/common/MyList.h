/*****************************************************************************
* MYLIST.H    �ļ�ע��
* @�ļ�     : MyList.h
* @ԭʼ·�� : E:\NTS-9000\5.Coding\inc\common
* @����     : Acidy
* @�������� : 2013/3/13 13:42
* @�汾     : V 1.0
* @˵��     : �Զ���list�ࣺ
*			  1��MyPtrListָ������ģ���࣬���ָ��������Զ��壬֧�����ͷ��ڴ�
*			  �ռ䣬���ж���������ͷ�����ڵ㡢����β�����ڵ㡢ָ���ڵ�ǰ����
*			  ��ڵ㡢ɾ��ָ���ڵ㡢��սڵ㡢��ѯ�ڵ�λ�õȽӿ�
*			  2��SetShared�ӿ��ṩ�˹������ã�������ʱ���̹߳���ʱ����ͨ���ý�
*			  �ڽ��м���
*			  3��SetAutoDelete�ṩ�˿����͹ر��Զ��ͷ��ڴ�ռ������
*			  4��ʹ����ֵ��ע����ǣ���ͬһ�������ĵ�ַ��Ϊ��������������ڵ�
*			  ��Ԫ��ʱ����ʱ�����Զ��ͷ��ڴ�Ļ����������ظ��ͷ�ͬһ���������
*			  �쳣�����Ҫ����ʹ���Զ������ڴ����
*****************************************************************************/

#ifndef __INCLUDE_MYLIST_H_LUZG_ACIDY_20130314__
#define __INCLUDE_MYLIST_H_LUZG_ACIDY_20130314__

#include <list>

#include "MyLock.h"

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif //!__cplusplus
#endif //!NULL

// ���б��������ָ�룬type��ָ��ָ����ָ���������
template <class type>
class MyPtrList
{
public:
#pragma pack(4)
	struct NODE
	{
		NODE()
		{
			pData = NULL;
			pNext = NULL;
			pPrev = NULL;
		};

		type *pData;	// �洢������
		NODE *pNext;	// ��һ���ڵ�
		NODE *pPrev;	// ǰһ���ڵ�
	};
#pragma pack()

	NODE *m_pTail;	// β�ڵ�
	NODE *m_pHead;	// ͷ�ڵ�

private:
	int m_iSize;
	bool m_bAutoDel;
	MyLock *m_pLock;

public:
	MyPtrList()
	{
		m_pTail = NULL;
		m_pHead = NULL;
		m_iSize = 0;
		m_bAutoDel = false;
		m_pLock = NULL;
	};

	~MyPtrList()
	{
		Clear();
		if(m_pLock != NULL)
		{
			delete m_pLock;
			m_pLock = NULL;
		}
	};

	void SetAutoDelete(bool b)
	{
		m_bAutoDel = b;
	};
	bool IsShared() {return m_pLock != NULL;};
	void SetShared(bool b) 
	{
		if(b)
		{
			if(m_pLock == NULL)
				m_pLock = new MyLock();
		}
		else
		{
			if(m_pLock != NULL)
			{
				delete m_pLock;
				m_pLock = NULL;
			}
		}
	};

private:
	NODE *NewNode(NODE *pPrev, NODE *pNext)
	{
		NODE *pNewNode = new NODE;
		if (pNewNode == NULL)
		{
			return NULL;
		}

		pNewNode->pPrev = pPrev;
		pNewNode->pNext = pNext;
		return pNewNode;
	}

public:
	// ��ȡ������
	int GetCount()
	{
		return m_iSize;
	}

	// �ж������Ƿ�Ϊ��
	bool IsEmpty()
	{
		return (m_iSize == 0);
	}

	// �����ڵ㣨Ĭ�ϴ�β�����ӣ�
	bool Add(type *pData)
	{
		return AddTail(pData);
	}

	// ������β�����ڵ�
	bool AddTail(type *pData)
	{
		lock();

		NODE *p = m_pTail;
		NODE *pNew = NewNode(m_pTail,NULL); // �½��ڵ�
		if (pNew == NULL)
		{
			unlock();
			return false;
		}
		pNew->pData = pData;

		if(p == NULL) // ��ǰ����Ϊ��ʱ���½�һ��
		{
			if (m_pHead == NULL)
			{
				m_pHead = m_pTail = pNew;
			}
			else // ��β������
			{
				p = m_pHead;
				while(p->pNext)
					p = p->pNext;
				pNew->pPrev = p;
				p->pNext = pNew;
				m_pTail = pNew;
			}
		}
		else 
		{
			// ���ٲ��뵽β�ڵ�
			p->pNext = pNew;
			m_pTail = pNew;
		}

		m_iSize ++;
		unlock();
		return true;
	};

	// ������ͷ�����ڵ�
	bool AddHead(type *pData)
	{
		lock();

		NODE *p = m_pHead;
		NODE *pNew = NewNode(NULL,m_pHead); // �½��ڵ�
		if (pNew == NULL)
		{
			unlock();
			return false;
		}
		pNew->pData = pData;

		if (p == NULL)
		{
			if (m_pTail == NULL)
			{
				m_pHead = m_pTail = pNew;
			} 
			else
			{
				p = m_pTail;
				while(p->pPrev)
					p = p->pPrev;
				pNew->pNext = p;
				p->pPrev = pNew;
				m_pHead = pNew;
			}
		}
		else 
		{
			p->pPrev = pNew;
			m_pHead = pNew;
		}

		m_iSize ++;
		unlock();
		return true;
	};

	/***********************************************************************
	* @����	: MyPtrList<type>::InsertBefore
	* @����	: public 
	* @�޶�	:
	* @����	: type * pNode ���ο��ڵ㣩
	* @����	: type * pData �������ڵ㣩
	* @����	: bool ��true����ɹ���false����ʧ�ܣ�
	* @����	: Acidy
	* @����	: 2013/3/13 16:53
	* @˵��	: ָ���ڵ�ǰ�����½ڵ�
	***********************************************************************/
	bool InsertBefore(type *pNode, type *pData)
	{
		lock();

		NODE *p = m_pHead;
		NODE *prev = NULL;
		NODE *pNew = NewNode(NULL,NULL); // �½��ڵ�
		if (pNew == NULL)
		{
			unlock();
			return false;
		}
		pNew->pData = pData;

		if (p != NULL)
		{
			do 
			{
				prev = p->pPrev;
				if (p->pData == pNode)
				{
					pNew->pNext = p;
					pNew->pPrev = p->pPrev;
					if (prev == NULL)
						m_pHead = pNew;
					else
						p->pPrev->pNext = pNew;
					p->pPrev = pNew;

					m_iSize++;
					unlock();
					return true;
				}
				p = p->pNext;
			} while (p != NULL);
		}
		else if ((p == NULL) && (m_pTail != NULL))
		{
			p = m_pTail;
			do 
			{
				prev = p->pPrev;
				if (p->pData == pNode)
				{
					pNew->pNext = p;
					pNew->pPrev = p->pPrev;
					if (prev == NULL)
						m_pHead = pNew;
					else
						p->pPrev->pNext = pNew;
					p->pPrev = pNew;

					m_iSize++;
					unlock();
					return true;
				}
				p = p->pPrev;
			} while (p != NULL);

		}

		unlock();
		return false; // �Ҳ����ο��ڵ㣬�����ʧ��
	};

	/***********************************************************************
	* @����	: MyPtrList<type>::InsertAfter
	* @����	: public 
	* @�޶�	:
	* @����	: type * pNode ���ο��ڵ㣩
	* @����	: type * pData �������ڵ㣩
	* @����	: bool ��true�����ɹ���false�����ʧ�ܣ�
	* @����	: Acidy
	* @����	: 2013/3/13 18:26
	* @˵��	: ��ָ���ڵ�֮������½ڵ�
	***********************************************************************/
	bool InsertAfter(type *pNode, type *pData)
	{
		lock();

		NODE *p=m_pHead;
		NODE *pNew = NewNode(NULL,NULL); // �½��ڵ�
		if (pNew == NULL)
		{
			unlock();
			return false;
		}
		pNew->pData = pData;

		if (p != NULL)
		{
			do 
			{
				if (p->pData == pNode)
				{
					pNew->pNext = p->pNext;
					pNew->pPrev = p;
					if (p->pNext == NULL)
						m_pTail = pNew;
					else
						p->pNext->pPrev = pNew;
					p->pNext = pNew;

					m_iSize++;
					unlock();
					return true;
				}
				p = p->pNext;
			} while (p != NULL);
		}
		else if ((p == NULL) && (m_pTail != NULL))
		{
			p = m_pTail;
			do 
			{
				if (p->pData == pNode)
				{
					pNew->pNext = p->pNext;
					pNew->pPrev = p;
					if (p->pNext == NULL)
						m_pTail = pNew;
					else
						p->pNext->pPrev = pNew;
					p->pNext = pNew;

					m_iSize++;
					unlock();
					return true;
				}
				p = p->pPrev;
			} while (p != NULL);
		}

		unlock();
		return false;
	};

	/***********************************************************************
	* @����	: MyPtrList<type>::At
	* @����	: public 
	* @�޶�	:
	* @����	: int i ��λ�ã�
	* @����	: type * �����ص�Ԫ�أ�
	* @����	: Acidy
	* @����	: 2013/3/14 14:17
	* @˵��	: ��ȡ˳������ָ��λ�õ�Ԫ��
	***********************************************************************/
	type * At(int i)
	{
		lock();
		NODE *p=m_pHead;
		int cnt=0;
		while(p != NULL)
		{
			if(i == cnt)
			{
				unlock();
				return p->pData;
			}
			p = p->pNext;
			cnt ++;
		}
		unlock();
		return NULL;
	}

	// ������[]��������֧���������鷽ʽȡֵ
	type * operator[](int i)
	{
		return At(i);
	}

	/***********************************************************************
	* @����	: MyPtrList<type>::GetTail
	* @����	: public 
	* @�޶�	:
	* @����	: type* ������ָ��Ԫ�أ�
	* @����	: Acidy
	* @����	: 2013/3/14 14:24
	* @˵��	: ��ȡβ�ڵ�Ԫ��
	***********************************************************************/
	type* GetTail()
	{
		NODE *p = m_pTail;
		if(m_pTail == NULL)
		{
			//return NULL;
			if (m_pHead == NULL)
				return NULL;
			p = m_pHead;
			while(p->pNext != NULL)
				p = p->pNext;
			return p->pData;
		}
		return m_pTail->pData;
	}

	/***********************************************************************
	* @����	: MyPtrList<type>::GetHead
	* @����	: public 
	* @�޶�	:
	* @����	: type* ������ָ��Ԫ�أ�
	* @����	: Acidy
	* @����	: 2013/3/14 14:24
	* @˵��	: ��ȡͷ�ڵ�Ԫ��
	***********************************************************************/
	type* GetHead()
	{
		NODE *p = m_pHead;
		if(m_pHead == NULL)
		{
			//return NULL;
			if (m_pTail == NULL)
				return NULL;
			p = m_pTail;
			while(p->pPrev != NULL)
				p = p->pPrev;
			return p->pData;
		}
		return m_pHead->pData;
	}

	/***********************************************************************
	* @����	: MyPtrList<type>::Find
	* @����	: public 
	* @�޶�	:
	* @����	: type * pData ����
	* @����	: int �����ظ�Ԫ���������е�һ�γ��ֵ�λ�ã�
	* @����	: Acidy
	* @����	: 2013/3/14 15:58
	* @˵��	: ����Ԫ��
	***********************************************************************/
	int Find(type *pData)
	{
		lock();
		NODE *p=m_pHead;
		int cnt=0;
		while(p != NULL)
		{
			if(p->pData == pData)
			{
				unlock();
				return cnt;
			}
			p = p->pNext;
			cnt ++;
		}
		unlock();
		return -1;
	}

	/***********************************************************************
	* @����	: MyPtrList<type>::Replace
	* @����	: public 
	* @�޶�	:
	* @����	: int i ����Ҫ�滻�Ľڵ�λ�ã�
	* @����	: type * pData ����ֵ��
	* @����	: bool ��true��ɹ��滻��false��ʧ�ܣ�
	* @����	: Acidy
	* @����	: 2013/3/14 15:52
	* @˵��	: �滻ָ���ڵ�λ�õ�Ԫ��ֵ
	***********************************************************************/
	bool Replace(int i, type *pData)
	{
		lock();
		NODE *p=m_pHead;
		int cnt=0;
		while(p != NULL)
		{
			if(i == cnt)
			{
				p->pData = pData;
				unlock();
				return true;
			}
			p = p->pNext;
			cnt ++;
		}
		unlock();
		return false;
	}

	/***********************************************************************
	* @����	: MyPtrList<type>::Remove
	* @����	: public 
	* @�޶�	:
	* @����	: type * pData ����Ҫɾ����Ԫ�أ�
	* @����	: bool ����
	* @����	: Acidy
	* @����	: 2013/3/14 14:26
	* @˵��	: ɾ��Ԫ�ص���ĳֵ�����нڵ�
	***********************************************************************/
	bool Remove(type *pData)
	{
		lock();
		NODE *p=m_pHead;
		while(p != NULL)
		{
			if(p->pData == pData)
			{
				if (p != m_pHead)
					p->pPrev->pNext = p->pNext;
				else
					m_pHead = p->pNext;
				if (p != m_pTail)
					p->pNext->pPrev = p->pPrev;
				else
					m_pTail = p->pPrev;

				NODE *pOld = p->pNext;
				m_iSize --;
				if(m_bAutoDel)
					delete p->pData;
				delete p;
				p = pOld;
				continue;
			}
			p = p->pNext;
		}
		unlock();
		return true;
	}

	/***********************************************************************
	* @����	: MyPtrList<type>::Remove
	* @����	: public 
	* @�޶�	:
	* @����	: int i ����Ҫɾ���Ľڵ�λ�ã�
	* @����	: bool ����
	* @����	: Acidy
	* @����	: 2013/3/14 15:48
	* @˵��	: ɾ��ָ��λ�õ�ĳһ�ڵ�
	***********************************************************************/
	bool Remove(int i)
	{
		lock();
		NODE *p=m_pHead;
		int cnt=0;
		while(p != NULL)
		{
			if(i == cnt)
			{
				if (p != m_pHead)
					p->pPrev->pNext = p->pNext;
				else
					m_pHead = p->pNext;
				if (p != m_pTail)
					p->pNext->pPrev = p->pPrev;
				else
					m_pTail = p->pPrev;

				m_iSize --;
				if(m_bAutoDel)
					delete p->pData;
				delete p;
				p = NULL;
				unlock();
				return true;
			}
			p = p->pNext;
			cnt ++;
		}
		unlock();
		return false;
	}

	/***********************************************************************
	* @����	: MyPtrList<type>::RemoveAll
	* @����	: public 
	* @�޶�	:
	* @����	: bool ����
	* @����	: Acidy
	* @����	: 2013/3/14 15:48
	* @˵��	: ɾ�����нڵ�
	***********************************************************************/
	bool RemoveAll()
	{
		while(GetCount()>0)
			Remove(0);
		return true;
	}

	/***********************************************************************
	* @����	: MyPtrList<type>::Clear
	* @����	: public 
	* @�޶�	:
	* @����	: bool ����
	* @����	: Acidy
	* @����	: 2013/3/14 16:01
	* @˵��	: �������
	***********************************************************************/
	bool Clear()
	{
		if (m_iSize <= 0)
			return false; // �Ѿ�Ϊ��
		lock();
		NODE *next;
		NODE *p=m_pHead;
		while(p != NULL)
		{
			next = p->pNext;
			if(m_bAutoDel)
				delete p->pData;
			delete p;
			p = next;
		}
		m_pHead = m_pTail = NULL;
		m_iSize  = 0;
		unlock();
		return true;
	}

	// ��ֵ����ֻ�ǽ�Ԫ�ظ�ֵ����������������������Ԫ��
	// ʹ�õ���ͬһƬ�ڴ棬����ʹ��SetAutoDelete(true)������
	// ��ֵ��SetAutoDelete(true)����ֵ��SetAutoDelete(false)
	MyPtrList &operator=(MyPtrList &obj) {
		if (this != &obj && this != NULL)
		{
			Clear();
			int cnt = obj.GetCount();
			for (int i=0;i<cnt;i++)
			{
				Add(obj.At(i));
			}
		}
		return *this;
	};

protected:
	// ����
	inline void lock()
	{
		if(m_pLock != NULL)
			m_pLock->lock();
	}

	// ����
	inline void unlock()
	{
		if(m_pLock != NULL)
			m_pLock->unlock();
	}

};

#endif //!__INCLUDE_MYLIST_H_LUZG_ACIDY_20130314__
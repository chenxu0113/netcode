/*****************************************************************************
* MYLIST.H    文件注释
* @文件     : MyList.h
* @原始路径 : E:\NTS-9000\5.Coding\inc\common
* @作者     : Acidy
* @创建日期 : 2013/3/13 13:42
* @版本     : V 1.0
* @说明     : 自定义list类：
*			  1、MyPtrList指针链表模板类，存放指针的类型自定义，支持自释放内存
*			  空间，其中定义了链表头新增节点、链表尾新增节点、指定节点前或后插
*			  入节点、删除指定节点、清空节点、查询节点位置等接口
*			  2、SetShared接口提供了共享设置，当链表时多线程共享时可以通过该接
*			  口进行加锁
*			  3、SetAutoDelete提供了开启和关闭自动释放内存空间的设置
*			  4、使用中值得注意的是，当同一个变量的地址作为两个链表或两个节点
*			  的元素时，此时设置自动释放内存的话，将导致重复释放同一对象而发生
*			  异常，因此要谨慎使用自动回收内存机制
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

// 该列表用来存放指针，type是指该指针所指对象的类型
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

		type *pData;	// 存储的数据
		NODE *pNext;	// 后一个节点
		NODE *pPrev;	// 前一个节点
	};
#pragma pack()

	NODE *m_pTail;	// 尾节点
	NODE *m_pHead;	// 头节点

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
	// 获取链表长度
	int GetCount()
	{
		return m_iSize;
	}

	// 判断链表是否为空
	bool IsEmpty()
	{
		return (m_iSize == 0);
	}

	// 新增节点（默认从尾部增加）
	bool Add(type *pData)
	{
		return AddTail(pData);
	}

	// 在链表尾新增节点
	bool AddTail(type *pData)
	{
		lock();

		NODE *p = m_pTail;
		NODE *pNew = NewNode(m_pTail,NULL); // 新建节点
		if (pNew == NULL)
		{
			unlock();
			return false;
		}
		pNew->pData = pData;

		if(p == NULL) // 当前链表为空时，新建一个
		{
			if (m_pHead == NULL)
			{
				m_pHead = m_pTail = pNew;
			}
			else // 烂尾链表处理
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
			// 快速插入到尾节点
			p->pNext = pNew;
			m_pTail = pNew;
		}

		m_iSize ++;
		unlock();
		return true;
	};

	// 在链表头新增节点
	bool AddHead(type *pData)
	{
		lock();

		NODE *p = m_pHead;
		NODE *pNew = NewNode(NULL,m_pHead); // 新建节点
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
	* @函数	: MyPtrList<type>::InsertBefore
	* @访问	: public 
	* @限定	:
	* @参数	: type * pNode （参考节点）
	* @参数	: type * pData （新增节点）
	* @返回	: bool （true插入成功，false插入失败）
	* @作者	: Acidy
	* @日期	: 2013/3/13 16:53
	* @说明	: 指定节点前插入新节点
	***********************************************************************/
	bool InsertBefore(type *pNode, type *pData)
	{
		lock();

		NODE *p = m_pHead;
		NODE *prev = NULL;
		NODE *pNew = NewNode(NULL,NULL); // 新建节点
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
		return false; // 找不到参考节点，则插入失败
	};

	/***********************************************************************
	* @函数	: MyPtrList<type>::InsertAfter
	* @访问	: public 
	* @限定	:
	* @参数	: type * pNode （参考节点）
	* @参数	: type * pData （新增节点）
	* @返回	: bool （true则插入成功，false则插入失败）
	* @作者	: Acidy
	* @日期	: 2013/3/13 18:26
	* @说明	: 在指定节点之后插入新节点
	***********************************************************************/
	bool InsertAfter(type *pNode, type *pData)
	{
		lock();

		NODE *p=m_pHead;
		NODE *pNew = NewNode(NULL,NULL); // 新建节点
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
	* @函数	: MyPtrList<type>::At
	* @访问	: public 
	* @限定	:
	* @参数	: int i （位置）
	* @返回	: type * （返回的元素）
	* @作者	: Acidy
	* @日期	: 2013/3/14 14:17
	* @说明	: 读取顺序链表指定位置的元素
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

	// 重载了[]操作符，支持类似数组方式取值
	type * operator[](int i)
	{
		return At(i);
	}

	/***********************************************************************
	* @函数	: MyPtrList<type>::GetTail
	* @访问	: public 
	* @限定	:
	* @返回	: type* （返回指针元素）
	* @作者	: Acidy
	* @日期	: 2013/3/14 14:24
	* @说明	: 获取尾节点元素
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
	* @函数	: MyPtrList<type>::GetHead
	* @访问	: public 
	* @限定	:
	* @返回	: type* （返回指针元素）
	* @作者	: Acidy
	* @日期	: 2013/3/14 14:24
	* @说明	: 获取头节点元素
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
	* @函数	: MyPtrList<type>::Find
	* @访问	: public 
	* @限定	:
	* @参数	: type * pData （）
	* @返回	: int （返回该元素在链表中第一次出现的位置）
	* @作者	: Acidy
	* @日期	: 2013/3/14 15:58
	* @说明	: 查找元素
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
	* @函数	: MyPtrList<type>::Replace
	* @访问	: public 
	* @限定	:
	* @参数	: int i （将要替换的节点位置）
	* @参数	: type * pData （新值）
	* @返回	: bool （true则成功替换，false则失败）
	* @作者	: Acidy
	* @日期	: 2013/3/14 15:52
	* @说明	: 替换指定节点位置的元素值
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
	* @函数	: MyPtrList<type>::Remove
	* @访问	: public 
	* @限定	:
	* @参数	: type * pData （将要删除的元素）
	* @返回	: bool （）
	* @作者	: Acidy
	* @日期	: 2013/3/14 14:26
	* @说明	: 删除元素等于某值的所有节点
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
	* @函数	: MyPtrList<type>::Remove
	* @访问	: public 
	* @限定	:
	* @参数	: int i （将要删除的节点位置）
	* @返回	: bool （）
	* @作者	: Acidy
	* @日期	: 2013/3/14 15:48
	* @说明	: 删除指定位置的某一节点
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
	* @函数	: MyPtrList<type>::RemoveAll
	* @访问	: public 
	* @限定	:
	* @返回	: bool （）
	* @作者	: Acidy
	* @日期	: 2013/3/14 15:48
	* @说明	: 删除所有节点
	***********************************************************************/
	bool RemoveAll()
	{
		while(GetCount()>0)
			Remove(0);
		return true;
	}

	/***********************************************************************
	* @函数	: MyPtrList<type>::Clear
	* @访问	: public 
	* @限定	:
	* @返回	: bool （）
	* @作者	: Acidy
	* @日期	: 2013/3/14 16:01
	* @说明	: 清空链表
	***********************************************************************/
	bool Clear()
	{
		if (m_iSize <= 0)
			return false; // 已经为空
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

	// 赋值运算只是将元素赋值给了新链表，因此两个链表的元素
	// 使用的是同一片内存，谨慎使用SetAutoDelete(true)，建议
	// 左值设SetAutoDelete(true)，右值设SetAutoDelete(false)
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
	// 加锁
	inline void lock()
	{
		if(m_pLock != NULL)
			m_pLock->lock();
	}

	// 解锁
	inline void unlock()
	{
		if(m_pLock != NULL)
			m_pLock->unlock();
	}

};

#endif //!__INCLUDE_MYLIST_H_LUZG_ACIDY_20130314__
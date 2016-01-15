/*********************************************************************
 * ��Ȩ���� (C)2014
 *
 * �ļ����ƣ� 
 * �ļ���ʶ�� 
 * ����˵���� ͨ��ͷ�ļ�
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� 
 * ������ڣ� 
 *
 * �޸ļ�¼1��
 *    �޸����ڣ�2014��1��16��
 *    �� �� �ţ�V1.0
 *    �� �� �ˣ�New
 *    �޸����ݣ�����
 **********************************************************************/
#ifndef _COMM_H_
#define _COMM_H_

#include "pub.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#define   MAX_TASK_NUM    ((WORD32)4)    /* ֧����󲢷�����������������ͨѶ���� */ 

class  CCommLink
{
public:    
    INT                    dwacfd;      /* ͨ��fd                   */
    WORD32                 dwAddr;      /* �÷���˵�ַ             */    
    WORD32                 dwUser;      /* �û���ʶ��������չ       */    
    WORD32                 dwPad;       /* ��չ�ֶ�                 */ 
    WORD32                 dwRcvOff;    /* ����ƫ��                 */ 
    WORD32                 dwxid;       /* ���Ӵ�����             */
    WORD32                 dwtype;      /* �������� */    
    class  CCommLink       *ptNext;     /* ָ����һ������    */
    WORD32 SetRcvSize(WORD32 size);
    WORD32 receive();
    WORD32 send2(BYTE *ptbuf, WORD32 dwlen);
    TaskEntryProto         pUsercall;   /* �û��ص�          */
    WORD32 SetUsercall(TaskEntryProto pcall);
    CCommLink(WORD32 type,WORD32 fd,WORD32 addr);
    ~CCommLink();    
private:
    BYTE                   *pucRcvBuf;  /* �û����ջ���  */
    WORD32                 rcvlen;
    BYTE                   *pucSndBuf;  /* �û����ͻ���  */
    WORD32                 sndlen;
};


class  CCommCtl
{
public:
    WORD32                 dwserv;      /* �÷�������                     */
    WORD32                 nodeid;      /* ȫ�ֽڵ��ʶ�����ּ�Ⱥ�ڽڵ�   */
    WORD32                 dwUser;      /* �����û���ʶ�����ֱ��ض����   */
    pthread_t              serv_th;     /* ͨ�ŷ�����߳�                 */
    pthread_t              cli_th;      /* ͨ�ſͻ����߳�                 */
    INT                    dwservfd;    /* �����fd         */
    int                    dwclientfd;  /* �ͻ���fd         */
    class  CCommLink       *ptLinkItem; /* ����˽���fd         */
    fd_set                 tRcvSock;    /* �����׽��ּ���   */
    WORD32                 dwflags;     /* ��ʶ                       */
    WORD32                 dwNum;       /* ����˽����������� */
    WORD32                 dwType;      /* ��������                       */    
    pthread_mutex_t        mutex;       /* ͨѶҪ������ܣ���������滻�������� */
    WORD32   AppendLink(WORD32 fd, WORD32 addr);
    WORD32   CloseLink(WORD32 fd);
    WORD32   ClientInit(WORD32 dwIp, WORD32 dwPort);
    WORD32   ServInit(WORD32 dwIp, WORD32 dwPort);
    WORD32   LinkSend(WORD32 addr, BYTE *ptbuf, WORD32 dwlen);
    CCommCtl();
    ~CCommCtl();

private:

};
WORD32  getipaddr();
#ifdef __cplusplus
}
#endif
#endif

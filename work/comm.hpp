/*********************************************************************
 * 版权所有 (C)2014
 *
 * 文件名称： 
 * 文件标识： 
 * 其它说明： 通信头文件
 * 当前版本： V1.0
 * 作    者： 
 * 完成日期： 
 *
 * 修改记录1：
 *    修改日期：2014年1月16日
 *    版 本 号：V1.0
 *    修 改 人：New
 *    修改内容：创建
 **********************************************************************/
#ifndef _COMM_H_
#define _COMM_H_

#include "pub.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#define   MAX_TASK_NUM    ((WORD32)4)    /* 支持最大并发的任务数，不包括通讯任务 */ 

class  CCommLink
{
public:    
    INT                    dwacfd;      /* 通信fd                   */
    WORD32                 dwAddr;      /* 该服务端地址             */    
    WORD32                 dwUser;      /* 用户标识，后续扩展       */    
    WORD32                 dwPad;       /* 扩展字段                 */ 
    WORD32                 dwRcvOff;    /* 接收偏移                 */ 
    WORD32                 dwxid;       /* 连接传输编号             */
    WORD32                 dwtype;      /* 处理类型 */    
    class  CCommLink       *ptNext;     /* 指向下一个连接    */
    WORD32 SetRcvSize(WORD32 size);
    WORD32 receive();
    WORD32 send2(BYTE *ptbuf, WORD32 dwlen);
    TaskEntryProto         pUsercall;   /* 用户回调          */
    WORD32 SetUsercall(TaskEntryProto pcall);
    CCommLink(WORD32 type,WORD32 fd,WORD32 addr);
    ~CCommLink();    
private:
    BYTE                   *pucRcvBuf;  /* 用户接收缓存  */
    WORD32                 rcvlen;
    BYTE                   *pucSndBuf;  /* 用户发送缓存  */
    WORD32                 sndlen;
};


class  CCommCtl
{
public:
    WORD32                 dwserv;      /* 该服务类型                     */
    WORD32                 nodeid;      /* 全局节点标识，区分集群内节点   */
    WORD32                 dwUser;      /* 本地用户标识，区分本地多进程   */
    pthread_t              serv_th;     /* 通信服务端线程                 */
    pthread_t              cli_th;      /* 通信客户端线程                 */
    INT                    dwservfd;    /* 服务端fd         */
    int                    dwclientfd;  /* 客户端fd         */
    class  CCommLink       *ptLinkItem; /* 服务端接纳fd         */
    fd_set                 tRcvSock;    /* 接收套接字集合   */
    WORD32                 dwflags;     /* 标识                       */
    WORD32                 dwNum;       /* 服务端接纳连接数量 */
    WORD32                 dwType;      /* 进程类型                       */    
    pthread_mutex_t        mutex;       /* 通讯要求高性能，后面可能替换成其它的 */
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

/*********************************************************************
 * 版权所有 (C)2013
 *
 * 文件名称： 
 * 文件标识： 
 * 其它说明： 通信组件
 * 当前版本： V1.0
 * 作    者： 
 * 完成日期： 
 *
 * 修改记录1：
 *    修改日期：2013年11月13日
 *    版 本 号：V1.0
 *    修 改 人：New
 *    修改内容：创建
 **********************************************************************/
#include "comm.hpp"

const WORD32  g_max_cli=((WORD32)100);
const WORD32  g_max_rcv=((WORD32)16*4096);
const WORD32  g_max_snd=((WORD32)8*4096);


#define ETH_HW_ADDR_LEN  ((WORD32)6)



char           ethname[]      = "eth0";        /* proxy ethinet name */
TaskEntryProto   g_ptcallback   = NULL;
int oss_setsockaddr(struct sockaddr_in  *sin, unsigned long addr, unsigned short port)
{
    sin->sin_family        = AF_INET;
    sin->sin_addr.s_addr   = htonl(addr);
    sin->sin_port          = htons(port);
    return 0;
}

WORD32  getipaddr()
{
    struct ifreq   ifr;
    int            fd;
    int            err;
    unsigned long  ip;
    
    strncpy(ifr.ifr_name, ethname,sizeof(ethname));
    fd   = socket(AF_INET,SOCK_DGRAM,0);
    err  = ioctl(fd,SIOCGIFADDR,&ifr);
    if(err)
    {
        R_Printf("ioctl error! err %d!\n",errno);
        close(fd);
        return -1;
    }
    memcpy(&ip,ifr.ifr_addr.sa_data + 2,sizeof(ip)); 
    R_Printf("ip = 0x%x!\n",ntohl(ip));
    err = ioctl(fd,SIOCGIFHWADDR,&ifr);
    if (err != -1)
    {
        R_Printf("mac 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x!\n",ifr.ifr_hwaddr.sa_data[0],ifr.ifr_hwaddr.sa_data[1],ifr.ifr_hwaddr.sa_data[2],ifr.ifr_hwaddr.sa_data[3],ifr.ifr_hwaddr.sa_data[4],ifr.ifr_hwaddr.sa_data[5]);
    
    }else
    {
        R_Printf("ioctl error %d!\n",errno);
    }
    close(fd);
    return (ntohl(ip));
}

char cmdstr[] = "top";
unsigned long getcpu()
{
    char  cmdstr[40] = {0};
    char  str[40]    = {0};
    FILE  *pd;
   
    if ((pd = popen(cmdstr,"r"))== NULL)
    {
        R_Printf("error to get top!\n");
        return -1;
    }
    fgets(str,40,pd);
    pclose(pd);
    R_Printf("end to get top %d %d %d!\n",str[0],str[1],str[2]);
    return 0;
}


unsigned long GetControlNetMac(char * pucAddr)
{
    struct ifreq   ifr;
    int            fd;
    int            err;
    unsigned long  ip;
    
    if (0 == pucAddr)
    {
        R_Printf("input parameters wrong!\n");
        return -1;
    }
    strncpy(ifr.ifr_name, ethname,sizeof(ethname));
    fd   = socket(AF_INET,SOCK_DGRAM,0);
    if (fd == -1)
    {
    	R_Printf("socket create error,%d!\n",errno);
    	return -1;
    }
    err  = ioctl(fd,SIOCGIFHWADDR,&ifr);    
    if(err == -1)
    {    	
        R_Printf("ioctl error! err %d!\n",errno);
        close(fd);
        return -1;
    }
    close(fd);
    memcpy(pucAddr,ifr.ifr_hwaddr.sa_data,ETH_HW_ADDR_LEN); 
    return 0;
}
CCommLink::CCommLink(WORD32 type,WORD32 fd,WORD32 addr)
{
    dwacfd    = fd;
    dwtype    = type;
    dwAddr    = addr; 
    rcvlen    = g_max_rcv;
    sndlen    = g_max_snd;
    
    pucRcvBuf = (BYTE *)Vos_Malloc(MEM_COMM_TYPE, rcvlen);
    if(NULL == pucRcvBuf)
    {
        rcvlen = 0;
    }
    pucSndBuf = (BYTE *)Vos_Malloc(MEM_COMM_TYPE, sndlen);
    if(NULL == pucSndBuf)
    {
        sndlen = 0;
    }
    pUsercall = NULL; 
    ptNext = NULL;
}
CCommLink::~CCommLink()
{
	  dwAddr    = 0;
    if(pucRcvBuf)
    {
        Vos_Free(pucRcvBuf);
    }
    if(pucSndBuf)
    {
        Vos_Free(pucSndBuf);
    }
}

WORD32 CCommLink::SetUsercall(TaskEntryProto pcall)
{
    pUsercall = pcall;
}

CCommCtl::CCommCtl()
{
    dwNum      = 0;
    ptLinkItem = NULL;
    Vos_Init_Mutex(&mutex);
    /* ptLinkItem = new class CCommLink[dwlink]; */
}

CCommCtl::~CCommCtl()
{
    /* delete []ptLinkItem; */
}

WORD32  CCommLink::send2(BYTE *ptbuf, WORD32 dwlen)
{
    SWORD32      sdwSendCount;
    WORD32       dwSnd = dwlen;
    
    if(dwlen == 0)
    {
        R_Printf("packet socket send error!%d\n",errno);
        return -1;
    }
    /* 先不考虑阻塞缓存 */
    do
    {
        sdwSendCount = send(dwacfd,ptbuf+dwlen-dwSnd,dwSnd,0);
        if(-1 == sdwSendCount)
        {
            R_Printf("packet socket send error!%d\n",errno);
            close(dwacfd);
            return -1;
        }
        dwSnd -= sdwSendCount;
    }while(dwSnd >0);
    
    dwxid ++;
    return 0;
}

WORD32  CCommLink::SetRcvSize(WORD32 size)
{
    return 0;
}

WORD32  msg_free(class msg_header  *pkt)
{
    Vos_Free((BYTE *)pkt);
    return  0;
}

struct msg_header  *GetHeader(BYTE  *ptUser)
{
    return (class msg_header *)(ptUser - sizeof(class msg_header));
}



WORD32 CCommCtl::AppendLink(WORD32 fd, WORD32 addr)
{
    CCommLink     *ptlink = new class  CCommLink(0, fd, addr);
    Vos_Pthread_Mutex_Lock(&mutex);
    if(NULL == ptLinkItem)
    {
        ptLinkItem     = ptlink;
    }
    else
    {
        ptlink->ptNext = ptLinkItem;
        ptLinkItem     = ptlink;
    }
    dwNum ++;
    Vos_Pthread_Mutex_Unlock(&mutex);
    ptlink->dwUser = 1;
    
    
    return dwNum;
}

WORD32 CCommCtl::CloseLink(WORD32 fd)
{
    class CCommLink     *ptlink = ptLinkItem;
    class CCommLink     *ptlinkDel;
    
    while(1)
    {
        if(NULL == ptlink)
        {
            return LEOF;
        }
        if((ptlink->dwacfd == fd) || (ptlink->dwacfd == 0))
        {
            Vos_Pthread_Mutex_Lock(&mutex);
            ptLinkItem = ptlink->ptNext;
            ptlinkDel  = ptlink;
            ptlink     = ptlink->ptNext;
            dwNum --;  
            Vos_Pthread_Mutex_Unlock(&mutex);
            delete ptlinkDel; 
            continue;                  
        }
        break;
    }
    Vos_Pthread_Mutex_Lock(&mutex);
    while(ptlink->ptNext != NULL)
    {
        if(ptlink->ptNext->dwacfd == fd)
        {
            ptLinkItem = ptlink->ptNext;            
            dwNum --;
            Vos_Pthread_Mutex_Unlock(&mutex);
            delete   ptlink;
            return 0;
        }
        if(ptlink->ptNext->dwacfd == 0)
        {
        	  /* 这里没有其它的处理，疑问？ */
            ptLinkItem = ptlink->ptNext;
            dwNum --;            
        }
        ptlink = ptlink->ptNext;
    }   
    Vos_Pthread_Mutex_Unlock(&mutex);
    return LEOF;
}
WORD32 CCommCtl::LinkSend(WORD32 addr, BYTE *ptbuf, WORD32 dwlen)
{
    class CCommLink     *ptlink = ptLinkItem;
    WORD32              index = dwNum;
    
    if(dwNum == 0)
    {
        return LEOF;
    }
    /* 依托于本地任务的发送，要考虑互斥问题 */
    Vos_Pthread_Mutex_Lock(&mutex);
    do
    {
        if(ptlink->dwAddr == addr)
        {
            break;
        }
        index --;
        ptlink = ptlink->ptNext;
    }while((ptlink != NULL) && (index > 0));
    Vos_Pthread_Mutex_Unlock(&mutex);
    if(index == 0)
    {
        return LEOF;
    }
    return (ptlink->send2(ptbuf,dwlen));
}

WORD32 CCommLink::receive()
{
    class msg_header     *ptMsg;
    SWORD32              sdwRcvCount;
    WORD32               dwlen   = 0;
    
    sdwRcvCount = recv(dwacfd,pucRcvBuf+dwRcvOff,rcvlen-dwRcvOff,0);
    if (-1 == sdwRcvCount)
    {   /* 链路异常或者对端关闭 */
        close(dwacfd);
        return -1;
    }
    else if ((0 == sdwRcvCount) && (rcvlen-dwRcvOff > 0)) 
    {   /* 如果对端关闭链路，关闭本端 */
        close(dwacfd);
        return -1;
    }
    R_Printf("stream socket receive len %d,error!%d\n",sdwRcvCount, errno);
    
    while(1)
    {    
        if(dwRcvOff+sdwRcvCount < sizeof(class msg_header))
        {   /* 消息头不足，缓存消息退出 */
            dwRcvOff  += (WORD32)sdwRcvCount;
            break;
        }
        ptMsg = (class msg_header *)(pucRcvBuf);
        dwlen = ntohs(ptMsg->length);
        if(dwRcvOff+sdwRcvCount < dwlen)
        {   /* 消息内容不足，缓存消息退出 */
            dwRcvOff  += sdwRcvCount;
            break;
        }
        /* 派发消息流程，消息头ptMsg，长度ptMsg->dwLen,注意消息要拷贝 */
        if(pUsercall)
        {

            pUsercall((void *)ptMsg);
        }
        else if(g_ptcallback)
        {
        	  g_ptcallback((void *)ptMsg);
        }
        R_Printf("packet socket receive len %d,error!%d\n",dwlen, errno);
        /* 派发完成,剩余消息移动到缓存头，下次派发 */
        dwRcvOff = dwRcvOff+sdwRcvCount-dwlen;
        if(dwRcvOff)
        {
            memmove(pucRcvBuf,pucRcvBuf+dwlen, dwRcvOff);    
        }        
        sdwRcvCount       = 0;
    }    
    return 0;
}

void   *rcv_thread(void *arg)
{
    
    struct timeval       tWaitForSelect;
    INT                  recvSocket;    
    struct sockaddr_in   fromAddr;
    WORD32               dwLength;
    WORD32               dwnum;
    class  CCommLink     *ptlink;
    class  CCommLink     *ptnextlink;
    class  CCommCtl      *ptCtl = (class CCommCtl *)arg;
    WORD32               dwlen;
    SWORD32              sdwResult;


    
    while(1)
    {    
        tWaitForSelect.tv_sec  = 0;
        tWaitForSelect.tv_usec = 1000*1000;  /* 1毫秒 */
        
        FD_ZERO(&ptCtl->tRcvSock); 
        if(ptCtl->dwservfd)
        { 
            FD_SET((WORD32)ptCtl->dwservfd,&ptCtl->tRcvSock);
        }        
        
        if(ptCtl->dwclientfd)
        { 
            FD_SET((WORD32)ptCtl->dwclientfd,&ptCtl->tRcvSock);
        }       
        for (ptlink = ptCtl->ptLinkItem,dwnum = 0; dwnum < ptCtl->dwNum; dwnum ++,ptlink = ptlink->ptNext)
        {            
            if(ptlink->dwUser == 1)
            {
                FD_SET(ptlink->dwacfd, &ptCtl->tRcvSock);
            }
        }
        sdwResult = select(FD_SETSIZE, &ptCtl->tRcvSock, NULL, NULL,&tWaitForSelect);
        
        if (0 == sdwResult)
        {
            FD_ZERO(&ptCtl->tRcvSock); 
            R_Printf("rcv_thread failed in select,err %d!\n",errno);
            continue;
        }
        else if (sdwResult < 0)
        {
            if (EINTR == errno)
            {
                continue;
            }
            /* 错误 */
        }
        
        if(FD_ISSET(ptCtl->dwservfd,&ptCtl->tRcvSock))
        {
            dwLength   = sizeof(struct sockaddr);
            recvSocket = accept(ptCtl->dwservfd,(struct sockaddr *)&fromAddr,(socklen_t *)&dwLength);  
            
            if(recvSocket != -1)
            {
                if(ptCtl->dwNum < g_max_cli)
                {
                    ptCtl->AppendLink(recvSocket,ntohl(fromAddr.sin_addr.s_addr));
                    FD_SET((WORD32)recvSocket,&ptCtl->tRcvSock);
                    R_Printf("rcv_thread accept %d,err %d!\n",ptCtl->dwNum,errno);
                }
                else
                {
                    close(recvSocket);
                }
            }
        }
        
        for (ptlink = ptCtl->ptLinkItem,dwnum = 0; dwnum < ptCtl->dwNum; dwnum ++)
        {
        	  ptnextlink =ptlink->ptNext;
            if((ptlink->dwUser == 1) && (FD_ISSET(ptlink->dwacfd, &ptCtl->tRcvSock)))
            {                
                if(ptlink->receive() == -1)
                {
                    ptCtl->CloseLink(ptlink->dwacfd);
                }
            }    
            ptlink = ptnextlink; 
        }
    }

    return NULL;
}

WORD32  CCommCtl::ServInit(WORD32 dwIp, WORD32 dwPort)
{
    struct sockaddr   tSockAddr;
    WORD32            dwOption;
    WORD32            dwret;
    

    dwservfd  =  socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (-1 == dwservfd)
    {
        R_Printf("packet socket error!error %d\n",errno);
        return -1;
    }
    
    oss_setsockaddr((struct sockaddr_in *)&tSockAddr,getipaddr(),dwPort);
    
    if (-1 == bind(dwservfd,(struct sockaddr *)&tSockAddr,sizeof(tSockAddr)))
    {
        R_Printf("packet socket bind error!error %d\n",errno);
        close(dwservfd);
        dwservfd = 0;
        return -1;
    }
    dwOption                   = 1;
    
    ioctl(dwservfd,FIONBIO,&dwOption);    
    
    if (-1 == listen(dwservfd,5))
    {
        R_Printf("packet socket listen error!error %d\n",errno);
        close(dwservfd);
        dwservfd = 0;
        return -1;
    }
    
    FD_SET(dwservfd, &tRcvSock);
        
    dwret = Vos_CreateTask((CHAR *)"comm_server", COMM_SERV_PRI, COMM_SERV_STACK, 0, \
                           (TaskEntryProto)rcv_thread, (WORDPTR)this, SCHE_INVALID_CPUID);
    
    if(LEOF == dwret)
    {
        R_Printf("CCommCtl::ServInit failed in pthread_create serv,err %d!\n",errno);
        close(dwservfd);
        return -1;
    }
    
    /*
    if(0 != pthread_create((pthread_t *)&serv_th, NULL, rcv_thread, (void *)this)) 
    {
        R_Printf("ServInit failed in pthread_create serv,err %d!\n",errno);
        close(dwservfd);
        return -1;
    } */
    return dwret;
}

WORD32 CCommCtl::ClientInit(WORD32 dwIp, WORD32 dwPort)
{
    struct sockaddr   tSockAddr;
    WORD32            dwnum;
    WORD32            dwret;
    
   
    dwclientfd  =  socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (-1 == dwclientfd)
    {
        R_Printf("packet socket error!error %d\n",errno);
        return -1;
    }
    
/*    oss_setsockaddr((struct sockaddr_in *)&tSockAddr,getipaddr(),dwPort);
    
    if (-1 == bind(dwclientfd,(struct sockaddr *)&tSockAddr,sizeof(tSockAddr)))
    {
        R_Printf("packet socket bind error!error %d\n",errno);
        close(dwclientfd);
        return -1;
    }
*/    
    oss_setsockaddr((struct sockaddr_in *)&tSockAddr, dwIp, dwPort);
    if (-1 == connect(dwclientfd,(struct sockaddr *)&tSockAddr,sizeof(tSockAddr)))
    {
        R_Printf("packet socket connect error!error %d\n",errno);
        close(dwclientfd);
        return -1;
    }
    FD_SET(dwclientfd, &tRcvSock);
    
    dwnum = AppendLink(dwclientfd, dwIp);
    

  
    /*
    if(0 != pthread_create (&cli_th, NULL, (void *)rcv_thread, (BYTE *)NULL))
    {
        R_Printf("OVSClientInit failed in pthread_create scan,err %d!\n",errno);
        close(dwclientfd);
        return -1;
    }*/
    return dwnum;  
}



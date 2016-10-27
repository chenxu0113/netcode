#ifndef __PUBLICSTRUCT_H
#define __PUBLICSTRUCT_H
#include "netsdk.h"
#include<afxtempl.h>


#define WM_RT_ALARM		WM_USER + 13456

enum{	WND_NUM_1 = 1,
		WND_NUM_4 = 4,
		WND_NUM_9 = 9,
		WND_NUM_16 = 16};

#define MAINWND_PROP_HANDLE		((HANDLE)1)	//主窗口对应的属性句柄
#define MAX_VIDEO_NUM			16	//最多16路摄像机打开
#define MAX_NUM_3				3	//一个摄像机最多开3路视频
#define MAX_ROWCOL				4
#define MAX_NETWORKDIIO			100	//最大接入100个网络DI/DO模块

#define WIDTH_LEFTTREE			180
#define TIMER_SGVIDEO			1
#define TIMER_ASKDIDOSTATUS		2
#define PLAYBACK_MAX_FILE_NUM	64
#define	ALARM_FLASH_TIME		100000000		//告警闪烁次数，闪烁一个来回算两次

#define LENGTH_1024				1024
#define LENGTH_129				129	//
#define LENGTH_65				65	//
#define LENGTH_17				17	//IP地址字符串长度
#define LENGTH_12				12	//读取DO时的数组长度
#define LENGTH_14				14	//写DO状态的数组长度
#define LENGTH_4				4	//IP地址4个数字的长度
#define STATUS_CLOSE			0	//DI/DO常闭
#define STATUS_OPEN				1	//DI/DO常开

//推视频风格
#define STYLE_FLASH				0	//闪烁
#define STYLE_FULLSCREEN		1	//窗口满屏

#define WM_TRAYNOTIFICATION			(WM_USER+2013)		//托盘消息

#define PROTOCOL_NONE			0x00	//0x00
#define PROTOCOL_EVENT_DEAL		0x01	//事务处理标识
#define PROTOCOL_PROTOCAL		0x00	//协议处理标识，表示modbus协议
#define PROTOCOL_UNIT			0xff	//单元标识，表示主设备
#define PROTOCOL_FUNC_R			0x01	//读状态功能码，表示读寄存器
#define PROTOCOL_FUNC_W			0x0f	//写状态功能码，表示写寄存器
#define PROTOCOL_ADDRESS		0x64	//DO寄存器地址
#define PROTOCOL_TOTAL_NUM		0x64	//寄存器个数，最大64个
#define PROTOCOL_DATA_LENGTH	0x01	//写DO状态时，值的长度，1个字节

#pragma pack(1)

typedef struct _SocketHead
{
	unsigned short nDeal;		//事务处理标识，2字节
	unsigned short nProtocol;	//协议标识，2字节，0表示modbus
	unsigned short nLen;		//数据区长度
}SocketHead;


//网络模块配置信息表
typedef struct _NetworkDIDOInfoDef
{
	int nID;								//ID
	char szName[LENGTH_65];					//name
	char szIP[LENGTH_17];					//IP address
	int	nPort;								//port
	unsigned char cNum[PROTOCOL_TOTAL_NUM];	//DI/DO num,如是4路，num为(0, 3)
	unsigned char cMaxNum;					//DI/DO最大路数，如4路、8路，目前模块BB3是4路的
	unsigned char cAlarmValue;				//报警值，当此值为XX时，默认为告警，如设置为0（常闭），则变为1时为告警
	char szDesc[LENGTH_129];				//description
}NetworkDIDOInfoDef;

// 摄像机信息表
typedef struct _CameraInfoDef
{
	int			nID;			// 摄像机编号
	char		szCName[LENGTH_129];	// 摄像机名称
	BYTE		bChannel;				//摄像机通道号
	short		nType;// 摄像机生产厂家类别,0-海康, 1-数智源
	char		szDescription[LENGTH_129];	// 描述
} CameraInfoDef;


//网络DI/DO模块信息表，程序内部使用，主要用于从服务器获取数据、写数据下发
typedef struct _NetworkDIDOSocket
{
	int nIndex;				//用于对应的CClientSocket的下标，从0开始
	BOOL bInitSuccess;		//是否初始化成功
	SYSTEMTIME st;			//时间
	unsigned char cAlarmNum;	//网络DI/DO模块告警点号
	NetworkDIDOInfoDef ndc;	//网络DI/DO模块信息表
} NetworkDIDOSocket;


//XX摄像机的XX通道与XX网络DI/DO模块的第X路的关联关系
typedef struct _LinkInfoDef
{
	int nCamID;							//摄像机ID
	char cCamIP[LENGTH_17];				//摄像机IP地址
	BYTE bChannel;						//摄像机通道号
	int nNetworkID;						//网络DI/DO模块ID
	char cNetworkSocketIP[LENGTH_17];	//网络DI/DO模块IP
	BYTE bNetworkNum;					//网络DI/DO模块的第X个点，从0开始编号
} LinkInfoDef;


typedef struct _DEV_INFO 
{
	int	nListNum;		   //position in the list

	long lLoginID;			//login handle
	long lID;				//device ID
	char szDevName[60];		//device name
	char szIpaddress[60];		//device IP
	char szUserName[15];		//user name
	char szPsw[15];			//pass word
	int nPort;				//port number
	int nTotalChannel;		//total channel

	SDK_CONFIG_NET_COMMON NetComm;                  // net config
	H264_DVR_DEVICEINFO  NetDeviceInfo;

	BOOL bSerialID;//be SerialNumber login
	char szSerIP[DDNS_MAX_DDNS_IPSIZE];//server ip
	int nSerPort;           //server port
	char szSerialInfo[DDNS_MAX_SERIALINFO_SIZE];  //SerialNumber
	int devctype;
}DEV_INFO;
typedef	std::map<long , DEV_INFO*> Devc_Map;   //id index number is database index ID



typedef struct tagVIDEOPARAM 
{
	LONG lChannel;//通道号
	int nCameraNo;			// 摄像机编号
	DEV_INFO devinfo;
} VIDEOPARAM;


typedef struct _InputParams 
{
	int nYxIndex;
	int nCamNo;
}InputParams;

typedef struct _CAMINFO
{
	int nYXIndex[LENGTH_129];		//遥信索引（从0开始）
	int nCamNo[LENGTH_129];		//摄像机编号（从1开始）
} CAMINFO;


// 摄像机联动信息表
typedef struct _CameraLinkage
{
	int				nID;			// 序号
	int				nDIIndex;			// 开关量索引号
	short			nCameraID;			// 摄像机编号
	short			nPtzPreset;			// 摄像机预置点号
} CameraLinkage;


typedef struct _TIMESTRUCT 
{
	short year;
	short month;
	short day;
} TIMESTRUCT;


typedef struct _DATETIME 
{
	short		year;		// 年
	short		month;		// 月
	BYTE		day;		// 日
	BYTE		hour;		// 时
	BYTE		minute;		// 分
	BYTE		second;		// 秒
} DATETIME;



#pragma pack()


#define SET_SGVIDEO			_T("SGVIDEO")
#define SET_ISSGVIDEO		_T("ISSGVIDEO")
#define SET_PATH			_T("PATH")
#define SET_PICPATH			_T("PICPATH")
#define SET_RECORDPATH		_T("RECORDPATH")
#define SET_CAPPIC			_T("CAPPIC")
#define SET_NORMALVIDEO		_T("NORMALVIDEO")
#define SET_TIME_STOPREC	_T("STOPRECTIME")
#define SET_TIME_STOPVIDEO	_T("STOPVIDEOTIME")
#define SET_STARTTEMPREC	_T("STARTTEMPREC")

#endif
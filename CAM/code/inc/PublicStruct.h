#ifndef __PUBLICSTRUCT_H
#define __PUBLICSTRUCT_H
#include "netsdk.h"
#include<afxtempl.h>


#define WM_RT_ALARM		WM_USER + 13456

enum{	WND_NUM_1 = 1,
		WND_NUM_4 = 4,
		WND_NUM_9 = 9,
		WND_NUM_16 = 16};

#define MAINWND_PROP_HANDLE		((HANDLE)1)	//�����ڶ�Ӧ�����Ծ��
#define MAX_VIDEO_NUM			16	//���16·�������
#define MAX_NUM_3				3	//һ���������࿪3·��Ƶ
#define MAX_ROWCOL				4
#define MAX_NETWORKDIIO			100	//������100������DI/DOģ��

#define WIDTH_LEFTTREE			180
#define TIMER_SGVIDEO			1
#define TIMER_ASKDIDOSTATUS		2
#define PLAYBACK_MAX_FILE_NUM	64
#define	ALARM_FLASH_TIME		100000000		//�澯��˸��������˸һ������������

#define LENGTH_1024				1024
#define LENGTH_129				129	//
#define LENGTH_65				65	//
#define LENGTH_17				17	//IP��ַ�ַ�������
#define LENGTH_12				12	//��ȡDOʱ�����鳤��
#define LENGTH_14				14	//дDO״̬�����鳤��
#define LENGTH_4				4	//IP��ַ4�����ֵĳ���
#define STATUS_CLOSE			0	//DI/DO����
#define STATUS_OPEN				1	//DI/DO����

//����Ƶ���
#define STYLE_FLASH				0	//��˸
#define STYLE_FULLSCREEN		1	//��������

#define WM_TRAYNOTIFICATION			(WM_USER+2013)		//������Ϣ

#define PROTOCOL_NONE			0x00	//0x00
#define PROTOCOL_EVENT_DEAL		0x01	//�������ʶ
#define PROTOCOL_PROTOCAL		0x00	//Э�鴦���ʶ����ʾmodbusЭ��
#define PROTOCOL_UNIT			0xff	//��Ԫ��ʶ����ʾ���豸
#define PROTOCOL_FUNC_R			0x01	//��״̬�����룬��ʾ���Ĵ���
#define PROTOCOL_FUNC_W			0x0f	//д״̬�����룬��ʾд�Ĵ���
#define PROTOCOL_ADDRESS		0x64	//DO�Ĵ�����ַ
#define PROTOCOL_TOTAL_NUM		0x64	//�Ĵ������������64��
#define PROTOCOL_DATA_LENGTH	0x01	//дDO״̬ʱ��ֵ�ĳ��ȣ�1���ֽ�

#pragma pack(1)

typedef struct _SocketHead
{
	unsigned short nDeal;		//�������ʶ��2�ֽ�
	unsigned short nProtocol;	//Э���ʶ��2�ֽڣ�0��ʾmodbus
	unsigned short nLen;		//����������
}SocketHead;


//����ģ��������Ϣ��
typedef struct _NetworkDIDOInfoDef
{
	int nID;								//ID
	char szName[LENGTH_65];					//name
	char szIP[LENGTH_17];					//IP address
	int	nPort;								//port
	unsigned char cNum[PROTOCOL_TOTAL_NUM];	//DI/DO num,����4·��numΪ(0, 3)
	unsigned char cMaxNum;					//DI/DO���·������4·��8·��Ŀǰģ��BB3��4·��
	unsigned char cAlarmValue;				//����ֵ������ֵΪXXʱ��Ĭ��Ϊ�澯��������Ϊ0�����գ������Ϊ1ʱΪ�澯
	char szDesc[LENGTH_129];				//description
}NetworkDIDOInfoDef;

// �������Ϣ��
typedef struct _CameraInfoDef
{
	int			nID;			// ��������
	char		szCName[LENGTH_129];	// ���������
	BYTE		bChannel;				//�����ͨ����
	short		nType;// ����������������,0-����, 1-����Դ
	char		szDescription[LENGTH_129];	// ����
} CameraInfoDef;


//����DI/DOģ����Ϣ�������ڲ�ʹ�ã���Ҫ���ڴӷ�������ȡ���ݡ�д�����·�
typedef struct _NetworkDIDOSocket
{
	int nIndex;				//���ڶ�Ӧ��CClientSocket���±꣬��0��ʼ
	BOOL bInitSuccess;		//�Ƿ��ʼ���ɹ�
	SYSTEMTIME st;			//ʱ��
	unsigned char cAlarmNum;	//����DI/DOģ��澯���
	NetworkDIDOInfoDef ndc;	//����DI/DOģ����Ϣ��
} NetworkDIDOSocket;


//XX�������XXͨ����XX����DI/DOģ��ĵ�X·�Ĺ�����ϵ
typedef struct _LinkInfoDef
{
	int nCamID;							//�����ID
	char cCamIP[LENGTH_17];				//�����IP��ַ
	BYTE bChannel;						//�����ͨ����
	int nNetworkID;						//����DI/DOģ��ID
	char cNetworkSocketIP[LENGTH_17];	//����DI/DOģ��IP
	BYTE bNetworkNum;					//����DI/DOģ��ĵ�X���㣬��0��ʼ���
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
	LONG lChannel;//ͨ����
	int nCameraNo;			// ��������
	DEV_INFO devinfo;
} VIDEOPARAM;


typedef struct _InputParams 
{
	int nYxIndex;
	int nCamNo;
}InputParams;

typedef struct _CAMINFO
{
	int nYXIndex[LENGTH_129];		//ң����������0��ʼ��
	int nCamNo[LENGTH_129];		//�������ţ���1��ʼ��
} CAMINFO;


// �����������Ϣ��
typedef struct _CameraLinkage
{
	int				nID;			// ���
	int				nDIIndex;			// ������������
	short			nCameraID;			// ��������
	short			nPtzPreset;			// �����Ԥ�õ��
} CameraLinkage;


typedef struct _TIMESTRUCT 
{
	short year;
	short month;
	short day;
} TIMESTRUCT;


typedef struct _DATETIME 
{
	short		year;		// ��
	short		month;		// ��
	BYTE		day;		// ��
	BYTE		hour;		// ʱ
	BYTE		minute;		// ��
	BYTE		second;		// ��
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
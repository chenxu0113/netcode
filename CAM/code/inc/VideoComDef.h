//edit by ZHOUN
//2014.4.10

#pragma once

/*************************摄像机厂家类别**********************************/
#define CAMERA_TYPE_HAIKANG		0	//海康
#define CAMERA_TYPE_DAHUA		2	//大华
/*************************************************************************/

#ifndef LLONG
#ifdef WIN32
#define LLONG LONG
#else //WIN64 
#define LLONG INT64
#endif
#endif

#ifndef LDWORD
#ifdef WIN32
#define LDWORD DWORD
#else //WIN64 
#define LDWORD INT64
#endif
#endif

//函数指针定义
typedef void(CALLBACK *fDisConnect)(LLONG  lLoginID, char *pchDVRIP, LONG nDVRPort, LDWORD dwUser);

//海康参数集
typedef struct _CamParamsHK
{
	/*********全局通用**********/
	LONG lUserID;		//登录ID
	LONG lPlayHandle;	//实时播放句柄
	LONG lChannelID;	//通道ID
	/***************************/

	/**********登录用***********/
	char szDVRIP[32];	//摄像机ip
	WORD wDVRPort;		//摄像机端口
	char szUserName[32];	//用户名
	char szPassword[16];	//用户密码
	/***************************/

	/********实时预览用*********/	
	HWND hWnd;			//播放窗口句柄
	LONG lLinkMode;		//最高位(31)为0表示主码流，为1表示子，0－30位表示码流连接方式: 0：TCP方式,1：UDP方式,2：多播方式,3 - RTP方式，4-音视频分开(TCP)
	/***************************/

	/****云台控制、预置点用*****/
	int	nPTZType;		//云台控制类型（包含预置点）:PTZ_CONTROL_UP、PTZ_PRESET_MOVE等
	DWORD dwSpeed;		//调节步长（速度），适用于云台控制类型
	DWORD dwStop;			//是否停止，适用于云台控制类型
	DWORD dwPreset;		//预置点号，适用于预置点类型
	/***************************/

	/******调节画面效果用*******/	
	DWORD dwBrightness;	//亮度(1-10)
	DWORD dwContrast;	//对比度(1-10)
	DWORD dwHue;		//色度(1-10)
	DWORD dwSaturation;	//饱和度(1-10)
	/***************************/

	/**********抓图用***********/	
	char szPicFileName[MAX_PATH];			//抓图完整文件名
	/***************************/

	/********保存录像用*********/	
	char szRecordFileName[MAX_PATH];			//录像完整文件名
	/***************************/

}CamParamsHK;

//大华参数集
typedef struct _CamParamsDH
{
	/*********全局通用**********/
	LLONG lLoginID;		//登录ID
	LLONG lPlayHandle;	//实时播放句柄
	int	nChannelID;		//通道ID
	/***************************/

	/*********初始化用**********/
	fDisConnect cbDisConnect;		//断线回调函数 为NULL则不设置
	LDWORD dwUser;	//断线回调函数的用户数据
	/***************************/

	/**********登录用***********/
	char szDVRIP[32];	//摄像机ip
	WORD wDVRPort;	//摄像机端口
	char szUserName[32];	//用户名
	char szPassword[16];	//用户密码
	/***************************/

	/********实时预览用*********/	
	HWND hWnd;			//播放窗口句柄
	/***************************/

	/****云台控制、预置点用*****/
	int	nPTZType;		//云台控制类型（包含预置点）:PTZ_CONTROL_UP、PTZ_PRESET_MOVE等
	BYTE btStep;		//调节步长（速度），适用于云台控制类型
	BOOL bStop;		//是否停止，适用于云台控制类型
	BYTE btPreset;		//预置点号，适用于预置点类型
	/***************************/

	/******调节画面效果用*******/	
	BYTE btBrightness;	//亮度(0-255)
	BYTE btContrast;	//对比度(0-255)
	BYTE btHue;			//色度(0-255)
	BYTE btSaturation;	//饱和度(0-255)
	/***************************/

	/**********抓图用***********/	
	char szPicFileName[MAX_PATH];			//抓图完整文件名，目前只支持BMP后缀
	/***************************/

	/********保存录像用*********/	
	char szRecordFileName[MAX_PATH];			//录像完整文件名
	/***************************/

}CamParamsDH;

//云台控制
#define		PTZ_CONTROL_UP				0		//云台上
#define		PTZ_CONTROL_DOWN			1		//云台下
#define		PTZ_CONTROL_LEFT			2		//云台左
#define		PTZ_CONTROL_RIGHT			3		//云台右
#define		PTZ_CONTROL_ZOOMIN			4		//放大
#define		PTZ_CONTROL_ZOOMOUT			5		//缩小
#define		PTZ_CONTROL_FOCUS_ADD		6		//调焦+
#define		PTZ_CONTROL_FOCUS_DEC		7		//调焦-
#define		PTZ_CONTROL_APERTURE_ADD	8		//光圈+
#define		PTZ_CONTROL_APERTURE_DEC	9		//光圈-
//预置点
#define		PTZ_PRESET_MOVE		20		//转至预置点
#define		PTZ_PRESET_SET		21		//设置预置点
#define		PTZ_PRESET_DEL		22		//删除预置点
//画面效果（亮度，对比度，色度，饱和度）区间，以下注释以亮度为例，方便理解
#define		EFFECTVAL_MIN_COMMON	1			//亮度最小为1（通用，以统一外部传入）
#define		EFFECTVAL_MAX_COMMON	10			//亮度最大为10（通用，以统一外部传入）
#define		EFFECTVAL_MIN_DAHUA		0			//亮度最小为0（大华）
#define		EFFECTVAL_MAX_DAHUA		255			//亮度最大为255（大华）
#define		EFFECTVAL_MIN_HAIKANG	1			//亮度最小为1（海康）		
#define		EFFECTVAL_MAX_HAIKANG	10			//亮度最大为10（海康）


//共通参数集
typedef union _CamParams
{
	CamParamsHK stParaHK;
	CamParamsDH stParaDH;
}CamParams;

typedef struct _TSCameraDef
{
	BYTE btType;
	CamParams unParams;	//摄像机参数集
}TSCameraDef;
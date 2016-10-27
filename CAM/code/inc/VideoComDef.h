//edit by ZHOUN
//2014.4.10

#pragma once

/*************************������������**********************************/
#define CAMERA_TYPE_HAIKANG		0	//����
#define CAMERA_TYPE_DAHUA		2	//��
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

//����ָ�붨��
typedef void(CALLBACK *fDisConnect)(LLONG  lLoginID, char *pchDVRIP, LONG nDVRPort, LDWORD dwUser);

//����������
typedef struct _CamParamsHK
{
	/*********ȫ��ͨ��**********/
	LONG lUserID;		//��¼ID
	LONG lPlayHandle;	//ʵʱ���ž��
	LONG lChannelID;	//ͨ��ID
	/***************************/

	/**********��¼��***********/
	char szDVRIP[32];	//�����ip
	WORD wDVRPort;		//������˿�
	char szUserName[32];	//�û���
	char szPassword[16];	//�û�����
	/***************************/

	/********ʵʱԤ����*********/	
	HWND hWnd;			//���Ŵ��ھ��
	LONG lLinkMode;		//���λ(31)Ϊ0��ʾ��������Ϊ1��ʾ�ӣ�0��30λ��ʾ�������ӷ�ʽ: 0��TCP��ʽ,1��UDP��ʽ,2���ಥ��ʽ,3 - RTP��ʽ��4-����Ƶ�ֿ�(TCP)
	/***************************/

	/****��̨���ơ�Ԥ�õ���*****/
	int	nPTZType;		//��̨�������ͣ�����Ԥ�õ㣩:PTZ_CONTROL_UP��PTZ_PRESET_MOVE��
	DWORD dwSpeed;		//���ڲ������ٶȣ�����������̨��������
	DWORD dwStop;			//�Ƿ�ֹͣ����������̨��������
	DWORD dwPreset;		//Ԥ�õ�ţ�������Ԥ�õ�����
	/***************************/

	/******���ڻ���Ч����*******/	
	DWORD dwBrightness;	//����(1-10)
	DWORD dwContrast;	//�Աȶ�(1-10)
	DWORD dwHue;		//ɫ��(1-10)
	DWORD dwSaturation;	//���Ͷ�(1-10)
	/***************************/

	/**********ץͼ��***********/	
	char szPicFileName[MAX_PATH];			//ץͼ�����ļ���
	/***************************/

	/********����¼����*********/	
	char szRecordFileName[MAX_PATH];			//¼�������ļ���
	/***************************/

}CamParamsHK;

//�󻪲�����
typedef struct _CamParamsDH
{
	/*********ȫ��ͨ��**********/
	LLONG lLoginID;		//��¼ID
	LLONG lPlayHandle;	//ʵʱ���ž��
	int	nChannelID;		//ͨ��ID
	/***************************/

	/*********��ʼ����**********/
	fDisConnect cbDisConnect;		//���߻ص����� ΪNULL������
	LDWORD dwUser;	//���߻ص��������û�����
	/***************************/

	/**********��¼��***********/
	char szDVRIP[32];	//�����ip
	WORD wDVRPort;	//������˿�
	char szUserName[32];	//�û���
	char szPassword[16];	//�û�����
	/***************************/

	/********ʵʱԤ����*********/	
	HWND hWnd;			//���Ŵ��ھ��
	/***************************/

	/****��̨���ơ�Ԥ�õ���*****/
	int	nPTZType;		//��̨�������ͣ�����Ԥ�õ㣩:PTZ_CONTROL_UP��PTZ_PRESET_MOVE��
	BYTE btStep;		//���ڲ������ٶȣ�����������̨��������
	BOOL bStop;		//�Ƿ�ֹͣ����������̨��������
	BYTE btPreset;		//Ԥ�õ�ţ�������Ԥ�õ�����
	/***************************/

	/******���ڻ���Ч����*******/	
	BYTE btBrightness;	//����(0-255)
	BYTE btContrast;	//�Աȶ�(0-255)
	BYTE btHue;			//ɫ��(0-255)
	BYTE btSaturation;	//���Ͷ�(0-255)
	/***************************/

	/**********ץͼ��***********/	
	char szPicFileName[MAX_PATH];			//ץͼ�����ļ�����Ŀǰֻ֧��BMP��׺
	/***************************/

	/********����¼����*********/	
	char szRecordFileName[MAX_PATH];			//¼�������ļ���
	/***************************/

}CamParamsDH;

//��̨����
#define		PTZ_CONTROL_UP				0		//��̨��
#define		PTZ_CONTROL_DOWN			1		//��̨��
#define		PTZ_CONTROL_LEFT			2		//��̨��
#define		PTZ_CONTROL_RIGHT			3		//��̨��
#define		PTZ_CONTROL_ZOOMIN			4		//�Ŵ�
#define		PTZ_CONTROL_ZOOMOUT			5		//��С
#define		PTZ_CONTROL_FOCUS_ADD		6		//����+
#define		PTZ_CONTROL_FOCUS_DEC		7		//����-
#define		PTZ_CONTROL_APERTURE_ADD	8		//��Ȧ+
#define		PTZ_CONTROL_APERTURE_DEC	9		//��Ȧ-
//Ԥ�õ�
#define		PTZ_PRESET_MOVE		20		//ת��Ԥ�õ�
#define		PTZ_PRESET_SET		21		//����Ԥ�õ�
#define		PTZ_PRESET_DEL		22		//ɾ��Ԥ�õ�
//����Ч�������ȣ��Աȶȣ�ɫ�ȣ����Ͷȣ����䣬����ע��������Ϊ�����������
#define		EFFECTVAL_MIN_COMMON	1			//������СΪ1��ͨ�ã���ͳһ�ⲿ���룩
#define		EFFECTVAL_MAX_COMMON	10			//�������Ϊ10��ͨ�ã���ͳһ�ⲿ���룩
#define		EFFECTVAL_MIN_DAHUA		0			//������СΪ0���󻪣�
#define		EFFECTVAL_MAX_DAHUA		255			//�������Ϊ255���󻪣�
#define		EFFECTVAL_MIN_HAIKANG	1			//������СΪ1��������		
#define		EFFECTVAL_MAX_HAIKANG	10			//�������Ϊ10��������


//��ͨ������
typedef union _CamParams
{
	CamParamsHK stParaHK;
	CamParamsDH stParaDH;
}CamParams;

typedef struct _TSCameraDef
{
	BYTE btType;
	CamParams unParams;	//�����������
}TSCameraDef;
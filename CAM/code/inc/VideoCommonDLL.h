// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� VIDEOCOMMONDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// VIDEOCOMMONDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef VIDEOCOMMONDLL_EXPORTS
#define VIDEOCOMMONDLL_API __declspec(dllexport)
#else
#define VIDEOCOMMONDLL_API __declspec(dllimport)
#endif
#include "VideoComDef.h"

//ע�⣺
//�ⲿ��������������нӿں���ʱ������params������ֻ������ԱbtType���и�ֵ���Ա������������
//������params�ĸ�ֵ����������ҪҲ��Ӧ�����ⲿ���С��ⲿֻ��Ҫ���չ涨���������ε��ö�Ӧ�Ľӿڣ�
//��dll���Զ�����params�е���ֵ���ⲿֻ��Ҫ����ò����Ա��´ε���ʱ���롣
//���ڳ�params�����������������szIP��hWnd�ȣ�������Ҫ�ⲿ�ճ���ֵ��


//��ʼ��
VIDEOCOMMONDLL_API BOOL DLL_DVR_Init(TSCameraDef *params);

//����ʼ��
VIDEOCOMMONDLL_API void DLL_DVR_UnInit(TSCameraDef *params);

//��¼
VIDEOCOMMONDLL_API BOOL DLL_DVR_Login(TSCameraDef *params, const char *szIP, WORD wPort, const char *szUser, const char *szPassword);

//ע��
VIDEOCOMMONDLL_API BOOL DLL_DVR_Logout(TSCameraDef *params);

//ʵʱԤ��
VIDEOCOMMONDLL_API BOOL DLL_DVR_RealPlay(TSCameraDef *params, int nChannelID, HWND hWnd, LONG lLinkMode = 0);

//�ر�ʵʱԤ��
VIDEOCOMMONDLL_API BOOL DLL_DVR_StopRealPlay(TSCameraDef *params);

//��̨����
VIDEOCOMMONDLL_API BOOL DLL_DVR_PTZControl(TSCameraDef *params, int nPTZType, BYTE btStep, BOOL bStop = FALSE);

//����Ԥ�õ�
VIDEOCOMMONDLL_API BOOL DLL_DVR_Preset(TSCameraDef *params, int nPresetType, BYTE btPreset);

//��ȡ���Ԥ�õ����
VIDEOCOMMONDLL_API int DLL_DVR_GetPresetCount(BYTE nCamType);

//���ڻ���Ч��
VIDEOCOMMONDLL_API BOOL DLL_DVR_ClientSetVideoEffect(TSCameraDef *params, BYTE btCommonBrightness, BYTE btCommonContrast, 
													 BYTE btCommonHue, BYTE btCommonSaturation);

//ץͼ
VIDEOCOMMONDLL_API BOOL DLL_DVR_CapturePicture(TSCameraDef *params, const char *szPicFileName);

//��ʼ����ʵʱ¼��
VIDEOCOMMONDLL_API BOOL DLL_DVR_StartRealRecord(TSCameraDef *params, const char *szRecordFileName);

//ֹͣ����ʵʱ¼��
VIDEOCOMMONDLL_API BOOL DLL_DVR_StopRealRecord(TSCameraDef *params);
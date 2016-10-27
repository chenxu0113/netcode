// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� HAIKANGDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// HAIKANGDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef HAIKANGDLL_EXPORTS
#define HAIKANGDLL_API __declspec(dllexport)
#else
#define HAIKANGDLL_API __declspec(dllimport)
#endif

//��ʼ��
HAIKANGDLL_API BOOL DLL_DVR_HK_Init(CamParamsHK *params);

//����ʼ��
HAIKANGDLL_API void DLL_DVR_HK_UnInit(CamParamsHK *params);

//��¼
HAIKANGDLL_API BOOL DLL_DVR_HK_Login(CamParamsHK *params);

//ע��
HAIKANGDLL_API BOOL DLL_DVR_HK_Logout(CamParamsHK *params);

//ʵʱԤ��
HAIKANGDLL_API BOOL DLL_DVR_HK_RealPlay(CamParamsHK *params);

//�ر�ʵʱԤ��
HAIKANGDLL_API BOOL DLL_DVR_HK_StopRealPlay(CamParamsHK *params);

//��̨����
HAIKANGDLL_API BOOL DLL_DVR_HK_PTZControl(CamParamsHK *params);

//����Ԥ�õ�
HAIKANGDLL_API BOOL DLL_DVR_HK_Preset(CamParamsHK *params);

//���ڻ���Ч��
HAIKANGDLL_API BOOL DLL_DVR_HK_ClientSetVideoEffect(CamParamsHK *params);

//ץͼ
HAIKANGDLL_API BOOL DLL_DVR_HK_CapturePicture(CamParamsHK *params);

//��ʼ����ʵʱ¼��
HAIKANGDLL_API BOOL DLL_DVR_HK_StartRealRecord(CamParamsHK *params);

//ֹͣ����ʵʱ¼��
HAIKANGDLL_API BOOL DLL_DVR_HK_StopRealRecord(CamParamsHK *params);
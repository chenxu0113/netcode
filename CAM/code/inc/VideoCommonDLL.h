// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 VIDEOCOMMONDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// VIDEOCOMMONDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef VIDEOCOMMONDLL_EXPORTS
#define VIDEOCOMMONDLL_API __declspec(dllexport)
#else
#define VIDEOCOMMONDLL_API __declspec(dllimport)
#endif
#include "VideoComDef.h"

//注意：
//外部程序调用下列所有接口函数时，对于params参数，只需对其成员btType进行赋值，以表明摄像机类型
//其他对params的赋值操作都不需要也不应该在外部进行。外部只需要按照规定的流程依次调用对应的接口，
//本dll会自动更新params中的数值，外部只需要留存该参数以便下次调用时传入。
//对于除params以外的其他参数（如szIP、hWnd等），则需要外部照常传值。


//初始化
VIDEOCOMMONDLL_API BOOL DLL_DVR_Init(TSCameraDef *params);

//反初始化
VIDEOCOMMONDLL_API void DLL_DVR_UnInit(TSCameraDef *params);

//登录
VIDEOCOMMONDLL_API BOOL DLL_DVR_Login(TSCameraDef *params, const char *szIP, WORD wPort, const char *szUser, const char *szPassword);

//注销
VIDEOCOMMONDLL_API BOOL DLL_DVR_Logout(TSCameraDef *params);

//实时预览
VIDEOCOMMONDLL_API BOOL DLL_DVR_RealPlay(TSCameraDef *params, int nChannelID, HWND hWnd, LONG lLinkMode = 0);

//关闭实时预览
VIDEOCOMMONDLL_API BOOL DLL_DVR_StopRealPlay(TSCameraDef *params);

//云台控制
VIDEOCOMMONDLL_API BOOL DLL_DVR_PTZControl(TSCameraDef *params, int nPTZType, BYTE btStep, BOOL bStop = FALSE);

//操作预置点
VIDEOCOMMONDLL_API BOOL DLL_DVR_Preset(TSCameraDef *params, int nPresetType, BYTE btPreset);

//获取最大预置点个数
VIDEOCOMMONDLL_API int DLL_DVR_GetPresetCount(BYTE nCamType);

//调节画面效果
VIDEOCOMMONDLL_API BOOL DLL_DVR_ClientSetVideoEffect(TSCameraDef *params, BYTE btCommonBrightness, BYTE btCommonContrast, 
													 BYTE btCommonHue, BYTE btCommonSaturation);

//抓图
VIDEOCOMMONDLL_API BOOL DLL_DVR_CapturePicture(TSCameraDef *params, const char *szPicFileName);

//开始保存实时录像
VIDEOCOMMONDLL_API BOOL DLL_DVR_StartRealRecord(TSCameraDef *params, const char *szRecordFileName);

//停止保存实时录像
VIDEOCOMMONDLL_API BOOL DLL_DVR_StopRealRecord(TSCameraDef *params);
// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 HAIKANGDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// HAIKANGDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef HAIKANGDLL_EXPORTS
#define HAIKANGDLL_API __declspec(dllexport)
#else
#define HAIKANGDLL_API __declspec(dllimport)
#endif

//初始化
HAIKANGDLL_API BOOL DLL_DVR_HK_Init(CamParamsHK *params);

//反初始化
HAIKANGDLL_API void DLL_DVR_HK_UnInit(CamParamsHK *params);

//登录
HAIKANGDLL_API BOOL DLL_DVR_HK_Login(CamParamsHK *params);

//注销
HAIKANGDLL_API BOOL DLL_DVR_HK_Logout(CamParamsHK *params);

//实时预览
HAIKANGDLL_API BOOL DLL_DVR_HK_RealPlay(CamParamsHK *params);

//关闭实时预览
HAIKANGDLL_API BOOL DLL_DVR_HK_StopRealPlay(CamParamsHK *params);

//云台控制
HAIKANGDLL_API BOOL DLL_DVR_HK_PTZControl(CamParamsHK *params);

//操作预置点
HAIKANGDLL_API BOOL DLL_DVR_HK_Preset(CamParamsHK *params);

//调节画面效果
HAIKANGDLL_API BOOL DLL_DVR_HK_ClientSetVideoEffect(CamParamsHK *params);

//抓图
HAIKANGDLL_API BOOL DLL_DVR_HK_CapturePicture(CamParamsHK *params);

//开始保存实时录像
HAIKANGDLL_API BOOL DLL_DVR_HK_StartRealRecord(CamParamsHK *params);

//停止保存实时录像
HAIKANGDLL_API BOOL DLL_DVR_HK_StopRealRecord(CamParamsHK *params);
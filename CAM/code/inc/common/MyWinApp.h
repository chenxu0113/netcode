/**
 *
 * 文 件 名	: MyWinApp.h
 * 创建日期	: 2013/1/5 15:20
 * 作    者 : 陆治国
 * 修改日期	: 
 * 当前版本	: $Revision: 1.0 $
 * 功能描述 : CWinApp重载类，实现版本信息
 * 修改记录 : 
 *
 **/
#pragma once



// CMyWinApp

class CMyWinApp : public CWinApp
{
	DECLARE_DYNCREATE(CMyWinApp)

protected:
	CMyWinApp();           // 动态创建所使用的受保护的构造函数
	virtual ~CMyWinApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	/***********************************************************************
	* @函数	: CMyWinApp::SetVersion
	* @访问	: public 
	* @限定	:
	* @参数	: CString sVer(采用:<产品内部型号>.<阶段版本号>.<模块版本号>)
	* @返回	: void （NULL）
	* @作者	: Acidy
	* @日期	: 2013/1/5 15:25
	* @说明	: 设置模块版本号
	***********************************************************************/
	void SetVersion(const CString sVer);
	void GetVersion(CString &sVer);

	/***********************************************************************
	* @函数	: CMyWinApp::SetModuleDesc
	* @访问	: public 
	* @限定	:
	* @参数	: CString sDesc （描述）
	* @返回	: void （NULL）
	* @作者	: Acidy
	* @日期	: 2013/1/5 15:26
	* @说明	: 设置模块描述
	***********************************************************************/
	void SetModuleDesc(const CString sDesc);
	void GetModuleDesc(CString &sDesc);

private:
	CString m_sModule;		//模块名
	CString m_sBaseVer;		//基类库版本
	CString m_sModuleVer;	//模块版本
	CString m_sModuleDesc;	//模块描述
protected:
	DECLARE_MESSAGE_MAP()
};



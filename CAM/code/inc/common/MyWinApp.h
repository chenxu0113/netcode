/**
 *
 * �� �� ��	: MyWinApp.h
 * ��������	: 2013/1/5 15:20
 * ��    �� : ½�ι�
 * �޸�����	: 
 * ��ǰ�汾	: $Revision: 1.0 $
 * �������� : CWinApp�����࣬ʵ�ְ汾��Ϣ
 * �޸ļ�¼ : 
 *
 **/
#pragma once



// CMyWinApp

class CMyWinApp : public CWinApp
{
	DECLARE_DYNCREATE(CMyWinApp)

protected:
	CMyWinApp();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMyWinApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	/***********************************************************************
	* @����	: CMyWinApp::SetVersion
	* @����	: public 
	* @�޶�	:
	* @����	: CString sVer(����:<��Ʒ�ڲ��ͺ�>.<�׶ΰ汾��>.<ģ��汾��>)
	* @����	: void ��NULL��
	* @����	: Acidy
	* @����	: 2013/1/5 15:25
	* @˵��	: ����ģ��汾��
	***********************************************************************/
	void SetVersion(const CString sVer);
	void GetVersion(CString &sVer);

	/***********************************************************************
	* @����	: CMyWinApp::SetModuleDesc
	* @����	: public 
	* @�޶�	:
	* @����	: CString sDesc ��������
	* @����	: void ��NULL��
	* @����	: Acidy
	* @����	: 2013/1/5 15:26
	* @˵��	: ����ģ������
	***********************************************************************/
	void SetModuleDesc(const CString sDesc);
	void GetModuleDesc(CString &sDesc);

private:
	CString m_sModule;		//ģ����
	CString m_sBaseVer;		//�����汾
	CString m_sModuleVer;	//ģ��汾
	CString m_sModuleDesc;	//ģ������
protected:
	DECLARE_MESSAGE_MAP()
};



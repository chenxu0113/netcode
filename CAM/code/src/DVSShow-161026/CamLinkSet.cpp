// NetModuleSet.cpp : implementation file
//
#include <stdlib.h>
#include "stdAfx.h"
#include "DVSShow.h"
#include "CamLinkSet.h"
#include "Markup.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDVSShowApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSysSettings dialog


CCamLinkSettings::CCamLinkSettings(CWnd* pParent /*=NULL*/)
: CBCGPDialog(CCamLinkSettings::IDD, pParent)
{
	m_curID = -1;
	m_curListRow = -1;
	m_iCamPort = 34567;
	//{{AFX_DATA_INIT(CSysSettings)

	//}}AFX_DATA_INIT
}


void CCamLinkSettings::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysSettings)
	DDX_Control(pDX, IDC_LISTCTRL_CAM, m_camLinkCtrlList);
	DDX_Control(pDX, IDC_IPADDRESS_CAM, m_camIPCtrl);
	DDX_Control(pDX, IDC_EDIT_CAM_PORT, m_camPortEdit);
	DDX_Text(pDX, IDC_EDIT_CAM_PORT, m_iCamPort);
	DDV_MinMaxInt(pDX, m_iCamPort, 0, 65535); 
	DDX_Control(pDX, IDC_EDIT_TOTALCHANUM, m_totalChaEdit);
	DDX_Control(pDX, IDC_EDIT_CNAID, m_chaIDEdit);
	DDX_Control(pDX, IDC_COMBO_NETIP, m_netIPComboBox);
	DDX_Control(pDX, IDC_COMBO_NETDOID, m_netDoIDComboBox);


	DDX_Control(pDX, ID_CAM_ADD, m_btnAdd);
	DDX_Control(pDX, ID_CAM_MOD, m_btnModify);
	DDX_Control(pDX, ID_CAM_DEL, m_btnDel);

}


BEGIN_MESSAGE_MAP(CCamLinkSettings, CBCGPDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//{{AFX_MSG_MAP(CSysSettings)
	ON_BN_CLICKED(ID_CAM_ADD, &CCamLinkSettings::OnAddModule)
	ON_BN_CLICKED(ID_CAM_MOD, &CCamLinkSettings::OnModifyModule)
	ON_BN_CLICKED(ID_CAM_DEL, &CCamLinkSettings::OnDelModule)
	ON_NOTIFY(NM_CLICK, IDC_LISTCTRL_CAM, &CCamLinkSettings::OnNMclkQuotalist)

	//ON_EN_KILLFOCUS(IDC_LISTCTRL_NET, &CCamLinkSettings::OnEnKillfocusEditListitem)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_NETIP, &CCamLinkSettings::OnCbnSelchangeComboNetip)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysSettings message handlers

BOOL CCamLinkSettings::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();


	CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
	CBCGPButton::EnableWinXPTheme(TRUE);
	EnableVisualManagerStyle (TRUE, TRUE);	

	//
	//定额配置列表初始化
	LONG lStyle;
	lStyle = GetWindowLong(m_camLinkCtrlList.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_camLinkCtrlList.m_hWnd, GWL_STYLE, lStyle);//设置style
	DWORD dwStyle = m_camLinkCtrlList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_camLinkCtrlList.SetExtendedStyle(dwStyle); //设置扩展风格

	m_camLinkCtrlList.ShowScrollBar(SB_HORZ,FALSE);
	m_camLinkCtrlList.ShowScrollBar(SB_VERT,TRUE);


	m_camLinkCtrlList.DeleteAllItems();//清空
	//添加列
	m_camLinkCtrlList.InsertColumn(0,_T("ID号"), LVCFMT_CENTER, 40);
	m_camLinkCtrlList.InsertColumn(1,_T("摄像机IP"), LVCFMT_CENTER,80);
	m_camLinkCtrlList.InsertColumn(2,_T("端口号"), LVCFMT_CENTER, 50);
	m_camLinkCtrlList.InsertColumn(3,_T("总通道数"), LVCFMT_CENTER, 60);
	m_camLinkCtrlList.InsertColumn(4,_T("通道ID"), LVCFMT_CENTER, 50);
	m_camLinkCtrlList.InsertColumn(5,_T("网络模块IP"), LVCFMT_CENTER, 80);
	m_camLinkCtrlList.InsertColumn(6,_T("DO点号"), LVCFMT_CENTER, 60);

	m_btnModify.EnableWindow(FALSE);
	m_btnDel.EnableWindow(FALSE);

	EnterCriticalSection(&theApp.m_csCamLinkSet);
	POSITION posArea = theApp.m_mapVideoParam.GetStartPosition();

	int nRow = 0;
	while(posArea)
	{
		CString strIP = _T("");
		VIDEOPARAM* videoParam;
		theApp.m_mapVideoParam.GetNextAssoc(posArea, strIP, videoParam);

		CString strTemp = _T("");
		strTemp.Format(_T("%d"), videoParam->nCameraNo);
		m_camLinkCtrlList.InsertItem(nRow,strTemp);
		m_camLinkCtrlList.SetItemText(nRow,0,strTemp);
		strTemp = _T("");
		strTemp.Format(_T("%s"),videoParam->devinfo.szIpaddress);
		m_camLinkCtrlList.SetItemText(nRow,1,strTemp);
		strTemp = _T("");
		strTemp.Format(_T("%d"),videoParam->devinfo.nPort);
		m_camLinkCtrlList.SetItemText(nRow,2,strTemp);
		strTemp = _T("");
		strTemp.Format(_T("%d"),videoParam->devinfo.nTotalChannel);
		m_camLinkCtrlList.SetItemText(nRow,3,strTemp);
		strTemp = _T("");
		strTemp.Format(_T("%d"),videoParam->lChannel);
		m_camLinkCtrlList.SetItemText(nRow,4,strTemp);

		CString strKey = _T("");
		LinkInfoDef linkInfo;
		strKey.Format("%s,%d", videoParam->devinfo.szIpaddress, videoParam->lChannel);
		theApp.m_mapLinkInfoDef.Lookup(strKey,linkInfo);

		strTemp = _T("");
		strTemp.Format(_T("%s"),linkInfo.cNetworkSocketIP);
		m_camLinkCtrlList.SetItemText(nRow,5,strTemp);
		strTemp = _T("");
		strTemp.Format(_T("%d"),linkInfo.bNetworkNum);
		m_camLinkCtrlList.SetItemText(nRow,6,strTemp);
		nRow++;	
	}
	LeaveCriticalSection(&theApp.m_csCamLinkSet);
	theApp.CaptureNetworkDidoSocket();
	posArea = theApp.m_mapNetworkDidoSocket.GetStartPosition();

	nRow = 0;
	while(posArea)
	{
		CString strIP = _T("");
		NetworkDIDOSocket nds;
		theApp.m_mapNetworkDidoSocket.GetNextAssoc(posArea, strIP, nds);
		m_netIPComboBox.InsertString(nRow,strIP);
		nRow++;
	}
	m_netIPComboBox.SetCurSel(0);


	CString currComboStr=_T("");
	m_netIPComboBox.GetLBText(m_netIPComboBox.GetCurSel(), currComboStr);
	NetworkDIDOSocket nds={0};
	theApp.m_mapNetworkDidoSocket.Lookup(currComboStr, nds);
	theApp.ReleaseNetworkDidoSocket();

	m_netDoIDComboBox.ResetContent();
	for(int i=0; i<nds.ndc.cMaxNum;i++)
	{
		CString str;
		str.Format(_T("%d"),i);
		m_netDoIDComboBox.InsertString(i,str);
	}
	m_netDoIDComboBox.SetCurSel(0);
	return TRUE; 

}


bool CCamLinkSettings::isNum(string str)
{  
	//int first = str.find_first_of(strset);  
	//if(first == string::npos)   
	//{  
	//	return TRUE;  
	//}    
	//return FALSE;  

	stringstream sin(str);  
	double d;  
	char c;  
	if(!(sin >> d))  
		return false;  
	if (sin >> c)  
		return false;  
	return true;  
}


// 关闭对话框
void CCamLinkSettings::OnClose() 
{
	CBCGPDialog::OnClose();
}

void CCamLinkSettings::OnAddModule() 
{
	if(MessageBox(TEXT("是否添加"),TEXT("确认框"),MB_ICONINFORMATION|MB_YESNO)==IDNO)
	{
		return;
	}
	unsigned  char  *pIP;  
	CString  strIP;  
	DWORD  dwIP;
	int		iNetPort;
	BOOL	bAlarmValue = FALSE;
	int		iNetDoNum = 0;
	m_camIPCtrl.GetAddress(dwIP);  
	pIP  =  (unsigned  char*)&dwIP;  
	strIP.Format("%u.%u.%u.%u",*(pIP+3),  *(pIP+2),  *(pIP+1),  *pIP);

	CString strPort=_T("");
	m_camPortEdit.GetWindowText(strPort);
	iNetPort = atoi(strPort);

	CString strTotalChaNum=_T("");
	m_totalChaEdit.GetWindowText(strTotalChaNum);

	CString strChanID=_T("");
	m_chaIDEdit.GetWindowText(strChanID);

	CString currComboIPStr=_T("");
	m_netIPComboBox.GetLBText(m_netIPComboBox.GetCurSel(), currComboIPStr);

	CString currComboDoIDStr=_T("");
	m_netDoIDComboBox.GetLBText(m_netDoIDComboBox.GetCurSel(), currComboDoIDStr);
	int iNetDoID = _tstoi(currComboDoIDStr);

	EnterCriticalSection(&theApp.m_csCamLinkSet);
	POSITION posArea = theApp.m_mapVideoParam.GetStartPosition();
	int iCamId = 0;
	while(posArea)
	{
		CString strIP = _T("");
		VIDEOPARAM* video;
		theApp.m_mapVideoParam.GetNextAssoc(posArea, strIP, video);
		if (video->nCameraNo > iCamId)
		{
			iCamId = video->nCameraNo;
		}
	}
	LeaveCriticalSection(&theApp.m_csCamLinkSet);
	CString strInetID = _T("");
	strInetID.Format(_T("%d"),iCamId+1);

	if (strIP.Compare(_T("0.0.0.0"))==0 || iNetPort <= 0 || iNetPort > 65535)
	{
		::AfxMessageBox(_T("请输入正确的IP或端口号"));
		return;
	}

	int nRow = m_camLinkCtrlList.GetItemCount();
	m_camLinkCtrlList.InsertItem(nRow,strInetID);
	m_camLinkCtrlList.SetItemText(nRow,0,strInetID);
	m_camLinkCtrlList.SetItemText(nRow,1,strIP);
	m_camLinkCtrlList.SetItemText(nRow,2,strPort);
	m_camLinkCtrlList.SetItemText(nRow,3,strTotalChaNum);
	m_camLinkCtrlList.SetItemText(nRow,4,strChanID);
	m_camLinkCtrlList.SetItemText(nRow,5,currComboIPStr);
	m_camLinkCtrlList.SetItemText(nRow,6,currComboDoIDStr);

	EnterCriticalSection(&theApp.m_csCamLinkSet);

	DEV_INFO devInfo = {0};
	devInfo.devctype=0;
	devInfo.nTotalChannel =_tstoi(strTotalChaNum);
	devInfo.nPort = _tstoi(strPort);

	devInfo.lID = iCamId+1;
	devInfo.bSerialID = 0;		
	devInfo.nSerPort = 0;
	strcpy(devInfo.szSerIP,"");		
	strcpy(devInfo.szSerialInfo, "");//新增ddns记录		
	strcpy(devInfo.szDevName, strIP);
	strcpy(devInfo.szUserName, "admin");
	strcpy(devInfo.szPsw, "");
	strcpy(devInfo.szIpaddress, strIP);

	DEV_INFO *pDev = new DEV_INFO;
	memset( pDev, 0, sizeof(DEV_INFO) );
	memcpy( pDev, &devInfo, sizeof(DEV_INFO) );
	theApp.GetWorkspaceBar()->m_devMap[pDev->lID] = pDev;

	VIDEOPARAM* video = new VIDEOPARAM;
	video->lChannel = _tstoi(strChanID);
	video->nCameraNo = iCamId+1;
	video->devinfo = *pDev;
	video->lPlayWnd = -1;

	CString sKey = "";
	sKey.Format("%s,%d", video->devinfo.szIpaddress, video->lChannel);
	theApp.m_mapVideoParam.SetAt(sKey, video);

	LinkInfoDef lid;
	memset(&lid, 0, sizeof(LinkInfoDef));
	lid.bChannel = _tstoi(strChanID);
	lid.bNetworkNum = _tstoi(currComboDoIDStr);			//?????
	strcpy(lid.cCamIP, strIP);
	strcpy(lid.cNetworkSocketIP, currComboIPStr);
	lid.nCamID = iCamId+1;

	CString sIP = "";
	sIP.Format("%s,%d", lid.cCamIP, lid.bChannel);
	theApp.m_mapLinkInfoDef.SetAt(sIP, lid);

	LeaveCriticalSection(&theApp.m_csCamLinkSet);


	HTREEITEM hAddItem = theApp.GetWorkspaceBar()->FindTreeItem(theApp.GetWorkspaceBar()->m_wndTree.GetRootItem(),strIP);
	if (hAddItem != NULL)
	{
		HTREEITEM item = theApp.GetWorkspaceBar()->m_wndTree.InsertItem(strIP, 0, 0, hAddItem);
		//m_wndTree.SetItemData(item, (DWORD)i);
		theApp.GetWorkspaceBar()->m_wndTree.SetItemData(item, (DWORD)video);
	}
	else
	{
		HTREEITEM hAddItemNew = theApp.GetWorkspaceBar()->m_wndTree.InsertItem(strIP);
		HTREEITEM item = theApp.GetWorkspaceBar()->m_wndTree.InsertItem(strIP, 0, 0, hAddItemNew);
		//m_wndTree.SetItemData(item, (DWORD)i);
		theApp.GetWorkspaceBar()->m_wndTree.SetItemData(item, (DWORD)video);
	}
	HTREEITEM hRootItem = NULL;
	theApp.GetWorkspaceBar()->m_wndTree.Expand(hRootItem, TVE_EXPAND);
	theApp.GetWorkspaceBar()->m_wndTree.Invalidate();
	AfxGetApp()->DoWaitCursor(-1);
	SaveCamInfoToXML();
	m_curID = -1;
	m_curListRow = -1;
}

void CCamLinkSettings::OnModifyModule() 
{
	if(MessageBox(TEXT("是否修改"),TEXT("确认框"),MB_ICONINFORMATION|MB_YESNO)==IDNO)
	{
		return;
	}
	unsigned  char  *pIP;  
	CString  strIP;  
	DWORD  dwIP;
	int		iNetPort;
	BOOL	bAlarmValue = FALSE;
	int		iNetDoNum = 0;
	m_camIPCtrl.GetAddress(dwIP);  
	pIP  =  (unsigned  char*)&dwIP;  
	strIP.Format("%u.%u.%u.%u",*(pIP+3),  *(pIP+2),  *(pIP+1),  *pIP);

	CString strPort=_T("");
	m_camPortEdit.GetWindowText(strPort);
	iNetPort = atoi(strPort);

	CString strTotalChaNum=_T("");
	m_totalChaEdit.GetWindowText(strTotalChaNum);

	CString strChanID=_T("");
	m_chaIDEdit.GetWindowText(strChanID);

	CString currComboIPStr=_T("");
	m_netIPComboBox.GetLBText(m_netIPComboBox.GetCurSel(), currComboIPStr);

	CString currComboDoIDStr=_T("");
	m_netDoIDComboBox.GetLBText(m_netDoIDComboBox.GetCurSel(), currComboDoIDStr);
	int iNetDoID = _tstoi(currComboDoIDStr);


	CString strInetID = _T("");
	strInetID.Format(_T("%d"),m_curID);

	if (strIP.Compare(_T("0.0.0.0"))==0 || iNetPort <= 0 || iNetPort > 65535)
	{
		::AfxMessageBox(_T("请输入正确的IP或端口号"));
		return;
	}

	m_camLinkCtrlList.SetItemText(m_curListRow,0,strInetID);
	m_camLinkCtrlList.SetItemText(m_curListRow,1,strIP);
	m_camLinkCtrlList.SetItemText(m_curListRow,2,strPort);
	m_camLinkCtrlList.SetItemText(m_curListRow,3,strTotalChaNum);
	m_camLinkCtrlList.SetItemText(m_curListRow,4,strChanID);
	m_camLinkCtrlList.SetItemText(m_curListRow,5,currComboIPStr);
	m_camLinkCtrlList.SetItemText(m_curListRow,6,currComboDoIDStr);

	EnterCriticalSection(&theApp.m_csCamLinkSet);

	POSITION posArea = theApp.m_mapVideoParam.GetStartPosition();
	while(posArea)
	{
		CString strIP = _T("");
		VIDEOPARAM* video;
		theApp.m_mapVideoParam.GetNextAssoc(posArea, strIP, video);
		if (video->nCameraNo == m_curID)
		{
			theApp.m_mapVideoParam.RemoveKey(strIP);
			if (video != NULL)
			{
				delete video;
			}
		}
	}
	posArea = theApp.m_mapLinkInfoDef.GetStartPosition();
	while(posArea)
	{
		CString strIP = _T("");
		LinkInfoDef lid;
		theApp.m_mapLinkInfoDef.GetNextAssoc(posArea, strIP, lid);
		if ( lid.nCamID == m_curID)
		{
			theApp.m_mapLinkInfoDef.RemoveKey(strIP);
			break;
		}
	}

	DEV_INFO devInfo = {0};
	devInfo.devctype=0;
	devInfo.nTotalChannel =_tstoi(strTotalChaNum);
	devInfo.nPort = _tstoi(strPort);

	devInfo.bSerialID = 0;		
	devInfo.nSerPort = 0;
	devInfo.lID = m_curID;
	strcpy(devInfo.szSerIP,"");		
	strcpy(devInfo.szSerialInfo, "");//新增ddns记录		
	strcpy(devInfo.szDevName, strIP);
	strcpy(devInfo.szUserName, "admin");
	strcpy(devInfo.szPsw, "");
	strcpy(devInfo.szIpaddress, strIP);

	DEV_INFO *pDev = new DEV_INFO;
	memset( pDev, 0, sizeof(DEV_INFO) );
	memcpy( pDev, &devInfo, sizeof(DEV_INFO) );
	theApp.GetWorkspaceBar()->m_devMap[pDev->lID] = pDev;

	VIDEOPARAM* video = new VIDEOPARAM;
	video->lChannel = _tstoi(strChanID);
	video->nCameraNo = m_curID;
	video->devinfo = *pDev;
	video->lPlayWnd = -1;

	CString sKey = "";
	sKey.Format("%s,%d", video->devinfo.szIpaddress, video->lChannel);
	theApp.m_mapVideoParam.SetAt(sKey, video);

	LinkInfoDef lid;
	memset(&lid, 0, sizeof(LinkInfoDef));
	lid.bChannel = _tstoi(strChanID);
	lid.bNetworkNum = _tstoi(currComboDoIDStr);			//?????
	strcpy(lid.cCamIP, strIP);
	strcpy(lid.cNetworkSocketIP, currComboIPStr);
	lid.nCamID = m_curID;

	CString sIP = "";
	sIP.Format("%s,%d", lid.cCamIP, lid.bChannel);
	theApp.m_mapLinkInfoDef.SetAt(sIP, lid);

	LeaveCriticalSection(&theApp.m_csCamLinkSet);


	CString strName("");
	strName.Format("CAM %d",video->nCameraNo);
	HTREEITEM hDelItem = theApp.GetWorkspaceBar()->FindTreeItem(theApp.GetWorkspaceBar()->m_wndTree.GetRootItem(),strName);
	if (hDelItem)
	{
		theApp.GetWorkspaceBar()->m_wndTree.DeleteItem(hDelItem);
	}

	HTREEITEM hAddItem = theApp.GetWorkspaceBar()->FindTreeItem(theApp.GetWorkspaceBar()->m_wndTree.GetRootItem(),strIP);
	if (hAddItem != NULL)
	{
		HTREEITEM item = theApp.GetWorkspaceBar()->m_wndTree.InsertItem(strIP, 0, 0, hAddItem);
		//m_wndTree.SetItemData(item, (DWORD)i);
		theApp.GetWorkspaceBar()->m_wndTree.SetItemData(item, (DWORD)video);
	}
	else
	{
		HTREEITEM hAddItemNew = theApp.GetWorkspaceBar()->m_wndTree.InsertItem(strIP);
		HTREEITEM item = theApp.GetWorkspaceBar()->m_wndTree.InsertItem(strIP, 0, 0, hAddItemNew);
		//m_wndTree.SetItemData(item, (DWORD)i);
		theApp.GetWorkspaceBar()->m_wndTree.SetItemData(item, (DWORD)video);
	}
	HTREEITEM hRootItem = NULL;
	theApp.GetWorkspaceBar()->m_wndTree.Expand(hRootItem, TVE_EXPAND);
	theApp.GetWorkspaceBar()->m_wndTree.Invalidate();
	AfxGetApp()->DoWaitCursor(-1);
	SaveCamInfoToXML();
	m_curID = -1;
	m_curListRow = -1;
	
}

void CCamLinkSettings::OnDelModule()
{

	if(MessageBox(TEXT("是否删除"),TEXT("确认框"),MB_ICONINFORMATION|MB_YESNO)==IDNO)
	{
		return;
	}
	m_camLinkCtrlList.DeleteItem(m_curListRow);
	EnterCriticalSection(&theApp.m_csCamLinkSet);

	POSITION posArea = theApp.m_mapVideoParam.GetStartPosition();
	while(posArea)
	{
		CString strIP = _T("");
		VIDEOPARAM* video;
		theApp.m_mapVideoParam.GetNextAssoc(posArea, strIP, video);
		if (video->nCameraNo == m_curID)
		{
			theApp.m_mapVideoParam.RemoveKey(strIP);
			if (video != NULL)
			{
				delete video;
			}
		}
	}
	posArea = theApp.m_mapLinkInfoDef.GetStartPosition();
	while(posArea)
	{
		CString strIP = _T("");
		LinkInfoDef lid;
		theApp.m_mapLinkInfoDef.GetNextAssoc(posArea, strIP, lid);
		if ( lid.nCamID == m_curID)
		{
			theApp.m_mapLinkInfoDef.RemoveKey(strIP);
			break;
		}
	}
		LeaveCriticalSection(&theApp.m_csCamLinkSet);
		CString strName("");
		strName.Format("CAM %d",m_curID);
		HTREEITEM hDelItem = theApp.GetWorkspaceBar()->FindTreeItem(theApp.GetWorkspaceBar()->m_wndTree.GetRootItem(),strName);
		if (hDelItem)
		{
			theApp.GetWorkspaceBar()->m_wndTree.DeleteItem(hDelItem);
		}
		HTREEITEM hRootItem = NULL;
		theApp.GetWorkspaceBar()->m_wndTree.Expand(hRootItem, TVE_EXPAND);
		theApp.GetWorkspaceBar()->m_wndTree.Invalidate();
		AfxGetApp()->DoWaitCursor(-1);
		SaveCamInfoToXML();
		m_curID = -1;
		m_curListRow = -1;
}

void CCamLinkSettings::SaveCamInfoToXML() 
{
	CMarkup xml;
	if(xml.Load(GET_MODULE_FILE_INFO.strPath + "CamLinkInfo.xml"))
	{
		while(xml.FindChildElem("ip"))
		{	
			xml.RemoveChildElem();
		}
	}
	xml.OutOfElem();
	xml.Save(GET_MODULE_FILE_INFO.strPath + "CamLinkInfo.xml");
	EnterCriticalSection(&theApp.m_csCamLinkSet);
	POSITION posArea = theApp.m_mapVideoParam.GetStartPosition();
	while(posArea)
	{
		CString strIP = _T("");
		VIDEOPARAM* video;
		theApp.m_mapVideoParam.GetNextAssoc(posArea, strIP, video);

		CString strID=_T(""),strUserName=_T(""),strPsw=_T(""),strDevName=_T(""),strByChanNum=_T(""),strPort=_T("");
		CString strTotalChanNum=_T(""),strSerIP=_T(""),strSerPort=_T(""),strSerInfo=_T(""),strLoginType=_T(""),strSerID = _T("");
		CString strnetModulIp=_T(""),strnetDoDum=_T("");
		strID.Format(_T("%d"),video->nCameraNo);
		strIP.Format(_T("%s"),video->devinfo.szIpaddress);
		strPort.Format(_T("%d"),video->devinfo.nPort);
		strUserName.Format(_T("%s"),video->devinfo.szUserName);
		strPsw.Format(_T("%s"),video->devinfo.szPsw);
		strDevName.Format(_T("%s"),video->devinfo.szDevName);
		strByChanNum.Format(_T("%d"),video->lChannel);
		strTotalChanNum.Format(_T("%d"),video->devinfo.nTotalChannel);
		strSerIP.Format(_T("%s"),video->devinfo.szSerIP);
		strSerPort.Format(_T("%d"),video->devinfo.nSerPort);
		strSerID.Format(_T("%d"),video->devinfo.bSerialID);
		strSerInfo.Format(_T("%s"),video->devinfo.szSerialInfo);
		strLoginType.Format(_T("%d"),video->devinfo.devctype);
		CString strLinkKey = _T("");
		strLinkKey.Format(_T("%s,%s"),strIP,strByChanNum);
		LinkInfoDef lid;
		theApp.m_mapLinkInfoDef.Lookup(strLinkKey,lid);
		strnetModulIp.Format(_T("%s"),lid.cNetworkSocketIP);
		strnetDoDum.Format(_T("%d"),lid.bNetworkNum);

		CMarkup xml;
		if(xml.Load(GET_MODULE_FILE_INFO.strPath + "CamLinkInfo.xml"))
		{
			if(xml.FindElem("INFO"))
			{
				xml.IntoElem();
				xml.FindElem();
				xml.AddElem("ip");
				xml.IntoElem();
				xml.AddElem("ID",strID);
				xml.AddElem("ip2",strIP);
				xml.AddElem("DEVICENAME",strDevName);
				xml.AddElem("port",strPort);
				xml.AddElem("username",strUserName);
				xml.AddElem("pwd",strPsw);
				xml.AddElem("nTotalChannel",strTotalChanNum);
				xml.AddElem("byChanNum",strByChanNum);
				xml.AddElem("bSerialID",strSerID);
				xml.AddElem("szSerIP",strSerIP);
				xml.AddElem("nSerPort",strSerPort);
				xml.AddElem("szSerialInfo",strSerInfo);
				xml.AddElem("LoginType",strLoginType);
				xml.AddElem("netModulIp",strnetModulIp);
				xml.AddElem("netModulDoNum",strnetDoDum);
				xml.OutOfElem();
			}
		}
		xml.OutOfElem();
		xml.Save(GET_MODULE_FILE_INFO.strPath + "CamLinkInfo.xml");
	}

	LeaveCriticalSection(&theApp.m_csCamLinkSet);



}
BOOL CCamLinkSettings::IsIpv4str(const char *str)
{
	int i, a[4];
	char end;
	if( sscanf(str, "%d.%d.%d.%d%c", &a[0], &a[1], &a[2], &a[3], &end) != 4 )
		return FALSE;
	for(i=0; i<4; i++) if (a[i] < 0 || a[i] > 255) return FALSE;
	return TRUE;
}
void CCamLinkSettings::OnNMclkQuotalist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	CRect rc;

	if(pNMListView->iItem!=-1)
	{
		m_clickRow=pNMListView->iItem;//m_row为被选中行的行序号（int类型成员变量）
		CString strTempID = _T("");
		CString strTempIP = _T("");
		CString strTempPort = _T("");
		CString strTotalCha = _T("");
		CString strBychanNum = _T("");
		CString strNetIP = _T("");
		CString strTempDoNum = _T("");


		CString strTempAlarmValue = _T("");
		strTempID = m_camLinkCtrlList.GetItemText(pNMListView->iItem,0);//取得子项的内容
		strTempIP = m_camLinkCtrlList.GetItemText(pNMListView->iItem,1);//取得子项的内容
		strTempPort = m_camLinkCtrlList.GetItemText(pNMListView->iItem,2);//取得子项的内容
		strTotalCha = m_camLinkCtrlList.GetItemText(pNMListView->iItem,3);//取得子项的内容
		strBychanNum = m_camLinkCtrlList.GetItemText(pNMListView->iItem,4);//取得子项的内容
		strNetIP = m_camLinkCtrlList.GetItemText(pNMListView->iItem,5);//取得子项的内容
		strTempDoNum = m_camLinkCtrlList.GetItemText(pNMListView->iItem,6);//取得子项的内容
		DWORD  dwIP; 
		dwIP  =  inet_addr(strTempIP);
		unsigned  char  *pIP  =  (unsigned  char*)&dwIP;
		m_camIPCtrl.SetAddress(*pIP,  *(pIP+1),  *(pIP+2),  *(pIP+3));

		m_camPortEdit.SetWindowText(strTempPort);
		m_totalChaEdit.SetWindowText(strTotalCha);
		m_chaIDEdit.SetWindowText(strBychanNum);
		m_netIPComboBox.SetWindowText(strNetIP);
		theApp.CaptureNetworkDidoSocket();
		NetworkDIDOSocket nds={0};
		theApp.m_mapNetworkDidoSocket.Lookup(strNetIP, nds);
		theApp.ReleaseNetworkDidoSocket();
		

		m_netDoIDComboBox.ResetContent();
		for(int i=0; i<nds.ndc.cMaxNum;i++)
		{
			CString str;
			str.Format(_T("%d"),i);
			m_netDoIDComboBox.InsertString(i,str);
		}
		m_netDoIDComboBox.SetCurSel(_tstoi(strTempDoNum));
		//m_netDoIDComboBox.SetWindowText(strTempDoNum);
		m_btnModify.EnableWindow(TRUE);
		m_btnDel.EnableWindow(TRUE);
		m_curID = _tstoi(strTempID);
		m_curListRow = m_clickRow;
	}
	*pResult = 0;
}

void CCamLinkSettings::OnCbnSelchangeComboNetip()
{
	// TODO: 在此添加控件通知处理程序代码
	CString currComboStr=_T("");
	m_netIPComboBox.GetLBText(m_netIPComboBox.GetCurSel(), currComboStr);
	NetworkDIDOSocket nds={0};
	theApp.m_mapNetworkDidoSocket.Lookup(currComboStr, nds);
	theApp.ReleaseNetworkDidoSocket();

	m_netDoIDComboBox.ResetContent();
	for(int i=0; i<nds.ndc.cMaxNum;i++)
	{
		CString str;
		str.Format(_T("%d"),i);
		m_netDoIDComboBox.InsertString(i,str);
	}
	m_netDoIDComboBox.SetCurSel(0);
	theApp.ReleaseNetworkDidoSocket();
}

// NetModuleSet.cpp : implementation file
//
#include <stdlib.h>
#include "stdAfx.h"
#include "DVSShow.h"
#include "NetModuleSet.h"
#include "Markup.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDVSShowApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSysSettings dialog


CNetModuleSettings::CNetModuleSettings(CWnd* pParent /*=NULL*/)
: CBCGPDialog(CNetModuleSettings::IDD, pParent)
{
	m_iNetPort = 0;
	m_curID = -1;
	m_curListRow = -1;
	//{{AFX_DATA_INIT(CSysSettings)

	//}}AFX_DATA_INIT
}


void CNetModuleSettings::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysSettings)
	DDX_Control(pDX, IDC_LISTCTRL_NET, m_netModListCtrl);
	DDX_Control(pDX, IDC_IPADDRESS_NET, m_netIP);
	DDX_Control(pDX, IDC_EDIT_PORT, m_netPort);
	DDX_Text(pDX, IDC_EDIT_PORT, m_iNetPort);
	DDV_MinMaxInt(pDX, m_iNetPort, 0, 65535); 
	DDX_Control(pDX, IDC_COMBO_DOBUM, m_DoNumComboBox);
	DDX_Control(pDX, IDC_EDIT_LISTITEM, m_listEdit);

	DDX_Control(pDX, IDC_CHECK00, m_bAlarmValveCheckBox);
	DDX_Control(pDX, IDADD, m_btnAdd);
	DDX_Control(pDX, IDMODIFY, m_btnModify);
	DDX_Control(pDX, IDDEL, m_btnDel);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetModuleSettings, CBCGPDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//{{AFX_MSG_MAP(CSysSettings)s
	ON_BN_CLICKED(IDADD, &CNetModuleSettings::OnAddModule)
	ON_BN_CLICKED(IDMODIFY, &CNetModuleSettings::OnModifyModule)
	ON_BN_CLICKED(IDDEL, &CNetModuleSettings::OnDelModule)
	ON_NOTIFY(NM_CLICK, IDC_LISTCTRL_NET, &CNetModuleSettings::OnNMclkQuotalist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysSettings message handlers

BOOL CNetModuleSettings::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();


	CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
	CBCGPButton::EnableWinXPTheme(TRUE);
	EnableVisualManagerStyle (TRUE, TRUE);	

	//
	//定额配置列表初始化
	LONG lStyle;
	lStyle = GetWindowLong(m_netModListCtrl.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_netModListCtrl.m_hWnd, GWL_STYLE, lStyle);//设置style
	DWORD dwStyle = m_netModListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_netModListCtrl.SetExtendedStyle(dwStyle); //设置扩展风格

	m_netModListCtrl.ShowScrollBar(SB_HORZ,FALSE);
	m_netModListCtrl.ShowScrollBar(SB_VERT,TRUE);


	m_netModListCtrl.DeleteAllItems();//清空
	//添加列
	m_netModListCtrl.InsertColumn(0,_T("ID号"), LVCFMT_CENTER, 40);
	m_netModListCtrl.InsertColumn(1,_T("网络模块IP"), LVCFMT_CENTER,80);
	m_netModListCtrl.InsertColumn(2,_T("端口号"), LVCFMT_CENTER, 50);
	m_netModListCtrl.InsertColumn(3,_T("DO数量"), LVCFMT_CENTER, 50);
	m_netModListCtrl.InsertColumn(4,_T("告警值"), LVCFMT_CENTER, 50);

	m_listEdit.ShowWindow(SW_HIDE);//先隐藏编辑框
	m_btnModify.EnableWindow(FALSE);
	m_btnDel.EnableWindow(FALSE);

	m_DoNumComboBox.InsertString(0,_T("4"));
	m_DoNumComboBox.InsertString(1,_T("8"));
	m_DoNumComboBox.InsertString(2,_T("16"));
	m_DoNumComboBox.SetCurSel(0);

	theApp.CaptureNetworkDidoSocket();
	POSITION posArea = theApp.m_mapNetworkDidoSocket.GetStartPosition();

	int nRow = 0;
	while(posArea)
	{
		CString strIP = _T("");
		NetworkDIDOSocket netWorkIDDIDoSocket;
		theApp.m_mapNetworkDidoSocket.GetNextAssoc(posArea, strIP, netWorkIDDIDoSocket);

		CString strTemp = _T("");
		strTemp.Format(_T("%d"), netWorkIDDIDoSocket.ndc.nID);
		m_netModListCtrl.InsertItem(nRow,strTemp);
		m_netModListCtrl.SetItemText(nRow,0,strTemp);
		strTemp = _T("");
		strTemp.Format(_T("%s"),netWorkIDDIDoSocket.ndc.szIP);
		m_netModListCtrl.SetItemText(nRow,1,strTemp);
		strTemp = _T("");
		strTemp.Format(_T("%d"),netWorkIDDIDoSocket.ndc.nPort);
		m_netModListCtrl.SetItemText(nRow,2,strTemp);
		strTemp = _T("");
		strTemp.Format(_T("%d"),netWorkIDDIDoSocket.ndc.cMaxNum);
		m_netModListCtrl.SetItemText(nRow,3,strTemp);
		strTemp = _T("");
		strTemp.Format(_T("%d"),netWorkIDDIDoSocket.ndc.cAlarmValue);
		m_netModListCtrl.SetItemText(nRow,4,strTemp);
		nRow++;	
	}
	theApp.ReleaseNetworkDidoSocket();



	//OnSelectDoNum();
	//UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


bool CNetModuleSettings::isNum(string str)
{  
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
void CNetModuleSettings::OnClose() 
{
	CBCGPDialog::OnClose();
}

void CNetModuleSettings::OnAddModule() 
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
	m_netIP.GetAddress(dwIP);  
	pIP  =  (unsigned  char*)&dwIP;  
	strIP.Format("%u.%u.%u.%u",*(pIP+3),  *(pIP+2),  *(pIP+1),  *pIP);

	CString strPort=_T("");
	m_netPort.GetWindowText(strPort);
	iNetPort = atoi(strPort);
	CString currComboStr=_T("");
	m_DoNumComboBox.GetLBText(m_DoNumComboBox.GetCurSel(), currComboStr);
	iNetDoNum = atoi(currComboStr);
	CString strAlarmValye = _T("");
	strAlarmValye.Format(_T("%d"),m_bAlarmValveCheckBox.GetCheck());

	theApp.CaptureNetworkDidoSocket();
	POSITION posArea = theApp.m_mapNetworkDidoSocket.GetStartPosition();
	int iNetId = 0;
	while(posArea)
	{
		CString strIP = _T("");
		NetworkDIDOSocket netWorkIDDIDoSocket;
		theApp.m_mapNetworkDidoSocket.GetNextAssoc(posArea, strIP, netWorkIDDIDoSocket);
		if (netWorkIDDIDoSocket.ndc.nID > iNetId)
		{
			iNetId = netWorkIDDIDoSocket.ndc.nID;
		}
	}
	theApp.ReleaseNetworkDidoSocket();
	CString strInetID = _T("");
	strInetID.Format(_T("%d"),iNetId+1);
	
	if (strIP.Compare(_T("0.0.0.0"))==0 || iNetPort <= 0 || iNetPort > 65535)
	{
		::AfxMessageBox(_T("请输入正确的IP或端口号"));
		return;
	}
	
	CMarkup xml;
	if(xml.Load(GET_MODULE_FILE_INFO.strPath + "NetModulsInfo.xml"))
	{
		if(xml.FindElem("INFO"))
		{	
			xml.IntoElem();
			xml.FindElem();
			xml.AddElem("NetModulINFO");
			xml.IntoElem();
			xml.AddElem("ID",strInetID);
			xml.AddElem("ip",strIP);
			xml.AddElem("port",strPort);
			xml.AddElem("DoNums",currComboStr);
			xml.AddElem("DoAlarmValue",strAlarmValye);
			xml.OutOfElem();
		}
		xml.OutOfElem();

		xml.Save(GET_MODULE_FILE_INFO.strPath + "NetModulsInfo.xml");
		
		int nRow = m_netModListCtrl.GetItemCount();


		m_netModListCtrl.InsertItem(nRow,strInetID);
		m_netModListCtrl.SetItemText(nRow,0,strInetID);
		m_netModListCtrl.SetItemText(nRow,1,strIP);
		m_netModListCtrl.SetItemText(nRow,2,strPort);
		m_netModListCtrl.SetItemText(nRow,3,currComboStr);
		m_netModListCtrl.SetItemText(nRow,4,strAlarmValye);

		theApp.CaptureNetworkDidoSocket();
		NetworkDIDOSocket nds;
		SYSTEMTIME st;

		GetLocalTime(&st);

		memset(&nds, 0, sizeof(NetworkDIDOSocket));

		nds.nIndex = _tstoi(strInetID);
		nds.ndc.nID = _tstoi(strInetID);
		nds.ndc.cMaxNum = _tstoi(currComboStr);;
		nds.ndc.cAlarmValue = _tstoi(strAlarmValye);
		sprintf(nds.ndc.szName, "网络模块%02d", strInetID);
		strcpy(nds.ndc.szIP,strIP);

		for (int j=0; j<nds.ndc.cMaxNum; j++)
		{
			nds.ndc.cNum[j] = abs(_tstoi(strAlarmValye)-1);	//模块上的第n个DO状态，默认为0
		}
		nds.bInitSuccess = FALSE;
		nds.ndc.nPort = _tstoi(strPort);
		nds.st = st;
		theApp.m_mapNetworkDidoSocket.SetAt(strIP, nds);
		theApp.ReleaseNetworkDidoSocket();
		m_curID = -1;
		m_curListRow = -1;
	}
}

void CNetModuleSettings::OnModifyModule() 
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
	m_netIP.GetAddress(dwIP);  
	pIP  =  (unsigned  char*)&dwIP;  
	strIP.Format("%u.%u.%u.%u",*(pIP+3),  *(pIP+2),  *(pIP+1),  *pIP);

	CString strPort=_T("");
	m_netPort.GetWindowText(strPort);
	iNetPort = atoi(strPort);
	CString currComboStr=_T("");
	m_DoNumComboBox.GetLBText(m_DoNumComboBox.GetCurSel(), currComboStr);
	iNetDoNum = atoi(currComboStr);
	CString strAlarmValye = _T("");
	strAlarmValye.Format(_T("%d"),m_bAlarmValveCheckBox.GetCheck());

	if (strIP.Compare(_T("0.0.0.0"))==0 || iNetPort <= 0 || iNetPort > 65535)
	{
		::AfxMessageBox(_T("请输入正确的IP或端口号"));
		return;
	}

	CMarkup xml;
	if(xml.Load(GET_MODULE_FILE_INFO.strPath + "NetModulsInfo.xml"))
	{
		if(xml.FindElem("INFO"))
		{	
			xml.IntoElem();
			while(xml.FindElem("NetModulINFO"))
			{	
				xml.IntoElem();
				xml.FindElem("ID");
				int iID=atoi(xml.GetData());
				if (m_curID != iID)
				{
					xml.OutOfElem();
	/*				xml.OutOfElem();
					xml.OutOfElem();*/
					continue;
				}
				xml.FindElem("ip");
				xml.SetData(strIP);
				xml.FindElem("port");
				xml.SetData(strPort);
				xml.FindElem("DoNums");
				xml.SetData(currComboStr);
				xml.FindElem("DoAlarmValue");
				xml.SetData(strAlarmValye);
				xml.OutOfElem();
				break;
			}

		}
		xml.OutOfElem();
		xml.Save(GET_MODULE_FILE_INFO.strPath + "NetModulsInfo.xml");


		m_netModListCtrl.SetItemText(m_curListRow,1,strIP);
		m_netModListCtrl.SetItemText(m_curListRow,2,strPort);
		m_netModListCtrl.SetItemText(m_curListRow,3,currComboStr);
		m_netModListCtrl.SetItemText(m_curListRow,4,strAlarmValye);

		theApp.CaptureNetworkDidoSocket();
		POSITION posArea = theApp.m_mapNetworkDidoSocket.GetStartPosition();
		while(posArea)
		{
			CString strIP = _T("");
			NetworkDIDOSocket netWorkIDDIDoSocket;
			theApp.m_mapNetworkDidoSocket.GetNextAssoc(posArea, strIP, netWorkIDDIDoSocket);
			if (netWorkIDDIDoSocket.nIndex == m_curID)
			{
				theApp.m_mapNetworkDidoSocket.RemoveKey(strIP);
				break;
			}
		}


		NetworkDIDOSocket nds;
		SYSTEMTIME st;

		GetLocalTime(&st);

		memset(&nds, 0, sizeof(NetworkDIDOSocket));

		nds.nIndex = m_curID;
		nds.ndc.nID = m_curID;
		nds.ndc.cMaxNum = _tstoi(currComboStr);;
		nds.ndc.cAlarmValue = _tstoi(strAlarmValye);
		sprintf(nds.ndc.szName, "网络模块%02d", m_curID);
		strcpy(nds.ndc.szIP,strIP);

		for (int j=0; j<nds.ndc.cMaxNum; j++)
		{
			nds.ndc.cNum[j] = abs(_tstoi(strAlarmValye)-1);	

		}
		nds.bInitSuccess = FALSE;
		nds.ndc.nPort = _tstoi(strPort);
		nds.st = st;
		theApp.m_mapNetworkDidoSocket.SetAt(strIP, nds);
		theApp.ReleaseNetworkDidoSocket();
		m_curID = -1;
		m_curListRow = -1;
	}
}

void CNetModuleSettings::OnDelModule()
{
	if(MessageBox(TEXT("是否删除"),TEXT("确认框"),MB_ICONINFORMATION|MB_YESNO)==IDNO)
	{
		return;
	}
	CMarkup xml;
	if(xml.Load(GET_MODULE_FILE_INFO.strPath + "NetModulsInfo.xml"))
	{
		while(xml.FindChildElem("NetModulINFO"))
		{	
			xml.IntoElem();
			xml.FindChildElem("ID");
			int iID=atoi(xml.GetChildData());
			if (m_curID != iID)
			{
				xml.OutOfElem();
				continue;
			}
			xml.OutOfElem();
			xml.RemoveChildElem();
			break;
		}
		xml.Save(GET_MODULE_FILE_INFO.strPath + "NetModulsInfo.xml");


		m_netModListCtrl.DeleteItem(m_curListRow);

		theApp.CaptureNetworkDidoSocket();
		POSITION posArea = theApp.m_mapNetworkDidoSocket.GetStartPosition();
		while(posArea)
		{
			CString strIP = _T("");
			NetworkDIDOSocket netWorkIDDIDoSocket;
			theApp.m_mapNetworkDidoSocket.GetNextAssoc(posArea, strIP, netWorkIDDIDoSocket);
			if (netWorkIDDIDoSocket.nIndex == m_curID)
			{
				theApp.m_mapNetworkDidoSocket.RemoveKey(strIP);
				break;
			}
		}
		theApp.ReleaseNetworkDidoSocket();
		m_curID = -1;
		m_curListRow = -1;
	}
}

BOOL CNetModuleSettings::IsIpv4str(const char *str)
{
	int i, a[4];
	char end;
	if( sscanf(str, "%d.%d.%d.%d%c", &a[0], &a[1], &a[2], &a[3], &end) != 4 )
		return FALSE;
	for(i=0; i<4; i++) if (a[i] < 0 || a[i] > 255) return FALSE;
	return TRUE;
}
void CNetModuleSettings::OnNMclkQuotalist(NMHDR *pNMHDR, LRESULT *pResult)
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
			CString strTempDoNum = _T("");
			CString strTempAlarmValue = _T("");
			strTempID = m_netModListCtrl.GetItemText(pNMListView->iItem,0);//取得子项的内容
			strTempIP = m_netModListCtrl.GetItemText(pNMListView->iItem,1);//取得子项的内容
			strTempPort = m_netModListCtrl.GetItemText(pNMListView->iItem,2);//取得子项的内容
			strTempDoNum = m_netModListCtrl.GetItemText(pNMListView->iItem,3);//取得子项的内容
			strTempAlarmValue = m_netModListCtrl.GetItemText(pNMListView->iItem,4);//取得子项的内容
 
			DWORD  dwIP; 
			dwIP  =  inet_addr(strTempIP);
			unsigned  char  *pIP  =  (unsigned  char*)&dwIP;
			m_netIP.SetAddress(*pIP,  *(pIP+1),  *(pIP+2),  *(pIP+3));

			m_netPort.SetWindowText(strTempPort);
			m_DoNumComboBox.SetWindowText(strTempDoNum);
			int ischeck = _tstoi(strTempAlarmValue);
			m_bAlarmValveCheckBox.SetCheck(ischeck);
			m_btnModify.EnableWindow(TRUE);
			m_btnDel.EnableWindow(TRUE);
			m_curID = _tstoi(strTempID);
			m_curListRow = m_clickRow;
		}
	*pResult = 0;
}

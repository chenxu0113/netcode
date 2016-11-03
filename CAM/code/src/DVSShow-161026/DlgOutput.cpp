#include "stdafx.h"
#include "DVSShow.h"
#include "DlgOutput.h"
#include "DlgVideo1.h"
#include <IO.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		ALARM_FLASH_GAP		500			//告警闪烁间隔（ms）	
#define		TIMER_PAINT			1			//重绘定时器id

extern void CALLBACK DrawFun(long nport, HDC hDc,LONG nUser);
extern CDVSShowApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CDlgOutput dialog
void CALLBACK DrawFun(long nport, HDC hDc,LONG nUser)
{
	static HPEN penYellow = CreatePen(PS_SOLID, 2, RGB(255,255,0));
	
	CString str;
	str.Format(_T("channel%02d"),nport);
	SetTextColor(hDc, RGB(255,0,0));
	SetBkMode(hDc,TRANSPARENT);
	TextOut(hDc,10,10,str,str.GetLength());
}


CDlgOutput::CDlgOutput(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgOutput::IDD, pParent)
{
	m_nSubWndIndex = -1;
	m_lPlayHandle = -1;
	m_lChannelNum = -1;
	m_lPort = -1;
	m_nPlaydecHandle = -1;
	m_nYxIndex = -1;
	m_nWndStatus = 0;
	m_nCameraPort = -1;
	m_sCameraIP = "";
	m_bStartVideo = TRUE;
	m_bStopVideo = FALSE;
	m_bCapPic = FALSE;
	m_bTempRecord = FALSE;
	nSpeed = 4;
	nType = 0;
	m_dwBrightValue = 6;
	m_dwContrastValue = 6;
	m_dwSaturationValue = 6;
	m_dwHueValue = 6;
	m_nCamNo = -1;
	m_lPlayHandle = -1;
	m_nAlarmFlashCount = ALARM_FLASH_TIME;//告警闪烁次数，闪烁一个来回算两次

	m_bRecord = FALSE;
	m_bSound=false;

	for ( int i = 0; i < 100; i ++)
	{
		m_strInfoFrame[i] = "";
	}

	memset(&m_TSCamDef, 0, sizeof(TSCameraDef));
	m_bAlarmFlag = FALSE;
	m_dwAlarmTick = GetTickCount();	
	m_nPaintTimer = 0;
}

CDlgOutput::~CDlgOutput()
{
	KillTimer(m_nPaintTimer);
}

void CDlgOutput::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOutput)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOutput, CBCGPDialog)
	//{{AFX_MSG_MAP(CDlgOutput)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_RMENU_STOPVIDEO, OnRmenuStopvideo)
	ON_COMMAND(ID_RMENU_START_TEMPRECORD, OnRmenuStartTemprecord)
	ON_COMMAND(ID_RMENU_STOP_TEMPRECORD, OnRmenuStopTemprecord)
	ON_UPDATE_COMMAND_UI(ID_RMENU_START_TEMPRECORD, &CDlgOutput::OnUpdateRmenuStartTemprecord)
	ON_UPDATE_COMMAND_UI(ID_RMENU_STOP_TEMPRECORD, &CDlgOutput::OnUpdateRmenuStopTemprecord)
	ON_WM_TIMER()
	ON_COMMAND(ID_CANCELALARM, &CDlgOutput::OnCancelalarm)
	ON_UPDATE_COMMAND_UI(ID_CANCELALARM, &CDlgOutput::OnUpdateCancelalarm)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOutput message handlers
BOOL CDlgOutput::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();
// 	CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
// 	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
// 	CBCGPButton::EnableWinXPTheme(TRUE);
// 	EnableVisualManagerStyle (TRUE, TRUE);

	m_nPaintTimer = SetTimer(TIMER_PAINT, 200, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 左键单击视频显示窗口
void CDlgOutput::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();

	pVideo1->m_nSubWndIndex = m_nSubWndIndex;
	if (!m_bStartVideo)
	{
		m_bStopVideo = TRUE;
		m_bCapPic = TRUE;
		m_bStartVideo = FALSE;
	}
	else
	{
		m_bStopVideo = FALSE;
		m_bCapPic = FALSE;
		m_bStartVideo = TRUE;
	}

	CMainFrame* pFrame = theApp.GetMainFrame();

	for (int i=0; i<pFrame->m_nCurVideoNum; i++)
		theApp.m_dlgOutput[i].DrawOutputBorder();
	
	if (!m_bStartVideo)
	{
		if (pVideo1->m_pDlgPtzPreset)
			pVideo1->m_pDlgPtzPreset->RedrawComBox(m_TSCamDef.btType);	
	}	

	CBCGPDialog::OnLButtonDown(nFlags, point);
}

// 左键双击视频显示窗口
void CDlgOutput::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CMainFrame* pFrame = theApp.GetMainFrame();
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();

	pVideo1->m_nSubWndIndex = m_nSubWndIndex;
	pVideo1->FullScreen(m_nWndStatus);
	if (!m_bStartVideo)
	{
		m_bStopVideo = TRUE;
		m_bCapPic = TRUE;
		m_bStartVideo = FALSE;
	}
	else
	{
		m_bStopVideo = FALSE;
		m_bCapPic = FALSE;
		m_bStartVideo = TRUE;
	}
	for (int i=0; i<pFrame->m_nCurVideoNum; i++)
		theApp.m_dlgOutput[i].DrawOutputBorder();

	CBCGPDialog::OnLButtonDblClk(nFlags, point);
}


void __stdcall pProc(LONG nPort,LPCSTR pBuf,LONG nSize,LONG nWidth,LONG nHeight, LONG nStamp,LONG nType,LONG nUser)
{

}

FILE* pFile =NULL;
int __stdcall RealDataCallBack_V2(long lRealHandle,const PACKET_INFO_EX *pFrame, unsigned int dwUser)
{
	CDlgOutput *pDataChnl = (CDlgOutput*)dwUser;
	BOOL ret=H264_PLAY_InputData( pDataChnl->m_nPlaydecHandle, (unsigned char*)pFrame->pPacketBuffer, pFrame->dwPacketSize);

	return 1;
}

void CALLBACK drawOSDCall(LONG nPort,HDC hDc,LONG nUser)
{
	CDlgOutput *pThis = (CDlgOutput*)nUser;
	pThis->drawOSD(nPort,hDc);
}

int CDlgOutput::ConnectRealPlay(DEV_INFO *pDev, int nChannel)
{
	if(m_lPlayHandle != -1)
	{
		if(!H264_DVR_StopRealPlay(m_lPlayHandle, m_hWnd))
		{
			TRACE("H264_DVR_StopRealPlay fail m_iPlayhandle = %d", m_lPlayHandle);
		}
		if(m_bSound)
		{
			OnCloseSound();
		}
	}

	H264_DVR_CLIENTINFO playstru;

	m_iChannel = nChannel;
	playstru.nChannel = nChannel;
	playstru.nStream = 0;
	playstru.nMode = 0;
	playstru.hWnd = this->m_hWnd;
	

	//////////////////////////////////////////////////////////////////////////
	if ( m_nPlaydecHandle == -1 )
	{
		//open decoder
		BYTE byFileHeadBuf;
 		if (H264_PLAY_OpenStream(nChannel, &byFileHeadBuf, 1, SOURCE_BUF_MIN*50))
		{
			OSD_INFO_TXT osd;
			osd.bkColor = RGB(255,0,0);
			osd.color = RGB(255,0,0);
			osd.pos_x = 10;
			osd.pos_y = 10;
			osd.isTransparent = 1;
			osd.isBold = 1;
			strcpy(osd.text, "告警中");	//告警产生时，字幕叠加
			//H264_PLAY_SetOsdTex(nChannel, &osd);
			//设置osd叠加回调
	//		H264_PLAY_RigisterDrawFun(nChannel, drawOSDCall, (DWORD)this);
		}

		H264_PLAY_SetStreamOpenMode(nChannel, STREAME_REALTIME);	
		if ( H264_PLAY_Play(nChannel, this->m_hWnd) )
		{
			H264_PLAY_SetDisplayCallBack(nChannel, pProc, (LONG)this);
			m_nPlaydecHandle = nChannel;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	m_lPlayHandle = H264_DVR_RealPlay(pDev->lLoginID, &playstru);	
	if(m_lPlayHandle <= 0 )
	{
		DWORD dwErr = H264_DVR_GetLastError();	
		CString sTemp("");
		sTemp.Format("access %s channel%d fail, dwErr = %d",pDev->szDevName,nChannel, dwErr);
		::BCGPMessageBox(NULL, sTemp, "提示", MB_OK|MB_ICONWARNING);
	}
	else
	{		
		H264_DVR_MakeKeyFrame(pDev->lLoginID, nChannel, 0);	
		H264_DVR_SetRealDataCallBack_V2(m_lPlayHandle, RealDataCallBack_V2, (long)this);
	}
	m_lLogin = pDev->lLoginID;

	return m_lPlayHandle;
}



// 在当前选中的窗口周围画线
void CDlgOutput::DrawOutputBorder()
{
	if (!IsWindowVisible())
	{
		return;
	}

	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();
	CPen *pOldPen = NULL;
	CPen pPen;
	CRect rc(0,0,0,0);

	GetWindowRect(&rc);
	pVideo1->GetDlgItem(IDC_STATIC_PREVIEWBG)->ScreenToClient(&rc);
	if (m_bAlarmFlag && !m_bStartVideo)	//告警闪烁框
	{
		DWORD curtick = GetTickCount();
		DWORD tickgap = curtick - m_dwAlarmTick;
		int nCount = tickgap / ALARM_FLASH_GAP;
		if (nCount >= m_nAlarmFlashCount || nCount % 2 == 1)
		{
			if (pVideo1->m_nSubWndIndex == m_nSubWndIndex)
			{
				pPen.CreatePen(PS_SOLID, 2, RGB(0, 128, 255));
			}
			else
			{
				pPen.CreatePen(PS_SOLID, 2, RGB(125, 125, 116));  
			}
			if (nCount >= ALARM_FLASH_TIME)
			{
				m_bAlarmFlag = FALSE;
			}		
		}
		else
		{
			pPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));  
		}
	}
	else
	{
		if (pVideo1->m_nSubWndIndex == m_nSubWndIndex)
		{
			pPen.CreatePen(PS_SOLID, 2, RGB(0, 128, 255));
		}
		else
		{
			pPen.CreatePen(PS_SOLID, 2, RGB(125, 125, 116));  
		}
	}
	
	rc.left = rc.left - 2;
	rc.top = rc.top - 2;
	rc.right = rc.right + 2;
	rc.bottom = rc.bottom + 2;

	CDC *pDC = pVideo1->GetDlgItem(IDC_STATIC_PREVIEWBG)->GetDC();   
	ASSERT(pDC);

	pDC->SelectStockObject(NULL_BRUSH);
	pOldPen = pDC->SelectObject(&pPen);	
	pDC->Rectangle(&rc);
	
	if (pOldPen)
	{
		pDC->SelectObject(pOldPen);
	}

	ReleaseDC(pDC);
}


void CDlgOutput::drawOSD(LONG nPort,HDC hDc)
{
	m_strInfoFrame[nPort] = "ddddd";
	if ( m_strInfoFrame[nPort] != "" )
	{
		HFONT oldfont;
		oldfont = (HFONT)::GetCurrentObject( hDc, OBJ_FONT );


		//改变字体颜色
		::SetTextColor( hDc, RGB(255,0,0) );

		//更改字体
		LOGFONT lf;
		::GetObject( oldfont, sizeof(lf), &lf );
		strcpy( lf.lfFaceName, "Arial" );
		lf.lfWeight = FW_BOLD;
		HFONT out_ft;		//创建的字体对象
		out_ft = ::CreateFontIndirect( &lf );

		::SelectObject( hDc, out_ft );

		TextOut( hDc, 10, 10, "testtesttest", strlen("testtesttest") );

		::SelectObject( hDc, oldfont );
	}
}


void CDlgOutput::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DrawOutputBorder();	
}

void CDlgOutput::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);
}


void CDlgOutput::OnRButtonUp(UINT nFlags, CPoint point)
{
	CMainFrame* pFrame = theApp.GetMainFrame();
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();

	pVideo1->m_nSubWndIndex = m_nSubWndIndex;
	for (int i=0; i<pFrame->m_nCurVideoNum; i++)
		theApp.m_dlgOutput[i].DrawOutputBorder();

	if (!m_bStartVideo)
	{
		if (pVideo1->m_pDlgPtzPreset)
			pVideo1->m_pDlgPtzPreset->RedrawComBox(m_TSCamDef.btType);	
	}

	if (!m_bStartVideo)
	{
		CMenu pMenu;
		if (!pMenu.LoadMenu(IDR_MENU_RMENU1))
			return;

		// 设置临时录像的状态
		CMenu* pSubMenu = pMenu.GetSubMenu(0);
		if (m_bTempRecord)
		{
			pSubMenu->EnableMenuItem(ID_RMENU_START_TEMPRECORD, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
			pSubMenu->EnableMenuItem(ID_RMENU_STOP_TEMPRECORD, MF_BYCOMMAND|MF_ENABLED);
		}
		else
		{
			pSubMenu->EnableMenuItem(ID_RMENU_START_TEMPRECORD, MF_BYCOMMAND|MF_ENABLED);
			pSubMenu->EnableMenuItem(ID_RMENU_STOP_TEMPRECORD, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
		}

		CPoint pt;
		GetCursorPos(&pt);

		//BCG弹出菜单
		CBCGPContextMenuManager* pcm = theApp.GetContextMenuManager();
		CBCGPPopupMenu* pMenuTemp = pcm->ShowPopupMenu(pSubMenu->GetSafeHmenu(), pt.x, pt.y, this, TRUE);
	}

	CBCGPDialog::OnLButtonUp(nFlags, point);
}

void CDlgOutput::OnUpdateRmenuStartTemprecord(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bTempRecord);
}

void CDlgOutput::OnUpdateRmenuStopTemprecord(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bTempRecord);
}

void CDlgOutput::SetAlarmState()
{
	m_dwAlarmTick = GetTickCount();
	m_bAlarmFlag = TRUE;
}

void CDlgOutput::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_PAINT:
		OnPaint();
		break;
	default:
		break;
	}
	CBCGPDialog::OnTimer(nIDEvent);
}

BOOL CDlgOutput::PreTranslateMessage(MSG* pMsg)
{
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();

	if (theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_bStartVideo)
	{
		return FALSE;
	}
	if (pMsg->message == WM_KEYDOWN )
	{
		CMainFrame* pFrame = theApp.GetMainFrame();
		switch(pMsg->wParam)
		{
		case VK_UP: DLL_DVR_PTZControl(&theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_TSCamDef, 
						pFrame->m_pControl->m_btnPtzUp.m_iSubBtnIndex, (BYTE)theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].nSpeed, FALSE);
			break;
		case VK_DOWN: DLL_DVR_PTZControl(&theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_TSCamDef, 
						  pFrame->m_pControl->m_btnPtzDown.m_iSubBtnIndex, (BYTE)theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].nSpeed, FALSE);
			break;
		case VK_LEFT: DLL_DVR_PTZControl(&theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_TSCamDef, 
						  pFrame->m_pControl->m_btnPtzLeft.m_iSubBtnIndex, (BYTE)theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].nSpeed, FALSE);
			break;
		case VK_RIGHT: DLL_DVR_PTZControl(&theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_TSCamDef, 
						   pFrame->m_pControl->m_btnPtzRight.m_iSubBtnIndex, (BYTE)theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].nSpeed, FALSE);
			break;
		default: 
			break;
		}
	}

	if (pMsg->message == WM_KEYUP)
	{
		CMainFrame* pFrame = theApp.GetMainFrame();
		switch(pMsg->wParam)
		{
		case VK_UP: DLL_DVR_PTZControl(&theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_TSCamDef, 
						pFrame->m_pControl->m_btnPtzUp.m_iSubBtnIndex, (BYTE)theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].nSpeed, TRUE);
			break;
		case VK_DOWN: DLL_DVR_PTZControl(&theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_TSCamDef, 
						  pFrame->m_pControl->m_btnPtzDown.m_iSubBtnIndex, (BYTE)theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].nSpeed, TRUE);
			break;
		case VK_LEFT: DLL_DVR_PTZControl(&theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_TSCamDef, 
						  pFrame->m_pControl->m_btnPtzLeft.m_iSubBtnIndex, (BYTE)theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].nSpeed, TRUE);
			break;
		case VK_RIGHT: DLL_DVR_PTZControl(&theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_TSCamDef, 
						   pFrame->m_pControl->m_btnPtzRight.m_iSubBtnIndex, (BYTE)theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].nSpeed, TRUE);
			break;
		default: 
			break;
		}
	}
	return CBCGPDialog::PreTranslateMessage(pMsg);
}

void CDlgOutput::OnDisconnct()
{
	if (m_lPlayHandle > 0)
	{
		H264_DVR_DelRealDataCallBack_V2(m_lPlayHandle, RealDataCallBack_V2, (long)this);
		H264_DVR_StopRealPlay(m_lPlayHandle, m_hWnd);
		m_lPlayHandle = -1;
	}

	//close decoder
	if (m_nPlaydecHandle >= 0)
	{
		H264_PLAY_CloseStream(m_nPlaydecHandle);
		H264_PLAY_Stop(m_nPlaydecHandle);
		m_nPlaydecHandle = -1;
		m_lLogin = -1;
		Invalidate();
	}	

	if(m_bSound)
	{
		OnCloseSound();
	}	
	m_lLogin = -1;
	m_bStartVideo = TRUE;
	m_bStopVideo = FALSE;
	m_bCapPic = FALSE;	
	m_lChannelNum = -1;
	m_nCameraPort = -1;
	m_iChannel = -1;
	m_sCameraIP = "";
	m_nYxIndex = 0;
	m_nCamNo = 0;
}


void __stdcall CDlgOutput::videoInfoFramCallback(LONG nPort, LONG nType, LPCSTR pBuf,LONG nSize, LONG nUser)
{
	CDlgOutput *pThis = (CDlgOutput*)nUser;
	//收到信息帧, 0x03 代表GPRS信息
	if ( nType == 0x03 )
	{
		pThis->m_strInfoFrame[nPort] = pBuf;
	}
}


void CDlgOutput::GetColor(long *nBright, long *nContrast, long *nSaturation, long *nHue)
{
	if(m_lPlayHandle <= 0 )
	{
		return;
	}
	long nRegionNum = 0;
	H264_DVR_LocalGetColor(m_lPlayHandle, nRegionNum,nBright, nContrast, nSaturation, nHue);
}

void CDlgOutput::SetColor(long nBright, long nContrast, long nSaturation, long nHue)
{
	H264_DVR_LocalSetColor(m_lPlayHandle, 0, nBright, nContrast, nSaturation, nHue );
}



void CDlgOutput::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnRButtonDown(nFlags, point);
}

long CDlgOutput::GetLoginHandle()
{
	return m_lLogin;
}

long CDlgOutput::SetDevChnColor(SDK_CONFIG_VIDEOCOLOR* pVideoColor)
{
	return H264_DVR_SetDevConfig(m_lLogin,E_SDK_VIDEOCOLOR,m_iChannel,(char*)pVideoColor,sizeof(SDK_CONFIG_VIDEOCOLOR),3000);
}


void CDlgOutput::OnOpenSound()
{
	if(	H264_DVR_OpenSound(m_lPlayHandle))
	{
		m_bSound = true;
	}
}

void CDlgOutput::OnCloseSound()
{
	if( H264_DVR_CloseSound(m_lPlayHandle))
	{
		m_bSound=false;
	}
}

void CDlgOutput::SoundChangeWnd(int pCurIndex, CDlgOutput*preVideoWnd)
{
	if (pCurIndex != 1111)
	{
		if (true == preVideoWnd->m_bSound) //pre VideoWnd
		{	
			H264_DVR_CloseSound(preVideoWnd->m_lPlayHandle);
		}

		if (true == m_bSound) //cur VideoWnd
		{
			H264_DVR_OpenSound(m_lPlayHandle); 
		}
	} 
}


/////////////////////////////////////////////
// 开始临时录像
// Input: NONE
// Ouput: NONE
// Return: TRUE-SUCCESS,FALSE-FAILED
/////////////////////////////////////////////
BOOL CDlgOutput::StartLocalRecord()
{
	if (!CreateDir(theApp.m_sRecordPath))
	{
		return FALSE;
	}
	SYSTEMTIME st;
	CString sTime;
	char cFilename[MAX_PATH];

	GetLocalTime(&st);
	sTime.Format("%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay);
	
 	if(m_bStartVideo)
 		return FALSE;

	sprintf(cFilename, "\\%s_%02d_%04d-%02d-%02d_%02d-%02d-%02d_%03d.mp4",
							m_sCameraIP, m_lChannelNum, 
							st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	bool bRes = FALSE;
	char szFullPath[MAX_PATH] = {0};
	sprintf(szFullPath, "%s\\%s\\%s", theApp.m_sRecordPath, sTime, cFilename);

	bRes = H264_DVR_StartLocalRecord(m_lPlayHandle, szFullPath, MEDIA_FILE_NONE);

	m_bTempRecord = bRes;

	return bRes;
}

/////////////////////////////////////////////
// 停止临时录像
// Input: NONE
// Ouput: NONE
// Return: TRUE-SUCCESS,FALSE-FAILED
/////////////////////////////////////////////
BOOL CDlgOutput::StopLocalRecord()
{
	bool bRet = H264_DVR_StopLocalRecord(m_lPlayHandle);
	if (bRet)
	{
		m_bTempRecord = FALSE;
	}

	return bRet;
}


/////////////////////////////////////////////
// 停止录像
// Input: NONE
// Ouput: NONE
// Return: NONE
/////////////////////////////////////////////
void CDlgOutput::OnRmenuStopTemprecord() 
{
	if (!StopLocalRecord())
	{
		::BCGPMessageBox(NULL, "停止临时录像失败！", "提示", MB_OK|MB_ICONWARNING);
		return ;
	}
}

/////////////////////////////////////////////
// 关闭当前视频
// Input: NONE
// Ouput: NONE
// Return: NONE
///////////////////////////////////////////// 
void CDlgOutput::OnRmenuStopvideo() 
{
	CMainFrame* pFrame = theApp.GetMainFrame();
	pFrame->OnStopVideo();
}

/////////////////////////////////////////////
// 开始临时录像
// Input: NONE
// Ouput: NONE
// Return: NONE
/////////////////////////////////////////////
void CDlgOutput::OnRmenuStartTemprecord() 
{
	StartLocalRecord();
}

/////////////////////////////////////////////
// 创建录像文件夹
// Input: NONE
// Ouput: NONE
// Return: TRUE-SUCCESS,FALSE-FAILED
/////////////////////////////////////////////
BOOL CDlgOutput::CreateDir(CString sPath)
{
	CString sTemp;
	DWORD dwDirAttr;
	CTime time = CTime::GetCurrentTime();
	char szLan[256] = {0};

	sTemp = sPath;
	dwDirAttr = GetFileAttributes(sTemp);
	if ((dwDirAttr == -1) || !(dwDirAttr & FILE_ATTRIBUTE_DIRECTORY))
	{
		if (1 == theApp.CheckFileIsExist(sTemp))
		{
			if (!CreateDirectory(sTemp, NULL))
			{
				//				MessageBox("创建年月日目录失败！", "提示", MB_OK|MB_ICONWARNING);
				return FALSE;
			}	
		}
	}
	else
	{
		sTemp.Format(_T("%s\\%4d-%02d-%02d\\"), sPath, time.GetYear(), time.GetMonth(), time.GetDay());
		dwDirAttr=GetFileAttributes(sTemp);
		if ((dwDirAttr == -1) || !(dwDirAttr & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (1 == theApp.CheckFileIsExist(sTemp))
			{
				if (!CreateDirectory(sTemp, NULL))
				{
					//					MessageBox("创建年月日目录失败！", "提示", MB_OK|MB_ICONWARNING);
					return FALSE;
				}
			}
		}
	}

	sTemp.Format(_T("%s\\%4d-%02d-%02d\\"), sPath, time.GetYear(), time.GetMonth(), time.GetDay());
	if (0 == theApp.CheckFileIsExist(sTemp))
	{
		if (!CreateDirectory(sTemp, NULL))
			return FALSE;
	}

	return TRUE;
}


// 抓拍图片
void CDlgOutput::CapturePic()
{
	if (!CreateDir(theApp.m_sPicPath))
	{
		return;
	}

	SYSTEMTIME st;
	CString sTime;

	GetLocalTime(&st);
	sTime.Format("%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay);

	CString temp = theApp.m_sPicPath.Left(max(theApp.m_sPicPath.ReverseFind('\\'), theApp.m_sPicPath.ReverseFind('/')));
	if (temp.IsEmpty() || !PathIsDirectory(temp))
	{
		CString strTemp;
		strTemp.Format(_T("抓拍图片路径（%s）为无效的路径！"), theApp.m_sPicPath);
		::BCGPMessageBox(NULL, strTemp, "提示", MB_OK|MB_ICONWARNING);
		return;
	}
	////////////////////////
	if (!m_bStartVideo)
	{
		CString str;
		char cFilename[MAX_PATH];

		sprintf(cFilename, "%s_%02d_%04d-%02d-%02d_%02d-%02d-%02d_%03d.bmp",
			m_sCameraIP, m_lChannelNum, 
			st.wYear, st.wMonth, st.wDay, st.wHour,
			st.wMinute, st.wSecond, st.wMilliseconds);

		BOOL bRes = FALSE;
		char szFullPath[MAX_PATH] = {0};
		sprintf(szFullPath, "%s\\%s\\%s", theApp.m_sPicPath, sTime, cFilename);

		bRes = H264_DVR_LocalCatchPic(m_lPlayHandle, szFullPath);

		if (theApp.m_bCapPic)
		{
			if (bRes)
			{
				CString strTemp;
				strTemp.Format(_T("抓拍图片成功！\r\n\r\n图片保存在【%s】下！"), theApp.m_sPicPath + "\\" + sTime);
				::BCGPMessageBox(NULL, strTemp, "提示", MB_OK|MB_ICONWARNING);
			}
			else
			{
				CString strTemp;
				strTemp.Format(_T("抓拍图片失败！"));
				::BCGPMessageBox(NULL, strTemp, "提示", MB_OK|MB_ICONWARNING);
			}
		}
	}
}

/********************************************************************
function name:  void CDlgOutput::OnCancelalarm()
description:    取消前端网络DI/DO模块产生的告警，使其不再报警
input:	        none
output:	        void
returns:        none
history:        MSW 161025 ADD
notes:          拼接报文，并通过CSendThread线程下发到网络模块，网络模块回复后，由CSocketThread线程处理
*********************************************************************/
void CDlgOutput::OnCancelalarm()
{
	BOOL bRet = FALSE;
	LinkInfoDef lid;
	CString sIp = "";

	sIp.Format("%s,%d", m_sCameraIP, m_iChannel);
	memset(&lid, 0, sizeof(NetworkDIDOSocket));
	bRet = theApp.m_mapLinkInfoDef.Lookup(sIp, lid);
	if (!bRet)
	{
		::BCGPMessageBox(NULL, "未找到相关配置信息！", "提示", MB_OK|MB_ICONWARNING);
		return;
	}

	BYTE cSndBuf[LENGTH_14];
	NetworkDIDOSocket nds;
	POSITION pos = NULL;

	sIp.Format("%s", lid.cNetworkSocketIP);
	memset(&nds, 0, sizeof(NetworkDIDOSocket));

	theApp.CaptureNetworkDidoSocket();
	theApp.m_mapNetworkDidoSocket.Lookup(sIp, nds);
	theApp.ReleaseNetworkDidoSocket();

	BYTE bValue = PROTOCOL_NONE;

	for (int i=0; i<nds.ndc.cMaxNum; i++)
	{
		if (i != lid.bNetworkNum)
		{
			bValue = bValue + nds.ndc.cNum[i] * (BYTE)pow(2.0, i);//2-代表2进制
		}
		else
		{
			bValue = bValue + abs(nds.ndc.cNum[i]-1) * (BYTE)pow(2.0, i);//等于取反
		}
	}

	memset(cSndBuf, 0, LENGTH_14);
	cSndBuf[0] = PROTOCOL_NONE;
	cSndBuf[1] = PROTOCOL_EVENT_DEAL;
	cSndBuf[2] = PROTOCOL_PROTOCAL;
	cSndBuf[3] = PROTOCOL_PROTOCAL;
	cSndBuf[4] = PROTOCOL_NONE;
	cSndBuf[5] = 0x08;//6-11共8个字节
	cSndBuf[6] = PROTOCOL_UNIT;
	cSndBuf[7] = PROTOCOL_FUNC_W;
	cSndBuf[8] = PROTOCOL_NONE;
	cSndBuf[9] = PROTOCOL_ADDRESS;
	cSndBuf[10] = PROTOCOL_NONE;
	cSndBuf[11] = nds.ndc.cMaxNum;
	cSndBuf[12] = PROTOCOL_DATA_LENGTH;
	cSndBuf[13] = bValue;

	if (theApp.m_pSendThread)
	{
		theApp.m_pSendThread->SendSocketMsg(nds.nIndex, cSndBuf, LENGTH_14);
	}
	//取消告警闪烁框
	m_nAlarmFlashCount = 0;
}

void CDlgOutput::OnUpdateCancelalarm(CCmdUI *pCmdUI)
{
	BOOL bRet = FALSE;
	LinkInfoDef lid;
	CString sIp = "";

	sIp.Format("%s,%d", m_sCameraIP, m_iChannel);
	memset(&lid, 0, sizeof(NetworkDIDOSocket));
	bRet = theApp.m_mapLinkInfoDef.Lookup(sIp, lid);

	//////////////////////////////////////////////////////////////////////////
	NetworkDIDOSocket nds;

	memset(&nds, 0, sizeof(NetworkDIDOSocket));
	sIp.Format("%s", lid.cNetworkSocketIP);

	theApp.CaptureNetworkDidoSocket();
	theApp.m_mapNetworkDidoSocket.Lookup(sIp, nds);
	theApp.ReleaseNetworkDidoSocket();

	pCmdUI->Enable(bRet & (nds.ndc.cNum[lid.bNetworkNum]==nds.ndc.cAlarmValue));
}

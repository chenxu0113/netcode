// DlgVideo1.cpp : implementation file
//

#include "stdafx.h"
#include "DVSShow.h"
#include "DlgVideo1.h"
#include <math.h>
#include "DlgPTZPreset.h"
#include "DVSShowView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CDVSShowApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CDlgVideo1

IMPLEMENT_DYNCREATE(CDlgVideo1, CBCGPFormView)

CDlgVideo1::CDlgVideo1()
: CBCGPFormView(CDlgVideo1::IDD)
{
	m_nYxIndex = 0;
	m_nSubWndIndex = -1;
	m_nCurChanNum = -1;
	m_nTimeSGVideo = -1;
	m_nTimeAskDIDOStatus = -1;
	m_nVideoNum = 0;
	m_sCurCamIP = "";
	m_pDlgPtzPreset = NULL;

	m_bFullScreen = FALSE;
//	m_pDlgPtzPreset = new CDlgPTZPreset;
//	BOOL b = m_pDlgPtzPreset->Create(IDD_DLG_PTZPRESET, NULL);
//	m_pDlgPtzPreset->ShowWindow(SW_HIDE);
}

CDlgVideo1::~CDlgVideo1()
{
	ExitSDk();
	if (m_pDlgPtzPreset != NULL)
	{
		delete m_pDlgPtzPreset;
		m_pDlgPtzPreset = NULL;
	}
}

void CDlgVideo1::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVideo1)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVideo1, CBCGPFormView)
	//{{AFX_MSG_MAP(CDlgVideo1)
	ON_WM_SIZE()
	ON_COMMAND(ID_RMENU_CONTROL, OnRmenuControl)
	ON_COMMAND(ID_VIDEO_CAPPIC, OnVideoCappic)
	ON_COMMAND(ID_RMENU_PRESET, OnRmenuPreset)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVideo1 diagnostics

#ifdef _DEBUG
void CDlgVideo1::AssertValid() const
{
	CFormView::AssertValid();
}

void CDlgVideo1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDlgVideo1 message handlers
void CDlgVideo1::OnInitialUpdate() 
{
	CBCGPFormView::OnInitialUpdate();

	//initialize the SDK
	InitSDK();

	//ȥ��������
	CSize sizeTotal; 
	sizeTotal.cx = 0; 
	sizeTotal.cy = 0; 
	SetScrollSizes(MM_TEXT, sizeTotal); 
	GetParentFrame()->RecalcLayout();
	//////////////////////////////////

	InitChildWindow();

	// ��ʼ����Ƶ��Ĭ����������Ϊ1��
	int i = 0;
	for (i=0; i<MAX_VIDEO_NUM; i++)
	{
		m_nCamParam[0][i] = -1;		// ң��������>=0���±�ֵ
		m_nCamParam[1][i] = -1;		// ����������ȼ�
		m_nCamParam[2][i] = 0;		// ��������
		m_nCamParam[3][i] = -1;		// ��Ƶ���ڴ��ں�
		m_nCamParam[4][i] = 0;		// ң��������>=0�Ķ�ʱ���ۼ�ֵ
		m_nCamNo[0][i] = -1;
		m_nCamNo[1][i] = 0;
		theApp.m_dlgOutput[i].m_bStopVideo = FALSE;
		theApp.m_dlgOutput[i].m_bStartVideo = TRUE;
		theApp.m_dlgOutput[i].m_bCapPic = FALSE;
		//////////////////////////////////////////////////
		theApp.m_dlgOutput[i].m_dwBrightValue = 6;
		theApp.m_dlgOutput[i].m_dwContrastValue = 6;
		theApp.m_dlgOutput[i].m_dwSaturationValue = 6;
		theApp.m_dlgOutput[i].m_dwHueValue = 6;
		////////////////////////////////////////////////////	
	}
	m_nSubWndIndex = 0;

	m_nTimeSGVideo = SetTimer(TIMER_SGVIDEO, 1000, NULL);
	m_nTimeAskDIDOStatus = SetTimer(TIMER_ASKDIDOSTATUS, 2000, NULL);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

// 	int nNum = WND_NUM_4;
// 	pFrame->m_nCurVideoNum = nNum;
// 	ArrageOutputWnds(nNum);
	pFrame->m_wndWorkSpace.m_wndTree.SelectItem(pFrame->m_wndWorkSpace.m_hCamera);
}

void CDlgVideo1::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPFormView::OnSize(nType, cx, cy);

	CMainFrame* pFrame = theApp.GetMainFrame();
	if (pFrame)
	{
		pFrame->m_nCurVideoNum = ArrageOutputWnds(pFrame->m_nCurVideoNum);
	}
}

/////////////////////////////////////////////
// ��ʼ�����Ӵ���
// Input: NONE
// Ouput: NONE
// Return: NONE
/////////////////////////////////////////////
void CDlgVideo1::InitChildWindow()
{
	int i = 0;
	CString str;
	
	for (i=0; i<WND_NUM_16; i++)
	{
		theApp.m_dlgOutput[i].m_nSubWndIndex = i;
		theApp.m_dlgOutput[i].Create(IDD_DLG_OUTPUT, this);
		theApp.m_dlgOutput[i].ShowWindow(SW_HIDE);
		str.Format(_T("VIDEO %02d"), i+1);
		theApp.m_dlgOutput[i].GetDlgItem(IDC_STATIC_FLAG)->SetWindowText(str);
	}
}

/////////////////////////////////////////////
// ���������ڴ�С
// Input: nWndNum-��Ҫ��ʾ����Ƶ����
// Ouput: NONE
// Return: ����ʵ�ʴ��ڸ���
/////////////////////////////////////////////
UINT CDlgVideo1::ArrageOutputWnds(UINT nWndNum)
{
	if (nWndNum > MAX_VIDEO_NUM)
	{
		::BCGPMessageBox(NULL, "������󴰿���16��", "��ʾ", MB_OK|MB_ICONWARNING);
		return 0;
	}
	CRect rc;
	int nCol = 2;
	int nRow = 2;
	UINT nNum = (int)(sqrt(fabs(nWndNum*1.0)));

	if (nNum * nNum == nWndNum)
	{
		nCol = nNum;
		nRow = nNum;
	}
	
	else if ((nWndNum > (nNum * nNum)) && (nWndNum < (nNum+1)*(nNum+1)))
	{
		nCol = nNum + 1;
		nRow = nNum + 1;
	}

	int i = 0, j = 0, k = 0;
	int nWidth = 0, nHeight = 0;

	for (i=0; i<nCol*nRow; i++)		
	{
		int nShow = ((!m_bFullScreen)||(i==m_nSubWndIndex)) ? SW_SHOW : SW_HIDE;
		theApp.m_dlgOutput[i].ShowWindow(nShow);
	}
	for (i=nCol*nRow; i<MAX_VIDEO_NUM; i++)
		theApp.m_dlgOutput[i].ShowWindow(SW_HIDE);

	GetWindowRect(rc);
	nWidth = rc.Width()/nCol;
	nHeight = rc.Height()/nRow;

	k = 0;
	for (i=0; i<nRow; i ++)
	{
		for (j=0; j<nCol; j++)
		{
			if (k == m_nSubWndIndex && m_bFullScreen)
			{
				m_rcPreview.SetRect(j*nWidth+3, i*nHeight+3, (j+1)*nWidth-1, (i+1)*nHeight-1);
				GetWindowRect(rc);			
				theApp.m_dlgOutput[k].MoveWindow(0, 0, rc.Width(), rc.Height());
				k++;
			}
			else
			{
				theApp.m_dlgOutput[k++].MoveWindow((j%nCol)*nWidth+3, (i%nRow)*nHeight+3, nWidth-4, nHeight-4, TRUE);
			}		
		}
	}

	Invalidate(TRUE);
	if (nWndNum == 1)
		m_nSubWndIndex = 0;

	return nRow * nCol;
}



UINT CDlgVideo1::StartRTVideoEx(VIDEOPARAM* video, int nYxIndex, 
							  BOOL bManual /*= TRUE*/, int nStyle /*= STYLE_FLASH*/)
{
	CMainFrame* pFrame = theApp.GetMainFrame();
	if (NULL == pFrame)
	{
		return 0;
	}
	
	BOOL bFlag = FALSE;
	UINT uReturn = 1;

	if (video->nCameraNo == -1)
	{
		bFlag = TRUE;
		video->nCameraNo = GetCamNoByYXIndex(nYxIndex);
	}

	//������ֶ�ѡ��ģ���ȡ���豸��������
	int nWndIndex = m_nSubWndIndex;
	POSITION pos = theApp.m_mapVideoParam.GetStartPosition();
	while(pos)
	{
		CString strKey =_T("");
		VIDEOPARAM* videoFind = NULL;
		theApp.m_mapVideoParam.GetNextAssoc(pos, strKey, videoFind);
		if (videoFind->lPlayWnd == nWndIndex)
		{
			videoFind->lPlayWnd = -1;
			break;
		}
	}

	video->lPlayWnd = nWndIndex;
	if (!bManual)	//�����ֶ����Զ�Ѱ�ҿհ״���
	{
//		nWndIndex = GetSpaceWndNum();
		m_nCamParam[3][nWndIndex] = nWndIndex;
	}
//	video->hWnd = theApp.m_dlgOutput[nWndIndex].m_hWnd;

	EnterCriticalSection(&theApp.m_csMapCCamWndMgr);
	CCamWndMgr *pMgr = NULL;
	map<int, CCamWndMgr *>::iterator it = theApp.m_MapCCamWndMgr.find(video->nCameraNo);
	if (it == theApp.m_MapCCamWndMgr.end())
	{
		pMgr = new CCamWndMgr;
		pMgr->m_nCameraNo = video->nCameraNo;
		theApp.m_MapCCamWndMgr[video->nCameraNo] = pMgr;
	}
	else
	{
		pMgr = it->second;
		if (bManual)
		{
			//�ֶ��㿪��Ƶ
			if (pMgr->GetAttachedWndNum() >= 3)
			{
				//һ�����������ֶ���3��
				LeaveCriticalSection(&theApp.m_csMapCCamWndMgr);
				::BCGPMessageBox("һ�����������ֶ���3����Ƶ��", MB_OK|MB_ICONWARNING);
				return 0;
			}
		}
		else
		{
			CDlgOutput *pOutPut = NULL;
			pOutPut = pMgr->GetAttachedWnd(FIRST_WND);
			if (pOutPut != NULL)
			{
				if (nStyle == STYLE_FULLSCREEN)
				{
					CMainFrame* pFrame = theApp.GetMainFrame();
					//�Ȱ��Ѿ������Ĵ��ڸ�ԭ
					for (int i=0; i<pFrame->m_nCurVideoNum; i++)
					{
						if (theApp.m_dlgOutput[i].m_nWndStatus == 1)
						{
							theApp.m_dlgOutput[i].OnLButtonDblClk(0, CPoint(0, 0));
						}
					}
					//�ٰ�Ҫ��ʾ�Ĵ�������
					pOutPut->OnLButtonDblClk(0, CPoint(0, 0));
				}
				else 
				{
					pOutPut->SetAlarmState();
				}

// 				if (bFlag && nYxIndex>-1)
// 				{
// 					if (m_pDlgPtzPreset)
// 					{
// 						m_pDlgPtzPreset->RedrawComBox(pOutPut->m_TSCamDef.btType);
// 						m_pDlgPtzPreset->m_nCurSel = GetPtzPresetByYxIndex(nYxIndex) - 1;				
// 						m_pDlgPtzPreset->MoveToPtzPreSet(m_pDlgPtzPreset->m_nCurSel, pOutPut->m_nSubWndIndex);
// 					}
// 					if (theApp.m_bnStartTempRec || theApp.m_bsStartTempRec)
// 						theApp.m_dlgOutput[theApp.GetDlgVideo1()->m_nSubWndIndex].StartLocalRecord();
// 				}
				LeaveCriticalSection(&theApp.m_csMapCCamWndMgr);
//				return 0;
			}
		}
	}
	LeaveCriticalSection(&theApp.m_csMapCCamWndMgr);

	//////////////////////////////////////////////////

//	theApp.m_dlgOutput[nWndIndex].m_TSCamDef.btType = video->nType;
	BOOL bRes = TRUE;

	//login��˫��������������
	if(video->devinfo.lLoginID <= 0)
	{				
		long loginID = DevLogin(&video->devinfo);
		if (loginID > 0)
		{					
			video->devinfo.lLoginID = loginID;
			video->devinfo.lID = (long)&video->devinfo;
			theApp.GetWorkspaceBar()->m_devMap[video->devinfo.lID] = &video->devinfo;
		}
		else 
		{
			pFrame->m_bIsCameraNode = FALSE;
			return 1;
		}
	}
	//logout
	// 		else
	// 		{
	// 			DEV_INFO* pDev = (DEV_INFO *)m_wndTree.GetItemData(parentItem);
	// 			H264_DVR_Logout(pDev->lLoginID);
	// 			pDev->lLoginID=-1;
	// 			pDev->lID = (long)pDev;
	// 			m_devMap[pDev->lID] = pDev;
	// 		}
	pFrame->m_bIsCameraNode = TRUE;

	//channel node
	bRes = Connect(&video->devinfo, video->lChannel, nWndIndex);
	if (!bRes)
	{
		return FALSE;
	}
	//���ø澯���ߵ���˸����
	if (!bManual)	//��紥�����Զ���������Ƶ
	{
		theApp.m_dlgOutput[nWndIndex].m_nAlarmFlashCount = ALARM_FLASH_TIME;
	}
	if (bRes)
	{
		theApp.m_dlgOutput[nWndIndex].m_bStartVideo = FALSE;
		theApp.m_dlgOutput[nWndIndex].m_bStopVideo = TRUE;
		theApp.m_dlgOutput[nWndIndex].m_bCapPic = TRUE;	
		theApp.m_dlgOutput[nWndIndex].m_lChannelNum = video->lChannel;
		theApp.m_dlgOutput[nWndIndex].m_nCameraPort = video->devinfo.nPort;
		theApp.m_dlgOutput[nWndIndex].m_sCameraIP.Format(_T("%s"), video->devinfo.szIpaddress);
		theApp.m_dlgOutput[nWndIndex].m_nYxIndex = nYxIndex;
		theApp.m_dlgOutput[nWndIndex].m_nCamNo = video->nCameraNo;

		//������ʱ¼��
		if (theApp.m_bnStartTempRec && !bManual)
		{
			theApp.m_dlgOutput[m_nSubWndIndex].StartLocalRecord();
		}

		if (theApp.m_dlgOutput[nWndIndex].m_bCapPic)
		{
			ASSERT(pMgr != NULL);
			pMgr->AttachWnd(nWndIndex, &theApp.m_dlgOutput[nWndIndex]);
			if (!bManual)
			{
				if (nStyle == STYLE_FULLSCREEN)
				{
					CMainFrame* pFrame = theApp.GetMainFrame();
					//�Ȱ��Ѿ������Ĵ��ڸ�ԭ
					for (int i=0; i<pFrame->m_nCurVideoNum; i++)
					{
						if (theApp.m_dlgOutput[i].m_nWndStatus == 1)
						{
							theApp.m_dlgOutput[i].OnLButtonDblClk(0, CPoint(0, 0));
						}
					}
					//�ٰ�Ҫ��ʾ�Ĵ�������
					theApp.m_dlgOutput[nWndIndex].OnLButtonDblClk(0, CPoint(0, 0));
				}
				else 
				{
					theApp.m_dlgOutput[nWndIndex].SetAlarmState();
				}			
			}
		}

		
		///////////////////////////////
		if (bFlag && nYxIndex>-1)
		{
			if (m_pDlgPtzPreset)
			{
				m_pDlgPtzPreset->RedrawComBox(theApp.m_dlgOutput[nWndIndex].m_TSCamDef.btType);
				m_pDlgPtzPreset->m_nCurSel = GetPtzPresetByYxIndex(nYxIndex) - 1;		
				m_pDlgPtzPreset->MoveToPtzPreSet(m_pDlgPtzPreset->m_nCurSel, nWndIndex);
			}
		}
	}

	if (!bRes && bManual)
	{
		::BCGPMessageBox(IDS_STRING130, IDS_STRING132);
	}
	return uReturn;
}

int CDlgVideo1::Connect(DEV_INFO *pDev, int nChannel, int nWndIndex /* = -1 */)
{
	int nRet = 0;

	if ( !pDev )
	{
		return nRet;
	}

	//if device did not login,login first
	if ( pDev->lLoginID <= 0 )
	{
		H264_DVR_DEVICEINFO OutDev;
		int nError = 0;
		//���ó��������豸�����͵ȴ�ʱ��
		H264_DVR_SetConnectTime(3000, 1);//���ó�������1�Σ��ȴ�ʱ��3s
		UpdateData();
		long lLogin=0;

		lLogin = H264_DVR_Login(pDev->szIpaddress, (unsigned short)pDev->nPort, pDev->szUserName, pDev->szPsw,
			&OutDev, &nError,(SocketStyle)pDev->devctype);

		if ( lLogin <= 0 )
		{
			CString sTemp;	
			int nErr = H264_DVR_GetLastError();  
			if(nErr == H264_DVR_PASSWORD_NOT_VALID)
			{
				MessageBox("Error.PwdErr");
			}
			else
			{
				MessageBox("Error.NotFound");
			}

			return nRet;
		}

		pDev->lLoginID = lLogin;
		H264_DVR_SetupAlarmChan(lLogin);
	}

	int nWnd = 0;
	if ( nWndIndex >= 0 )
	{
		nWnd = nWndIndex;
	}

	if ( nWnd >= MAX_VIDEO_NUM )
	{
		return nRet ;
	}

	return theApp.m_dlgOutput[nWnd].ConnectRealPlay(pDev, nChannel);	
}


void CDlgVideo1::StopRTVideoEx(int nWndIndex)
{
	POSITION pos = theApp.m_mapVideoParam.GetStartPosition();
	while(pos)
	{
		CString strKey =_T("");
		VIDEOPARAM* videoFind = NULL;
		theApp.m_mapVideoParam.GetNextAssoc(pos, strKey, videoFind);
		if (videoFind->lPlayWnd == nWndIndex)
		{
			videoFind->lPlayWnd = -1;
			break;
		}
	}
	VIDEOPARAM videoParam;
	UINT nRet = 0;

	m_nCamParam[3][nWndIndex] = -1;
	if (!theApp.m_dlgOutput[nWndIndex].m_bStartVideo)
	{
		if (theApp.m_dlgOutput[nWndIndex].m_bTempRecord)
			theApp.m_dlgOutput[nWndIndex].StopLocalRecord();
		videoParam.devinfo.nPort = theApp.m_dlgOutput[nWndIndex].m_nCameraPort;

		theApp.m_dlgOutput[nWndIndex].OnDisconnct();
		
		EnterCriticalSection(&theApp.m_csMapCCamWndMgr);
		CCamWndMgr *pMgr = NULL;
		map<int, CCamWndMgr *>::iterator it = theApp.m_MapCCamWndMgr.find(theApp.m_dlgOutput[nWndIndex].m_nCamNo);
		if (it != theApp.m_MapCCamWndMgr.end())
		{
			pMgr = it->second;
			pMgr->DetachWnd(nWndIndex);
		}
		LeaveCriticalSection(&theApp.m_csMapCCamWndMgr);
		//////////////////////////

		theApp.m_dlgOutput[nWndIndex].Invalidate(TRUE);
		theApp.m_dlgOutput[nWndIndex].m_bStartVideo = TRUE;
		theApp.m_dlgOutput[nWndIndex].m_bStopVideo = FALSE;
		theApp.m_dlgOutput[nWndIndex].m_bCapPic = FALSE;
		theApp.m_dlgOutput[nWndIndex].m_lPlayHandle = -1;
		theApp.m_dlgOutput[nWndIndex].m_lChannelNum = -1;
		theApp.m_dlgOutput[nWndIndex].m_nCameraPort = -1;
		theApp.m_dlgOutput[nWndIndex].m_nYxIndex = -1;
		theApp.m_dlgOutput[nWndIndex].m_sCameraIP = "";
		theApp.m_dlgOutput[nWndIndex].DrawOutputBorder();
	}
}



int CDlgVideo1::GetSpaceWndNum()
{
	int i = 0;
	CMainFrame* pFrame = theApp.GetMainFrame();
	for (i=0; i<pFrame->m_nCurVideoNum; i++)
	{
		if (theApp.m_dlgOutput[i].m_bStartVideo)
			return i;
	}
	return 0;
}

/////////////////////////////////////////////
// ��ʱ��
// Input: NONE
// Ouput: NONE
// Return: NONE
/////////////////////////////////////////////
void CDlgVideo1::OnTimer(UINT nIDEvent) 
{
	CString str;

	if (nIDEvent == TIMER_ASKDIDOSTATUS)
	{
		//��ʱ����������ͱ��ģ���ȡ����ģ��DI/DO�����״̬
		theApp.m_pSendThread->AskDiStatus();
	}
	else if (nIDEvent == TIMER_SGVIDEO)
	{
		if (theApp.m_bAlarmVideo)		// ���ø澯����Ƶ
		{
			int i = 0, j = 0, nStopRec = 0, nStopVideo = 0;
			for (i=0; i<MAX_ROWCOL * MAX_ROWCOL; i++)
			{
				if (theApp.m_dlgOutput[i].m_bStartVideo)
				{
 					m_nCamParam[4][i] = 0;
// 					m_nCamParam[3][i] = -1;
					continue ;
				}
				m_nCamParam[4][i] ++;

				nStopRec = theApp.m_nStopRec;
				nStopVideo = theApp.m_nStopVideo;
	
				if (m_nCamParam[4][i] == nStopRec)		// ֹͣ¼��
				{
					theApp.m_dlgOutput[m_nCamParam[3][i]].StopLocalRecord();
				}
				if (m_nCamParam[4][i] == nStopVideo)// && theApp.m_dlgOutput[m_nCamParam[3][i]].m_nYxIndex >= 0)// �ر���Ƶ
				{
					if (m_nCamParam[3][i] >= 0) 
                    {
						StopRTVideoEx(m_nCamParam[3][i]);
						theApp.m_dlgOutput[m_nCamParam[3][i]].StopLocalRecord();
                        m_nCamParam[4][i] = 0;
                        m_nCamParam[3][i] = -1;
                    }                 
				}
			}
		}
	}
	CBCGPFormView::OnTimer(nIDEvent);
}



/////////////////////////////////////////////
// ͨ��ң�������Ż�ȡ��������
// Input: nYxIndex-ң��������
// Ouput: NONE
// Return: ң��������
/////////////////////////////////////////////
int CDlgVideo1::GetCamNoByYXIndex(int nYxIndex)
{
	CameraLinkage gr;
	POSITION pos = theApp.m_listGangedRule.GetHeadPosition();

	while (pos != NULL)
	{
		gr = theApp.m_listGangedRule.GetNext(pos);
		if (gr.nDIIndex == nYxIndex)
			return gr.nCameraID;
	}

	return -1;
}

/////////////////////////////////////////////
// ͨ��ң�������Ż�ȡԤ�õ��
// Input: nYxIndex-ң��������
// Ouput: NONE
// Return: Ԥ�õ��
/////////////////////////////////////////////
int CDlgVideo1::GetPtzPresetByYxIndex(int nYxIndex)
{
	CameraLinkage gr;
	POSITION pos = theApp.m_listGangedRule.GetHeadPosition();

	while (pos != NULL)
	{
		gr = theApp.m_listGangedRule.GetNext(pos);
		if (gr.nDIIndex == nYxIndex)
			return gr.nPtzPreset;
	}

	return -1;
}

/////////////////////////////////////////////
// ���ô����Ƿ����
// Input: nWndStatus-���������
// Ouput: NONE
// Return: NONE
/////////////////////////////////////////////
WINDOWPLACEMENT m_OldWndPlacement;
CRect m_FullScreenRect;
BOOL m_bFullScreen;

void CDlgVideo1::FullScreen(BYTE nWndStatus)
{
	if (nWndStatus == 0)		// ��������VIEW
	{
		theApp.m_dlgOutput[m_nSubWndIndex].m_nWndStatus = 1;
		m_bFullScreen = TRUE;
		CRect rc, rcPrev;
		GetWindowRect(rc);			
		theApp.m_dlgOutput[m_nSubWndIndex].GetWindowRect(m_rcPreview);
		ScreenToClient(m_rcPreview);
		theApp.m_dlgOutput[m_nSubWndIndex].MoveWindow(0, 0, rc.Width(), rc.Height());
		ShowVideoWindow(1);
		theApp.m_dlgOutput[m_nSubWndIndex].Invalidate(TRUE);
	}
	else if (nWndStatus == 1)	// ��������
	{
		theApp.m_dlgOutput[m_nSubWndIndex].m_nWndStatus = 0;
		m_bFullScreen = FALSE;
		theApp.m_dlgOutput[m_nSubWndIndex].MoveWindow(m_rcPreview);
		ShowVideoWindow(0);
		theApp.m_dlgOutput[m_nSubWndIndex].Invalidate(TRUE);
	}
}

/////////////////////////////////////////////
// ���ش���
// Input: bStatus-��ʾ������
// Ouput: NONE
// Return: NONE
/////////////////////////////////////////////
void CDlgVideo1::ShowVideoWindow(BYTE bStatus)
{
	int i = 0;
	BOOL bShowStatus = bStatus ? SW_HIDE : SW_SHOW;

	CMainFrame* pFrame = theApp.GetMainFrame();
	for (i=0; i<pFrame->m_nCurVideoNum; i++)
	{
		if (m_nSubWndIndex != i)
			theApp.m_dlgOutput[i].ShowWindow(bShowStatus);
	}
}

/////////////////////////////////////////////
// ��̨���ư�ť
// Input: NONE
// Ouput: NONE
// Return: NONE
/////////////////////////////////////////////
void CDlgVideo1::OnRmenuControl() 
{
	CMainFrame* pFrame = theApp.GetMainFrame();
	pFrame->OnControl();
}


/////////////////////////////////////////////
// ץ��ͼƬ
// Input: NONE
// Ouput: NONE
// Return: NONE
///////////////////////////////////////////// 
void CDlgVideo1::OnVideoCappic() 
{
	theApp.m_dlgOutput[theApp.GetDlgVideo1()->m_nSubWndIndex].CapturePic();
}

/////////////////////////////////////////////
// Ԥ�õ�����
// Input: NONE
// Ouput: NONE
// Return: NONE
/////////////////////////////////////////////
void CDlgVideo1::OnRmenuPreset() 
{
	if (NULL == m_pDlgPtzPreset)
		return;

	BOOL bVisible = FALSE;
	bVisible = m_pDlgPtzPreset->IsWindowVisible();
	m_pDlgPtzPreset->ShowWindow(!bVisible);
	if (!bVisible)
	{
		CWnd *pWnd = NULL;
		pWnd = (CWnd*)m_pDlgPtzPreset;
		if(pWnd == NULL)
			return ;
		pWnd->SetForegroundWindow();
		CRect rc, rc2;
		pWnd->GetWindowRect(rc2);
		GetWindowRect(rc);
		::SetWindowPos(pWnd->m_hWnd, HWND_TOPMOST, (rc.Width()-rc2.Width())/2,
			(rc.Height()-rc2.Height())/2,
			rc2.Width(), rc2.Height(), SWP_SHOWWINDOW);
	}
}


//message callback function
bool __stdcall MessCallBack(long lLoginID, char *pBuf,
							unsigned long dwBufLen, long dwUser)
{

	return true;
}

//device disconnect callback
void __stdcall DisConnectBackCallFunc(LONG lLoginID, char *pchDVRIP, 
									  LONG nDVRPort, DWORD dwUser)
{
	//ReConnect(lLoginID, pchDVRIP, nDVRPort);
}

//initialize the SDK
BOOL CDlgVideo1::InitSDK()
{
	//initialize
	BOOL bResult = H264_DVR_Init((fDisConnect)DisConnectBackCallFunc,(DWORD)this);

	//he messages received in SDK from DVR which need to upload�� such as alarm information��diary information��may do through callback function
	H264_DVR_SetDVRMessCallBack(MessCallBack,(DWORD)this);

	H264_DVR_SetConnectTime(5000, 3);

	return bResult;
}

long CDlgVideo1::DevLogin(DEV_INFO* pdev)
{
	if(pdev->bSerialID)//���֮ǰ��DDNS��ȡip;�����Ȼ�ȡ��̬ip
	{
		int maxDeviceNum = 1000;  //���֧���豸����1000
		DDNS_INFO *pDDNSInfo = new DDNS_INFO[maxDeviceNum];
		SearchMode searchmode;
		int nReNum = 0;  //ʵ�ʻ�õ��豸����		
		searchmode.nType = DDNS_SERIAL;		
		strcpy(searchmode.szSerIP,pdev->szSerIP);
		searchmode.nSerPort = pdev->nSerPort ;
		strcpy(searchmode.szSerialInfo, pdev->szSerialInfo);
		CString strTemp;
		BOOL bret = H264_DVR_GetDDNSInfo(searchmode, pDDNSInfo, maxDeviceNum, nReNum);
		if ( !bret )
		{
			delete pDDNSInfo;
			pDDNSInfo = NULL;
			return FALSE;
		}
		memcpy(pdev->szIpaddress,pDDNSInfo[0].IP,15);
		pdev->nPort=pDDNSInfo[0].MediaPort;
	}

	H264_DVR_DEVICEINFO OutDev;
	int nError = 0;
	//���ó��������豸�����͵ȴ�ʱ��
	H264_DVR_SetConnectTime(3000, 1);//���ó�������1�Σ��ȴ�ʱ��3s

	long lLogin=0;

	lLogin = H264_DVR_Login(pdev->szIpaddress, (unsigned short)pdev->nPort, pdev->szUserName, pdev->szPsw, &OutDev, &nError,(SocketStyle)pdev->devctype);

	if ( lLogin <= 0 )
	{
		int nErr = H264_DVR_GetLastError();  
		if(nErr == H264_DVR_PASSWORD_NOT_VALID)
		{
			::BCGPMessageBox(NULL, "�������", "��ʾ", MB_OK|MB_ICONWARNING);
		}
		else
		{
			::BCGPMessageBox(NULL, "�޷����ӵ�����������������������Ƿ���ڣ�", "��ʾ", MB_OK|MB_ICONWARNING);
		}
		return lLogin;
	}
	H264_DVR_SetupAlarmChan(lLogin);

	return lLogin;
}

BOOL CDlgVideo1::ExitSDk()
{
	H264_DVR_Cleanup();

	return TRUE;
}
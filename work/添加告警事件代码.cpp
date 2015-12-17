void CMainFrame::SendEventTimer()
{
	MMI_EVENT mmi_event;
	memset(&mmi_event,0,sizeof(MMI_EVENT));

	mmi_event.wLogicPlantNo = 0;//pRemoteItem->indexDef.FieldNo;

	CTime tmSCan = CTime::GetCurrentTime();

	mmi_event.wYear = (WORD)tmSCan.GetYear();
	mmi_event.wMonth = (WORD)tmSCan.GetMonth();
	mmi_event.wDay = (WORD)tmSCan.GetDay();
	mmi_event.wHour = (WORD)tmSCan.GetHour();
	mmi_event.wMinute = (WORD)tmSCan.GetMinute();
	mmi_event.wSecond = (WORD)tmSCan.GetSecond();
	mmi_event.wMilliseconds = 0;

	WideCharToMultiByte(CP_ACP, 0, NULL, -1, mmi_event.PlantName, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, NULL, -1, mmi_event.DeviceName, 0, NULL, NULL);
	//mmi_event.PlantName = NULL;
	//mmi_event.DeviceName = NULL;
	WORD PortNo = m_nThisInterface;

	CKServerList::iterator it;
	CSafeLock cs(&m_DisconServerListLock);
	for (it=m_DisconServerList.begin(); it!=m_DisconServerList.end(); ++it)
	{
		CKServer *pServer = dynamic_cast<CKServer *>(it->second);
		pServer->Start();
		if (!pServer->IsConnected())
		{
			CString strEventDesc = _T("");
			strEventDesc.Format(_T("%s 已断开"), pServer->GetProgID());
			strcpy(mmi_event.EventType, "OPC断开告警");
			int len = ::WideCharToMultiByte(CP_UTF8,0,strEventDesc,-1,NULL,0,NULL,NULL);
			WideCharToMultiByte(CP_ACP, 0, strEventDesc.GetBuffer(strEventDesc.GetLength()), -1, mmi_event.EventDesc, len, NULL, NULL);

			SendEvent(&mmi_event, PortNo);
		}
	}
}

BOOL CMainFrame::SendEvent(void* pEvent, WORD PortNo)
{
	if(pEvent == NULL)
	{
		return FALSE;
	}

	if(m_hProtocolDll == NULL)
	{
		m_hProtocolDll=::LoadLibrary(m_strSocketDllPath);
		if(m_hProtocolDll)
		{
			PDllInit pInitDll;
			pInitDll = (PDllInit)::GetProcAddress(m_hProtocolDll,"DllInit");

			HWND hAppWnd = NULL;
			CWinApp *pApp = AfxGetApp();
			if (pApp&&pApp->m_pMainWnd)
			{
				hAppWnd = pApp->m_pMainWnd->GetSafeHwnd();
			}
			if(pInitDll(hAppWnd, 0) == FALSE)	
			{
				return FALSE;
			}
		}
	}

	if (m_hProtocolDll)
	{
		int  (__stdcall  * Dll_Control)(HWND,MMI_EVENT*,WORD);
		(FARPROC & )Dll_Control = GetProcAddress(m_hProtocolDll, "Dll_SendEvent");

		CString strError = _T("");
		CWnd *pWnd = AfxGetApp()->GetMainWnd();
		HWND hWnd = NULL;
		if (pWnd)
		{
			hWnd = pWnd->GetSafeHwnd();
		}

		if (Dll_Control(NULL,(MMI_EVENT*)pEvent,PortNo))
		{
			return TRUE;
		}
	}

	return FALSE;
}
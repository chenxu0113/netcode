    // ����DLL�ӿ�
    Run *pRun = NULL;//Run() ����ΪDLL�������
    pRun = (Run *)::GetProcAddress(hPlugin,"Run");
    if (NULL != pRun)
    {
        m_csDllFuc.Lock();
        (* pRun)(TRUE);
        m_csDllFuc.Unlock();
        MYDEBUG(_T("���ģ�� %s .Run() �ӿڵ���ִ�гɹ�"),stuPlugin.szFileName);
    }
    else
    {
        MYWARN(_T("���ģ�� %s ::GetProcAddress��ȡ�ӿ�Run()ʧ��"),stuPlugin.szFileName);
        return FALSE;
    }
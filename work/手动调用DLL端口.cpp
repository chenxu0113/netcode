    // 调用DLL接口
    Run *pRun = NULL;//Run() 函数为DLL输出函数
    pRun = (Run *)::GetProcAddress(hPlugin,"Run");
    if (NULL != pRun)
    {
        m_csDllFuc.Lock();
        (* pRun)(TRUE);
        m_csDllFuc.Unlock();
        MYDEBUG(_T("插件模块 %s .Run() 接口调用执行成功"),stuPlugin.szFileName);
    }
    else
    {
        MYWARN(_T("插件模块 %s ::GetProcAddress获取接口Run()失败"),stuPlugin.szFileName);
        return FALSE;
    }
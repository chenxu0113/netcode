//:Ignore
// XTWndShadow.h: interface for the CXTWndShadow class.
//
// This file is a part of the Xtreme Toolkit for MFC.
// ©1998-2003 Codejock Software, All Rights Reserved.
//
// This source code can only be used under the terms and conditions 
// outlined in the accompanying license agreement.
//
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#if !defined(__XTWNDSHADOW_H__)
#define __XTWNDSHADOW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef   BOOL  (WINAPI *LayeredProc)  
(
  HWND hwnd,             // handle to layered window
  HDC hdcDst,            // handle to screen DC
  POINT *pptDst,         // new screen position
  SIZE *psize,           // new size of the layered window
  HDC hdcSrc,            // handle to surface DC
  POINT *pptSrc,         // layer position
  COLORREF crKey,        // color key
  BLENDFUNCTION *pblend, // blend function
  DWORD dwFlags          // options
);

class CXTShadowWnd;

// Uncomment this line to disable alpha shadow. DEBUG ONLY
// #define NOALPHASHADOW

//////////////////////////////////////////////////////////////////////
// CXTShadowsManager is a stand alone internal Toolkit class.  It is used to
// manage shadows for popup menus.  

class CXTShadowsManager
{

	class CShadowList : public CList<CXTShadowWnd*, CXTShadowWnd*>
	{
	public:
		void AddShadow(CXTShadowWnd* pShadow);
		void RemoveShadow(CXTShadowWnd* pShadow);
		friend class CXTShadowWnd;
	};

	CXTShadowsManager(void);

public:
	~CXTShadowsManager(void);

	void SetShadow(HWND hWnd, const CRect& rcExclude = CRect(0, 0, 0, 0));
	void SetShadow(CRect rc, HWND hwndHook);

private:

	BOOL AlphaShadow()
	{
	#ifdef NOALPHASHADOW
		return FALSE;		
	#endif

		return (UpdateLayeredWindow != NULL);
	}
	BOOL PseudoShadow()
	{
		if (AlphaShadow())
			return FALSE;

		return !xtAfxData.bUseSolidShadows;
	}
	void DestroyShadow(CXTShadowWnd*);
	CXTShadowWnd* CreateShadow(BOOL bHoriz, HWND hWnd, CRect rcExclude);
	CXTShadowWnd* CreateShadow(BOOL bHoriz, CRect rc, CRect rcExclude, HWND hwndHook);


private:
	
	LayeredProc UpdateLayeredWindow;
	HMODULE m_hLib;

	CShadowList m_lstShadow;
	CList<CXTShadowWnd*, CXTShadowWnd*> m_lstPool;


	static CXTShadowsManager s_managerInstance;
	friend class CXTShadowsManager* XTShadowsManager();
	friend class CXTShadowWnd;

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTShadowsManager* XTShadowsManager() {
	return &CXTShadowsManager::s_managerInstance;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

#endif // !defined(__XTWNDSHADOW_H__)

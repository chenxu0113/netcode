//:Ignore
// XTDockContext.h interface for the CXTDockContext class.
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
//:End Ignore

#if !defined(__XTDOCKCONTEXT_H__)
#define __XTDOCKCONTEXT_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

// class forwards
class CXTDockBar;
class CXTDockWindow;

//////////////////////////////////////////////////////////////////////
// CXTDockContext is a CDockContext derived class.  It is used by CXTDockWindow
// and CXTMiniDockFrameWnd for docking and sizing control bars.
//
class _XT_EXT_CLASS CXTDockContext : public CDockContext
{
public:
	//:Ignore
	// Listeners for drag state changes.
	typedef CXTCallbacks<CXTDockContext*, 4> LISTENERS;
	typedef LISTENERS::NOTIFYCB LISTENERCB;
	//:End Ignore

public:

    // Constructs a CXTDockContext object.
    //
    CXTDockContext(
		// Pointer to a CControlBar object.
        CControlBar* pBar);

    // Destroys a CXTDockContext object, handles cleanup and de-allocation.
    //
    virtual ~CXTDockContext();

protected:
	//:Ignore
	CTypedPtrArray<CObArray, CFrameWnd*> m_arrFramesPendingRecalc;
    // Cursor position.
	CPoint m_ptOrig;
	// Tells if full window drag is shall be executed.
	bool m_bFullWindowDrag;
	bool m_bRecalcPending;
	// Tells if the context currently tracking mouse during drag/resize operation
	bool m_bTracking;
	// Listeners for tracking state change notifications
	LISTENERS m_trackStateListeners;
	//:End Ignore

public:

	// This member function tells if this object is currently tracking a mouse
	// during a drag or resize operation. Returns 'true' if it is tracking
	// a mouse, otherwise it returns 'false'.
	//
	bool IsTracking() const;

    // This member function is called by CXTDockContext when a drag operation
	// completes.
    //
    void EndDrag();

    // This member function is called by CXTDockWindow and CXTMiniDockFrameWnd
	// whenever a drag operation begins.
    //
    virtual void StartDrag(
		// CPoint object that represents the current cursor location.
		CPoint pt);

    // This member function is called by CXTDockContext to determine whether
	// a control bar is dockable. Returns a DWORD value.
	//
    DWORD CanDock();

    // This member function is called by CXTDockContext to determine  whether
	// a control bar is dockable. Returns a DWORD value.
    //
    DWORD CanDock(
		// Size of the control bar.
        CRect rect,
		// Control bar style.
        DWORD dwDockStyle,
		// Points to a CDockBar pointer.
        CDockBar** ppDockBar = NULL);

    // This member function is called by CXTDockContext whenever a drag
	// operation begins. Returns TRUE if successful, otherwise returns FALSE.
    //
    BOOL Track();

    // This member function is called by CXTDockContext to get a pointer
	// to a CXTDockBar object. Returns a CXTDockBar pointer.
    //
    CXTDockBar* GetDockBar(
		// Control bar style.
        DWORD dwOverDockStyle);

    // This member function is used to toggle the docked state of a control
	// bar.
    //
    virtual void ToggleDocking();

    // This member function is called to enable diagonal sizing for 
    // the control bar.
    //
    void Stretch(
		// CPoint object that represents the current cursor location.
		CPoint pt);

	//:Ignore
	// This member function adds a listener for drag state change notifications.
	//
	void AddTrackStateListener(
		// A pointer to a CObject object.
		CObject* listener, 
		LISTENERCB cb);
	//:End Ignore

	// This member function removes a listener.
	//
	void RemoveListener(
		// A pointer to a CObject object.
		CObject* listener);

//:Ignore
    void DeferRecalcLayout(CFrameWnd* pFrame);    
	virtual void StartResize(int nHitTest, CPoint pt);
    void Move(CPoint pt);
    void DrawFocusRect(BOOL bRemoveRect = FALSE);
    void OnKey(int nChar, BOOL bDown);
    void UpdateState(BOOL* pFlag, BOOL bNewValue);
	bool CanDock(CFrameWnd* pFrame, DWORD& dwResult, CRect rect, DWORD dwDockStyle, CDockBar** ppDockBar);
	void FlushRecalcLayout(CFrameWnd* pFrame);
	virtual CFrameWnd* GetAlternateSite();

private:
    void InitLoop();
	void CancelLoop();
    void EndResize();

    void AdjustRectangle(CRect& rect, CPoint pt);    
    void FloatControlBar(DWORD dwFloatStyle);
//:End Ignore
    friend class CXTControlBar;
    friend class CXTTrackDockContextCleanup;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE bool CXTDockContext::IsTracking() const {
	return m_bTracking;
}

/////////////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTDOCKCONTEXT_H__)

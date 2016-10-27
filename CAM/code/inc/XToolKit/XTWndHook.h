//:Ignore
// XTWndHook.h interface for the CXTWndHook class.
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

#if !defined(__XTWNDHOOK_H__)
#define __XTWNDHOOK_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTWndHook is a CWnd derived class.  It is used to hook a CWnd object
// in order to intercept and act upon window messages that are received.
//
class _XT_EXT_CLASS CXTWndHook : public CObject
{
    DECLARE_DYNAMIC(CXTWndHook)

public:

    // Constructs a CXTWndHook object.
    //
    CXTWndHook();

    // Destroys a CXTWndHook object, handles cleanup and de-allocation.
    //
    virtual ~CXTWndHook();

protected:

    // The window hooked.
	HWND m_hWnd;
	// The original window procedure.
	WNDPROC m_pOldWndProc;
	// Next, in chain of hooks, for this window.
	CXTWndHook* m_pNextHook;

public:

    // This member function will check to see if the current window has
	// been hooked. Returns TRUE if the window has been hooked, otherwise
	// returns FALSE.
    //
    virtual BOOL IsWindowHooked();

    // This member function will hook a window so that its messages are
	// intercepted before they are passed on to the specified window. Returns
	// TRUE if successful, otherwise returns FALSE.
    //
    virtual BOOL HookWindow(
        // Points to a CWnd object that represents the window to hook.
		// If set to NULL, the window hook is removed.
        CWnd* pWnd);

    // This member function will install a new window procedure that directs
	// messages to the CXTWndHook. Returns TRUE if successful, otherwise returns
	// FALSE.
    //
    virtual BOOL HookWindow(
        // A handle to a window that represents the window whose window
		// procedure is to be hooked.  If set to NULL, the WNDPROC is removed.
        HWND hWnd);

    // This member function provides a Windows procedure (WindowProc) for
	// a CXTWndHook object.  It dispatches messages through the window's message
	// map. Returns an LRESULT value. The return value depends on the message.
    //
    virtual LRESULT WindowProc(
        // Specifies the Windows message to be processed.
        UINT message,
        // Provides additional information used in processing the message.
		// The parameter value depends on the message.
        WPARAM wParam,
        // Provides additional information used in processing the message.
		// The parameter value depends on the message.
        LPARAM lParam);

    // This member function calls the default window procedure.  The default
	// window procedure provides default processing for any window message
	// that an application does not process.  This member function ensures
	// that every message is processed. Returns an LRESULT value. The return
	// value depends on the message.
    //
    LRESULT Default();

    friend class CXTWindowMap;
};

// class forwards
class CXTMenuBar;

//////////////////////////////////////////////////////////////////////
// CXTMBarWndHook is a CXTWndHook derived class.  CXTMBarWndHook is used
// to trap menu related messages sent to the owning frame.  The same class
// is also used to trap messages sent to the MDI client window in an MDI
// app.  I should really use two classes for this, but it uses less code
// to share the same class.  Note, however, that there are two different
// INSTANCES of CXTMBarWndHook in CXTMenuBar: one for the frame, and one
// for the MDI client window.
//
class _XT_EXT_CLASS CXTMBarWndHook : public CXTWndHook
{
    DECLARE_DYNAMIC(CXTMBarWndHook)

public:

    // Constructs a CXTMBarWndHook object.
    //
    CXTMBarWndHook();

    // Destroys a CXTMBarWndHook object, handles cleanup and de-allocation.
    //
    virtual ~CXTMBarWndHook();

protected:

    // Points to the owner menu bar.
	CXTMenuBar* m_pMenuBar; 

public:

    // This member function will install a new window procedure that directs
	// messages to the CXTWndHook. Returns TRUE if successful, otherwise returns
	// FALSE.
    //
    virtual BOOL Install(
        // Pointer to the owner menu bar.
        CXTMenuBar* pMenuBar,
        // Handle to the frame window.
        HWND hWndToHook);

    // This member function provides a Windows procedure (WindowProc) for
	// a CXTWndHook object.  It dispatches messages through the window's message
	// map. Returns an LRESULT value. The return value depends on the message.
    //
    virtual LRESULT WindowProc(
        // Specifies the Windows message to be processed.
        UINT message,
        // Provides additional information used in processing the message.
		// The parameter value depends on the message.
        WPARAM wParam,
        // Provides additional information used in processing the message.
		// The parameter value depends on the message.
        LPARAM lParam);

   friend class CXTMenuBar;
};

//////////////////////////////////////////////////////////////////////
// CXTMBarMDIWndHook is a CXTMBarWndHook derived class. CXTMBarWndHook
// is used to trap menu related messages sent to the owning frame.  The
// same class is also used to trap messages sent to the MDI client window
// in an MDI app.  I should really use two classes for this, but it uses
// less code to share the same class.  Note, however, that there are two
// different INSTANCES of CXTMBarWndHook in CXTMenuBar; one for the frame,
// and one for the MDI client window.
//
class _XT_EXT_CLASS CXTMBarMDIWndHook : public CXTMBarWndHook
{
    DECLARE_DYNAMIC(CXTMBarMDIWndHook)

public:
	// This member function provides a Windows procedure (WindowProc) for
	// a CXTWndHook object. It dispatches messages through the window's message
	// map. Returns an LRESULT value. The return value depends on the message.
    //
    virtual LRESULT WindowProc(
        // Specifies the Windows message to be processed.
        UINT message,
        // Provides additional information used in processing the message.
		// The parameter value depends on the message.
        WPARAM wParam,
        // Provides additional information used in processing the message.
		// The parameter value depends on the message.
        LPARAM lParam);

	friend class CXTMenuBar;
};

//////////////////////////////////////////////////////////////////////
// CXTWindowMap is a CMapPtrToPtr derived class.  It is used to create
// a CXTWindowMap class object.
//
class _XT_EXT_CLASS CXTWindowMap : private CMapPtrToPtr
{
    DECLARE_DYNAMIC(CXTWindowMap)

public:

    // Constructs a CXTWindowMap object.
    //
    CXTWindowMap();

    // Destroys a CXTWindowMap object, handles cleanup and de-allocation.
    //
    virtual ~CXTWindowMap();

    // This member function adds a hook to the map, that is, it associates
	// the hook with the window.
    //
    void Add(
        // Valid window handle.
        HWND hWnd,
        // A pointer to a CXTWndHook object.
        CXTWndHook* pXTWndHook);

    // This member function removes a hook from the map.
    //
    void Remove(
        // A pointer to a CXTWndHook object.
        CXTWndHook* pXTWndHook);

    // This member function removes all the hooks for a window.
    //
    void RemoveAll(
        // Valid window handle.
        HWND hWnd);

    // This member function is a subclassed window procedure for message
	// hooks, and replaces AfxWndProc, or whatever else was there before.
	// Returns an LRESULT object
	//
    static LRESULT CALLBACK HookWndProc(
        // Valid window handle.
        HWND hWnd,
        // Specifies the Windows message to be processed.
        UINT message,
        // Provides additional information used in processing the message.
		// The parameter value depends on the message.
        WPARAM wParam,
        // Provides additional information used in processing the message.
		// The parameter value depends on the message.
        LPARAM lParam);

    // This member function finds the first hook associated with the window.
	// Returns a CXTWndHook pointer.
    //
    CXTWndHook* Lookup(
        // Valid window handle.
        HWND hWnd);
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTWndHook::IsWindowHooked() {
    return (m_hWnd != NULL);
}
AFX_INLINE BOOL CXTWndHook::HookWindow(CWnd* pWnd) {
    return HookWindow(pWnd->GetSafeHwnd());
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTWNDHOOK_H__)

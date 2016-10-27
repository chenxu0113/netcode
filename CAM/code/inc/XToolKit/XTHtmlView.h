//:Ignore
// XTHtmlView.h interface for the CXTHtmlView class.
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

#if !defined(__XTHTMLVIEW_H__)
#define __XTHTMLVIEW_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTHtmlView is a CFormView derived class. The CXTHtmlView class provides
// the functionality of the WebBrowser control within the context of MFC's
// document/view architecture.  The WebBrowser control is a window in which
// the user can browse sites on the World Wide Web, as well as folders
// in the local file system and on a network.  The WebBrowser control supports
// hyperlinking, Uniform Resource Locator (URL) navigation, and maintains
// a history list.
//
// [b]Using the CXTHtmlView Class in an MFC Application[/b]
//
// In the standard MFC framework application (either SDI or MDI based),
// the view object is commonly derived from a specialized set of classes.
// These classes, all derived from CView, provide specialized functionality
// beyond that provided by CView.
//
// Basing the application's view class on CXTHtmlView provides the view
// with the WebBrowser control.  This effectively makes the application
// a web browser. The preferred method of creating a web browser-style
// application is to use the MFC AppWizard, and specify CXTHtmlView as
// the view class.  For more information on implementing and using the
// WebBrowser control within MFC applications,  see Web Browser-Style Applications.
//
// The functionality of CXTHtmlView is designed for applications that access
// the Web (and/or HTML documents).  The following CXTHtmlView member functions
// apply to the Internet Explorer application only.  These functions will
// succeed on the WebBrowser control, but they will have no visible effect. 
//
class _XT_EXT_CLASS CXTHtmlView : public CFormView
{
	DECLARE_DYNCREATE(CXTHtmlView)
	DECLARE_EVENTSINK_MAP()

protected:

    // Constructs a CXTHtmlView object.
	//
	CXTHtmlView();

	// Destroys a CXTHtmlView object, handles cleanup and de-allocation.
	//
    virtual ~CXTHtmlView();

	// Points to the IWebBrowser2 object associated with this view.
	IWebBrowser2* m_pBrowserApp;
	// 'true' when loading the browser.	
	bool m_bLoading;
	// 'true' to display a loading message while a page is updating.	
	bool m_bLoadingMessage;
	// Loading message string.
	CString m_strLoading;

public:

	// CWnd object that represents the internet browser control associated
	// with this view.
	CWnd m_wndBrowser;				

	// Call this member function to retrieve the type name of the contained
	// active document. Applies to Internet Explorer and WebBrowser. Returns
	// a CString object containing the type name of the contained active document.
	//
	CString GetType() const;

	// Call this member function to retrieve the distance between the internal
	// left edge of the WebBrowser control and the left edge of its container.
	// Applies to Internet Explorer and WebBrowser.  Returns the left-edge
	// distance, in pixels.
	//
	long GetLeft() const;

	// Call this member function to set the horizontal position of the Internet
	// Explorer main window.
	//
	void SetLeft(
		// The screen coordinate of the left edge of the main window.
		long nNewValue);

	// Call this member function to retrieve the screen coordinate of the
	// top edge of the WebBrowser control’s main window.  Applies to Internet
	// Explorer and WebBrowser. Returns the address of a variable that receives
	// the screen coordinate of the main window's top edge.
	//
	long GetTop() const;

	// Call this member function to set the distance between the internal top edge 
	// of the WebBrowser control and the top edge of its container.  Applies to 
	// Internet Explorer and WebBrowser.
	//
	void SetTop(
		// The screen coordinate of the top edge of the main window.
		long nNewValue);

	// Call this member function to retrieve the height, in pixels, of the
	// WebBrowser control’s frame window.  Returns the control’s frame window
	// height, in pixels.
	//
	long GetHeight() const;

	// Call this member function to set the height of the Internet Explorer
	// main window.  Applies to Internet Explorer and WebBrowser.
	//
	void SetHeight(
		// The height, in pixels, of the main window.
		long nNewValue);

	// Call this member function to set the visibility state of the WebBrowser
	// control. Applies to Internet Explorer and WebBrowser.
	//
	void SetVisible(
		// Nonzero if the control is visible, otherwise zero.
		BOOL bNewValue);

	// Call this member function to determine if the contained object is visible.
	// Applies to Internet Explorer and WebBrowser.  Returns nonzero if the
	// object is visible, otherwise returns zero.
	//
	BOOL GetVisible() const;

	// Call this member function to get the name of the resource being displayed 
	// in the WebBrowser.  If the resource is an HTML page on the World Wide
	// Web, the name is the title of that page.  If the resource is a folder
	// or file on the network or local computer, the name is the UNC or full
	// path of the folder or file. Applies to Internet Explorer and WebBrowser.
	// Returns a CString object containing the name of the resource currently
	// displayed in the WebBrowser.
	//
	CString GetLocationName() const;

	// Call this member function to retrieve the ready state of the WebBrowser
	// object. Applies to Internet Explorer and WebBrowser. Returns a READYSTATE
	// value, as described in the Platform SDK.
	//
	READYSTATE GetReadyState() const;

	// Call this member function to determine whether the web browser is operating 
	// offline.  Applies to Internet Explorer and WebBrowser.  Returns nonzero if 
	// the web browser is currently offline, otherwise returns zero.
	//
	BOOL GetOffline() const;

	// Call this member function to set a value indicating whether the WebBrowser 
	// control is currently operating in offline mode.  In offline mode, the
	// browser reads HTML pages from the local cache rather than from the
	// source document.  Applies to Internet Explorer and WebBrowser.
	//
	void SetOffline(
		// Nonzero to read from the local cache, otherwise zero.
		BOOL bNewValue);

	// Call this member function to determine whether any dialog boxes can
	// be shown in the WebBrowser control.  Applies to Internet Explorer and
	// WebBrowser.  Returns nonzero if dialog boxes cannot be displayed from
	// the WebBrowser control, otherwise returns zero.
	//
	BOOL GetSilent() const;

	// Call this member function to set a value indicating whether any dialog
	// boxes can be shown.  Applies to Internet Explorer and WebBrowser.
	//
	void SetSilent(
		// If nonzero, dialog boxes will not be displayed.  If zero, dialog boxes
		// will be displayed. The default value is zero.
		BOOL bNewValue);

	// Call this member function to determine whether Internet Explorer is
	// the top-level container of the WebBrowser control.  Applies to Internet
	// Explorer and WebBrowser.  Returns nonzero if the container is the top-level
	// container, otherwise returns zero.
	//
	BOOL GetTopLevelContainer() const;

	// Call this member function to retrieve the URL of the resource that
	// the WebBrowser control is currently displaying.  If the resource is
	// a folder or file on the network or local computer, the name is the
	// UNC or full path of the folder or file.  Applies to Internet Explorer
	// and WebBrowser.  Returns a CString object containing the URL of the
	// resource currently displayed in the WebBrowser.
	//
	CString GetLocationURL() const;

	// Call this member function to determine whether the WebBrowser control
	// is engaged in a navigation or downloading operation.  Applies to Internet
	// Explorer and WebBrowser.  Returns nonzero if the web browser is busy,
	// otherwise returns zero.
	//
	BOOL GetBusy() const;

	// Call this member function to retrieve the automation object supported
	// by the application that contains the WebBrowser control.  Applies to
	// Internet Explorer and WebBrowser.  Returns a pointer to the IDispatch
	// interface of the active document object.
	//
	LPDISPATCH GetApplication() const;

	// Call this member function to retrieve a pointer to the parent object
	// of the WebBrowser control.  Applies to Internet Explorer and WebBrowser.
	// Returns a pointer to the IDispatch interface of the object that is
	// the parent of the WebBrowser control.
	//
	LPDISPATCH GetParentBrowser() const;

	// Call this member function to retrieve an object that points to the
	// container of the web browser.  Applies to Internet Explorer and WebBrowser.
	// Returns a pointer to the IDispatch interface of the active document
	// object.
	//
	LPDISPATCH GetContainer() const;

	// Call this member function to retrieve the HTML document for the active
	// document.  Applies to Internet Explorer and WebBrowser.  Returns a
	// pointer to the IDispatch interface of the active document object.
	//
	LPDISPATCH GetHtmlDocument() const;

	// Call this member function to retrieve the full path of the file that
	// Internet Explorer is currently displaying.  Applies to Internet Explorer.
	// If you use this call with a WebBrowser control, it will return no error,
	// but it will ignore this call.  Returns a CString object containing
	// the path and name of the currently displayed file.  If no path and
	// filename exist, GetFullName returns an empty CString.
	//
	CString GetFullName() const;

	// Call this member function to determine whether the toolbar is visible.
	// Returns a value indicating whether the toolbar is visible.  Nonzero
	// if the toolbar is visible, otherwise zero.
	//
	int GetToolBar() const;

	// Call this member function to show or hide the Internet Explorer toolbar. 
	// Applies to Internet Explorer.  If you use this call with a WebBrowser
	// control, it will not return an error, but it will ignore this call.
	//
	void SetToolBar(
		// Indicates whether to display the toolbar.  Nonzero if the toolbar
		// is to be displayed, otherwise zero.
		int nNewValue);

	// Call this member function to determine whether the menu bar is visible.
	// Applies to Internet Explorer and WebBrowser.  Returns nonzero if the
	// menu bar is visible, otherwise returns zero.
	//
	BOOL GetMenuBar() const;

	// Call this member function to show or hide the Internet Explorer menu bar. 
	// Applies to Internet Explorer.  If you use this call with a WebBrowser
	// control, it will not return an error, but it will ignore this call.
	//
	void SetMenuBar(
		// Nonzero to show the menu bar, otherwise zero.
		BOOL bNewValue);

	// Call this member function to determine whether the WebBrowser control
	// is operating in full-screen mode or in normal window mode.  In full-screen
	// mode, the Internet Explorer main window is maximized and the status
	// bar, toolbar, menu bar, and title bar are hidden.  Applies to Internet
	// Explorer and WebBrowser.  Returns nonzero if the WebBrowser is operating
	// in full-screen mode, otherwise returns zero.
	//
	BOOL GetFullScreen() const;

	// Call this member function to set Internet Explorer to either full-screen
	// or normal window mode.  In full-screen mode, the Internet Explorer
	// main window is maximized and the status bar, toolbar, menu bar, and
	// title bar are hidden. Applies to Internet Explorer.  If you use this
	// call with a WebBrowser control, it will not return an error, but it will
	// ignore this call.
	//
	void SetFullScreen(
		// Nonzero for full-screen mode, otherwise zero.
		BOOL bNewValue);

	// Call this member function to query a command status.  QueryStatusWB
	// implements the behavior of the IOleCommandTarget::QueryStatus method.
	// Applies to Internet Explorer and WebBrowser.  Returns the address of
	// the OLECMDF value that receives the status of the command.
	//
	OLECMDF QueryStatusWB(
		// The OLECMDID value of the command for which the caller needs status
		// information.
		OLECMDID cmdID) const;

	// Call this member function to determine whether the WebBrowser object
	// is registered as a top-level browser for target name resolution.  Applies
	// to Internet Explorer and WebBrowser.  Returns nonzero if the browser
	// is registered as a top-level browser, otherwise returns zero.
	//
	BOOL GetRegisterAsBrowser() const;

	// Call this member function to set a value indicating whether the WebBrowser 
	// control is registered as a top-level browser for target name resolution.
	// A top-level browser is the browser set in the registry as the default
	// browser.  Applies to Internet Explorer and WebBrowser.
	//
	void SetRegisterAsBrowser(
		// Determines whether Internet Explorer is registered as a top-level
		// browser. If nonzero, the web browser is registered as a top-level
		// browser. If zero, it is not a top-level browser. The default value
		// is zero. 
		BOOL bNewValue);

	// Call this member function to determine whether the WebBrowser control
	// is registered as a drop target for navigation.  Applies to Internet
	// Explorer and WebBrowser.  Returns nonzero if the browser is registered
	// as a drop target, otherwise returns zero.
	//
	BOOL GetRegisterAsDropTarget() const;

	// Call this member function to set a value indicating whether the WebBrowser 
	// control is registered as a drop target for navigation.  Applies to
	// Internet Explorer and WebBrowser.
	//
	void SetRegisterAsDropTarget(
		// Determines if the WebBrowser control is registered as a drop target
		// for navigation.  If nonzero, the object is registered as a drop target.
		// If zero, it is not a drop target.
		BOOL bNewValue);

	// Call this member function to determine whether the web browser is in
	// theater mode.  When the web browser is in theater mode, the browser
	// main window fills the entire screen, a toolbar with a minimal set of
	// navigational tools appears, and the status bar appears in the upper
	// right-hand corner of the screen.  Applies to Internet Explorer and
	// WebBrowser.  Returns nonzero if the web browser is in theater mode,
	// otherwise returns zero.
	//
	BOOL GetTheaterMode() const;

	// Call this member function to set a value indicating whether the WebBrowser
	// control is in theater mode.  When the web browser is in theater mode,
	// the browser main window fills the entire screen, a toolbar with a minimal
	// set of navigational tools appears, and the status bar appears in the
	// upper right-hand corner of the screen.  Applies to Internet Explorer
	// and WebBrowser.
	//
	void SetTheaterMode(
		// Nonzero to set the WebBrowser control to theater mode, otherwise zero.
		// The default value is zero.
		BOOL bNewValue);

	// Call this member function to retrieve Internet Explorer’s address bar.
	// Applies to Internet Explorer.  If you use this call with a WebBrowser
	// control, it will return no error, but it will ignore this call.  Returns
	// nonzero if the address bar is visible, otherwise returns zero.
	//
	BOOL GetAddressBar() const;

	// Call this member function to show or hide the Internet Explorer object's
	// address bar.  Applies to Internet Explorer.  If you use this call with
	// a WebBrowser control, it will return no error, but it will ignore this
	// call.
	//
	void SetAddressBar(
		// Nonzero to show address bar, otherwise zero.
		BOOL bNewValue);

	// Call this member function to determine whether the WebBrowser control
	// displays a status bar.  Applies to Internet Explorer.  If you use this
	// call with a WebBrowser control, it will return no error, but it will
	// ignore this call.  Returns nonzero if the status bar can be displayed,
	// otherwise returns zero.
	//
	BOOL GetStatusBar() const;

	// Call this member function to display the status bar.  Applies to Internet 
	// Explorer.  If you use this call with a WebBrowser control, it will
	// return no error, but it will ignore this call.
	//
	void SetStatusBar(
		// Nonzero if the status bar is visible, otherwise zero.
		BOOL bNewValue);

	// This member function navigates backward one item in the history list.
	// Applies to Internet Explorer and WebBrowser.
	//
	void GoBack();

	// This member function navigates forward one item in the history list.
	// Applies to Internet Explorer and WebBrowser.
	//
	void GoForward();

	// This member function navigates to the current home or start page specified
	// in the Internet Explorer Internet Options dialog box or the Internet
	// Properties dialog box, accessed from the Control Panel.  Applies to
	// Internet Explorer and WebBrowser.
	//
	void GoHome();

	// This member function navigates to the current search page, as specified
	// in the Internet Explorer Internet Options dialog box or the Internet
	// Properties dialog box, accessed from the Control Panel.  Applies to
	// Internet Explorer and WebBrowser.
	//
	void GoSearch();

	// Call this member function to navigate to the resource identified by
	// a URL.  Applies to Internet Explorer and WebBrowser.
	//
	void Navigate(
		// A caller-allocated string that contains the URL to navigate to, or
		// the full path of the file to display.
		LPCTSTR URL,
		// The flags of a variable that specifies whether to add the resource
		// to the history list, whether to read to or write from the cache, and
		// whether to display the resource in a new window.  The variable can
		// be a combination of the values defined by the BrowserNavConstants
		// enumeration.
		DWORD dwFlags = 0,
		// A pointer to a string that contains the name of the frame in which
		// to display the resource.
		LPCTSTR lpszTargetFrameName = NULL,
		// A pointer to a value that specifies the HTTP headers to send to the
		// server.  These headers are added to the default Internet Explorer
		// headers. The headers can specify such things as the action required
		// of the server, the type of data being passed to the server, or a status
		// code.  This parameter is ignored if the URL is not an HTTP URL.
		LPCTSTR lpszHeaders = NULL,
		// A pointer to the data to send with the HTTP POST transaction. For
		// example, the POST transaction is used to send data gathered by an
		// HTML form. If this parameter does not specify any post data, Navigate
		// issues an HTTP GET transaction.  This parameter is ignored if the URL
		// is not an HTTP URL.
		LPVOID lpvPostData = NULL, 
		// Data to send with the HTTP POST transaction.  For example, the POST
		// transaction is used to send data gathered by an HTML form.  If this
		// parameter does not specify any post data, Navigate issues an HTTP
		// GET transaction.  This parameter is ignored if the URL is not an HTTP URL.
		DWORD dwPostDataLen = 0);

	// Call this member function to navigate to the resource identified by
	// a URL, or to the file identified by a full path.  This member function
	// extends the Navigate member function by supporting browsing on special
	// folders, such as Desktop and My Computer, that are represented by the
	// parameter 'pIDL'.  Applies to Internet Explorer and WebBrowser.
	//
	void Navigate2(
		// A pointer to an ITEMIDLIST structure.
		LPITEMIDLIST pIDL,
		// The flags of a variable that specifies whether to add the resource
		// to the history list, whether to read to or write from the cache, and
		// whether to display the resource in a new window. The variable can
		// be a combination of the values defined by the BrowserNavConstants
		// enumeration.
		DWORD dwFlags = 0,
		// A pointer to a string that contains the name of the frame in which
		// to display the resource.
		LPCTSTR lpszTargetFrameName = NULL);

	// Call this member function to navigate to the resource identified by
	// a URL, or to the file identified by a full path.  This member function
	// extends the Navigate member function by supporting browsing on special
	// folders, such as Desktop and My Computer, that are represented by the
	// parameter 'pIDL'.  Applies to Internet Explorer and WebBrowser.
	//
	void Navigate2(
		// A pointer to a string containing the URL.
		LPCTSTR lpszURL, 
		// The flags of a variable that specifies whether to add the resource
		// to the history list, whether to read to or write from the cache, and
		// whether to display the resource in a new window.  The variable can
		// be a combination of the values defined by the BrowserNavConstants
		// enumeration.
		DWORD dwFlags = 0, 
		// A pointer to a string that contains the name of the frame in which
		// to display the resource.
		LPCTSTR lpszTargetFrameName = NULL,
		// A pointer to a value that specifies the HTTP headers to send to the
		// server. These headers are added to the default Internet Explorer headers.
		// The headers can specify such things as the action required of the
		// server, the type of data being passed to the server, or a status code.
		// This parameter is ignored if the URL is not an HTTP URL.
		LPCTSTR lpszHeaders = NULL, 
		// Data to send with the HTTP POST transaction.  For example, the POST
		// transaction is used to send data gathered by an HTML form.  If this
		// parameter does not specify any post data, Navigate2 issues an HTTP
		// GET transaction.  This parameter is ignored if the URL is not an HTTP URL.
		LPVOID lpvPostData = NULL,
		// Length in bytes of the data pointed to by the 'lpvPostData' parameter.
		DWORD dwPostDataLen = 0);

	// Call this member function to navigate to the resource identified by
	// a URL, or to the file identified by a full path.  This member function
	// extends the Navigate member function by supporting browsing on special
	// folders, such as Desktop and My Computer, that are represented by the
	// parameter 'pIDL'.  Applies to Internet Explorer and WebBrowser.
	//
	void Navigate2(
		// A pointer to a string containing the URL.
		LPCTSTR lpszURL, 
		// A pointer to a string that contains the name of the frame in which
		// to display the resource.
		DWORD dwFlags, 
		// A reference to a CByteArray object.
		CByteArray& baPostedData, 
		// A pointer to a string that contains the name of the frame in which
		// to display the resource. 
		LPCTSTR lpszTargetFrameName = NULL, 
		// A pointer to a value that specifies the HTTP headers to send to the
		// server. These headers are added to the default Internet Explorer headers.
		// The headers can specify such things as the action required of the
		// server, the type of data being passed to the server, or a status code.
		// This parameter is ignored if the URL is not an HTTP URL.
		LPCTSTR lpszHeader = NULL);

	// This member function reloads the URL or file that the web browser is
	// currently displaying. Refresh contains no parameters for setting the
	// refresh level.  Applies to Internet Explorer and WebBrowser.
	//
	void Refresh();

	// This member function reloads the file that Internet Explorer is currently
	// displaying. Unlike Refresh, Refresh2 contains a parameter that specifies
	// the refresh level.  Applies to Internet Explorer and WebBrowser.
	//
	void Refresh2(
		// The address of the variable specifying the refresh level.  The possible
		// variables are defined in RefreshConstants, in the Platform SDK.
		int nLevel);

	// Call this member function to cancel any pending navigation or download
	// operation and stop any dynamic page elements, such as background sounds
	// and animations.  Applies to Internet Explorer and WebBrowser.
	//
	void Stop();

	// Call this member function to set the property associated with a given
	// object.  Applies to Internet Explorer and WebBrowser.
	//
	void PutProperty(
		// A string containing the property to set.
		LPCTSTR lpszProperty, 
		// The new value of the property indicated by 'lpszProperty'.
		const VARIANT& vtValue);

	// Call this member function to set the property associated with a given
	// object.  Applies to Internet Explorer and WebBrowser.
	//
	void PutProperty(
		// A pointer to a string containing the name of the property to set.
		LPCTSTR lpszPropertyName, 
		// The new value of the property.
		double dValue);

	// Call this member function to set the property associated with a given
	// object.  Applies to Internet Explorer and WebBrowser.
	//
	void PutProperty(
		// A pointer to a string containing the name of the property to set.
		LPCTSTR lpszPropertyName, 
		// A pointer to a string containing the new value of the property.
		LPCTSTR lpszValue);

	// Call this member function to set the property associated with a given
	// object.  Applies to Internet Explorer and WebBrowser.
	//
	void PutProperty(
		// A pointer to a string containing the name of the property to set.
		LPCTSTR lpszPropertyName, 
		// The new value of the property.
		long lValue);

	// Call this member function to set the property associated with a given
	// object.  Applies to Internet Explorer and WebBrowser.
	//
	void PutProperty(
		// A pointer to a string containing the name of the property to set.
		LPCTSTR lpszPropertyName, 
		// The new value of the property.
		short nValue);

	// Call this member function to get the value of the property currently
	// associated with the control.  Applies to Internet Explorer and WebBrowser.
	// Returns nonzero if completed successfully, otherwise returns zero.
	//
	BOOL GetProperty(
		// A pointer to a string containing the property to retrieve.
		LPCTSTR lpszProperty, 
		// A reference to a CString object that receives the current value of
		// the property.
		CString& strValue);

	// Call this member function to get the value of the property currently
	// associated with the control.  Applies to Internet Explorer and WebBrowser.
	// Returns a COleVariant object.
	//
	COleVariant GetProperty(
		// A pointer to a string containing the property to retrieve.
		LPCTSTR lpszProperty);

	// Call this member function to execute a command in the WebBrowser or
	// Internet Explorer.  See IWebBrowser2::ExecWB in the Platform SDK.
	//
	void ExecWB(
		// The command to execute.
		OLECMDID cmdID, 
		// The options set for executing the command.
		OLECMDEXECOPT cmdexecopt, 
		// A variant used for specifying the command input arguments.
		VARIANT* pvaIn, 
		// A variant used for specifying the command output arguments.
		VARIANT* pvaOut);

	// Call this member function to load the specified resource into the WebBrowser
	// control.  Applies to Internet Explorer and WebBrowser.  Returns nonzero if 
	// successful, otherwise returns zero.
	//
	BOOL LoadFromResource(
		// A pointer to a string containing the name of the resource to load.
		LPCTSTR lpszResource);

	// Call this member function to load the specified resource into the WebBrowser 
	// control.  Applies to Internet Explorer and WebBrowser.  Returns nonzero if 
	// successful, otherwise returns zero.
	//
	BOOL LoadFromResource(
		// The ID of the buffer containing the name of the resource to load.
		UINT nRes);

//:Ignore
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
//:End Ignore

	// This member function is called by the framework to draw the view. 
	// Applies to Internet Explorer and WebBrowser.
	//
	virtual void OnDraw(
		// A pointer to the device context to be used for rendering an image.
		CDC* pDC);

	// Call this member function to create a WebBrowser control or container
	// for the Internet Explorer executable.  Returns nonzero if successful,
	// otherwise returns zero.
	//
	virtual BOOL Create(
		// Points to a null-terminated character string that names the Windows
		// class. The class name can be any name registered with the 
		// AfxRegisterWndClass global function or the RegisterClass Windows function.
		// If NULL, it uses the predefined default CFrameWnd attributes.
		LPCTSTR lpszClassName, 
		// Points to a null-terminated character string that represents the window's
		// name.
		LPCTSTR lpszWindowName, 
		// Specifies the window style attributes. By default, the WS_VISIBLE
		// and WS_CHILD Windows styles are set.
		DWORD dwStyle, 
		// A reference to a RECT structure specifying the size and position of
		// the window.  The rectDefault value allows Windows to specify the size
		// and position of the new window.
		const RECT& rect, 
		// A pointer to the parent window of the control.
		CWnd* pParentWnd, 
		// The ID number of the view.  By default, set to AFX_IDW_PANE_FIRST.
		UINT nID, 
		// A pointer to a CCreateContext.  It is NULL by default.
		CCreateContext* pContext = NULL);

	// This member function is called by the framework after a navigation
	// to a hyperlink completes (on either a window or frameset element).
	// The URL parameter can be a PIDL in the case of a shell name space entity
	// for which there is no URL representation.  Note that the URL contained
	// in 'strURL' can be different from the URL that the browser was told to
	// navigate to, because this URL is the canonicalized and qualified URL.
	// For example, if an application specifies a URL of "www.microsoft.com"
	// in a call to Navigate or Navigate2, the URL passed by OnNavigateComplete2
	// will be  "http://www.microsoft.com/". Also, if the server has redirected
	// the browser to a different URL, the redirected URL will be reflected
	// here.
	//
	virtual void OnNavigateComplete2(
		// A string expression that evaluates to the URL, UNC file name, or PIDL (a 
		// pointer to an item identifier list) that was navigated to.
		LPCTSTR strURL);

	// This member function is called by the framework to cause an event to fire 
	// before a navigation occurs in the web browser.
	//
	virtual void OnBeforeNavigate2(
		// Pointer to a string containing the URL to navigate to.
		LPCTSTR lpszURL, 
		// Reserved for future use.
		DWORD nFlags, 
		// A string that contains the name of the frame in which to display the
		// resource, or NULL if no named frame is targeted for the resource.
		LPCTSTR lpszTargetFrameName, 
		// A reference to a CByteArray object containing the data to send to
		// the server if the HTTP POST transaction is being used.
		CByteArray& baPostedData, 
		// A pointer to a string containing additional HTTP headers to send to
		// the server (HTTP URLs only).  The headers can specify such things
		// as the action required of the server, the type of data being passed
		// to the server, or a status code.
		LPCTSTR lpszHeaders, 
		// A pointer to a cancel flag.  An application can set this parameter
		// to nonzero to cancel the navigation operation, or to zero to allow
		// it to proceed.
		BOOL* pbCancel);

	// This member function is called by the framework to notify an application
	// that the text of the status bar associated with the WebBrowser control
	// has changed.
	//
	virtual void OnStatusTextChange(
		// A string that contains the new status bar text.
		LPCTSTR lpszText);

	// This member function is called by the framework to notify an application 
	// that the progress of a download operation has been updated.  The container
	// can use the information provided by this event to display the number
	// of bytes downloaded so far or to update a progress indicator.
	//
	virtual void OnProgressChange(
		// Amount of total progress to show, or -1 when progress is complete.
		long nProgress, 
		// Maximum progress value.
		long nProgressMax);

	// This member function is called by the framework to notify an application
	// that the enabled state of a web browser command has changed.
	//
	virtual void OnCommandStateChange(
		// Identifier of the command whose enabled state has changed.
		long nCommand, 
		// Enabled state.  This parameter is nonzero if the command is enabled,
		// or zero if it is disabled.
		BOOL bEnable);

	// This member function is called by the framework to begin downloading
	// a document.  This event is fired shortly after the OnBeforeNavigate2
	// event, unless the navigation is canceled.  Any animation or "busy"
	// indication that the container needs to display should be connected
	// to this event.
	//
	virtual void OnDownloadBegin();

	// This member function is called by the framework to indicate that a
	// navigation operation finished, was halted, or failed.
	//
	virtual void OnDownloadComplete();

	// This member function is called by the framework to notify an application
	// if the title of a document in the WebBrowser control becomes available
	// or changes.  For HTML, the title might change.  While HTML is still
	// downloading, the URL of the document is set as the title.  After the
	// real title (if there is one) is parsed from the HTML, the title is
	// changed to reflect the actual title.
	//
	virtual void OnTitleChange(
		// The new document title.
		LPCTSTR lpszText);

	// This member function is called by the framework to notify an application
	// that PutProperty has changed the value of a property.
	//
	virtual void OnPropertyChange(
		// A pointer to a string containing the name of the property.
		LPCTSTR lpszProperty);

	// This member function is called by the framework when a new window is
	// to be created for displaying a resource.  This event precedes the creation
	// of a new window from within the WebBrowser.
	//
	virtual void OnNewWindow2(
		// A pointer to an interface pointer that, optionally, receives the 
		// IDispatch interface pointer of a new WebBrowser or Internet Explorer
		// object.
		LPDISPATCH* ppDisp, 
		// A pointer to a cancel flag.  An application can set this parameter
		// to nonzero to cancel the navigation operation, or to zero to allow
		// it to proceed.
		BOOL* Cancel);

	// This member function is called by the framework to notify an application
	// that a document has reached the READYSTATE_COMPLETE state.  Not every
	// frame will fire this event, but each frame that fires an OnDownloadBegin
	// event will fire a corresponding OnDocumentComplete event.  The URL
	// indicated by 'lpszURL' can be different from the URL that the browser
	// was told to navigate to, because this URL is the canonicalized and
	// qualified URL.  For example, if an application specifies a URL of 
	// "www.microsoft.com" in a call to Navigate or Navigate2, the URL passed
	// by OnNavigateComplete2 will be "http://www.microsoft.com/". Also, if
	// the server has redirected the browser to a different URL, the redirected
	// URL will be reflected here.
	//
	virtual void OnDocumentComplete(
		// A pointer to a string that evaluates to the URL, UNC file name, or
		// a PIDL (a pointer to an item identifier list) that was navigated to.
		LPCTSTR lpszURL);

	// This member function is called by the framework to notify an application
	// that the Internet Explorer application is ready to quit.
	//
	virtual void OnQuit();

	// This member function is called by the framework when the window for
	// the WebBrowser should be shown or hidden.  This allows the object control
	// host window to behave the same way the Internet Explorer window would
	// behave.
	//
	virtual void OnVisible(
		// Nonzero if the object is visible or zero otherwise.
		BOOL bVisible);

	// This member function is called by the framework when the ToolBar property
	// has changed.
	//
	virtual void OnToolBar(
		// Nonzero if Internet Explorer's toolbar is visible or zero otherwise.
		BOOL bToolBar);

	// This member function is called by the framework when the MenuBar property 
	// has changed.
	//
	virtual void OnMenuBar(
		// Nonzero if the Internet Explorer menu bar is visible or zero otherwise.
		BOOL bMenuBar);

	// This member function is called by the framework when the StatusBar property 
	// has changed.
	//
	virtual void OnStatusBar(
		// Nonzero if Internet Explorer's status bar is visible or zero otherwise.
		BOOL bStatusBar);

	// This member function is called by the framework when the FullScreen property 
	// has changed.
	//
	virtual void OnFullScreen(
		// Nonzero if Internet Explorer is in full screen mode or zero otherwise.
		BOOL bFullScreen);

	// This member function is called by the framework when the TheaterMode property 
	// has changed.
	//
	virtual void OnTheaterMode(
		// Nonzero if Internet Explorer is in theater mode or zero otherwise.
		BOOL bTheaterMode);

	//:Ignore
    //{{AFX_VIRTUAL(CXTHtmlView)
	protected:
	virtual void NavigateComplete2(LPDISPATCH pDisp, VARIANT* URL);
	virtual void BeforeNavigate2(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers,   BOOL* Cancel);
	virtual void DocumentComplete(LPDISPATCH pDisp, VARIANT* URL);
	//}}AFX_VIRTUAL
    //:End Ignore

	//:Ignore
    //{{AFX_MSG(CXTHtmlView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnFilePrint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
    //:End Ignore
	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTHtmlView::SetRegisterAsBrowser(BOOL bNewValue) {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_RegisterAsBrowser((short) (bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
}
AFX_INLINE void CXTHtmlView::SetRegisterAsDropTarget(BOOL bNewValue) {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_RegisterAsDropTarget((short) (bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
}
AFX_INLINE void CXTHtmlView::SetTheaterMode(BOOL bNewValue) {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_TheaterMode((short) (bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
}
AFX_INLINE void CXTHtmlView::SetVisible(BOOL bNewValue) {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_Visible((short) (bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
}
AFX_INLINE void CXTHtmlView::SetMenuBar(BOOL bNewValue) {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_MenuBar((short) (bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
}
AFX_INLINE void CXTHtmlView::SetToolBar(int nNewValue) {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_ToolBar(nNewValue);
}
AFX_INLINE void CXTHtmlView::SetOffline(BOOL bNewValue) {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_Offline((short) (bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
}
AFX_INLINE void CXTHtmlView::SetSilent(BOOL bNewValue) {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_Silent((short) (bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
}
AFX_INLINE void CXTHtmlView::GoBack() {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->GoBack();
}
AFX_INLINE void CXTHtmlView::GoForward() {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->GoForward();
}
AFX_INLINE void CXTHtmlView::GoHome() {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->GoHome();
}
AFX_INLINE void CXTHtmlView::GoSearch() {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->GoSearch();
}
AFX_INLINE void CXTHtmlView::Refresh() {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->Refresh();
}
AFX_INLINE void CXTHtmlView::Refresh2(int nLevel) {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->Refresh2(COleVariant((long) nLevel, VT_I4));
}
AFX_INLINE void CXTHtmlView::Stop() {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->Stop();
}
AFX_INLINE void CXTHtmlView::SetFullScreen(BOOL bNewValue) {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_FullScreen((short) (bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
}
AFX_INLINE void CXTHtmlView::SetAddressBar(BOOL bNewValue) {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_AddressBar((short) (bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
}
AFX_INLINE void CXTHtmlView::SetHeight(long nNewValue) {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_Height(nNewValue);
}
AFX_INLINE void CXTHtmlView::PutProperty(LPCTSTR lpszPropertyName, long lValue) {
	ASSERT(m_pBrowserApp != NULL); ASSERT(m_pBrowserApp != NULL); PutProperty(lpszPropertyName, COleVariant(lValue, VT_UI4));
}
AFX_INLINE void CXTHtmlView::PutProperty(LPCTSTR lpszPropertyName, short nValue) {
	ASSERT(m_pBrowserApp != NULL); ASSERT(m_pBrowserApp != NULL); PutProperty(lpszPropertyName, COleVariant(nValue, VT_UI2));
}
AFX_INLINE void CXTHtmlView::PutProperty(LPCTSTR lpszPropertyName, LPCTSTR lpszValue) {
	ASSERT(m_pBrowserApp != NULL); ASSERT(m_pBrowserApp != NULL); PutProperty(lpszPropertyName, COleVariant(lpszValue, VT_BSTR));
}
AFX_INLINE void CXTHtmlView::PutProperty(LPCTSTR lpszPropertyName, double dValue) {
	ASSERT(m_pBrowserApp != NULL); ASSERT(m_pBrowserApp != NULL); PutProperty(lpszPropertyName, COleVariant(dValue));
}
AFX_INLINE void CXTHtmlView::SetTop(long nNewValue) {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_Top(nNewValue);
}
AFX_INLINE void CXTHtmlView::SetLeft(long nNewValue) {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_Left(nNewValue);
}
AFX_INLINE void CXTHtmlView::SetStatusBar(BOOL bNewValue) {
	ASSERT(m_pBrowserApp != NULL); m_pBrowserApp->put_StatusBar((short) (bNewValue ? AFX_OLE_TRUE : AFX_OLE_FALSE));
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTHTMLVIEW_H__)


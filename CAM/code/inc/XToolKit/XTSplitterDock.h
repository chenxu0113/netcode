//:Ignore
// XTSplitterDock.h interface for the CXTSplitterRowDock class.
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

#if !defined(__XTSPLITTERDOCK_H__)
#define __XTSPLITTERDOCK_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

// class forwards.
class CXTDockBar;

//////////////////////////////////////////////////////////////////////
// CXTSplitterDock is a stand alone base class.  It is used to create a
// CXTSplitterDock class object.
//
class _XT_EXT_CLASS CXTSplitterDock
{
public:

    // Constructs a CXTSplitterDock object.
	//
	CXTSplitterDock(
		// A pointer to a valid CXTDockBar object.
		CXTDockBar *pBar,
		// A reference to a valid CRect object.
		const CRect &rcCurrent,
		// Type of splitter bar, either XT_SPLITTER_VERT or XT_SPLITTER_HORZ.
		const int nType,
		// Index into a control bar array.
		const int nPos);

	// Destroys a CXTSplitterDock object, handles clean up and de-allocation.
	//
	virtual ~CXTSplitterDock();
	
	//:Ignore
	/////////////////////////////////////////////////////////////////////////
	// Current Size and Position
	/////////////////////////////////////////////////////////////////////////
	//:End Ignore

	// Current size of the splitter rect.
	CRect m_rcCurrent;
	// Current size of the tracking rect.
	CRect m_rcTrack;
	// Represents the current cursor position.
	CPoint m_ptCurrent;

	//:Ignore
	/////////////////////////////////////////////////////////////////////////
	// Width of the Row
	/////////////////////////////////////////////////////////////////////////
	//:End Ignore

	// Original height of the splitter.
	int m_nStartHeight;
	// Current height of the splitter.
	int m_nCurrentHeight;
	// Maximum height for the splitter.
	int m_nMaxHeight;
	// Minimum height for the splitter.
	int m_nMinHeight;

	//:Ignore
	/////////////////////////////////////////////////////////////////////////
	// Vertical or Horizontal
	/////////////////////////////////////////////////////////////////////////
	//:End Ignore

	// Type of splitter bar either, XT_SPLITTER_VERT or XT_SPLITTER_HORZ.
	int m_nType;
	// Index into a control bar array.
	int m_nControlBar;
	// Pointer to the dockbar.
	CXTDockBar*	m_pDockBar;

	// This member function is called to move the splitter during tracking
	// operations. Returns 'true' if successful, otherwise returns 'false'.
	//
	virtual bool Move(
		// Represents the current cursor position.
		CPoint point);

	// This member function is called during tracking operations. Returns
	// 'true' if successful, otherwise returns 'false'.
	//
	virtual bool StartTrack(
		// Represents the current cursor position.
		CPoint point);

	// This member function is called to terminate the tracking operation.
	// Returns 'true' if successful, otherwise returns 'false'.
	//
	virtual bool EndTrack(
		// Represents the current cursor position.
		CPoint point);

	// This member function is called to determine if the splitter is horizontal.
	// Returns TRUE if the splitter is horizontal, otherwise returns FALSE.
	//
	BOOL IsHorz();

	// This member function is called to determine if the cursor position
	// is inside of the splitter area. Returns TRUE if the cursor position
	// specified by 'point' falls inside of the splitter area, otherwise returns
	// FALSE.
	//
	BOOL HitTest(
		// A reference to a valid CPoint object.
		const CPoint &point);

	// This member function is called to set the length for the splitter.
	//
	void SetLength(
		// New size for the splitter.
		int nLength);

	// This member function is called to display the tracking rect when the
	// splitter is moved. Returns 'true' if successful, otherwise returns
	// 'false'.
	//
	bool OnInvertTracker();

	// This member function is called to draw the splitter during paint operations.
	//
	void Draw(
		// A CDC pointer to a valid device context.
		CDC *pDC);
	
protected:

	// This member function is called internally to set the new height for
	// the splitter bar based on the current control bar size.
	//
	virtual void SetNewHeight();

	// This member function is called internally to calculate the new height
	// for the splitter bar based on the current control bar size.
	//
	virtual void CalcMaxMinHeight();
};

//////////////////////////////////////////////////////////////////////
// CXTSplitterRowDock is a CXTSplitterDock derived class.  It is used to
// create a CXTSplitterRowDock class object.
//
class _XT_EXT_CLASS CXTSplitterRowDock : public CXTSplitterDock  
{
public:

    // Constructs a CXTSplitterRowDock object.
	//
	CXTSplitterRowDock(
		// A pointer to a valid CXTDockBar object.
		CXTDockBar *pBar,
		// A reference to a valid CRect object.
		const CRect &rcCurrent,
		// Type of splitter bar, either XT_SPLITTER_VERT or XT_SPLITTER_HORZ.
		const int nType,
		// Index into a control bar array.
		const int nPos);

	// This member function is called to move the splitter during tracking
	// operations. Returns 'true' if successful, otherwise returns 'false'.
	//
	virtual bool Move(
		// Represents the current cursor position.
		CPoint point);

protected:

	// This member function is called internally to set the new height for
	// the splitter bar based on the current control bar size.
	//
	virtual void SetNewHeight();

	// This member function is called internally to calculate the new height
	// for the splitter bar based on the current control bar size.
	//
	virtual void CalcMaxMinHeight();
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTSplitterDock::IsHorz() {
	return (m_nType == XT_SPLITTER_HORZ);
}
AFX_INLINE BOOL CXTSplitterDock::HitTest(const CPoint &point) {
	return m_rcCurrent.PtInRect(point);
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTSPLITTERDOCK_H__)

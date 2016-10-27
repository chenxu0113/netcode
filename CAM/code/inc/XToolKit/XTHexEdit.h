//:Ignore
// XTHexEdit.h interface for the CXTHexEdit class.
//
// This file is a part of the Xtreme Toolkit for MFC.
// ©1998-2003 Codejock Software, All Rights Reserved.
//
// This source code can only be used under the terms and conditions 
// outlined in the accompanying license agreement.
//
// support@codejock.com
// http://www.codejock.com
//--------------------------------------------------------------------
// Based on the code created by Andreas Saurwein
// mailto:saurwein@uniwares.de
//--------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
//:End Ignore

#if !defined(__XTHEXEDIT_H__)
#define __XTHEXEDIT_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTHexEdit is a CEdit derived class.  It is used to create an edit control
// which will allow editing in hex display format.
//
class _XT_EXT_CLASS CXTHexEdit : public CEdit
{
	DECLARE_DYNAMIC(CXTHexEdit)

public:

    // Constructs a CXTHexEdit object.
	//
	CXTHexEdit();

	// Destroys a CXTHexEdit object, handles cleanup and de-allocation.
	//
    virtual ~CXTHexEdit();

public:
	
	// Enumerated value that represents the current editing mode.
	//
	enum EDITMODE
	{ 
		// No editing mode specified.
		EDIT_NONE, 
		// ASCII editing mode.
		EDIT_ASCII, 
		// High editing mode.
		EDIT_HIGH, 
		// Low editing mode.
		EDIT_LOW 
	};

	// Pointer to data.
	LPBYTE m_pData;
	// Length of data.
	int m_nLength;
	// Maximum size of data.
	UINT m_nMaxLength;
	// Offset of first visible byte on the screen.
	int m_nTopIndex;
	// Base address for data.
	DWORD m_dwBaseAddress;
	// Address under the cursor.
	int m_nCurrentAddress;
	// Current editing mode: address/hex/ASCII.
	EDITMODE m_eEditMode;
	// Start address of the selection.
	int m_nSelStart;
	// End address of the selection.
	int m_nSelEnd;
	// Bytes per row.
	int m_nBytePerRow;
	// Calculate bytes-per-row dynamically.
	BOOL m_bDynamicBPR;
	// Lines per page.
	int m_nLinesPerPage;
	// 'true' when showing the address part.
	BOOL m_bShowAddress;
	// 'true' when showing the ASCII part.
	BOOL m_bShowAscii;
	// 'true' when showing the hex part.
	BOOL m_bShowHex;
	// 4/8 byte address.
	BOOL m_bAddressIsWide;
	// Height, in pixels, of a single line displayed in the control.  
	int m_nLineHeight;
	// Width, in pixels, of a single character displayed in the control (uniform
	// width).
	int m_nNullWidth;
	// Value used internally to indicate that the font and size info need to be
	// updated on the next paint cycle.
	BOOL m_bUpdate;
	// Internally used value - X position where hex part starts.
	int m_nOffHex;
	// Internally used value - X position where ASCII part starts.
	int m_nOffAscii;
	// Internally used value - X position where address part starts.
	int m_nOffAddress;
	// Current caret position.
	CPoint m_ptEditPos;
    // A boolean value indicating whether or not deletions are allowed.
	bool m_bAllowDeletes;

protected:
	
	// TRUE to display a flashing caret at the current caret position.
	BOOL m_bShowCaret;
	// Current size of caret.
	CSize m_szCaret;
	// RGB value that represents the background color of the hex edit control.
	COLORREF m_crBack;
	// RGB value that represents the text color of the hex edit control.
	COLORREF m_crText;
	// RGB value that represents the disabled background color for the hex
	// edit control.
	COLORREF m_crDisabledBack;
	// RGB value that represents the disabled text color of the hex edit control.
	COLORREF m_crDisabledText;
	// RGB value that represents the highlighted text color of the hex edit
	// control.
	COLORREF m_crHighlightText;
	// RGB value that represents the highlighted background color of the hex
	// edit control.
	COLORREF m_crHighlightBack;
	// RGB value that represents the disabled highlighted text color of the
	// hex edit control.
	COLORREF m_crDisabledHighlightText;
	// RGB value that represents the disabled highlighted background color of
	// the hex edit control.
	COLORREF m_crDisabledHighlightBack;
	// Represents the default font used by the hex edit control.
	CFont m_fontHex;

public:

	//:Ignore
    //{{AFX_VIRTUAL(CXTHexEdit)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
    //:End Ignore

public:

	// Call this member function to return the current background color for
	// the hex edit control.  The current background color depends on the
	// enabled, disabled, or read-only state of the control. Returns an RGB
	// value that represents the current background color for the control.
	//
	COLORREF GetActualBackColor();

	// Call this member function to return the current text color for the hex
	// edit control.  The current text color depends on the enabled, disabled,
	// or read-only state of the control. Returns an RGB value that represents
	// the current text color for the control.
	//
	COLORREF GetActualTextColor();

	// Call this member function to return the current highlight background
	// color for the hex edit control.  The current highlight background color
	// depends on the enabled, disabled, or read-only state of the control.
	// Returns an RGB value that represents the current highlight background
	// color for the control.
	//
	COLORREF GetActualHighlightBackColor();

	// Call this member function to return the current highlight text color
	// for the hex edit control.  The current highlight text color depends
	// on the enabled, disabled, or read-only state of the control. Returns
	// an RGB value that represents the current highlight text color for the
	// control.
	//
	COLORREF GetActualHighlightTextColor();

	// Call this member function to set the background color for the hex edit
	// control. 
	//
	void SetBackColor(
		// An RGB value that represents the background color of the hex edit
		// control.
		COLORREF crBack);

	// Call this member function to retrieve the current background color
	// for the hex edit control. Returns an RGB value that represents the
	// current background color for the hex edit control.
	//
	COLORREF GetBackColor();

	// Call this member function to set the text color for the hex edit control.
	//
	void SetTextColor(
		// An RGB value that represents the text color of the hex edit control.
		COLORREF crText);

	// Call this member function to retrieve the current text color for the
	// hex edit control. Returns an RGB value that represents the current
	// text color for the hex edit control.
	//
	COLORREF GetTextColor();

	// Call this member function to set the disabled background color for
	// the hex edit control.
	//
	void SetDisabledBackColor(
		// An RGB value that represents the disabled background color of the
		// hex edit control.
		COLORREF crDisabledBack);

	// Call this member function to retrieve the current disabled background
	// color for the hex edit control. Returns an RGB value that represents
	// the current disabled background color for the hex edit control.
	//
	COLORREF GetDisabledBackColor();

	// Call this member function to set the disabled text color for the hex
	// edit control.
	//
	void SetDisabledTextColor(
		// An RGB value that represents the disabled text color of the hex edit
		// control.
		COLORREF crDisabledText);

	// Call this member function to retrieve the current disabled text color
	// for the hex edit control. Returns an RGB value that represents the
	// current disabled text color for the hex edit control.
	//
	COLORREF GetDisabledTextColor();

	// Call this member function to set the highlighted text color for the
	// hex edit control.
	//
	void SetHighlightTextColor(
		// An RGB value that represents the highlighted text color of the hex
		// edit control.
		COLORREF crHighlightText);

	// Call this member function to retrieve the current highlighted text
	// color for the hex edit control. Returns an RGB value that represents
	// the current highlighted text color for the hex edit control.
	//
	COLORREF GetHighlightTextColor();

	// Call this member function to set the highlight background color for
	// the hex edit control.
	//
	void SetHighlightBackColor(
		// An RGB value that represents the highlight background color of the
		// hex edit control.
		COLORREF crHighlight);

	// Call this member function to retrieve the current highlight background
	// color for the hex edit control. Returns an RGB value that represents
	// the current highlight background color for the hex edit control.
	//
	COLORREF GetHighlightBackColor();

	// Call this member function to set the disabled highlight text color for
	// the hex edit control.
	//
	void SetDisabledHighlightTextColor(
		// An RGB value that represents the disabled highlight text color of
		// the hex edit control.
		COLORREF crDisabledHighlightText);

	// Call this member function to retrieve the current disabled highlight
	// text color for the hex edit control. Returns an RGB value that represents
	// the current disabled highlight text color for the hex edit control.
	//
	COLORREF GetDisabledHighlightTextColor();

	// Call this member function to set the disabled highlight background
	// color for the hex edit control.
	//
	void SetDisabledHighlightBackColor(
		// An RGB value that represents the disabled highlight background color
		// of the hex edit control.
		COLORREF crDisabledHighlight);

	// Call this member function to retrieve the current disabled highlight
	// background color for the hex edit control. Returns an RGB value that
	// represents the current disabled highlight background color for the
	// hex edit control.
	//
	COLORREF GetDisabledHighlightBackColor();
	
	// This member function sets the font for the hex edit control to the
	// default "Courier New" font.
	//
	void RestoreDefaultFont();

	// This member function sets the font used by the hex edit control.
	//
	void SetHexFont(
		// Points to a valid CFont object.
		CFont* pFont);

	// This member function sets the font used by the hex edit control.
	//
	void SetHexFont(
		// Points to a valid LOGFONT structure.
		LOGFONT* pLogFont);

	// This member function enables or disables a flashing screen caret.
	//
	void EnableCaret(
		// 'true' to enable caret, 'false' to disable.
		bool bEnable);

	// This member function enables or disables a vertical and horizontal scroll
	// bar display.
	//
	void EnableScrollBars(
		// 'true' to display vertical scroll bar.
		bool bVertical,
		// 'true' to display horizontal scroll bar.
		bool bHorizontal);

	// Call this member function to set the base address for the hex edit
	// control. Returns a DWORD value.
	//
	DWORD SetAddressBase(
		// DWORD value that represents the base address.
		DWORD dwBase);

    // Call this method to extract the data in the hex edit control. Returns
	// the length of the data.
	//
	int GetData(
		// Pointer to the buffer that will be receiving the data.
        LPBYTE pData,
		// Length of the target data buffer.
        int nBufferLength);

	// Call this member function to initialize the hex edit control with data
	// specified by 'pData'.
	//
	void SetData(
		// Pointer to a buffer that contains the initial data for the control.
        LPBYTE pData,
		// Length of the data buffer.
        int nLength,
		// Optional maximum length that the buffer can grow, or -1 if you
        // want to allow the buffer to grow indefinitely.
        int nMaxLength = 0);
       
    // Call this method to allow or disallow the deletion of characters
	// within the control.  For instance, calling SetAllowDeletes(false) will
	// disable the delete, backspace, and other such keys. Returns the previous
	// value.
	//
    bool SetAllowDeletes(
		// The new value for this setting.
        bool bVal = true);

    // This member function retrieves the current value of the Allow Deletes
	// setting. Returns 'true' if the Allow Deletes setting is enabled, or
	// 'false' if it is disabled. See the SetAllowDeletes() function for more
	// information.
	//
    bool GetAllowDeletes() const;

	// Call this member function to get the size of the current selection.
	// Returns a CSize object.
	//
	CSize GetSel();

	// Call this member function to set the current selection.
	//
	void SetSel(
		// Represents the position of the first character in the selection.
		int nSelStart,
		// Represents the position of the last character in the selection.
		int nSelEnd);

	// Call this member function to set the number of bytes per row.
	//
	void SetBPR(
		// Represents the number of bytes to display.
		int nBytePerRow);

	// Call this member function to set the display options for the hex edit
	// control.
	//
	void SetOptions(
		// TRUE to display the address value.
		BOOL bShowAddress,
		// TRUE to display the hex value.
		BOOL bShowHex,
		// TRUE to display the ASCII value.
		BOOL bShowAscii,
		// TRUE if the display is 8 byte, FALSE for 4 byte display.
		BOOL bAddressIsWide);

protected:

	//:Ignore
	void ScrollIntoView(int pData);
	void RepositionCaret(int pData);
	void Move(int x, int y);
	BOOL IsSelected();
	void UpdateScrollbars();
	void CreateEditCaret();
	void CreateAddressCaret();
	CPoint CalcPos(int x, int y);
	void SelInsert(int nSelStart, int nLength);
	void SelDelete(int nSelStart, int nSelEnd);
	void NormalizeSel();
	void RecalcBPR();
	void DoVScroll(UINT nSBCode, bool bMoveCaret);
    //:End Ignore

	//:Ignore
    //{{AFX_MSG(CXTHexEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg UINT OnGetDlgCode();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnEditClear();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditUndo();
	afx_msg void OnNcPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE UINT CXTHexEdit::OnGetDlgCode()  {
	return DLGC_WANTALLKEYS;
}
AFX_INLINE void CXTHexEdit::SetOptions(BOOL bShowAddress, BOOL bShowHex, BOOL bShowAscii, BOOL bAddressIsWide) {
	m_bShowHex = bShowHex; m_bShowAscii = bShowAscii; m_bShowAddress = bShowAddress; m_bAddressIsWide = bAddressIsWide; m_bUpdate = TRUE; RepositionCaret(m_nCurrentAddress);
}
AFX_INLINE void CXTHexEdit::SetBPR(int nBytePerRow) {
	m_nBytePerRow = nBytePerRow; m_bUpdate = TRUE; RepositionCaret(m_nCurrentAddress);
}
AFX_INLINE BOOL CXTHexEdit::IsSelected() {
	return m_nSelStart != 0xffffffff;
}
AFX_INLINE CSize CXTHexEdit::GetSel() {
	return CSize(m_nSelStart, m_nSelEnd);
}
AFX_INLINE int CXTHexEdit::GetData(LPBYTE pData, int nLength) {
	memcpy(pData, m_pData, __min(nLength, m_nLength)); return m_nLength;
}
AFX_INLINE void CXTHexEdit::SetBackColor(COLORREF crBack) {
	m_crBack = crBack;
}
AFX_INLINE void CXTHexEdit::SetTextColor(COLORREF crText) {
	m_crText = crText;
}
AFX_INLINE void CXTHexEdit::SetDisabledBackColor(COLORREF crDisabledBack) {
	m_crDisabledBack = crDisabledBack;
}
AFX_INLINE void CXTHexEdit::SetDisabledTextColor(COLORREF crDisabledText) {
	m_crDisabledText = crDisabledText;
}
AFX_INLINE void CXTHexEdit::SetHighlightTextColor(COLORREF crHighlightText) {
	m_crHighlightText = crHighlightText;
}
AFX_INLINE void CXTHexEdit::SetHighlightBackColor(COLORREF crHighlightBack) {
	m_crHighlightBack = crHighlightBack;
}
AFX_INLINE void CXTHexEdit::SetDisabledHighlightTextColor(COLORREF crDisabledHighlightText) {
	m_crDisabledHighlightText = crDisabledHighlightText;
}
AFX_INLINE void CXTHexEdit::SetDisabledHighlightBackColor(COLORREF crDisabledHighlightBack) {
	m_crDisabledHighlightBack = crDisabledHighlightBack;
}
AFX_INLINE COLORREF CXTHexEdit::GetBackColor() {
	return m_crBack;
}
AFX_INLINE COLORREF CXTHexEdit::GetTextColor() {
	return m_crText;
}
AFX_INLINE COLORREF CXTHexEdit::GetDisabledBackColor() {
	return m_crDisabledBack;
}
AFX_INLINE COLORREF CXTHexEdit::GetDisabledTextColor() {
	return m_crDisabledText;
}
AFX_INLINE COLORREF CXTHexEdit::GetHighlightTextColor() {
	return m_crHighlightText;
}
AFX_INLINE COLORREF CXTHexEdit::GetHighlightBackColor() {
	return m_crHighlightBack;
}
AFX_INLINE COLORREF CXTHexEdit::GetDisabledHighlightTextColor() {
	return m_crDisabledHighlightText;
}
AFX_INLINE COLORREF CXTHexEdit::GetDisabledHighlightBackColor() {
	return m_crDisabledHighlightBack;
}
AFX_INLINE void CXTHexEdit::EnableCaret(bool bEnable) {
	m_bShowCaret = bEnable;
}

AFX_INLINE bool CXTHexEdit::SetAllowDeletes(bool bVal) {
    const bool bOldVal = m_bAllowDeletes; m_bAllowDeletes = bVal; return bOldVal;
}

AFX_INLINE bool CXTHexEdit::GetAllowDeletes() const {
    return m_bAllowDeletes;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTHEXEDIT_H__)


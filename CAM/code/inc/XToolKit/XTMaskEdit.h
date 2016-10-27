//:Ignore
// XTMaskEdit.h interface for the CXTMaskEdit class.
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

#if !defined(__XTMASKEDIT_H__)
#define __XTMASKEDIT_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTMaskEdit is a CXTFlatEdit derived class.  It allows text masking to be
// applied to the control to format it for special editing restrictions.
//
class _XT_EXT_CLASS CXTMaskEdit : public CXTFlatEdit
{
	DECLARE_DYNAMIC(CXTMaskEdit)

public:

    // Constructs a CXTMaskEdit object.
    //
	CXTMaskEdit();

	// Destroys a CXTMaskEdit object, handles cleanup and de-allocation.
    //
    virtual ~CXTMaskEdit();

protected:

    // Current position of the first character in the current selection.
	int m_nStartChar;
	// Current position of the first non-selected character past the end of
	// the current selection.
	int m_nEndChar;
	// 'true' to use the edit mask.
	bool m_bUseMask;
	// 'true' to over type the text, set with VK_INSERT key press.
	bool m_bOverType;
	// 'true' to redo, or 'false' to undo.
	bool m_bRedo;
	// 'true' if mask edit has been modified.
	bool m_bModified;
	// Prompt character used to identify the text entry.
	TCHAR m_chPrompt;
	// Buffer that holds the actual edit text.
	CXTString m_strWindowText;
	// Buffer that holds the actual edit mask value.
	CString m_strMask;
	// Literal format that restricts where the user can enter text.
	CXTString m_strLiteral;
	// Contains the edit controls default display text.
	CString m_strDefault;
	// Holds the contents of the undo buffer.
	CString m_strUndoBuffer;
	// Holds the contents of the redo buffer.
	CString m_strRedoBuffer;

public:

	// Call this member function to enable or disable the mask for the mask
	// edit control.
	//
	void SetUseMask(
		// 'true' to enable the mask.  'false' to disable the mask.
		bool bUseMask);

	// This member function is called to determine if the mask for the edit
	// control can be used. Returns 'true' if the mask can be used, otherwise
	// returns 'false'.
	//
	bool CanUseMask();

	// This member function is called to enable or disable type over, also
	// known as insert mode.
	//
	void SetOverType(
		// 'true' to enable type over.
		bool bOverType);

	// This member function is called to determine if type over has been enabled.
	// Returns 'true' if type over is enabled, otherwise returns 'false'.
	//
	bool CanOverType();

	// This member function is called to determine if the index specified
	// by 'iPos' is a valid index for the currently displayed edit text. Returns
	// 'true' if the index is valid, otherwise returns 'false'.
	//
	bool PosInRange(
		// Index of the character to check.
		int iPos);

	// This member function retrieves the character that is currently used as
	// the mask prompt.  The mask prompt indicates that the field is editable.
	// Returns a TCHAR data type.
	//
	TCHAR GetPromptChar();

	// This member function is called to set the prompt character that is
	// displayed to the user that indicates the field can be edited.
	//
	void SetPromptChar(
		// A TCHAR data type.
		TCHAR ch);

	// This member function is called to perform a cut operation using the
	// currently selected text.
	//
	void MaskCut();

	// This member function is called to perform a copy operation using the
	// currently selected text.
	//
	void MaskCopy();

	// This member function is called to perform a paste operation using the
	// current clipboard text.
	//
	void MaskPaste();

	// This member function is called to clear the current text selection.
	//
	void MaskClear();

	// This member function is called to undo the previous action.
	//
	void MaskUndo();

	// This member function is called to select all text in the mask edit
	// control.
	//
	void MaskSelectAll();

	// This member function is called to determine if the text has been modified.
	// Returns 'true' if the text has changed, otherwise returns 'false'.
	//
	bool IsModified();

	// This member function will set the mask for the edit control. The values 
	// that can be set are:
	//
	// [pre][b]Mask Character[/b]    [b]Description[/b]
	// -------------------------------------------------
	// 0                 Numeric (0-9)
	// 9                 Numeric (0-9) or space (' ')
	// #                 Numeric (0-9) or space (' ') or ('+') or ('-')
	// L                 Alpha (a-Z) 
	// ?                 Alpha (a-Z) or space (' ') 
	// A                 Alpha numeric (0-9 and a-Z)
	// a                 Alpha numeric (0-9 and a-Z) or space (' ') 
	// &                 All print character only 
	// H                 Hex digit (0-9 and A-F)
	// X                 Hex digit (0-9 and A-F) and space (' ')
	// >                 Forces characters to upper case (A-Z)
	// <                 Forces characters to lower case (a-z)[/pre]
	//
	virtual void SetEditMask(
		// The format for the mask field.  For example, if you wanted to set
		// the mask for a phone number, and you only wanted digits to be entered,
		// your mask might look like this; _T("(000) 000-0000"). 
		LPCTSTR lpszMask,
		// The literal format is entered here. Wherever you place an underscore
		// ('_') is where the user will be allowed to enter data only. Using
		// the phone number example; _T("(___) ___-____"). 
		LPCTSTR lpszLiteral,
		// Text that is to be displayed when the control is initialized.  For
		// example; _T("(800) 555-1212"). If NULL, 'lpszLiteral' is used to initialize
		// the edit text.
		LPCTSTR lpszDefault=NULL);

	// This member function is used internally to validate the character indicated
	// by 'nChar'. Returns 'true' if successful, otherwise returns 'false'.
    //
	virtual bool CheckChar(
		// Contains the character code value of the key.
		UINT& nChar,
		// 'true' to enable beep.  'false' to disable beep.
		bool bBeep=true);

	// This member function is used internally to process the character passed
	// in by 'nChar' whose index is specified by 'nEndPos'. Returns 'true' if
	// successful, otherwise returns 'false'.
    //
	virtual bool ProcessMask(
		// Contains the character code value of the key.
		UINT& nChar,
		// Index of character in display string.
		int nEndPos);

	//:Ignore
    //{{AFX_VIRTUAL(CXTMaskEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	virtual bool CorrectPosition(int& iPos, bool bForward=true, bool bBeep=true);
	virtual void DeleteCharAt(int iPos);
	virtual void InsertCharAt(int iPos, TCHAR nChar);
	virtual bool CopyToClipboard(LPCTSTR lpszText);
	virtual CString GetMaskedText(int iPos=0);
	virtual BOOL SelectionMade();
    //:End Ignore

protected:
	//:Ignore
    //{{AFX_MSG(CXTMaskEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditCut( );
	afx_msg void OnEditCopy( ); 
	afx_msg void OnEditPaste( ); 
	afx_msg void OnEditClear( ); 
	afx_msg void OnEditUndo( );
	afx_msg void OnEditSelectAll( );
	//}}AFX_MSG
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTMaskEdit::SetUseMask(bool bUseMask ) {
	m_bUseMask = bUseMask;
}
AFX_INLINE bool CXTMaskEdit::CanUseMask() {
	return m_bUseMask;
}
AFX_INLINE void CXTMaskEdit::SetOverType(bool bOverType) {
	m_bOverType = bOverType;
}
AFX_INLINE bool CXTMaskEdit::CanOverType() {
	return m_bOverType;
}
AFX_INLINE bool CXTMaskEdit::PosInRange(int iPos) {
	return ( ( iPos >= 0 ) && ( iPos < m_strLiteral.GetLength( ) ) );
}
AFX_INLINE TCHAR CXTMaskEdit::GetPromptChar() {
	return m_chPrompt;
}
AFX_INLINE BOOL CXTMaskEdit::SelectionMade() {
	GetSel( m_nStartChar, m_nEndChar ); return ( m_nStartChar != m_nEndChar );
}
AFX_INLINE bool CXTMaskEdit::IsModified() {
	return m_bModified;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTDateEdit is a CXTMaskEdit derived class.  It is specifically 
// geared toward editing date fields.
//
class _XT_EXT_CLASS CXTDateEdit : public CXTMaskEdit
{
	DECLARE_DYNAMIC(CXTDateEdit)

public:

    // Constructs a CXTDateEdit object.
    //
	CXTDateEdit();

	// This member function will set the time based on the text string 
	// passed in as 'strDate'.
    //
	virtual void SetDateTime(
		// A reference to a COleDateTime object that represents the 
		// date to display.
		COleDateTime& dt);

	// This member function will set the time based on the text string 
	// passed in as 'strDate'.
    //
	virtual void SetDateTime(
		// A NULL terminated string that represents the date to display.
		CString strDate);

	// This member function will retrieve a COleDateTime object that 
	// represents the currently displayed date. Returns a COleDateTime object
	// representing the currently displayed date.
    //
	virtual COleDateTime GetDateTime();

	// This member function will retrieve a CString object that represents 
	// the currently displayed date. Returns a CString object representing
	// the currently displayed date.
    //
	virtual CString GetWindowDateTime();

	// This member function will read the date string passed in as 'lpszData'
	// and is converted and returned as a COleDateTime object. Returns a COleDateTime
	// object that represents the converted date string.
    //
	virtual COleDateTime ReadOleDateTime(
		// A NULL terminated string that represents the date to convert.
		LPCTSTR lpszData);

	// This member function is used internally to process the character passed
	// in by 'nChar' whose index is specified by 'nEndPos'. Returns 'true' if
	// successful, otherwise returns 'false'.
    //
	virtual bool ProcessMask(
		// Contains the character code value of the key.
		UINT nChar,
		// Index of character in display string.
		int nEndPos);

	// This member function will read the date passed in as 'dt', and format
	// the 'strData' string as DD/MM/YY.
    //
	virtual void FormatOleDateTime(
		// String reference that is filled with the date.
		CString& strData,
		// COleDateTime object that represents the date to format.
		COleDateTime dt);
};

//////////////////////////////////////////////////////////////////////
// CXTTimeEdit is a CXTDateEdit derived class.  It is specifically geared
// toward editing time fields.
//
class _XT_EXT_CLASS CXTTimeEdit : public CXTDateEdit
{
	DECLARE_DYNAMIC(CXTTimeEdit)

public:

    // Constructs a CXTTimeEdit object.
    //
	CXTTimeEdit();

protected:

	// Represents the hours to display.
	int m_iHours;
	// Represents the minutes to display.
	int	m_iMins;
	// 'true' if military time is used.
	bool m_bMilitary;

public:

	// This member function will update the hours displayed.
    //
	virtual void SetHours(
		// The new hour to be displayed.
		int nHours);

	// This member function will update the minutes displayed.
    //
	virtual void SetMins(
		// The new minutes to be displayed.
		int nMins);

	// This member function will update the hours and minutes that are displayed
	// for the time edit control.
	//
	virtual void SetTime(
		// The new hour to be displayed.
		int nHours,
		// The new minutes to be displayed.
		int nMins);

	// This member function will retrieve the hours displayed for the time edit
	// control. Returns an integer value that represents the hours displayed.
	//
	int GetHours() const;

	// This member function will retrieve the minutes displayed for the time
	// edit control. Returns an integer value that represents the minutes displayed.
	//
	int GetMins() const;

	// This member function is used internally to process the character
	// passed in by 'nChar' whose index is specified by 'nEndPos'. Returns 'true'
	// if successful, otherwise returns 'false'.
	//
	virtual bool ProcessMask(
		// Contains the character code value of the key.
		UINT nChar,
		// Index of the character in the display string.
		int nEndPos);

	// This member function will read the time passed in as 'dt',
	// and format the 'strData' string as HH:MM.
    //
	virtual void FormatOleDateTime(
		// String reference that is filled with the time. 
		CString& strData,
		// COleDateTime object that represents the time to format.
		COleDateTime dt);
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTTimeEdit::GetHours() const {
	return m_iHours;
}
AFX_INLINE int CXTTimeEdit::GetMins() const {
	return m_iMins;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTMASKEDIT_H__)


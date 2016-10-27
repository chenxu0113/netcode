//:Ignore
// XTFontCombo.h interface for the CXTFontCombo class.
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

#if !defined(__XTFONTCOMBO_H__)
#define __XTFONTCOMBO_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTFontCombo is a CXTFlatComboBox derived class.  It is used to create
// a combo box that displays a drop list of available fonts for your system.
// The fonts are displayed in their various styles.
//
class _XT_EXT_CLASS CXTFontCombo : public CXTFlatComboBox
{
    DECLARE_DYNAMIC(CXTFontCombo)

public:

    // Constructs a CXTFontCombo object.
    //
    CXTFontCombo();

    // Destroys a CXTFontCombo object, handles cleanup and de-allocation.
    //
    virtual ~CXTFontCombo();
    
protected:

	// Represents system metrics for SM_CYHSCROLL.
	int m_cyHScroll;
	// Represents system metrics for SM_CYEDGE.
	int m_cyEdge;
	// String displayed for symbol characters.
	CString m_strSymbol;
	// Enumerated style indicating how to display the font list.
	eSTYLE m_eStyle;
	// True type font image list.
	CImageList m_ilFontType;

public:

    // Call this member function to get the logfont for the currently selected
	// item. Returns 'true' if successful, otherwise returns 'false'.
    //
    virtual bool GetSelFont(
		// Reference to an XT_LOGFONT structure.
        XT_LOGFONT& lf);

    // Call this member function to get the logfont for the currently selected
	// item. Returns 'true' if successful, otherwise returns 'false'.
    //
	virtual bool GetSelFont(
		// A reference to a valid CString object to receive the logfont face
		// name.
		CString& strFaceName);

    // Call this member function to select the logfont for the list box.
    // Returns 'true' if successful, otherwise returns 'false'.
    //
    virtual bool SetSelFont(
		// Reference to an XT_LOGFONT structure.
        XT_LOGFONT& lf);

    // Call this member function to select the logfont for the list box.
    // Returns 'true' if successful, otherwise returns 'false'.
    //
	virtual bool SetSelFont(
		// A NULL terminated string that represents the logfont face name.
		CString strFaceName);

	// Call this member function to set the font display style for the font
	// list box.  There are three styles to choose from.  They include displaying
	// the font in the default GUI font, displaying the font in its own font
	// style, or displaying both the font name in the default GUI font and
	// a sample to the right.
	// 
	void SetListStyle(
		// Specifies the style for the font list box.  Styles can be any one of
		// the following combinations:
		// [ul]
		// [li][b]XT_FLB_NAME_GUI[/b] - Display font name with GUI font style.[/li]
		// [li][b]XT_FLB_NAME_SAMPLE[/b] - Display font name with its own font
		//        style.[/li]
		// [li][b]XT_FLB_BOTH[/b] - Display font name with GUI font style, then
		//        a sample display to the right.[/li]
		// [/ul]
		eSTYLE eStyle);

	// Call this member function to initialize the font list box and populate it
	// with a list of avaliable fonts.
	//
	virtual void InitControl(
		// A NULL terminated string that represents the logfont face name.
        LPCTSTR lpszFaceName=NULL,
        // The minimum allowable width of the list box portion of the combo
		// box, in pixels.
        UINT nWidth=0,
		// TRUE to enable autocompletion, otherwise FALSE.
        BOOL bEnable=TRUE );

    //:Ignore
    //{{AFX_VIRTUAL(CXTFontCombo)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	//}}AFX_VIRTUAL
    //:End Ignore
    
    //:Ignore
    //{{AFX_MSG(CXTFontCombo)
	//}}AFX_MSG
    //:End Ignore
	
    DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTFontCombo::SetListStyle(eSTYLE eStyle) {
    m_eStyle = eStyle;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTFONTCOMBO_H__)

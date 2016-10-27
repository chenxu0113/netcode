//:Ignore
// XTPropertyGridItemFont.h interface for the CXTPropertyGridItemFont class.
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

#if !defined(__XTPROPERTYGRIDITEMFONT_H__)
#define __XTPROPERTYGRIDITEMFONT_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTPropertyGridItemFont is a CXTPropertyGridItem derived class.  
// It is used to create Font-value item in a Property Grid control.
//
class _XT_EXT_CLASS CXTPropertyGridItemFont : public CXTPropertyGridItem
{
public:
	
	// Constructs a CXTPropertyGridItemFont object.
	//
	CXTPropertyGridItemFont(
		// Caption of the item.
		CString strCaption,
		// Initial value
		LOGFONT& font);

	// Constructs a CXTPropertyGridItemFont object.
	//
	CXTPropertyGridItemFont(
		// Identifier of the item.
		UINT nID,
		// Initial value
		LOGFONT& font);

	// Destroys a CXTPropertyGridItemFont object.
	//
	virtual ~CXTPropertyGridItemFont(void);

private:
	
	//:Ignore
	LOGFONT m_lfValue;
	//:End Ignore

public:

	// Call this method to change item's value
	//
	void SetFont(
		// The new value of the item. 	
		LOGFONT& font);
	
	// Call this method to get the LOGFONT value of the item.
	// 
	void GetFont(
		//Pointer to the LOGFONT structure to receive the font information.
		LOGFONT* lf);

protected:
	
	//:Ignore
	virtual void OnInplaceButtonDown();
	//:End Ignore
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPropertyGridItemFont::GetFont(LOGFONT* lf) {
	ASSERT(lf != NULL); memcpy(lf, &m_lfValue, sizeof(LOGFONT));
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTPROPERTYGRIDITEMFONT_H__)

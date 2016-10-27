//:Ignore
// XTPropertyGridItemColor.h interface for the CXTPropertyGridItemColor class.
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

#if !defined(__XTPROPERTYGRIDITEMCOLOR_H__)
#define __XTPROPERTYGRIDITEMCOLOR_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTPropertyGridItemColor is a CXTPropertyGridItem derived class.  
// It is used to create RGB-value item in a Property Grid control.
//
class _XT_EXT_CLASS CXTPropertyGridItemColor : public CXTPropertyGridItem
{
public:

	// Constructs a CXTPropertyGridItemColor object.
	//
	CXTPropertyGridItemColor(
		// Caption of the item
		CString strCaption,
		// Initial value
		COLORREF clr = 0);

	// Constructs a CXTPropertyGridItemColor object.
	//
	CXTPropertyGridItemColor(
		// Identifier of the item
		UINT nID,
		// Initial value
		COLORREF clr = 0);

	// Destroys a CXTPropertyGridItemColor object.
	//
	virtual ~CXTPropertyGridItemColor(void);

private:

	//:Ignore
	COLORREF m_clrValue;
	//:End Ignore

public:

	// Call this method to change item's value
	//
	void SetColor(
		// The new RGB-value of the item. 	
		COLORREF clr);

	// Call this method to get the RGB value of the item.
	// 
	COLORREF GetColor();

	// The StringToRGB function converts a string to a COLORREF.
	static COLORREF StringToRGB(
		// String that is converted to a COLORREF
		CString str);

	// The RGBToString function converts a COLORREF to a string.
	static CString RGBToString(
		// COLORREF that is converted to a string
		COLORREF clr);

protected:

	//:Ignore
	virtual BOOL OnDrawItemValue(CDC& dc, CRect rcValue);
	virtual CRect GetValueRect();
	virtual void SetValue(CString strValue);
	virtual void OnInplaceButtonDown();
	//:End Ignore
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE COLORREF CXTPropertyGridItemColor::GetColor() {
	return m_clrValue;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif //#if !defined(__XTPROPERTYGRIDITEMCOLOR_H__)

//:Ignore
// XTPropertyGridItemBool.h interface for the CXTPropertyGridItemBool class.
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

#if !defined(__XTPROPERTYGRIDITEMBOOL_H__)
#define __XTPROPERTYGRIDITEMBOOL_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTPropertyGridItemBool is a CXTPropertyGridItem derived class.  
// It is used to create Boolean-value item in a Property Grid control.
//
class _XT_EXT_CLASS CXTPropertyGridItemBool : public CXTPropertyGridItem
{
public:

	// Constructs a CXTPropertyGridItemBool object.
	//
	CXTPropertyGridItemBool(
		// Caption of the item.
		CString strCaption,
		// Initial value
		BOOL bValue = FALSE);

	// Constructs a CXTPropertyGridItemBool object.
	//
	CXTPropertyGridItemBool(
		// Identifier of the item.
		UINT nID,
		// Initial value
		BOOL bValue = FALSE);

	// Destroys a CXTPropertyGridItemBool object.
	//
	virtual ~CXTPropertyGridItemBool(void);

	// Call this method to change item's value
	//
	void SetBool(
		// The new BOOL-value of the item. 	
		BOOL bValue);
	
	// Call this method to get the Boolean value of the item.
	// 
	BOOL GetBool();

//:Ignore
protected:
	void SetValue(CString strValue);
	void _Init(BOOL bValue);

private:
	BOOL m_bValue;
//:End Ignore

};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPropertyGridItemBool::GetBool() {
	return m_bValue;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTPROPERTYGRIDITEMBOOL_H__)

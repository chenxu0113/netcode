//:Ignore
// XTPropertyGridItemNumber.h interface for the CXTPropertyGridItemNumber class.
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

#if !defined(__XTPROPERTYGRIDITEMNUMBER_H__)
#define __XTPROPERTYGRIDITEMNUMBER_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTPropertyGridItemNumber is a CXTPropertyGridItem derived class.  
// It is used to create integer value item in a Property Grid control.
//
class _XT_EXT_CLASS CXTPropertyGridItemNumber : public CXTPropertyGridItem
{
public:

	// Constructs a CXTPropertyGridItemNumber object.
	//
	CXTPropertyGridItemNumber(
		// Caption of the item.
		CString strCaption,
		// Initial value
		long nValue = 0);

	// Constructs a CXTPropertyGridItemNumber object.
	//
	CXTPropertyGridItemNumber(
		// Identifier of the item.
		UINT nID,
		// Initial value
		long nValue = 0);
	
	// Destroys a CXTPropertyGridItemNumber object.
	//
	virtual ~CXTPropertyGridItemNumber(void);

private:

	//:Ignore
	long m_nValue;
	//:End Ignore

public:

	// Call this method to change item's value
	//
	void SetNumber(
		// The new integer value of the item. 			
		long nValue);

	// Call this method to get the integer value of the item.
	// 
	long GetNumber();

protected:
	
	//:Ignore
	virtual void SetValue(CString strValue);
	//:End Ignore
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE long CXTPropertyGridItemNumber::GetNumber() {
	return m_nValue;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTPROPERTYGRIDITEMNUMBER_H__)

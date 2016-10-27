//:Ignore
// XTPropertyGridItemSize.h interface for the CXTPropertyGridItemSize class.
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

#if !defined(__XTPROPERTYGRIDITEMSIZE_H__)
#define __XTPROPERTYGRIDITEMSIZE_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTPropertyGridItemSize is a CXTPropertyGridItem derived class.  
// It is used to create size-value item in a Property Grid control.
//
class _XT_EXT_CLASS CXTPropertyGridItemSize : public CXTPropertyGridItem
{
public:
	// Constructs a CXTPropertyGridItemSize object.
	//
	CXTPropertyGridItemSize(
		// Caption of the item.
		CString strCaption,
		// Initial value
		CSize size = 0);

	// Constructs a CXTPropertyGridItemSize object.
	//
	CXTPropertyGridItemSize(
		// Identifier of the item.
		UINT nID,
		// Initial value
		CSize size = 0);

	// Destroys a CXTPropertyGridItemSize object.
	//
	virtual ~CXTPropertyGridItemSize(void);

private:
	
	//:Ignore
	class CXTPropertyGridItemSizeWidth;
	class CXTPropertyGridItemSizeHeight;

    CSize                          m_szValue;
    CXTPropertyGridItemSizeWidth*  m_itemWidth;
    CXTPropertyGridItemSizeHeight* m_itemHeight;
	//:End Ignore

public:
	
	// Call this method to change item's value
	//
	void SetSize(
		// The new Size value of the item. 					
		CSize size);

	// Call this method to get the size value of the item.
	// 
	CSize GetSize();

private:

	//:Ignore
	void SetWidth(CString strWidth);
	void SetHeight(CString strHeight);

	CString SizeToString(CSize size);
	CSize StringToSize(CString str);

	virtual void OnAddChildItem();
	virtual void SetValue(CString strValue);
	
	void UpdateChilds();

	friend class CXTPropertyGridItemSizeWidth;
	friend class CXTPropertyGridItemSizeHeight;
	//:End Ignore
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE CSize CXTPropertyGridItemSize::GetSize() {
	return m_szValue;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTPROPERTYGRIDITEMSIZE_H__)

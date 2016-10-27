//:Ignore
// XTMemDC.h interface for the CXTMemDC class.
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

#if !defined(__XTMEMDC_H__)
#define __XTMEMDC_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTMemDC is a CDC derived class.  CXTMemDC is an extension of CDC that
// helps eliminate screen flicker when windows are resized, by painting
// to an off screen bitmap.  The class then uses CDC::BitBlt to copy the
// bitmap back into the current device context after all items have been
// painted.
//
class _XT_EXT_CLASS CXTMemDC : public CDC  
{
public:
    DECLARE_DYNAMIC(CXTMemDC);
    
	// Constructs a CXTMemDC object.
	//
    CXTMemDC(
		// A Pointer to the current device context. 
        CDC* pDC,
		// Represents the size of the area to paint.
        const CRect& rect,
		// An RGB value that represents the background color of the area to paint.
		// Defaults to COLOR_3DFACE.  Pass in a value of -1 to disable background
		// painting.
        COLORREF clrColor=xtAfxData.clr3DFace);

    // Destroys a CXTMemDC object, handles cleanup and de-allocation.
	//
    virtual ~CXTMemDC();

	// This member function is called to set the valid flag to false so the
	// offscreen device context will not be drawn.
	//
	void Discard();

    // This member function gets content from the given DC.
	//
	void FromDC();

    // This member function retrieves a reference to the CBitmap object
	// associated with the memory device context. Returns a reference to the
	// CBitmap object associated with the memory device context.
	//
	CBitmap& GetBitmap();
	
protected:
    
	// Saves the CDC passed in constructor.
	CDC* m_pDC;
	// Rectangle of the drawing area.
	CRect m_rc;
	// Offscreen bitmap.
	CBitmap m_bitmap;
	// Original GDI object.
	HBITMAP m_hOldBitmap;
	// Flag used for autodraw in destructor.
	BOOL m_bValid;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE CBitmap& CXTMemDC::GetBitmap() {
	return m_bitmap;
}
AFX_INLINE void CXTMemDC::Discard() {
	m_bValid = FALSE;
}
AFX_INLINE void CXTMemDC::FromDC() {
	BitBlt(0, 0, m_rc.Width(), m_rc.Height(), m_pDC, m_rc.left, m_rc.top, SRCCOPY);            
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTMEMDC_H__)

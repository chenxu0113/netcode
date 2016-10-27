//:Ignore
// XTResizeRect.h: interface for the CXTResizeRect class.
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
// Based on the resizable classes created by Torben B. Haagh. Used by permission.
// http://www.codeguru.com/dialog/torbenResizeDialog.shtml
//--------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
//:End Ignore

#if !defined(__XTRESIZERECT_H__)
#define __XTRESIZERECT_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// Float data type used by resizing windows.
//
typedef float XT_RESIZE;

//////////////////////////////////////////////////////////////////////
// XT_RESIZERECT structure is a stand alone structure class.  It defines
// the coordinates of the upper-left and lower-right corners of a rectangle.
//
struct XT_RESIZERECT
{
	// Specifies the x-coordinate of the upper-left corner of a rectangle.
	XT_RESIZE left;
	// Specifies the y-coordinate of the upper-left corner of a rectangle.
	XT_RESIZE top;
	// Specifies the x-coordinate of the lower-right corner of a rectangle.
	XT_RESIZE right;
	// Specifies the y-coordinate of the lower-right corner of a rectangle.
	XT_RESIZE bottom;
};

//////////////////////////////////////////////////////////////////////
// CXTResizeRect is an XT_RESIZERECT structure derived class.  The 
// CXTResizeRect class is similar to an XT_RESIZERECT structure.  CXTResizeRect
// also includes member functions to manipulate CXTResizeRect objects and
// XT_RESIZERECT structures.
//
class _XT_EXT_CLASS CXTResizeRect: public XT_RESIZERECT
{
public:

    // Constructs a CXTResizeRect object.
    //
	CXTResizeRect();

    // Constructs a CXTResizeRect object.
    //
	CXTResizeRect(
		// Refers to the RECT structure with the coordinates for CXTResizeRect.
		const RECT& rc);

    // Constructs a CXTResizeRect object.
    //
	CXTResizeRect(
		// Refers to the XT_RESIZERECT structure with the coordinates for 
		// CXTResizeRect.
		const XT_RESIZERECT& rrc);

    // Constructs a CXTResizeRect object.
    //
	CXTResizeRect(
		// Specifies the left position of CXTResizeRect.
		XT_RESIZE l,
		// Specifies the top of CXTResizeRect.
		XT_RESIZE t,
		// Specifies the right position of CXTResizeRect.
		XT_RESIZE r,
		// Specifies the bottom of CXTResizeRect.
		XT_RESIZE b);

	// This operator copies the dimensions of a rectangle to CXTResizeRect.
	// Returns a reference to a CXTResizeRect object.
	//
	CXTResizeRect& operator = (
		// Refers to a source rectangle. It can be a RECT or CRect.
		const RECT& rc);

	// This operator copies the dimensions of a rectangle to CXTResizeRect.
	// Returns a reference to a CXTResizeRect object.
	//
	CXTResizeRect& operator = (
		// Refers to a source rectangle. It can be a XT_RESIZERECT or CXTResizeRect.
		const XT_RESIZERECT& rrc);

	// This operator adds the specified offsets to CXTResizeRect or inflates
	// CXTResizeRect. Returns a reference to a CXTResizeRect object.
	//
	CXTResizeRect& operator += (
		// Points to an XT_RESIZERECT structure or a CXTResizeRect object that
		// contains the number of units to inflate each side of CXTResizeRect.
		const XT_RESIZERECT& rrc);

	// This operator adds the given offsets to CRect or inflates CRect. Returns
	// the resulting CXTResizeRect object.
	//
	CXTResizeRect operator + (
		// Points to an XT_RESIZERECT structure or a CXTResizeRect object that
		// contains the number of units to inflate each side of the return value.
		const XT_RESIZERECT& rrc);

	// This operator creates the intersection of CXTResizeRect and a rectangle,
	// and returns the resulting CXTResizeRect.  Returns a CXTResizeRect that
	// is the intersection of CXTResizeRect and 'rrc'.  The intersection is
	// the largest rectangle that is contained in both rectangles.
	//
	CXTResizeRect operator & (
		// Contains an XT_RESIZERECT or a CXTResizeRect.
		const XT_RESIZERECT& rrc);

	// This operator determines whether CXTResizeRect is equal to a rectangle.
	// Returns 'true' if equal, otherwise returns 'false'.
	//
	bool operator == (
		// Refers to a source rectangle. It can be an XT_RESIZERECT or a CXTResizeRect.
		const XT_RESIZERECT& rrc);

	// This operator determines whether CXTResizeRect is not equal to a rectangle.
	// Returns 'false' if equal, otherwise returns 'true'.
	//
	bool operator != (
		// Refers to a source rectangle. It can be an XT_RESIZERECT or a CXTResizeRect.
		const XT_RESIZERECT& rrc);

	// This operator converts a CXTResizeRect to a CRect.  When you use this
	// function, you do not need the address-of (&) operator.  This operator
	// will be automatically used when you pass a CXTResizeRect object to
	// a function that expects a CRect.
	//
	operator CRect();

	// This member function determines if CXTResizeRect is normalized.  Returns
	// 'true' if normalized, otherwise returns 'false'.
	//
	bool IsNormalized();

	// This member function calculates the width of a CXTResizeRect by subtracting
	// the left value from the right value.  The resulting value can be negative.
	// Returns the width of a CXTResizeRect.
	//
	XT_RESIZE Width();

	// This member function calculates the height of a CXTResizeRect by subtracting
	// the top value from the bottom value.  The resulting value can be negative.
	// Returns the height of a CXTResizeRect.
	//
	XT_RESIZE Height();
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTResizeRect::CXTResizeRect(const RECT& rc) {
	(operator =)(rc);
}
AFX_INLINE CXTResizeRect::CXTResizeRect(const XT_RESIZERECT& rrc) {
	(operator =)(rrc);
}
AFX_INLINE CXTResizeRect CXTResizeRect::operator + (const XT_RESIZERECT& rrc) { 
	return CXTResizeRect(left + rrc.left, top + rrc.top, right + rrc.right, bottom += rrc.bottom); 
}
AFX_INLINE bool CXTResizeRect::IsNormalized() {
	return ((left <= right) && (top <= bottom));
}
AFX_INLINE bool CXTResizeRect::operator == (const XT_RESIZERECT& rrc) { 
	return left==rrc.left && top==rrc.top && right==rrc.right && bottom==rrc.bottom; 
}
AFX_INLINE bool CXTResizeRect::operator != (const XT_RESIZERECT& rrc) { 
	return !operator==(rrc); 
}
AFX_INLINE CXTResizeRect::operator CRect() { 
	return CRect((int) left, (int) top, (int) right, (int) bottom); 
}
AFX_INLINE XT_RESIZE CXTResizeRect::Width() { 
	return right-left; 
}
AFX_INLINE XT_RESIZE CXTResizeRect::Height() { 
	return bottom-top; 
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//:Associate with "CXTResizeRect"
//////////////////////////////////////////////////////////////////////
// Resize.
#define SZ_RESIZE(x)	CXTResizeRect(0,0,x,x) 
// Reposition.
#define SZ_REPOS(x)		CXTResizeRect(x,x,x,x) 
// Horizontal resize.
#define SZ_HORRESIZE(x)	CXTResizeRect(0,0,x,0) 
// Horizontal reposition.
#define SZ_HORREPOS(x)	CXTResizeRect(x,0,x,0) 
// Vertical resize.
#define SZ_VERRESIZE(x)	CXTResizeRect(0,0,0,x) 
// Vertical reposition.
#define SZ_VERREPOS(x)	CXTResizeRect(0,x,0,x) 

//////////////////////////////////////////////////////////////////////

#endif // !defined(__XTRESIZERECT_H__)

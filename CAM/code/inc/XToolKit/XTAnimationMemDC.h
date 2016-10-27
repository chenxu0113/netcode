//:Ignore
// XTAnimationMemDC.h : header file
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

#if !defined(___XTANIMATIONMEMDC_H__)
#define ___XTANIMATIONMEMDC_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

// Enumerated type used to determine animation effect.
//
enum ANIMATIONTYPE
{
	// As defined in the "Display" settings.
	animateWindowsDefault,
	// Any of the first three in random selection.
	animateRandom,
	// Unfold top to bottom.
	animateUnfold,
	// Slide in from left.
	animateSlide,
	// Fade-in.
	animateFade,
    // No animation.
	animateNone
};
	
typedef void (*xtAnimationProc)(CRect rc, CDC* pDestDC, CDC* pSrcDC, int nType, int nSteps, int nAnimationTime);

//////////////////////////////////////////////////////////////////////
// CXTAnimationMemDC is a CXTMemDC derived class.  This class is a replacement
// for the commonly used XTMemDC.  By calling Animate(), the contained
// bitmap is drawn on the screen with a given effect. This effect is equal
// to the Menu effect in Windows&reg and Office&trade;.
//
class _XT_EXT_CLASS CXTAnimationMemDC : public CXTMemDC
{
public: 
	// Constructs a CXTAnimationMemDC object.
    //
	CXTAnimationMemDC(
		// A pointer to a CDC object.
        CDC* pDC,
		// An address of a CRect object.
        const CRect& rect,
		// An RGB value that represents the current system face color of 
		// three-dimensional display elements.
        COLORREF clrColor=GetSysColor(COLOR_3DFACE));

	// Destroys a CXTAnimationMemDC object, handles cleanup and de-allocation.
    //
	virtual ~CXTAnimationMemDC();

	// This member function performs the animation.
	//
	void Animate(
		// Type of animation to perform.
		int nType = animateWindowsDefault,
		// Number of steps to take during animation.
		int nSteps = 10,
		// Amount of time to rest, in milliseconds, between each step.
		int nAnimationTime = 1000);

	// This member implement default animation effects - Fade, Slide and Unfold. You can add new animation effect
	// to call SetCustomAnimation member.
	//
	static void DefaultAnimation(
		// Bounding rectangle
		CRect rc,
		// Pointer to device context you must draw to.
		CDC* pDestDC,
		// Device context that contain bitmap you must to take.
		CDC* pSrcDC,
		// Type of animation to perform. For custom animation you must use numbers greater than 6.
		int nType,
		// Number of steps to take during animation.
		int nSteps,
		// Amount of time to rest, in milliseconds, between each step.
		int nAnimationTime);

	// Call this member function to setup new Animation effects. 
	// You must call DefaultAnimation in your function.
	// for example:
	//[pre]
	//int CMainFrame::OnCreate()
	//{
	//	CXTCoolMenu::m_nAnimationType = 10; // our animation type
	//	CXTAnimationMemDC::SetCustomAnimation(CustomAnimation); // custom animation procedure.
	//	...
	//}
	//...
	//void CMainFrame::CustomAnimation(CRect rc, CDC* pDestDC, CDC* pSrcDC, int nType, int nSteps, int nAnimationTime)
	//{
	//	if (nType == 10)
	//	{
	//		// do custom animation
	//	} 
	//	else
	//	{
	//		CXTAnimationMemDC::DefaultAnimation(rc, pDestDC, pSrcDC, nType, nSteps, nAnimationTime);
	//	}	
	//}
	//[/pre]
	//
	static void SetCustomAnimation(
		// pointer to custom animation function 
		xtAnimationProc pCustom);

private:
	static xtAnimationProc m_pCustomAnimation;
};

#endif // !defined(___XTANIMATIONMEMDC_H__)

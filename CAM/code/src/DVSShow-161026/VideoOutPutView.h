#if !defined(AFX_VIDEOOUTPUTVIEW_H__B821FAEF_A7D1_49D2_A189_1EE9BED98BB5__INCLUDED_)
#define AFX_VIDEOOUTPUTVIEW_H__B821FAEF_A7D1_49D2_A189_1EE9BED98BB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VideoOutPutView.h : header file
//

#define MAX_VIDEOOUTPUT		512
#define IDC_STATIC_VIDEOOUT	65000
// 参数设置
#define ERROR_OUTNUMBER		100	// 超过最大视频数
#define ERROR_CREATESTATIC	101 // 控件创建错误



/////////////////////////////////////////////////////////////////////////////
// CVideoOutPutView view


class CVideoOutPutView : public CView
{
public:
	CVideoOutPutView();           // protected constructor used by dynamic creation
	virtual ~CVideoOutPutView();
	DECLARE_DYNCREATE(CVideoOutPutView)

// Attributes
public:
	CStatic m_wndStatic[MAX_VIDEOOUTPUT];

// Operations
public:
	UINT DVR_StartVideo(VIDEOPARAM* pVideoParam, long& lPlayHandle, int nWndIndex);
	UINT DVR_StopVideo(VIDEOPARAM* pVideoParam, long lPlayHandle);
	BOOL DVR_Init();

	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoOutPutView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CVideoOutPutView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOOUTPUTVIEW_H__B821FAEF_A7D1_49D2_A189_1EE9BED98BB5__INCLUDED_)

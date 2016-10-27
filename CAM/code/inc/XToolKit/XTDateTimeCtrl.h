//:Ignore
// XTDateTimeCtrl.h interface for the CXTDateTimeCtrl class.
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

#if !defined(__XTDATETIMECTRL_H__)
#define __XTDATETIMECTRL_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

// class forwards
class CXTMonthCalCtrl;

//////////////////////////////////////////////////////////////////////
// CXTDateTimeCtrl is a CWnd derived class.  A CXTDateTimeCtrl object encapsulates
// the functionality of a date and time picker control.  The date and time
// picker control (DTP control) provides a simple interface to exchange
// date and time information with a user. This interface contains fields,
// each of which displays a part of the date and time information stored
// in the control.  The user can change the information stored in the control
// by changing the content of the string in a given field.  The user can
// move from field to field using the mouse or the keyboard.
//
// You can customize the date and time picker control by applying a variety
// of styles to the object when you create it.  See Date and Time Picker
// Control Styles in the Platform SDK for more information about styles
// specific to the date and time picker control.  You can set the display
// format of the DTP control using format styles.  These format styles
// are described in Preset DTP Display Formats in the Platform SDK.
//
// The date and time picker control also uses notifications and callbacks,
// which are described in Using CXTDateTimeCtrl in the Xtreme Toolkit online
// help.
//
class _XT_EXT_CLASS CXTDateTimeCtrl : public CWnd
{
    DECLARE_DYNAMIC(CXTDateTimeCtrl)

public:

    // Constructs a CXTDateTimeCtrl object.
    //
    CXTDateTimeCtrl();

    // Destroys a CXTDateTimeCtrl object, handles cleanup and de-allocation.
    //
    virtual ~CXTDateTimeCtrl();

    // This member function creates the date and time picker control and
	// attaches it to the CXTDateTimeCtrl object. Returns nonzero if creation
	// was successful, otherwise returns 0.
    //
    BOOL Create(
        // Specifies the combination of date time control 
        // styles. See Date and Time Picker Control Styles 
        // in the Platform SDK for more information about 
        // date and time picker styles.
        DWORD dwStyle,
        // A reference to a RECT structure, which is the 
        // position and size of the date and time picker 
        // control.
        const RECT& rect,
        // A pointer to a CWnd object that is the parent 
        // window of the date and time picker control. It 
        // must not be NULL.
        CWnd* pParentWnd,
        // Specifies the date and time picker control's 
        // control ID.
        UINT nID);

    // This member function retrieves the color for a given portion of
	// the month calendar within the date and time picker control.  Returns
	// a COLORREF value that represents the color setting for the specified
	// portion of the month calendar control if successful.  The function
	// returns -1 if unsuccessful. 
    //
    COLORREF GetMonthCalColor(
        // An int value specifying which color area of the month calendar
		// to retrieve. For a list of values, see the 'iColor' parameter for 
		// SetMonthCalColor.
        int iColor) const;

    // This member function sets the color for a given portion of the month
	// calendar within a date and time picker control.  Returns a COLORREF
	// value that represents the previous color setting for the specified
	// portion of the month calendar control if successful.  Otherwise, the
	// message returns -1.
	//
    COLORREF SetMonthCalColor(
        // An int value specifying which area of the month calendar 
        // control to set. This value can be one of the following:
        //[ul]
        //[li][b]MCSC_BACKGROUND[/b] - Set the background color displayed
		//       between months.[/li]
        //[li][b]MCSC_MONTHBK[/b] - Set the background color displayed
		//       within a month.[/li]
        //[li][b]MCSC_TEXT[/b] - Set the color used to display text within
		//       a month.[/li]
        //[li][b]MCSC_TITLEBK[/b] - Set the background color displayed
		//       in the calendar's title.[/li]
        //[li][b]MCSC_TITLETEXT[/b] - Set the color used to display text
		//       within the calendar's title.[/li]
        //[li][b]MCSC_TRAILINGTEXT[/b] - Set the color used to display
		//       header and trailing-day text. Header and trailing days are the
		//       days from the previous and following months that appear on the
		//       current calendar.[/li]
        //[/ul]
        int iColor,
        // A RGB value representing the color that will be set 
        // for the specified area of the month calendar.
        COLORREF ref);

    // This member function sets the display of a date and time picker
	// control based on a given format string. Returns nonzero if successful,
	// otherwise returns zero.
    //
    BOOL SetFormat(
        // A pointer to a zero-terminated format string that 
        // defines the desired display. Setting this parameter to 
        // NULL will reset the control to the default format 
        // string for the current style.
        LPCTSTR pstrFormat);

    // This member function retrieves the date and time picker's child
	// month calendar control. Returns a pointer to a DDX_XTMonthCalCtrl object,
	// or NULL if unsuccessful or the window is not visible.
    //
    CXTMonthCalCtrl* GetMonthCalCtrl() const;

    // This member function retrieves the font currently used by the date
	// and time picker control's child month calendar control. Returns a pointer
	// to a CFont object, or returns NULL if unsuccessful.      
    //
    CFont* GetMonthCalFont() const;

    // This member function sets the font to be used by the date and time
	// picker control's child month calendar control.
    //
    void SetMonthCalFont(
        // Handle to the font that will be set. 
        HFONT hFont,
        // Specifies whether the control should be redrawn 
        // immediately upon setting the font. Setting this 
        // parameter to TRUE causes the control to redraw itself. 
        BOOL bRedraw = TRUE);

    // This member function sets the minimum and maximum allowable system
	// times for a date and time picker control.  Returns nonzero if successful,
	// otherwise returns zero. 
    //
    BOOL SetRange(
        // A pointer to a COleDateTime object containing the 
        // earliest time allowed in the CXTDateTimeCtrl object.
        const COleDateTime* pMinRange,
        // A pointer to a COleDateTime object containing the 
        // latest time allowed in the CXTDateTimeCtrl object. 
        const COleDateTime* pMaxRange);

    // This member function sets the minimum and maximum allowable system
	// times for a date and time picker control.  Returns nonzero if successful,
	// otherwise returns zero. 
    //
    BOOL SetRange(
        // A pointer to a CTime object containing the earliest 
        // time allowed in the CXTDateTimeCtrl object.
        const CTime* pMinRange,
        // A pointer to a CTime object containing the latest 
        // time allowed in the CXTDateTimeCtrl object. 
        const CTime* pMaxRange);

    // This member function retrieves the current minimum and maximum allowable
	// system times for a date and time picker control. Returns a DWORD value
	// containing flags that indicate which ranges are set.
    //
    DWORD GetRange(
        // A pointer to a COleDateTime object containing the 
        // earliest time allowed in the CXTDateTimeCtrl object.
        COleDateTime* pMinRange,
        // A pointer to a COleDateTime object containing the 
        // latest time allowed in the CXTDateTimeCtrl object. 
        COleDateTime* pMaxRange) const;

    // This member function retrieves the current minimum and maximum allowable
	// system times for a date and time picker control. Returns a DWORD value
	// containing flags that indicate which ranges are set.
    //
    DWORD GetRange(
        // A pointer to a CTime object containing the earliest 
        // time allowed in the CXTDateTimeCtrl object.
        CTime* pMinRange,
        // A pointer to a CTime object containing the latest 
        // time allowed in the CXTDateTimeCtrl object.
        CTime* pMaxRange) const;
    
    // This member function sets the time in a date and time picker control.
	// Returns nonzero if successful, otherwise returns zero. 
    //
    BOOL SetTime(
        // A pointer to a CTime object containing the time to 
        // which the control will be set.
        const CTime* pTimeNew);

    // This member function sets the time in a date and time picker control.
	// Returns nonzero if successful, otherwise returns zero.
    //
    BOOL SetTime(
        // A reference to a COleDateTime object containing the time to 
        // which the control will be set.
        const COleDateTime& timeNew);

    // This member function sets the time in a date and time picker control.
	// Returns nonzero if successful, otherwise returns zero. 
    //
    BOOL SetTime(
        // A pointer to a SYSTEMTIME structure containing 
        // the time to which the control will be set.
        LPSYSTEMTIME pTimeNew = NULL);

    // This member function retrieves the currently selected time from
	// a date and time picker control and places it in a specified SYSTEMTIME
	// structure. Returns a DWORD value equal to the dwFlag member set in
	// the NMDATETIMECHANGE structure.
    //
    DWORD GetTime(
        // A reference to a CTime object that will receive 
        // the system time information.
        CTime& timeDest) const;

    // This member function retrieves the currently selected time from
	// a date and time picker control and places it in a specified SYSTEMTIME
	// structure. Returns nonzero if the time is successfully written to the
	// COleDateTime object, otherwise returns zero.
    //
    BOOL GetTime(
        // A reference to a COleDateTime object that will 
        // receive the system time information.
        COleDateTime& timeDest) const;

    // This member function retrieves the currently selected time from
	// a date and time picker control and places it in a specified SYSTEMTIME
	// structure. Returns a DWORD value equal to the dwFlag member set in
	// the NMDATETIMECHANGE structure.
    //
    DWORD GetTime(
        // A pointer to the SYSTEMTIME structure to receive 
        // the system time information. Must not be NULL.
        LPSYSTEMTIME pTimeDest) const;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTDateTimeCtrl::CXTDateTimeCtrl() {

}
AFX_INLINE CFont* CXTDateTimeCtrl::GetMonthCalFont() const {
    ASSERT(::IsWindow(m_hWnd)); return CFont::FromHandle((HFONT) ::SendMessage(m_hWnd, DTM_GETMCFONT, 0, 0));
}
AFX_INLINE CXTMonthCalCtrl* CXTDateTimeCtrl::GetMonthCalCtrl() const {
    ASSERT(::IsWindow(m_hWnd)); return (CXTMonthCalCtrl*) CWnd::FromHandle((HWND) ::SendMessage(m_hWnd, DTM_GETMONTHCAL, 0, 0));
}
AFX_INLINE void CXTDateTimeCtrl::SetMonthCalFont(HFONT hFont, BOOL bRedraw /* = TRUE */) {
    ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, DTM_SETMCFONT, (WPARAM) hFont, MAKELONG(bRedraw, 0));
}
AFX_INLINE COLORREF CXTDateTimeCtrl::SetMonthCalColor(int iColor, COLORREF ref) {
    ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, DTM_SETMCCOLOR, (WPARAM) iColor, (LPARAM) ref);
}
AFX_INLINE DWORD CXTDateTimeCtrl::GetTime(LPSYSTEMTIME pTimeDest) const {
    ASSERT(::IsWindow(m_hWnd)); ASSERT(pTimeDest != NULL); return (DWORD) ::SendMessage(m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM) pTimeDest);
}
AFX_INLINE COLORREF CXTDateTimeCtrl::GetMonthCalColor(int iColor) const {
    ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, DTM_GETMCCOLOR, (WPARAM) iColor, 0);
}
AFX_INLINE BOOL CXTDateTimeCtrl::SetFormat(LPCTSTR pstrFormat) {
    ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, DTM_SETFORMAT, 0, (LPARAM) pstrFormat);
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTMonthCalCtrl is a CWnd derived class.  A CXTMonthCalCtrl object encapsulates
// the functionality of a month calendar control.  The month calendar control
// provides the user with a simple calendar interface, from which the user
// can select a date.  The user can change the display by: 
// [ul]
// [li]Scrolling backward and forward, from month to month.[/li]
// [li]Clicking the Today text to display the current day (if the MCS_NOTODAY
// style is not used).[/li]
// [li]Picking a month or a year from a popup menu.[/li]
// [/ul]
// You can customize the month calendar control by applying a variety of
// styles to the object when you create it.  These styles are described
// in Month Calendar Control Styles in the Platform SDK.
//
// The month calendar control can display more than one month, and it can
// indicate special days, such as holidays, by bolding the date.
//
// For more information on using the month calendar control, see Using
// CXTMonthCalCtrl in the Xtreme Toolkit online help.
//
class _XT_EXT_CLASS CXTMonthCalCtrl : public CWnd
{
  DECLARE_DYNAMIC(CXTMonthCalCtrl)
      
public:
    
    // Constructs a CXTMonthCalCtrl object.
    //
    CXTMonthCalCtrl();

    // Destroys a CXTMonthCalCtrl object, handles cleanup and de-allocation.
    //
    virtual ~CXTMonthCalCtrl();

    // This member function creates a month calendar control and attaches
	// it to the DDX_XTMonthCalCtrl object. Returns nonzero if initialization
	// was successful, otherwise returns zero.
    //
    BOOL Create(
        // Specifies the combination of Windows styles applied to the month
		// calendar control. See Month Calendar Control Styles in the Platform
		// SDK for more information about the styles.
        DWORD dwStyle,
        // A reference to a RECT structure. Contains the position
        // and size of the month calendar control
        const RECT& rect,
        // A pointer to a CWnd object that is the parent window
        // of the month calendar control. It must not be NULL.
        CWnd* pParentWnd,
        // Specifies the month calendar control’s control ID.
        UINT nID);

    // This member function creates a month calendar control and attaches
	// it to the DDX_XTMonthCalCtrl object. Returns nonzero if initialization
	// was successful, otherwise returns zero.
    //
    BOOL Create(
        // Specifies the combination of Windows styles applied to the month
		// calendar control. See Month Calendar Control Styles in the Platform
		// SDK for more information about the styles.
        DWORD dwStyle,
        // A reference to a POINT structure that identifies the location
        // of the month calendar control.
        const POINT& pt,
        // A pointer to a CWnd object that is the parent window
        // of the month calendar control. It must not be NULL.
        CWnd* pParentWnd,
        // Specifies the month calendar control’s control ID.
        UINT nID);
    
    // This member function retrieves the minimum size required to show
	// a full month in a month calendar control. Returns nonzero and 'lpRect'
	// receives the applicable bounding information.  If unsuccessful, this
	// member function returns zero.
    //
    BOOL GetMinReqRect(
        // A pointer to a RECT structure that will receive
        // bounding rectangle information. This parameter must be a valid
        // address and cannot be NULL. 
        RECT* pRect) const;

    // This member function sets the scroll rate for a month calendar control.
	// Returns the previous scroll rate. If the scroll rate has not been previously
    // set, the return value is zero. 
    //
    int SetMonthDelta(
        // The number of months to be set as the control's scroll rate.
		// If this value is zero, the month delta is reset to the default, which
		// is the number of months displayed in the control. 
        int iDelta);

    // This member function retrieves the scroll rate for a month calendar
	// control.  The scroll rate is the number of months that the control
	// moves its display when the user clicks a scroll button once. Returns
	// an integer value that represents the scroll rate for the month calendar
	// control.
    //
    int GetMonthDelta() const;

    // This member function sets the day of the week to be displayed in
	// the leftmost column of the calendar. Returns nonzero if the previous
	// first day of the week is set to a value other than that of 
	// LOCALE_IFIRSTDAYOFWEEK, which is the day indicated in the control panel
	// setting. Otherwise, this function returns zero.
    //
    BOOL SetFirstDayOfWeek(
        // An integer value representing which day is to be set as the
		// first day of the week. This value must be one of the day numbers.
		// See GetFirstDayOfWeek for a description of the day numbers.
        int iDay,
        // A pointer to an integer indicating the first day of
        // the week previously set.
        int* lpnOld = NULL);

    // This member function gets the first day of the week to be displayed
	// in the left most column of the calendar.  Returns an integer value
	// that represents the first day of the week. The days of the week are
	// represented by integers, as follows:
    //
    //[pre][b]Value[/b]   [b]Day of the Week[/b]
    //[b]0[/b]       [b]Monday[/b]
    //[b]1[/b]       [b]Tuesday[/b]
    //[b]2[/b]       [b]Wednesday[/b]
    //[b]3[/b]       [b]Thursday[/b]
    //[b]4[/b]       [b]Friday[/b]
    //[b]5[/b]       [b]Saturday[/b]
    //[b]6[/b]       [b]Sunday[/b][/pre]  
    //
    int GetFirstDayOfWeek(
        // A pointer to a BOOL value. If the value is nonzero, the control's
		// setting does not match the setting in the control panel. 
        BOOL* pbLocal = NULL) const;

    // This member function gets the color of a specified area of a month
	// calendar control.  Returns a COLORREF value specifying the color associated
    // with the portion of the month calendar control, if successful.
    // Otherwise, this member function returns -1.
    //
    COLORREF GetColor(
        // The region of the month calendar control from which the color
		// is retrieved. For a list of values, see the 'nRegion' parameter of SetColor.
        int nRegion) const;

    // This member function sets the color of a specified area of a month
	// calendar control. Returns a COLORREF value that represents the previous
    // color setting for the specified portion of the month calendar
    // control, if successful. Otherwise this message returns -1. 
    //
    COLORREF SetColor(
        // An integer value specifying which month calendar color 
        // to set. This value can be one of the following:
        //[ul]
        //[li][b]MCSC_BACKGROUND[/b] - The background color displayed between
		//       months.[/li]  
        //[li][b]MCSC_MONTHBK[/b] - The background color displayed within
		//       the month.[/li]
        //[li][b]MCSC_TEXT[/b] - The color used to display text within a month.[/li]
        //[li][b]MCSC_TITLEBK[/b] - The background color displayed in the
		//       calendar's title.[/li]
        //[li][b]MCSC_TITLETEXT[/b] - The color used to display text within
		//       the calendar's title.[/li]
        //[li][b]MCSC_TRAILINGTEXT[/b] - The color used to display header
		//       and trailing-day text. Header and trailing days are the days
		//       from the previous and following months that appear on the current
		//       calendar.[/li]
        //[/ul]
        int nRegion,
        // A COLORREF value for the new color setting for the specified
        // portion of the month calendar control.
        COLORREF ref);

    // This member function repaints the month calendar control to its
	// minimum, one-month size.  Returns nonzero if the month calendar control
    // is sized to its minimum, otherwise returns zero. 
    //
    BOOL SizeMinReq(
        // Specifies whether the control is to be repainted. By default,
		// TRUE. If FALSE, no repainting occurs.
        BOOL bRepaint = TRUE);

    // This member function sets the calendar control to the current day.
    //
    void SetToday(
        // A reference to a COleDateTime object that contains the current
		// date.
        const COleDateTime& refDateTime);

    // This member function sets the calendar control to the current day.
    //
    void SetToday(
        // A pointer to a CTime object containing the current date information.
        const CTime* pDateTime);

    // This member function sets the calendar control to the current day.
    //
    void SetToday(
        // A pointer to a SYSTEMTIME structure that
        // contains the current date information. 
        const LPSYSTEMTIME pDateTime);

    // This member function retrieves the date information for the date specified
    // as "today" for a month calendar control. Returns nonzero if successful,
	// otherwise returns zero.
    //
    BOOL GetToday(
        // A reference to a CTime object indicating the current day.
        CTime& refTime) const;

    // This member function retrieves the date information for the date specified
    // as "today" for a month calendar control. Returns nonzero if successful,
	// otherwise returns zero.
    //
    BOOL GetToday(
        // A reference to a COleDateTime object indicating the current day.
        COleDateTime& refDateTime) const;

    // This member function retrieves the date information for the date specified
    // as "today" for a month calendar control.  Returns nonzero if successful,
	// otherwise returns zero.
    //
    BOOL GetToday(
        // A pointer to a SYSTEMTIME structure that
        // will receive the date information.  This parameter must be
        // a valid address and cannot be NULL.
        LPSYSTEMTIME pDateTime) const;

    // This member function retrieves the system time as indicated by the
	// currently selected date. Returns nonzero if successful, otherwise returns zero.
    //
    BOOL GetCurSel(
        // A pointer to a SYSTEMTIME structure that will receive the currently selected
		// date information. This parameter must be a valid address and cannot
		// be NULL.
        LPSYSTEMTIME pDateTime) const;

    // This member function retrieves the system time as indicated by the
	// currently selected date.  Returns nonzero if successful, otherwise
	// returns zero.
    //
    BOOL GetCurSel(
        // A reference to a CTime object. It receives the current time.
        CTime& refDateTime) const;

    // This member function retrieves the system time as indicated by the
	// currently selected date. Returns nonzero if successful, otherwise returns zero.
    //
    BOOL GetCurSel(
        // A reference to a COleDateTime object. It receives the current time.
        COleDateTime& refDateTime) const;

    // This member function sets the currently selected date for a month calendar
    // control. Returns nonzero if successful, otherwise returns zero.
    //
    BOOL SetCurSel(
        // Pointer to a SYSTEMTIME structure that contains the date to
		// be set as the current selection.
        const LPSYSTEMTIME pDateTime);

    // This member function sets the currently selected date for a month calendar
    // control.  Returns nonzero if successful, otherwise returns zero.
    //
    BOOL SetCurSel(
        // A reference to a CTime object indicating the currently selected
		// month calendar control. 
        const CTime& refDateTime);

    // This member function sets the currently selected date for a month calendar
    // control. Returns nonzero if successful, otherwise returns zero.
    //
    BOOL SetCurSel(
        // A reference to a COleDateTime indicating the currently selected
		// month calendar control. 
        const COleDateTime& refDateTime);

    // This member function sets the display for days in a month calendar control.
    // Returns nonzero if successful, otherwise returns zero.
    //
    BOOL SetDayState(
        // Value indicating how many elements are in the array that 'pStates'
		// points to.
        int nMonths,
        // A pointer to a MONTHDAYSTATE array of values that define
        // how the month calendar control will draw each day in its 
        // display. The MONTHDAYSTATE data type is a bit field, where 
        // each bit (1 through 31) represents the state of a day in a 
        // month. If a bit is on, the corresponding day will be displayed
        // in bold; otherwise it will be displayed with no emphasis. 
        LPMONTHDAYSTATE pStates);

    // This member function sets the maximum number of days that can be selected
    // in a month calendar control. Returns nonzero if successful, otherwise
	// returns zero.
    //
    BOOL SetMaxSelCount(
        // Specifies the value that will be set to represent the maximum 
        // number of selectable days.
        int nMax);

    // This member function retrieves the current maximum number of days that can
    // be selected in a month calendar control. Returns an integer value
    // that represents the total number of days that can be selected
    // for the control.
    //
    int GetMaxSelCount() const;

    // This member function sets the minimum and maximum allowable dates for a month
    // calendar control. Returns nonzero if successful, otherwise returns zero.
    //
    BOOL SetRange(
        // A pointer to a COleDateTime object containing
        // the date at the lowest end of the range. 
        const COleDateTime* pMinRange,
        // A pointer to a COleDateTime object containing the
        // date at the highest end of the range.
        const COleDateTime* pMaxRange);

    // This member function sets the minimum and maximum allowable dates for a month
    // calendar control. Returns nonzero if successful, otherwise returns zero.
    //
    BOOL SetRange(
        // A pointer to a CTime object containing the date at the lowest
		// end of the range. 
        const CTime* pMinRange,
        // A pointer to a CTime object containing the date at the highest
		// end of the range.
        const CTime* pMaxRange);

    // This member function sets the minimum and maximum allowable dates for a month
    // calendar control. Returns nonzero if successful, otherwise returns zero.
    //
    BOOL SetRange(
        // A pointer to a SYSTEMTIME structure containing
        // the date at the lowest end of the range. 
        const LPSYSTEMTIME pMinRange,
        // A pointer to a SYSTEMTIME structure containing the
        // date at the highest end of the range.
        const LPSYSTEMTIME pMaxRange);

    // This member function retrieves the current minimum and maximum dates set
    // in a month calendar control.
    //
    // Returns a DWORD that can be zero (no limits are set) or a combination
	// of the following values that specify limit information:
    //[ul]
    //[li][b]GDTR_MAX[/b] - A maximum limit is set for the control; pMaxRange
    //          is valid and contains the applicable date information.[/li]
    //[li][b]GDTR_MIN[/b] - A minimum limit is set for the control; pMinRange
    //          is valid and contains the applicable date information.[/li]
    //[/ul]
    DWORD GetRange(
        // A pointer to a COleDateTime object containing
        // the date at the lowest end of the range. 
        COleDateTime* pMinRange,
        // A pointer to a COleDateTime object containing the
        // date at the highest end of the range.
        COleDateTime* pMaxRange) const;

    // This member function retrieves the current minimum and maximum dates set
    // in a month calendar control.
    //
    // Returns a DWORD that can be zero (no limits are set) or a combination
	// of the following values that specify limit information:             
    //[ul]
    //[li][b]GDTR_MAX[/b] - A maximum limit is set for the control; pMaxRange
    //          is valid and contains the applicable date information.[/li]
    //[li][b]GDTR_MIN[/b] - A minimum limit is set for the control; pMinRange
    //          is valid and contains the applicable date information.[/li]
    //[/ul]
    DWORD GetRange(
        // A pointer to a CTime object containing the date at the lowest
		// end of the range. 
        CTime* pMinRange,
        // A pointer to a CTime object containing the date at the highest
		// end of the range.
        CTime* pMaxRange) const;

    // This member function retrieves the current minimum and maximum dates set
    // in a month calendar control.
    //
    // Returns a DWORD that can be zero (no limits are set) or a combination
	// of the following values that specify limit information:             
    //[ul]
    //[li][b]GDTR_MAX[/b] - A maximum limit is set for the control; pMaxRange
    //          is valid and contains the applicable date information.[/li]
    //[li][b]GDTR_MIN[/b] - A minimum limit is set for the control; pMinRange
    //          is valid and contains the applicable date information.[/li]
    //[/ul]
    DWORD GetRange(
        // A pointer to a SYSTEMTIME structure containing
        // the date at the lowest end of the range. 
        LPSYSTEMTIME pMinRange,
        // A pointer to a COleDateTime object, a CTime object,
        // or SYSTEMTIME structure containing the date at the highest
        // end of the range.
        LPSYSTEMTIME pMaxRange) const;

    // This member function retrieves date information representing the
	// high and low limits of a month calendar control's display. Returns
	// an integer that represents the range, in months, spanned by the two
	// limits indicated by 'refMinRange' and 'refMaxRange'.
    //
    int GetMonthRange(
        // A reference to a COleDateTime object
        // containing the minimum date allowed.
        COleDateTime& refMinRange,
        // A reference to a COleDateTime object containing
        // the maximum date allowed.
        COleDateTime& refMaxRange,
        // Value specifying the scope of the range limits to
        // be retrieved. This value must be one of the following:
        //[ul]
        //[li][b]GMR_DAYSTATE[/b] - Include preceding and trailing months
		//       of visible range that are only partially displayed.[/li]
        //[li][b]GMR_VISIBLE[/b] - Include only those months that are entirely
		//       displayed.[/li] 
		//[/ul]
        DWORD dwFlags) const;

    // This member function retrieves date information representing the
	// high and low limits of a month calendar control's display. Returns
	// an integer that represents the range, in months, spanned by the two
	// limits indicated by 'refMinRange' and 'refMaxRange'.
    //
    int GetMonthRange(
        // A reference to a CTime object containing the minimum date allowed.
        CTime& refMinRange,
        // A reference to a CTime object containing the maximum date allowed.
        CTime& refMaxRange,
        // Value specifying the scope of the range limits to
        // be retrieved. This value must be one of the following:
        //[ul]
        //[li][b]GMR_DAYSTATE[/b] - Include preceding and trailing months
		//       of visible range that are only partially displayed.[/li]
        //[li][b]GMR_VISIBLE[/b] - Include only those months that are entirely
		//       displayed.[/li]
        //[/ul]
        DWORD dwFlags) const;

    // This member function retrieves date information representing the
	// high and low limits of a month calendar control's display. Returns
	// an integer that represents the range, in months, spanned by the two
	// limits indicated by 'refMinRange' and 'refMaxRange'.
    //
    int GetMonthRange(
        // A pointer to a SYSTEMTIME structure containing the date at the
		// lowest end of the range.
        LPSYSTEMTIME pMinRange,
        // A pointer to a SYSTEMTIME structure containing the date at the
		// highest end of the range.
        LPSYSTEMTIME pMaxRange,
        // Value specifying the scope of the range limits to
        // be retrieved. This value must be one of the following:
        //[ul]
        //[li][b]GMR_DAYSTATE[/b] - Include preceding and trailing months
		//       of visible range that are only partially displayed.[/li]
        //[li][b]GMR_VISIBLE[/b] - Include only those months that are entirely
		//       displayed.[/li]
        //[/ul]
        DWORD dwFlags) const;

    // This member function sets the selection for a month calendar control
	// to a given date range. Returns nonzero if successful, otherwise returns zero. 
    //
    BOOL SetSelRange(
        // A pointer to a COleDateTime object containing
        // the date at the lowest end of the range. 
        const COleDateTime& pMinRange,
        // A pointer to a COleDateTime object containing the
        // date at the highest end of the range.
        const COleDateTime& pMaxRange);

    // This member function sets the selection for a month calendar control
	// to a given date range. Returns nonzero if successful, otherwise returns zero. 
    //
    BOOL SetSelRange(
        // A pointer to a CTime object containing the date at the lowest
		// end of the range. 
        const CTime& pMinRange,
        // A pointer to a CTime object containing the date at the highest
		// end of the range.
        const CTime& pMaxRange);

    // This member function sets the selection for a month calendar control
	// to a given date range. Returns nonzero if successful, otherwise returns zero.
    //
    BOOL SetSelRange(
        // A pointer to a SYSTEMTIME structure containing the date at the
		// lowest end of the range. 
        const LPSYSTEMTIME pMinRange,
        // A pointer to a SYSTEMTIME structure containing the date at the
		// highest end of the range.
        const LPSYSTEMTIME pMaxRange);

    // This member function retrieves date information representing the
	// upper and lower limits of the date range currently selected by the
	// user. Returns nonzero if successful, otherwise returns zero.
    //
    BOOL GetSelRange(
        // A reference to a COleDateTime object containing the minimum
		// date allowed.
        COleDateTime& refMinRange,
        // A reference to a COleDateTime object containing the maximum
		// date allowed.
        COleDateTime& refMaxRange) const;

    // This member function retrieves date information representing the
	// upper and lower limits of the date range currently selected by the
	// user. Returns nonzero if successful, otherwise returns zero.
    //
    BOOL GetSelRange(
        // A reference to a CTime object containing the minimum date allowed.
        CTime& refMinRange,
        // A reference to a CTime object containing the maximum date allowed.
        CTime& refMaxRange) const;

    // This member function retrieves date information representing the
	// upper and lower limits of the date range currently selected by the
	// user. Returns nonzero if successful, otherwise returns zero.
    //
    BOOL GetSelRange(
        // A pointer to a SYSTEMTIME structure containing
        // the date at the lowest end of the range.
        LPSYSTEMTIME pMinRange,
        // A pointer to a SYSTEMTIME structure containing the
        // date at the highest end of the range.
        LPSYSTEMTIME pMaxRange) const;

    // This member function determines which portion of a month calendar
	// control is at a given point on the screen. Returns a DWORD value equal
    // to the 'uHit' member of the MCHITTESTINFO structure.
    //
    DWORD HitTest(
        // A pointer to a MCHITTESTINFO structure containing hit testing
		// points for the month calendar control.
        PMCHITTESTINFO pMCHitTest);
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTMonthCalCtrl::CXTMonthCalCtrl() {
 
}
AFX_INLINE DWORD CXTMonthCalCtrl::HitTest(PMCHITTESTINFO pMCHitTest) {
    ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, MCM_HITTEST, 0, (LPARAM) pMCHitTest);
}
AFX_INLINE BOOL CXTMonthCalCtrl::GetMinReqRect(RECT* pRect) const {
    ASSERT(m_hWnd != NULL); return (BOOL) ::SendMessage(m_hWnd, MCM_GETMINREQRECT, 0, (LPARAM) pRect);
}
AFX_INLINE int CXTMonthCalCtrl::SetMonthDelta(int iDelta) {
    ASSERT(m_hWnd != NULL); return (int) ::SendMessage(m_hWnd, MCM_SETMONTHDELTA, (WPARAM) iDelta, 0);
}
AFX_INLINE int CXTMonthCalCtrl::GetMonthDelta() const {
    ASSERT(m_hWnd != NULL); return (int) ::SendMessage(m_hWnd, MCM_GETMONTHDELTA, 0, 0);
}
AFX_INLINE COLORREF CXTMonthCalCtrl::GetColor(int nRegion) const {
    ASSERT(m_hWnd != NULL); return (COLORREF) ::SendMessage(m_hWnd, MCM_GETCOLOR, (WPARAM) nRegion, 0);
}
AFX_INLINE COLORREF CXTMonthCalCtrl::SetColor(int nRegion, COLORREF ref) {
    ASSERT(m_hWnd != NULL); return (COLORREF) ::SendMessage(m_hWnd, MCM_SETCOLOR, (WPARAM) nRegion, (LPARAM) ref);
}
AFX_INLINE BOOL CXTMonthCalCtrl::SetMaxSelCount(int nMax) {
    ASSERT(m_hWnd != NULL); return (BOOL) ::SendMessage(m_hWnd, MCM_SETMAXSELCOUNT, nMax, 0);
}
AFX_INLINE int CXTMonthCalCtrl::GetMaxSelCount() const {
    ASSERT(m_hWnd != NULL); return (int) ::SendMessage(m_hWnd, MCM_GETMAXSELCOUNT, 0, 0);
}
AFX_INLINE void CXTMonthCalCtrl::SetToday(const LPSYSTEMTIME pDateTime) {
    ASSERT(m_hWnd != NULL); ::SendMessage(m_hWnd, MCM_SETTODAY, 0, (LPARAM) pDateTime);
}
AFX_INLINE BOOL CXTMonthCalCtrl::GetToday(LPSYSTEMTIME pDateTime) const {
    ASSERT(m_hWnd != NULL); return (BOOL) ::SendMessage(m_hWnd, MCM_GETTODAY, 0, (LPARAM) pDateTime);
}
AFX_INLINE BOOL CXTMonthCalCtrl::SetCurSel(const LPSYSTEMTIME pDateTime) {
    ASSERT(m_hWnd != NULL); return (BOOL) ::SendMessage(m_hWnd, MCM_SETCURSEL, 0, (LPARAM) pDateTime);
}
AFX_INLINE BOOL CXTMonthCalCtrl::GetCurSel(LPSYSTEMTIME pDateTime) const {
    ASSERT(m_hWnd != NULL); return (BOOL) ::SendMessage(m_hWnd, MCM_GETCURSEL, 0, (LPARAM) pDateTime);
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTDATETIMECTRL_H__)

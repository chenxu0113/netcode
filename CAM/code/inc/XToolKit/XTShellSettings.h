//:Ignore
// XTShellSettings.h: interface for the CXTShellSettings class.
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

#if !defined(__XTSHELLSETTINGS_H__)
#define __XTSHELLSETTINGS_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTShellSettings is a SHELLFLAGSTATE struct derived class. CXTShellSettings
// extends the SHELLFLAGSTATE struct for easy access to the shell flag
// state settings.
//
class _XT_EXT_CLASS CXTShellSettings : public SHELLFLAGSTATE
{
public:

   // Constructs a CXTShellSettings object.
   //
   CXTShellSettings();

   // Destroys a CXTShellSettings object, handles cleanup and de-allocation.    
   //
   virtual ~CXTShellSettings();

public:

   // This member function checks to see if the Show All Files option is
   // enabled. Returns 'true' if enabled, otherwise returns 'false'.
   //
   bool ShowAllFiles();

   // This member function checks to see if the Hide File Extensions for
   // Known File Types option is disabled. Returns 'true' if enabled, otherwise
   // returns 'false'.
   //
   bool ShowExtensions();

   // This member function checks to see if the Display Delete Confirmation
   // Dialog box in the Recycle Bin is enabled. Returns 'true' if enabled,
   // otherwise returns 'false'.
   //
   bool NoConfirmRecycle();

   // This member function checks to see if the Do Not Show Hidden Files
   // option is selected. Returns 'true' if enabled, otherwise returns 'false'.
   //
   bool ShowSysFiles();

   // This member function checks to see if the Display Compressed Files
   // and Folders with Alternate Color option is enabled. Returns 'true'
   // if enabled, otherwise returns 'false'.
   //
   bool ShowCompColor();

   // This member function checks to see if the Double-Click to Open an
   // Item option is enabled. Returns 'true' if enabled, otherwise returns
   // 'false'.
   //
   bool DoubleClickOpen();

   // This member function checks to see if the Active Desktop – View as
   // Web Page option is enabled. Returns 'true' if enabled, otherwise
   // returns 'false'.
   //
   bool HTMLDesktop();

   // This member function checks to see if the Classic Style option is
   // enabled. Returns 'true' if enabled, otherwise returns 'false'.
   //
   bool Win95Classic();

   // This member function checks to see if the Allow All Uppercase Names
   // option is enabled. Returns 'true' if enabled, otherwise returns 'false'.
   //
   bool AllowUpperCase();

   // This member function checks to see if the Show File Attributes in
   // Detail View option is enabled. Returns 'true' if enabled, otherwise
   // returns 'false'.
   //
   bool ShowAttributes();

   // This member function checks to see if the Show Map Network Drive
   // Button in Toolbar option is enabled. Returns 'true' if enabled, otherwise
   // returns 'false'.
   //
   bool ShowMapNetworkDrive();

   // This member function checks to see if the Show Info Tips for Items
   // in Folders & Desktop option is enabled. Returns 'true' if enabled,
   // otherwise returns 'false'.
   //
   bool ShowInfoTips();
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE bool CXTShellSettings::ShowAllFiles() {
    return fShowAllObjects != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::ShowExtensions() {
    return fShowExtensions != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::NoConfirmRecycle() {
    return fNoConfirmRecycle != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::ShowSysFiles() {
    return fShowSysFiles != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::ShowCompColor() {
    return fShowCompColor != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::DoubleClickOpen() {
    return fDoubleClickInWebView != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::HTMLDesktop() {
    return fDesktopHTML != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::Win95Classic() {
    return fWin95Classic != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::AllowUpperCase() {
    return fDontPrettyPath != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::ShowAttributes() {
    return fShowAttribCol != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::ShowMapNetworkDrive() {
    return fMapNetDrvBtn != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::ShowInfoTips() {
    return fShowInfoTip != 0 ? true : false;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

#endif // !defined(__XTSHELLSETTINGS_H__)

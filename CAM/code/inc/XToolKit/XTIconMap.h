//:Ignore
// XTIconMap.h: interface for the CXTIconMap class.
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

#if !defined(__XTICONMAP_H__)
#define __XTICONMAP_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTIconMap is a CObject derived class.  This class stores command to
// icon associations. Icons are owned by this map and will be destroyed
// once this object is destroyed.
//
class CXTIconMap : public CObject
{
	struct MapEntry
	{
		// Image icon.
		HICON hIcon;
		// Disabled image icon.
		HICON hDisabledIcon;
		// Hot image icon.
		HICON hHotIcon;
	};

	// A map of commands to their icons.
	CMap<UINT, UINT, MapEntry, MapEntry&> m_map;

public:
	// Constructs a CXTIconMap object.
	//
	CXTIconMap();

	// Destroys a CXTIconMap object, handles cleanup and de-allocation. It
	// destroys the icons currently in the map.
	//
	virtual ~CXTIconMap();
	
	// This member function removes and destroys all icons in the map.
	//
	void RemoveAll();

	// This member function sets a command to icon association. The existing
	// icon, if any, will be destroyed.
	//
	void SetAt(
		// A UINT value that represents the command ID associated with the icon.
		UINT nCmdID, 
		// Handle of the icon to associate with 'nCmdID'.
		HICON hIcon,
		// Handle of the disabled icon to associate with 'nCmdID'.
		HICON hDisabledIcon,
		// Handle of the hot icon to associate with 'nCmdID'.
		HICON hHotIcon);

	// This member function looks up an icon by its command ID. Returns TRUE
	// if the icon exists, or FALSE if the icon does not exist.
	//
	BOOL Lookup(
		// A UINT value that represents the command ID associated with the icon.
		UINT nCmdID, 
		// Reference to an HICON object to receive the handle of the icon associated
		// with 'nCmdID'.
		HICON& hIcon,
		// Reference to an HICON object to receive the handle of the disabled
		// icon associated with 'nCmdID'.
		HICON& hDisabledIcon,
		// Reference to an HICON object to receive the handle of the hot
		// icon associated with 'nCmdID'.
		HICON& hHotIcon);

private:
	static void DestroyEntry(MapEntry& entry);
};

#endif // !defined(__XTICONMAP_H__)

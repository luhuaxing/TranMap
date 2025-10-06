// Identifier.cpp : Implementation of CIdentifier

#include "stdafx.h"
#include "Identifier.h"

//****************************************************************
//			HotTracking()
//****************************************************************
void  CIdentifier::get_HotTracking(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _hotTracking;
	return ;
}
void  CIdentifier::put_HotTracking(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_hotTracking = newVal;
	return ;
}

//****************************************************************
//			IdentifierMode()
//****************************************************************
void  CIdentifier::get_IdentifierMode(tkIdentifierMode* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _mode;
	return ;
}
void  CIdentifier::put_IdentifierMode(tkIdentifierMode newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_mode = newVal;
	return ;
}

//****************************************************************
//			IdentifierColor()
//****************************************************************
void  CIdentifier::get_OutlineColor(OLE_COLOR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _color;
	return ;
}
void  CIdentifier::put_OutlineColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_color = newVal;
	return ;
}

//****************************************************************
//			IdentifierColor()
//****************************************************************
void  CIdentifier::get_ActiveLayer(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _activeLayer;
	return ;
}
void  CIdentifier::put_ActiveLayer(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_activeLayer = newVal;
	return ;
}



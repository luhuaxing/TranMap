#include "stdafx.h"
#include "LabelClass.h"

// ***********************************************************
//		LabelText
// ***********************************************************
// Passing pointer to LabelInfo structure through COM
void  CLabelClass::get_Text(BSTR* retval)						
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*retval = _label->text.AllocSysString();
	return ;
};
void  CLabelClass::put_Text(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	_label->text = OLE2CA(newVal);
	return ;
};

// ***********************************************************
//		LabelData
// ***********************************************************
char* CLabelClass::get_LabelData()
{
	return reinterpret_cast<char*>(_label);
}
void CLabelClass::put_LabelData(char* newVal)
{
	if (newVal == NULL) return;
	
	// if the memory was allocated in this class we should free it;
	if (_canDelete)
		delete _label;

	_label = reinterpret_cast<CLabelInfo*>(newVal);
	
	_canDelete = false; // we can't delete the object hereafter as we didn't allocate the memory;
}

// ***********************************************************
//		ScreenExtents
// ***********************************************************
void  CLabelClass::get_ScreenExtents(CExtents** retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CExtents* ext = NULL;
	if (_label->horizontalFrame)
	{
		ext = new CExtents();
		ext->SetBounds( _label->horizontalFrame->left, _label->horizontalFrame->top, 0.0,
						_label->horizontalFrame->right, _label->horizontalFrame->bottom, 0.0);
		*retval = ext;
	}
	else if (_label->rotatedFrame)
	{
		ext = new CExtents();
		CRect* rect = _label->rotatedFrame->BoundingBox();
		ext->SetBounds( rect->left, rect->top, 0.0, rect->right, rect->bottom, 0.0);
		*retval = ext;
	}
	else
	{
		*retval = NULL;
	}
	return ;
}

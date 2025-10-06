#pragma once
#include "LabelOptions.h"
#include "Extents.h"

// CLabelClass
class  CLabelClass
{
public:
	CLabelClass()
	{
		_canDelete = true;
		_label = new CLabelInfo();
		gReferenceCounter.AddRef(tkInterface::idLabel);
	}
	~CLabelClass()
	{
		if (_label != NULL && _canDelete) 
			delete _label;
		gReferenceCounter.Release(tkInterface::idLabel);
	}



public:
	void get_Visible(VARIANT_BOOL* retval)			{*retval = _label->visible ? VARIANT_TRUE : VARIANT_FALSE;	return ;};
	void put_Visible(VARIANT_BOOL newVal)				{_label->visible = newVal?true:false;	return ;};		
	
	void get_Rotation(double* retval)					{*retval = _label->rotation;		return ;};
	void put_Rotation(double newVal)					{_label->rotation = newVal;		return ;};
	
	void get_Text(BSTR* retval);						
	void put_Text(BSTR newVal);
	
	void get_X(double* retval)						{*retval = _label->x;				return ;};
	void put_X(double newVal)							{_label->x = newVal;				return ;};
	
	void get_Y(double* retval)						{*retval = _label->y;				return ;};
	void put_Y(double newVal)							{_label->y = newVal;				return ;};
	
	void get_Category(long* retval)					{*retval = _label->category;		return ;};
	void put_Category(long newVal)					{_label->category = newVal;		return ;};

	void get_IsDrawn(VARIANT_BOOL* retval)			{*retval = _label->isDrawn;		return ;};

	void get_ScreenExtents(CExtents** retval);

private:
	CLabelInfo* _label;
	bool _canDelete;		// CLabelInfo can be allocated locally, then we need to delete it
							// For example, when client create CLabel class with new command.
							// In case CLabelInfo is a pointer to the element of CLabels vector we must not delete it
							// For a example if a client receive reference to some existing label and then releases 
							// CLabelClass.

public:
	char* get_LabelData();
	void put_LabelData(char* newVal);
};



#include "stdafx.h"
#include "Chart.h"
#include "Extents.h"

// **********************************************************
//		PositionX
// **********************************************************
void CChart::get_PositionX(double* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _chartData->x;
	return ;
}
void CChart::put_PositionX(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_chartData->x = newVal;
	return ;
}

// **********************************************************
//		PositionY
// **********************************************************
void CChart::get_PositionY(double* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _chartData->y;
	return ;
}
void CChart::put_PositionY(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_chartData->y = newVal;
	return ;
}

// **********************************************************
//		ChartVisible
// **********************************************************
void CChart::get_Visible(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _chartData->visible;
	return ;
}
void CChart::put_Visible(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_chartData->visible = newVal?true:false;
	return ;
}

// **********************************************************
//		ChartVisible
// **********************************************************
void CChart::get_IsDrawn(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*retVal = _chartData->isDrawn;
	return ;
}
void CChart::put_IsDrawn(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_chartData->isDrawn = newVal?true:false;
	return ;
}

// ***********************************************************
//		ScreenExtents
// ***********************************************************
void CChart::get_ScreenExtents(CExtents** retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CExtents* ext = NULL;

	if (_chartData->frame)
	{
		ext = new CExtents();
		ext->SetBounds( _chartData->frame->left,_chartData->frame->top, 0.0,
						_chartData->frame->right, _chartData->frame->bottom, 0.0);
		*retval = ext;
	}
	else
	{
		*retval = NULL;
	}
	return ;
}


// ***********************************************************
//		LabelData
// ***********************************************************
char* CChart::get_ChartData()
{
	return reinterpret_cast<char*>(_chartData);
}
void CChart::put_ChartData(char* newVal)
{
	if (newVal == NULL) return;
	
	// if the memory was allocated in this class we should free it;
	if (_canDelete)
	{
		delete _chartData;
		_chartData = NULL;
	}

	_chartData = reinterpret_cast<CChartInfo*>(newVal);
	_canDelete = false; // we can't delete the object hereafter as we didn't allocate the memory;
}
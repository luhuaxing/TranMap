
#pragma once
#include "ShapeInfo.h"
#include "Extents.h"

// CChart
class  CChart 
{
public:
	CChart()
	{
		_canDelete = true;
		_chartData = new CChartInfo();
	}
	~CChart()
	{
		if (_chartData != NULL && _canDelete) 
			delete _chartData;
	}



public:
	void get_PositionX(double* retVal);
	void put_PositionX(double newVal);
	void get_PositionY(double* retVal);
	void put_PositionY(double newVal);
	void get_Visible(VARIANT_BOOL* retVal);
	void put_Visible(VARIANT_BOOL newVal);
	void get_IsDrawn(VARIANT_BOOL* retVal);
	void put_IsDrawn(VARIANT_BOOL newVal);
	void get_ScreenExtents(CExtents** retval);
	
private:
	CChartInfo* _chartData;
	bool _canDelete;		// CChartInfo can be allocated locally, then we need to delete it
							// For example, when client create CChart class with new command.
							// In case CChartInfo is a pointer to the element of CShapefile vector we must not delete it
							// For a example if a client receive reference to some existing chart and then releases 
							// CChart class.
public:
	char* get_ChartData();
	void put_ChartData(char* newVal);
};

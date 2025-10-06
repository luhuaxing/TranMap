#pragma once

// CChartField
class  CChartField 
{
public:
	CChartField()
	{
		_name =  SysAllocString(L"");
		_index = -1;
		_color = RGB(255, 255, 255);
	}

	~CChartField()
	{
		SysFreeString(_name);
	}



public:
	void get_Index( long* retVal);
	void put_Index( long newVal);
	void get_Color( OLE_COLOR* retVal);
	void put_Color( OLE_COLOR newVal);
	void get_Name( BSTR* retVal);
	void put_Name( BSTR newVal);

private:	
	BSTR _name;
	int _index;
	OLE_COLOR _color;

};
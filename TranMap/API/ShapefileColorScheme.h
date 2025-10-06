
#ifdef OLD_API

#ifndef __SHAPEFILELEGEND_H_
#define __SHAPEFILELEGEND_H_

/////////////////////////////////////////////////////////////////////////////
// CShapefileColorScheme
class  CShapefileColorScheme
{
public:
	CShapefileColorScheme()
	{	_globalCallback = NULL;
		_lastErrorCode = tkNO_ERROR;
		_layerHandle = -1;
		_key = SysAllocString(L"");
	}
	~CShapefileColorScheme()
	{	for( int i = 0; i < (int)_allBreaks.size(); i++ )
		{	if( _allBreaks[i] != NULL )
				_allBreaks[i]->Release();
			_allBreaks[i] = NULL;
		}
		SysFreeString(_key);
		if( _globalCallback != NULL )
			_globalCallback->Release();
		_globalCallback = NULL;
	}


// CShapefileColorScheme
public:
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);	
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);	
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);	
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_FieldIndex(/*[out, retval]*/ long *pVal);
	void put_FieldIndex(/*[in]*/ long newVal);
	void get_LayerHandle(/*[out, retval]*/ long *pVal);
	void put_LayerHandle(/*[in]*/ long newVal);
	void get_ColorBreak(/*[in]*/ long Index, /*[out, retval]*/ CShapefileColorBreak * *pVal);
	void put_ColorBreak(/*[in]*/ long Index, /*[in]*/ CShapefileColorBreak * newVal);
	void Add(/*[in]*/ CShapefileColorBreak * Break, /*[out,retval]*/ long * result);
	void Remove(/*[in]*/ long Index);
	void NumBreaks(/*[out, retval]*/ long * result);
	void InsertAt(/*[in]*/ int Position, /*[in]*/ CShapefileColorBreak *Break, /*[out,retval]*/ long *result);

private:

	long _fieldIndex;
	long _layerHandle;
	std::vector<CShapefileColorBreak *> _allBreaks;
	long _lastErrorCode;
	ICallback * _globalCallback;
	BSTR _key;

public: 

	void ErrorMessage(long ErrorCode);
};

#endif //__SHAPEFILELEGEND_H_

#endif
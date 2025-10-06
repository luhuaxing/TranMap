#pragma once

// CPointClass
class  CPointClass
{
public:
	CPointClass()
	{	
		_x = 0.0;
		_y = 0.0;
		_z = 0.0;
		_m = 0.0;
		//gReferenceCounter.AddRef(tkInterface::idPoint);     // it's additional overhead, use for debugging only
	}
	~CPointClass()
	{	
		//gReferenceCounter.Release(tkInterface::idPoint);
	}


// CPointClass
public:
	void get_M(/*[out, retval]*/ double *pVal);
	void put_M(/*[in]*/ double newVal);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_Z(/*[out, retval]*/ double *pVal);
	void put_Z(/*[in]*/ double newVal);
	void get_Y(/*[out, retval]*/ double *pVal);
	void put_Y(/*[in]*/ double newVal);
	void get_X(/*[out, retval]*/ double *pVal);
	void put_X(/*[in]*/ double newVal);
	void Clone(CPointClass** retVal);

private:
	double _x;
	double _y;
	double _z;      // TODO: it's quite a waste to allocate Z and M for flat types
	double _m;

};


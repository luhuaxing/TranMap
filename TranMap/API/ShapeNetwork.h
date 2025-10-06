#pragma once
#include "graph.h"
#include "heap.h"
#include "Shapefile.h"
#include "Utils.h"

struct dPOINT
{
	double x;
	double y;
};

// CShapeNetwork
class  CShapeNetwork
{
public:
	CShapeNetwork()
	{
		_lastErrorCode = tkNO_ERROR;
		_globalCallback = NULL;
		_key = SysAllocString(L"");
		_network = NULL;
		_currentNode = -1;
		_networkSize = 0;
		_netshpfile = NULL;
	}
	~CShapeNetwork()
	{
		::SysFreeString(_key);
		if( _netshpfile != NULL )
			delete _netshpfile;
		_netshpfile = NULL;
		if( _network != NULL )
			delete [] _network;
		_network = NULL;
	}


// IShapeNetwork
public:
	void Close(/*[out, retval]*/ VARIANT_BOOL * retval);
	void Open(/*[in]*/ CShapefile * sf, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void get_ParentIndex(/*[out, retval]*/ long *pVal);
	void put_ParentIndex(/*[in]*/ long newVal);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_AmbigShapeIndex(/*[in]*/ long Index, /*[out, retval]*/ long *pVal);
	void get_NetworkSize(/*[out, retval]*/ long *pVal);
	void get_NumDirectUps(/*[out, retval]*/ long *pVal);
	void get_DistanceToOutlet(/*[in]*/ long PointIndex, /*[out, retval]*/ double *pVal);
	void get_CurrentShapeIndex(/*[out, retval]*/ long *pVal);
	void get_CurrentShape(/*[out, retval]*/ CShape * *pVal);
	void get_Shapefile(/*[out, retval]*/ CShapefile * *pVal);
	void MoveToOutlet(/*[out, retval]*/ VARIANT_BOOL * retval);
	void MoveTo(/*[in]*/long ShapeIndex, /*[out, retval]*/ VARIANT_BOOL * retval);
	void MoveDown(/*[out, retval]*/VARIANT_BOOL *retval);
	void MoveUp(/*[in]*/long UpIndex, /*[out, retval]*/VARIANT_BOOL * retval);
	void RasterizeD8(/*[in]*/VARIANT_BOOL UseNetworkBounds, /*[in, optional]*/CGridHeader * Header, /*[in, optional]*/ double Cellsize, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ CGrid ** retval);
	void DeleteShape(/*[in]*/long ShapeIndex, /*[out, retval]*/VARIANT_BOOL * retval);
	void Build(/*[in]*/CShapefile *Shapefile, /*[in]*/long ShapeIndex, /*[in]*/long FinalPointIndex, /*[in]*/double Tolerance,/*[in]*/AmbiguityResolution ar, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ long * retval);

private:
	
	struct snraspnt
	{
	public:
		snraspnt()
		{
			column = 0;
			row = 0;
			length = 0;
		}
		snraspnt(const snraspnt & r)
		{
			column = r.column;
			row = r.row;
			length = r.length;
		}
		snraspnt(long c, long r, double l = 0.0)
		{
			column = c;
			row = r;
			length = l;
		}
		snraspnt operator=(const snraspnt & r)
		{
			column = r.column;
			row = r.row;
			length = r.length;
			return *this;
		}
		bool operator==(const snraspnt & r)
		{
			if (column == r.column && row == r.row)
				return true;
			return false;
		}
		void increment_length(double increment)
		{
			length += increment;
		}

		long row;
		long column;
		double length;
	};

	struct shpNetNode
	{
		shpNetNode()
		{
			distanceToOutlet = -1;
			length = 0;
			parentIndex = -1;
			downIndex = -1;
			upIndex = -1;
			used = false;
		}

		shpNetNode operator= (const shpNetNode & sn)
		{
			distanceToOutlet = sn.distanceToOutlet;
			length = sn.length;
			downIndex = sn.downIndex;
			upIndex = sn.upIndex;
			parentIndex = sn.parentIndex;
			used = sn.used;
			for (int i = 0; i < (int)sn.up.size(); i++)
				up.push_back(sn.up[i]);
			for (int j = 0; j < (int)sn.pbIndex.size(); j++)
				pbIndex.push_back(sn.pbIndex[j]);
			return *this;
		}

		~shpNetNode()
		{
		}

		std::deque<long> up;
		std::deque<long> pbIndex;

		double distanceToOutlet;
		double length;
		long downIndex;
		long upIndex;
		long parentIndex;
		bool used;
	};

private:
	long _lastErrorCode;
	ICallback * _globalCallback;
	BSTR _key;

	shpNetNode * _network;
	long _currentNode;
	long _networkSize;
	CShapefile * _netshpfile;
	std::deque<long> _ambigShapeIndex;

private:
	long UpEnd(edge * e, dPOINT * downpoint, double tolerance);
	long DownEnd(edge * e, dPOINT * downpoint, double tolerance);
	void CopyShape(bool reversePoints, CShape * oldshape, CShape * newshape);
	void CopyField(CField * oldshape, CField * newshape);
	bool IsAligned();
	short RasterDirection(snraspnt & source, snraspnt & sink);
	void recPrintShpNetwork(shpNetNode * allnodes, long index, ofstream & out);
	void PrintShpNetwork(shpNetNode * allnodes, long outlet, const char * filename);
};

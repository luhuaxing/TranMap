#pragma once
#include <algorithm>
#include "float.h"

#ifndef MAX
#  define MIN(a,b)      ((a<b) ? a : b)
#  define MAX(a,b)      ((a>b) ? a : b)
#endif

class CJenksBreaks
{
	public:
	// Constructor
	CJenksBreaks(std::vector<double>* values, int numClasses);
	~CJenksBreaks(){}

	// data structures
	struct JenksData
	{
	public:	
		double value;
		double square;
		int classId;	// number of break to which a value belongs
	};

	struct JenksBreak
	{
	public:
		double value;
		double square;
		int count;
		double SDev;
		int startId;
		int endId;
		
		JenksBreak()
		{
			value = 0.0;
			square = 0.0;
			count = 0;
			SDev = 0.0;
			startId = -1;
			endId = -1;
		}
		
		// the formula for every class is: sum((a[k])^2) - sum(a[k])^2/n
		void RefreshStandardDeviations()
		{
			SDev = square - pow(value, 2.0)/(double)(endId - startId + 1);
		}
	};
	
private:
	// data members
	std::vector<JenksData> _values;
	std::vector<JenksBreak> _classes;
	int _numClasses;
	int _numValues;
	int _previousMaxId;		// to prevent stalling in the local optimum
	int _previousTargetId;
	int _leftBound;			// the id of classes between which optimization takes place
	int _rightBound;		// initially it's all classes, then it's reducing
	bool _init;

// functions
public:
	std::vector<long>* get_Results();
	void Optimize();
	std::vector<int>* SolveAsDP(std::vector<double>& data, int numClasses);
	bool Initialized()
	{
		return _init;
	}
private:
	double get_SumStandardDeviations();
	bool FindShift();
	void MakeShift(int maxId, int targetId, int valueId);
	std::vector<int>* BuildEqualBreaks(std::vector<double>& data, int numClasses);
};

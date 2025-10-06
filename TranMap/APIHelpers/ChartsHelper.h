#pragma once
class ChartsHelper
{
public:
	static bool ReadChartFields(CShapefile* sf, std::vector<double*>* values);
	static bool ReadChartField(CShapefile* sf, std::vector<double>* values, int fieldIndex);
};


#pragma once
class GeoProcessing
{
public:
	static void CopyFields(CShapefile* sfSubject, CShapefile* sfOverlay, CShapefile* sfResult, map<long, long>& fieldMap, bool mergeFields = false);
	
};


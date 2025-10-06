#pragma once
class LabelsHelper
{
public:
	static void UpdateLabelsPositioning(CShapefile* sf);
	static CString GetFloatNumberFormat(CLabels* labels);
	static long GetCount(CLabels* labels);
};


#include "stdafx.h"
#include "measuring.h"
#include "MeasuringHelper.h"

// *****************************************************************
//	   get_ErrorMsg()
// *****************************************************************
void  CMeasuring::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		USES_CONVERSION;
	*pVal = A2BSTR(ErrorMsg(ErrorCode));
	return ;
}

// ************************************************************
//		get_LastErrorCode()
// ************************************************************
void  CMeasuring::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;
	return ;
}

// ************************************************************
//		get_GlobalCallback()
// ************************************************************
void  CMeasuring::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pVal = _globalCallback;
	return ;
}

// ************************************************************
//		put_GlobalCallback()
// ************************************************************
void  CMeasuring::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_globalCallback = newVal;
	return ;
}


// ************************************************************
//		get/put_Key()
// ************************************************************
void  CMeasuring::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;
	*pVal = OLE2BSTR(_key);
	return ;
}
void  CMeasuring::put_Key(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	::SysFreeString(_key);
	_key = OLE2BSTR(newVal);
	return ;
}

// *******************************************************
//		get_Length()
// *******************************************************
void  CMeasuring::get_IsStopped(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _measuring->IsStopped() ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

// *******************************************************
//		get_PointCount()
// *******************************************************
void  CMeasuring::get_PointCount(long* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _measuring->GetPointCount();
	return ;
}

// *******************************************************
//		get_PointXY()
// *******************************************************
void  CMeasuring::get_PointXY(long pointIndex, double* x, double* y, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pointIndex < 0 || pointIndex >= (long)_measuring->GetPointCount())
	{
		ErrorMsg(tkINDEX_OUT_OF_BOUNDS);
		*retVal = VARIANT_FALSE;
	}
	else
	{
		MeasurePoint* pnt = _measuring->GetPoint(pointIndex);
		if (pnt)
		{
			*x = pnt->Proj.x;
			*y = pnt->Proj.y;
			*retVal = VARIANT_TRUE;
		}
	}
	return ;
}

// *******************************************************
//		MeasuringType()
// *******************************************************
void  CMeasuring::get_MeasuringType(tkMeasuringType* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _measuring->GetMeasuringType();
	return ;
}
void  CMeasuring::put_MeasuringType(tkMeasuringType newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->SetMeasuringType(newVal);
	return ;
}

// *******************************************************
//		MeasuringType()
// *******************************************************
void  CMeasuring::get_Persistent(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _measuring->Persistent;
	return ;
}
void  CMeasuring::put_Persistent(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->Persistent = newVal;
	return ;
}

// *******************************************************
//		get_Length()
// *******************************************************
void  CMeasuring::get_Length(double* retVal) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _measuring->GetDistance();
	return ;
}

// *******************************************************
//		get_AreaWithClosingVertex()
// *******************************************************
void  CMeasuring::get_AreaWithClosingVertex(double lastPointProjX, double lastPointProjY, double* retVal) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _measuring->GetArea(true, lastPointProjX, lastPointProjY);
	return ;
}

// *******************************************************
//		get_Area()
// *******************************************************
void  CMeasuring::get_Area(double* retVal) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _measuring->GetArea(false, 0.0, 0.0);
	return ;
}

// *******************************************************
//		UndoPoint()
// *******************************************************
void  CMeasuring::UndoPoint(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = VARIANT_FALSE;
	*retVal = _measuring->UndoPoint() ? VARIANT_TRUE: VARIANT_FALSE;
	return ;
}

// *******************************************************
//		Stop()
// *******************************************************
void  CMeasuring::FinishMeasuring()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->Stop();
	return ;
}

// *******************************************************
//		Clear()
// *******************************************************
void  CMeasuring::Clear()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->Clear();
	return ;
}

// *******************************************************
//		SegmentLength()
// *******************************************************
void  CMeasuring::get_SegmentLength(int segmentIndex, double* retVal) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	long errorCode = tkNO_ERROR;
	*retVal = _measuring->GetSegmentLength(segmentIndex, errorCode);
	return ;
}

// *******************************************************
//		ShowBearing()
// *******************************************************
void  CMeasuring::get_ShowBearing(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _measuring->ShowBearing ? VARIANT_TRUE: VARIANT_FALSE;
	return ;
}

void  CMeasuring::put_ShowBearing(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->ShowBearing = newVal ? true: false;
	return ;
}

// *******************************************************
//		IsUsingEllipsoid()
// *******************************************************
void  CMeasuring::get_IsUsingEllipsoid(VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _measuring->GetTransformationMode() != tmNotDefined;
	return ;
}

// *******************************************************
//		AreaDisplayMode()
// *******************************************************
void  CMeasuring::get_AreaUnits(tkAreaDisplayMode* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*retVal = _measuring->AreaDisplayMode;
	return ;
}

void  CMeasuring::put_AreaUnits(tkAreaDisplayMode newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->AreaDisplayMode = newVal;
	return ;
}

// *******************************************************
//		get_IsEmpty()
// *******************************************************
void  CMeasuring::get_IsEmpty(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->GetPointCount() == 0 ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

// *******************************************************
//		BearingType()
// *******************************************************
void  CMeasuring::get_BearingType(tkBearingType* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->BearingType;
	return ;
}

void  CMeasuring::put_BearingType(tkBearingType newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->BearingType = newVal;
	return ;
}

// *******************************************************
//		ShowLength()
// *******************************************************
void  CMeasuring::get_ShowLength(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->ShowLength ? VARIANT_TRUE: VARIANT_FALSE;
 	return ;
}

void  CMeasuring::put_ShowLength(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->ShowLength = newVal ? true : false;
	return ;
}

// *******************************************************
//		LengthDisplayMode()
// *******************************************************
void  CMeasuring::get_LengthUnits(tkLengthDisplayMode* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->LengthUnits;
	return ;
}

void  CMeasuring::put_LengthUnits(tkLengthDisplayMode newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->LengthUnits = newVal;
	return ;
}

// *******************************************************
//		AngleFormat()
// *******************************************************
void  CMeasuring::get_AngleFormat(tkAngleFormat* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->AngleFormat;
	return ;
}

void  CMeasuring::put_AngleFormat(tkAngleFormat newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->AngleFormat = newVal;
	return ;
}

// *******************************************************
//		AnglePrecision()
// *******************************************************
void  CMeasuring::get_AnglePrecision(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->AnglePrecision;
	return ;
}

void  CMeasuring::put_AnglePrecision(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->AnglePrecision = newVal;
	return ;
}

// *******************************************************
//		AreaPrecision()
// *******************************************************
void  CMeasuring::get_AreaPrecision(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->AreaPrecision;
	return ;
}

void  CMeasuring::put_AreaPrecision(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->AreaPrecision = newVal;
	return ;
}

// *******************************************************
//		LengthPrecision()
// *******************************************************
void  CMeasuring::get_LengthPrecision(LONG* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->LengthPrecision;
	return ;
}

void  CMeasuring::put_LengthPrecision(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->LengthPrecision = newVal;
	return ;
}

// *******************************************************
//		PointsVisible()
// *******************************************************
void  CMeasuring::get_PointsVisible(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->PointsVisible ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

void  CMeasuring::put_PointsVisible(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->PointsVisible = newVal ? true : false;
	return ;
}

// *******************************************************
//		LineColor()
// *******************************************************
void  CMeasuring::get_LineColor(OLE_COLOR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->LineColor;
	return ;
}

void  CMeasuring::put_LineColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->LineColor = newVal;
	return ;
}

// *******************************************************
//		FillColor()
// *******************************************************
void  CMeasuring::get_FillColor(OLE_COLOR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->FillColor;
	return ;
}

void  CMeasuring::put_FillColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->FillColor = newVal;
	return ;
}

// *******************************************************
//		FillTransparency()
// *******************************************************
void  CMeasuring::get_FillTransparency(BYTE* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->FillTransparency;
	return ;
}

void  CMeasuring::put_FillTransparency(BYTE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->FillTransparency = newVal;
	return ;
}

// *******************************************************
//		LineWidth()
// *******************************************************
void  CMeasuring::get_LineWidth(FLOAT* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->LineWidth;
	return ;
}

void  CMeasuring::put_LineWidth(FLOAT newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->LineWidth = newVal;
	return ;
}

// *******************************************************
//		LineStipple()
// *******************************************************
void  CMeasuring::get_LineStyle(tkDashStyle* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->LineStyle;
	return ;
}

void  CMeasuring::put_LineStyle(tkDashStyle newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->LineStyle = newVal;
	return ;
}

// *******************************************************
//		PointLabelsVisible()
// *******************************************************
void  CMeasuring::get_PointLabelsVisible(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->PointLabelsVisible ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

void  CMeasuring::put_PointLabelsVisible(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->PointLabelsVisible = newVal ? true: false;
	return ;
}

// *******************************************************
//		ShowTotalLength()
// *******************************************************
void  CMeasuring::get_ShowTotalLength(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _measuring->ShowTotalLength ? VARIANT_TRUE : VARIANT_FALSE;
	return ;
}

void  CMeasuring::put_ShowTotalLength(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_measuring->ShowTotalLength = newVal ? true : false;
	return ;
}

// *******************************************************
//		UndoButton()
// *******************************************************
void  CMeasuring::get_UndoButton(tkUndoShortcut* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _undoButton;
	return ;
}

void  CMeasuring::put_UndoButton(tkUndoShortcut newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	_undoButton = newVal;
	return ;
}

// *******************************************************
//		Serialize()
// *******************************************************
void  CMeasuring::Serialize(BSTR* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPLXMLNode* node = MeasuringHelper::Serialize(_measuring, "MeasuringClass");
	Utility::SerializeAndDestroyXmlTree(node, retVal);

	return ;
}

// *******************************************************
//		Deserialize()
// *******************************************************
void  CMeasuring::Deserialize(BSTR state, VARIANT_BOOL* retVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;
	*retVal = VARIANT_FALSE;

	CString s = OLE2CA(state);
	CPLXMLNode* node = CPLParseXMLString(s.GetString());
	if (node)
	{
		CPLXMLNode* nodeMeasuring = CPLGetXMLNode(node, "=MeasuringClass");
		if (nodeMeasuring)
		{
			MeasuringHelper::Deserialize(_measuring, nodeMeasuring);
			*retVal = VARIANT_TRUE;
		}
		CPLDestroyXMLNode(node);
	}

	return ;
}
// ********************************************************************************************************
// File name: Shape.h
// Description: Declaration of the CShape
// ********************************************************************************************************
// The contents of this file are subject to the Mozilla Public License Version 1.1 (the "License"); 
// you may not use this file except in compliance with the License. You may obtain a copy of the License at 
// http://www.mozilla.org/MPL/ 
// Software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF 
// ANY KIND, either express or implied. See the License for the specific language governing rights and 
// limitations under the License. 
//
// The Original Code is MapWindow Open Source. 
//
// The Initial Developer of this version of the Original Code is Daniel P. Ames using portions created by 
// Utah State University and the Idaho National Engineering and Environmental Lab that were released as 
// public domain in March 2004.  
//
// -------------------------------------------------------------------------------------------------------
// Contributor(s): (Open source contributors should list themselves and their modifications here). 

#pragma once
//
#include "ShapeInterfaces.h"
#include "OgrConverter.h"
#include "ShapeWrapper.h"
#include "ShapeWrapperCOM.h"
#include "ShapePoint.h"
#include "Extents.h"
class CExtents;

// ******************************************************
//	 CShape declaration
// ******************************************************
class  CShape
{
public:
	CShape();
	~CShape();

// ******************************************************
//	 CShape interface
// ******************************************************
public:
	void get_Extents(/*[out, retval]*/ CExtents **pVal);
	void DeletePart(/*[in]*/ long PartIndex, /*[out, retval]*/ VARIANT_BOOL * retval);
	void InsertPart(/*[in]*/ long PointIndex, /*[in, out]*/ long * PartIndex, /*[out, retval]*/ VARIANT_BOOL * retval);
	void DeletePoint(/*[in]*/ long PointIndex, /*[out, retval]*/ VARIANT_BOOL * retval);
	void InsertPoint(/*[in]*/ CPointClass * NewPoint, /*[in, out]*/long * PointIndex, /*[out, retval]*/ VARIANT_BOOL * retval);
	void Create(/*[in]*/ ShpfileType _shptype, /*[out, retval]*/ VARIANT_BOOL * retval);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback **pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_Part(/*[in]*/ long PartIndex, /*[out, retval]*/ long *pVal);
	void put_Part(/*[in]*/ long PartIndex, /*[in]*/ long newVal);
	void get_Point(/*[in]*/ long PointIndex, /*[out, retval]*/ CPointClass **pVal);
	void put_Point(/*[in]*/ long PointIndex, /*[in]*/ CPointClass * newVal);
	void get_ShapeType(/*[out, retval]*/ ShpfileType *pVal);
	void put_ShapeType(/*[in]*/ ShpfileType newVal);
	void get_NumParts(/*[out, retval]*/ long *pVal);
	void get_NumPoints(/*[out, retval]*/ long *pVal);
	void SerializeToString(/*[out, retval]*/ BSTR * Serialized);
	void CreateFromString(/*[in]*/ BSTR Serialized, /*[out, retval]*/ VARIANT_BOOL * retval);
	void PointInThisPoly(/*[in]*/ CPointClass * pt, /*[out, retval]*/ VARIANT_BOOL *retval);
	void get_Centroid(CPointClass** pVal);
	void get_Length(double *pVal);
	void get_Perimeter(double *pVal);
	void get_Area(double *pVal);
	// OGRGeometry methods and properties (lsu 06-aug-2009)
	void Relates(CShape* Shape, tkSpatialRelation Relation, VARIANT_BOOL* retval);
	void Clip(CShape* Shape, tkClipOperation Operation, CShape** retval);
	void Distance(CShape* Shape, DOUBLE* retval);
	void Buffer(DOUBLE Distance, long nQuadSegments, CShape** retval);
	void Contains(CShape* Shape, VARIANT_BOOL* retval);
	void Crosses(CShape* Shape, VARIANT_BOOL* retval);
	void Disjoint(CShape* Shape, VARIANT_BOOL* retval);
	void Equals(CShape* Shape, VARIANT_BOOL* retval);
	void Intersects(CShape* Shape, VARIANT_BOOL* retval);
	void Overlaps(CShape* Shape, VARIANT_BOOL* retval);
	void Touches(CShape* Shape, VARIANT_BOOL* retval);
	void Within(CShape* Shape, VARIANT_BOOL* retval);
	void Boundary(CShape** retval);
	void ConvexHull(CShape** retval);
	void get_IsValid(VARIANT_BOOL* retval);
	void get_XY(long PointIndex, double* x, double* y, VARIANT_BOOL* retval);
	void get_PartIsClockWise(long PartIndex, VARIANT_BOOL* retval);
	void ReversePointsOrder(long PartIndex, VARIANT_BOOL* retval);
	void GetIntersection(CShape* Shape, VARIANT* Results, VARIANT_BOOL* retval);
	void get_Center(CPointClass **pVal);
	void get_EndOfPart(long PartIndex, long* retval);	
	void get_PartAsShape(long PartIndex, CShape** retval);
	void get_IsValidReason(BSTR* retval);
	void get_InteriorPoint(CPointClass** retval);
	void Clone(CShape** retval);
	void Explode(VARIANT* Results, VARIANT_BOOL* retval);
	void put_XY(LONG pointIndex, DOUBLE x, DOUBLE y, VARIANT_BOOL* retVal);
	void ExportToBinary(VARIANT* bytesArray, VARIANT_BOOL* retVal);
	void ImportFromBinary(VARIANT bytesArray, VARIANT_BOOL* retVal);
	void FixUp(CShape** retval);
	void AddPoint(double x, double y, long* pointIndex);
	void ExportToWKT(BSTR* retVal);
	void ImportFromWKT(BSTR Serialized, VARIANT_BOOL *retVal);
	void CopyFrom(CShape* sourceShape, VARIANT_BOOL* retVal);
	void ClosestPoints(CShape* shape2, CShape** result);
	void put_M(LONG pointIndex, double m, VARIANT_BOOL* retVal);
	void put_Z(LONG pointIndex, double z, VARIANT_BOOL* retVal);
	void get_M(LONG pointIndex, double* m, VARIANT_BOOL* retVal);
	void get_Z(LONG pointIndex, double* z, VARIANT_BOOL* retVal);
	void BufferWithParams(DOUBLE Ditances, LONG numSegments, VARIANT_BOOL singleSided, tkBufferCap capStyle, tkBufferJoin joinStyle, DOUBLE mitreLimit, CShape** retVal);
	void Move(DOUBLE xProjOffset, DOUBLE yProjOffset);
	void Rotate(DOUBLE originX, DOUBLE originY, DOUBLE angle);
	void get_ShapeType2D(ShpfileType* pVal);
	void SplitByPolyline(CShape* polyline, VARIANT* results, VARIANT_BOOL* retVal);
	void get_IsEmpty(VARIANT_BOOL* pVal);

private:
	BSTR _key;
	long _lastErrorCode;
	ICallback * _globalCallback;
	CString _isValidReason;

	// either CShapeWrapper or CShapeWrapperCOM
	IShapeWrapper* _shp;

	// forces to use fast shape wrapper class to hold points information
	bool _useFastMode;							
	
private:
	void ErrorMessage(long ErrorCode);
	void PointInThisPolyFast(CPointClass * pt, VARIANT_BOOL *retval);
	void PointInThisPolyRegular(CPointClass * pt, VARIANT_BOOL *retval);
	
public:
	bool ExplodeCore(std::vector<CShape*>& vShapes);
	CShapeWrapperCOM* InitComWrapper(CShapeWrapper* shpOld);
	CShapeWrapper* InitShapeWrapper(CShapeWrapperCOM* shpOld);
	bool FixupShapeCore(ShapeValidityCheck validityCheck);
	bool put_ShapeWrapper(CShapeWrapper* data);
	IShapeWrapper* get_ShapeWrapper();
	void put_fastMode(bool state);				// toggles the storing mode for the shape
	bool get_fastMode();
	double get_SegmentAngle( long segementIndex);
	void put_fastModeAdd(bool state);
	void get_LabelPosition(tkLabelPositioning method, double& x, double& y, double& rotation, tkLineLabelOrientation orientation);
	bool get_Z(long PointIndex, double* z);
	bool get_M(long PointIndex, double* m);
	bool get_XY(long PointIndex, double* x, double* y);
	bool get_XYM(long PointIndex, double* x, double* y, double* m);
	bool get_XYZ(long PointIndex, double* x, double* y, double* z);
	bool get_XYZM(long PointIndex, double& x, double& y, double& z, double& m);
	bool get_ExtentsXY(double& xMin, double& yMin, double& xMax, double& yMax);
	bool get_ExtentsXYZM(double& xMin, double& yMin, double& xMax, double& yMax, double& zMin, double& zMax, double& mMin, double& mMax);
	bool ValidateBasics(ShapeValidityCheck& failedCheck, CString& err);
	void get_LabelPositionAutoChooseMethod(tkLabelPositioning method, double& x, double& y, double& rotation, tkLineLabelOrientation orientation);
	bool SplitByPolylineCore(CShape* polyline, vector<CShape*>& results);
	
	
};

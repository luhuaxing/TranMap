#pragma once
#include <set>
#include "IndexSearching.h"
#include "QTree.h"
#include "ClipperConverter.h"
#include "ShapeInfo.h"
#include "ColoringGraph.h"
#include "ShapeDrawingOptions.h"
#include "Labels.h"
#include "ShapefileCategories.h"
#include "ShapeValidationInfo.h"
#include "FieldStatOperations.h"
#include "Charts.h"
#include "Table.h"
#include "GlobalVars.h"
#include "ILayer.h"


//Shapefile File Info
#define HEADER_BYTES_16 50
#define HEADER_BYTES_32 100
#define FILE_CODE      9994
#define VERSION        1000
#define UNUSEDSIZE 5
#define UNUSEDVAL		0     	    
#define RECORD_HEADER_LENGTH_32 8
#define RECORD_SHAPE_TYPE_32 8

// *********************************************************************
// CShapefile declaration
// *********************************************************************
class  CShapefile: public ILayer
{
public:
	
	CShapefile();
	virtual ~CShapefile();

// *********************************************************************
//		CShapefile interface
// *********************************************************************
public:
	virtual eLayerType GetLayerType() { return eLayerType::SHAPFILE_LAYER; }
	void QuickPoints(/*[in]*/ long ShapeIndex, /*[in, out]*/ long * NumPoints, /*[out, retval]*/ SAFEARRAY ** retval);
	void QuickExtents(/*[in]*/ long ShapeIndex, /*[out, retval]*/ CExtents ** retval);
	bool QuickPoint(/*[in]*/ long ShapeIndex, /*[in]*/ long PointIndex, /*[out, retval]*/ CPointClass ** retval);
	void get_Filename(/*[out, retval]*/ BSTR *pVal);
	void get_FileHandle(/*[out, retval]*/ long *pVal);
	void get_EditingTable(/*[out, retval]*/ VARIANT_BOOL *pVal);
	void get_CellValue(/*[in]*/ long FieldIndex, /*[in]*/ long ShapeIndex, /*[out, retval]*/ VARIANT *pVal);
	void get_Field(/*[in]*/ long FieldIndex, /*[out, retval]*/ CField * *pVal);
	void get_FieldByName(/*[in]*/ BSTR Fieldname, /*[out, retval] */ CField * *pVal);
	void StopEditingTable(/*[in, optional, defaultvalue(TRUE)]*/ VARIANT_BOOL ApplyChanges, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void StartEditingTable(/*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void EditCellValue(/*[in]*/ long FieldIndex, /*[in]*/ long ShapeIndex, /*[in]*/ VARIANT NewVal, /*[out, retval]*/ VARIANT_BOOL * retval);
	void EditDeleteField(/*[in]*/ long FieldIndex, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void EditInsertField(/*[in]*/ CField * NewField, /*[in,out]*/long * FieldIndex, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void StopEditingShapes(/*[in, optional, defaultvalue(TRUE)]*/VARIANT_BOOL ApplyChanges,/*[in,optional,defaultvalue(TRUE)]*/VARIANT_BOOL StopEditTable, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void StartEditingShapes(/*[in,optional,defaultvalue(TRUE)]*/VARIANT_BOOL StartEditTable,/*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void SelectShapes(/*[in]*/ CExtents * BoundBox, /*[in, optional, defaultvalue(0.0)]*/double Tolerance, /*[in, optional, defaultvalue(INTERSECTION)]*/ SelectMode SelectMode, /*[in, out]*/ VARIANT * Result, /*[out, retval]*/ VARIANT_BOOL * retval);
	void EditDeleteShape(/*[in]*/long ShapeIndex, /*[out, retval]*/ VARIANT_BOOL * retval);
	void EditInsertShape(/*[in]*/CShape * Shape, /*[in, out]*/ long * ShapeIndex, /*[out, retval]*/ VARIANT_BOOL * retval);
	void EditClear(/*[out, retval]*/ VARIANT_BOOL * retval);
	void Close(/*[out, retval]*/ VARIANT_BOOL * retval);
	void SaveAs(/*[in]*/ BSTR ShapefileName, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void CreateNew(/*[in]*/ BSTR ShapefileName, /*[in]*/ ShpfileType ShapefileType, /*[out, retval]*/ VARIANT_BOOL * retval);
	void CreateNewWithShapeID(/*[in]*/ BSTR ShapefileName, /*[in]*/ ShpfileType ShapefileType, /*[out, retval]*/ VARIANT_BOOL * retval);
	void Open(/*[in]*/ BSTR ShapefileName, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/VARIANT_BOOL * retval);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_CdlgFilter(/*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_EditingShapes(/*[out, retval]*/ VARIANT_BOOL *pVal);
	void get_Shape(/*[in]*/long ShapeIndex, /*[out, retval]*/ CShape * *pVal);
	void get_ShapefileType(/*[out, retval]*/ ShpfileType *pVal);
	void get_Extents(/*[out, retval]*/ CExtents * *pVal);
	void get_NumFields(/*[out, retval]*/ long *pVal);
	void get_NumShapes(/*[out, retval]*/ long *pVal);
	void get_Projection(/*[out, retval]*/ BSTR *pVal);
	void put_Projection(/*[in]*/BSTR proj4String);
	void get_NumPoints(/*[in]*/ long ShapeIndex, /*[out, retval]*/ long *pVal);
	void get_UseSpatialIndex(/*[out, retval]*/VARIANT_BOOL *pVal);
	void put_UseSpatialIndex(/*[in]*/VARIANT_BOOL pVal);
 	void get_HasSpatialIndex(/*[out, retval]*/VARIANT_BOOL *pVal);
	void put_HasSpatialIndex(/*[in]*/VARIANT_BOOL pVal);
    void CreateSpatialIndex(/*[in]*/BSTR ShapefileName, /*[out, retval]*/ VARIANT_BOOL  *pVal);
	void Resource(/*[in]*/ BSTR newSrcPath, /*[out, retval]*/ VARIANT_BOOL * retval);
	void IsSpatialIndexValid(/*[out, retval]*/ VARIANT_BOOL  *pVal);
	void put_SpatialIndexMaxAreaPercent(/*[in]*/ DOUBLE newVal);
	void get_SpatialIndexMaxAreaPercent(/*[out, retval]*/ DOUBLE* pVal);
	void get_CanUseSpatialIndex(/*[in]*/ CExtents* pArea, /*[out, retval]*/ VARIANT_BOOL* pVal);
	void PointInShape(LONG ShapeIndex, DOUBLE x, DOUBLE y, VARIANT_BOOL* retval);
	void PointInShapefile(DOUBLE x, DOUBLE y, LONG* ShapeIndex);
	void BeginPointInShapefile(VARIANT_BOOL* retval);
	void EndPointInShapefile(void);
	void get_CacheExtents(VARIANT_BOOL * pVal);
	void put_CacheExtents(VARIANT_BOOL newVal);
	void RefreshExtents(VARIANT_BOOL * pVal);
	void RefreshShapeExtents(LONG ShapeId, VARIANT_BOOL *pVal);
	void QuickQueryInEditMode(/*[in]*/CExtents * BoundBox,int ** Result, int* ResultCount);
	void get_UseQTree(VARIANT_BOOL * pVal);
	void put_UseQTree(VARIANT_BOOL pVal);
	void Save(/*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void GetIntersection(/*[in]*/VARIANT_BOOL SelectedOnlyOfThis, /*[in]*/CShapefile* sf, /*[in]*/VARIANT_BOOL SelectedOnly, /*[in]*/ ShpfileType fileType, /*[in, optional, defaultvalue(NULL)]*/ ICallback * cBack, /*[out, retval]*/ CShapefile** retval);
	void SelectByShapefile(/*[in]*/CShapefile* sf,  /*[in]*/tkSpatialRelation Relation, /*[in]*/VARIANT_BOOL SelectedOnly, /*[in, out]*/ VARIANT *Result, /*[in, optional, defaultvalue(NULL)]*/ ICallback* cBack, /*[out, retval]*/VARIANT_BOOL *retval);
	void get_SelectionDrawingOptions(/*[out, retval]*/CShapeDrawingOptions** pVal);
	void put_SelectionDrawingOptions(/*[in]*/CShapeDrawingOptions* newVal);
	void get_ShapeSelected(/*[in]*/long ShapeIndex,/*[out, retval]*/ VARIANT_BOOL* pVal);
	void put_ShapeSelected(/*[in]*/long ShapeIndex, /*[in]*/ VARIANT_BOOL newVal);
	void get_NumSelected(/*[out, retval]*/long *pVal);
	void SelectAll();
	void SelectNone();
	void InvertSelection();
	void Dissolve(long FieldIndex, VARIANT_BOOL SelectedOnly, CShapefile** sf);
	void get_Labels(CLabels** pVal);
	void put_Labels(CLabels* newVal);
	void GenerateLabels(long FieldIndex, tkLabelPositioning Method, VARIANT_BOOL LargestPartOnly, long* Count);
	void Clone(CShapefile** retVal);
	void get_DefaultDrawingOptions(CShapeDrawingOptions** pVal);
	void put_DefaultDrawingOptions(CShapeDrawingOptions* newVal);
	void get_Categories(CShapefileCategories** pVal);
	void put_Categories(CShapefileCategories* newVal);
	void get_Charts(CCharts** pVal);
	void put_Charts(CCharts* newVal);
	void get_ShapeCategory(/*[in]*/long ShapeIndex,/*[out, retval]*/ long* pVal);
	void put_ShapeCategory(/*[in]*/long ShapeIndex, /*[in]*/ long newVal);
	void get_Table(CTableClass** retVal);
	void get_VisibilityExpression(BSTR* retval);
	void put_VisibilityExpression(BSTR newVal);
	void get_FastMode(VARIANT_BOOL* retval);		// in fast editing mode CShapeWrapper class is used to store shape points
	void put_FastMode(VARIANT_BOOL newVal);		// there are some restrictions on editing this mode though
	void get_MinDrawingSize(LONG* pVal);
	void put_MinDrawingSize(LONG newVal);
	void get_SourceType(tkShapefileSourceType* pVal);
	void BufferByDistance(double Distance, LONG nSegments, VARIANT_BOOL SelectedOnly, VARIANT_BOOL MergeResults, CShapefile** sf);
	void get_GeometryEngine(tkGeometryEngine* pVal);
	void put_GeometryEngine(tkGeometryEngine pVal);
	void Difference(VARIANT_BOOL SelectedOnlySubject, CShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay, CShapefile** retval);
	void Clip(VARIANT_BOOL SelectedOnlySubject, CShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay, CShapefile** retval);
	void SymmDifference(VARIANT_BOOL SelectedOnlySubject, CShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay, CShapefile** retval);
	void Union(VARIANT_BOOL SelectedOnlySubject, CShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay, CShapefile** retval);
	void ExplodeShapes(VARIANT_BOOL SelectedOnly, CShapefile** retval);
	void AggregateShapes(VARIANT_BOOL SelectedOnly, LONG FieldIndex, CShapefile** retval);
	void ExportSelection(CShapefile** retval);
	void Sort(LONG FieldIndex, VARIANT_BOOL Ascending, CShapefile** retval);
	void Merge(VARIANT_BOOL SelectedOnlyThis, CShapefile* sf, VARIANT_BOOL SelectedOnly, CShapefile** retval);
	void get_SelectionColor(OLE_COLOR* retval);
	void put_SelectionColor(OLE_COLOR newVal);
	void get_SelectionAppearance(tkSelectionAppearance* retval);
	void put_SelectionAppearance(tkSelectionAppearance newVal);
	void get_CollisionMode(tkCollisionMode* retval);
	void put_CollisionMode(tkCollisionMode newVal);
	void get_SelectionTransparency(BYTE* retval);
	void put_SelectionTransparency(BYTE newVal);
	void put_StopExecution(CStopExecution* stopper);
	void Serialize(VARIANT_BOOL SaveSelection, BSTR* retVal);
	void Serialize2(VARIANT_BOOL SaveSelection, VARIANT_BOOL SerializeCategories, BSTR* retVal);
	void Deserialize(VARIANT_BOOL LoadSelection, BSTR newVal);
	void get_GeoProjection(CGeoProjection** retVal);
	void put_GeoProjection(CGeoProjection* pVal);
	void Reproject(CGeoProjection* newProjection, LONG* reprojectedCount, CShapefile** retVal);
	void ReprojectInPlace(CGeoProjection* newProjection, LONG* reprojectedCount, VARIANT_BOOL* retVal);
	void SimplifyLines(DOUBLE Tolerance, VARIANT_BOOL SelectedOnly, CShapefile** retVal);
	void FixUpShapes(CShapefile** retVal, VARIANT_BOOL* fixed);
	void GetRelatedShapes(long referenceIndex, tkSpatialRelation relation, VARIANT* resultArray, VARIANT_BOOL* retval);
	void GetRelatedShapes2(CShape* referenceShape, tkSpatialRelation relation, VARIANT* resultArray, VARIANT_BOOL* retval);
	void get_Identifiable(VARIANT_BOOL* retVal);
	void put_Identifiable(VARIANT_BOOL newVal);
	void HasInvalidShapes(VARIANT_BOOL* result);
	void EditAddShape(CShape* shape, long* shapeIndex);
	void EditAddField(BSTR name, FieldType type, int precision, int width, long* fieldIndex);
	void GetClosestVertex(double x, double y, double maxDistance, long* shapeIndex, long* pointIndex, double* distance, VARIANT_BOOL* retVal);
	void get_ShapeCategory2(long ShapeIndex, BSTR* categoryName);
	void put_ShapeCategory2(long ShapeIndex, BSTR categoryName);
	void get_ShapeCategory3(long ShapeIndex, CShapefileCategory** category);
	void put_ShapeCategory3(long ShapeIndex, CShapefileCategory* category);
	void Dump(BSTR ShapefileName, ICallback *cBack, VARIANT_BOOL *retval);
	void LoadDataFrom(BSTR ShapefileName, ICallback *cBack, VARIANT_BOOL *retval);
	void Segmentize(CShapefile** retVal);
	void get_LastInputValidation(CShapeValidationInfo** retVal);
	void get_LastOutputValidation(CShapeValidationInfo** retVal);
	void ClearCachedGeometries();
	void AggregateShapesWithStats(VARIANT_BOOL SelectedOnly, LONG FieldIndex, CFieldStatOperations* statOperations, CShapefile** retval);
	void DissolveWithStats(long FieldIndex, VARIANT_BOOL SelectedOnly, CFieldStatOperations* statOperations, CShapefile** sf);
	void get_ShapeRotation(long ShapeIndex, double* pVal);
	void put_ShapeRotation(long ShapeIndex, double newVal);
	void get_ShapeVisible(long ShapeIndex, VARIANT_BOOL* pVal);
	void get_Volatile(VARIANT_BOOL* retval);
	void put_Volatile(VARIANT_BOOL newVal);
	void EditUpdateShape(long shapeIndex, CShape* shpNew, VARIANT_BOOL* retVal);
	void get_ShapeModified(long ShapeIndex, VARIANT_BOOL* retVal);
	void put_ShapeModified(long ShapeIndex, VARIANT_BOOL newVal);
	void Validate(tkShapeValidationMode validationMode, VARIANT_BOOL selectedOnly, CShapeValidationInfo** results);
	void get_UndoList(CUndoList** pVal);
	void get_InteractiveEditing(VARIANT_BOOL* pVal);
	void put_InteractiveEditing(VARIANT_BOOL newVal);
	void get_ShapeIsHidden(LONG shapeIndex, VARIANT_BOOL* pVal);
	void put_ShapeIsHidden(LONG shapeIndex, VARIANT_BOOL newVal);
	void get_Snappable(VARIANT_BOOL* pVal);
	void put_Snappable(VARIANT_BOOL newVal);
	void get_ShapefileType2D(ShpfileType* pVal);
	void get_FieldIndexByName(BSTR fieldName, LONG* pVal);
	void Move(DOUBLE xProjOffset, DOUBLE yProjOffset, VARIANT_BOOL* retVal);
	void RemoveSpatialIndex(VARIANT_BOOL* retVal);
	void get_ShapeRendered(LONG ShapeIndex, VARIANT_BOOL* pVal);

private:

	// data for point in shapefile test
	struct ShapeHeader
	{
		double MinX, MinY;
		double MaxX, MaxY;
		int NumParts;
		int NumPoints;
	};
	struct PolygonShapefile
	{
		ShapeHeader shpHeader;
		std::vector<Point2D> Points;
		std::vector<int> Parts;
	};

private:

	std::vector<PolygonShapefile> _polySf;
	
	tkShapefileSourceType _sourceType;		// is it disk-based or in-memory?
	ShpfileType _shpfiletype;
	BSTR _key;
	long _lastErrorCode;
	
	//Extent Information
	double _minX;
	double _minY;
	double _minZ;
	double _maxX;
	double _maxY;
	double _maxZ;
	double _minM;
	double _maxM;

	//Disk access
	FILE * _shpfile;
	FILE * _shxfile;

	CStringW _shpfileName;
	CStringW _shxfileName;
	CStringW _dbffileName;
	CStringW _prjfileName;
	
	std::vector<ShapeData*> _shapeData;
	std::vector<long> _shpOffsets;		//(32 bit words)

	// table is initialized in CreateNew or Open methods
	// it is is destroyed in Close() method
	// in case table is null, shapefile will be considered uninitialized
	CTableClass*  _table;
	CGeoProjection* _geoProjection;
	CStopExecution* _stopExecution;
	CCharts* _charts;
	CLabels* _labels;
	CShapefileCategories* _categories;
	ICallback * _globalCallback;
	CShapeDrawingOptions* _selectDrawOpt;	
	CShapeDrawingOptions* _defaultDrawOpt;
	CShapeValidationInfo* _inputValidation;

	CShapeValidationInfo* _outputValidation;
	CUndoList* _undoList;
	
	VARIANT_BOOL _hotTracking;
	bool _geosGeometriesRead;
	tkCollisionMode _collisionMode;		// collision mode for point shapefiles
	tkGeometryEngine _geometryEngine;		// GEOS or Clipper
	bool _writing;		// is currently writing to the file
	bool _reading;		// is currently reading data into memory
	
	BSTR _expression;		// visibility expression
	BOOL _isEditingShapes;		//Flag for Disk vs. Memory
	long _nextShapeHandle;		// the next unique handle to assign
	VARIANT_BOOL _interactiveEditing;
	VARIANT_BOOL _snappable;
	
	// When this flag is on CShapeWrapper will be used in the Shape class to store the points
	// otherwise usual COM points
	BOOL _fastMode;
	int _minDrawingSize;	// objects which are less than this value in pixels for current scale, will drawn as point
	BOOL _cacheExtents;	// extents won't be recalculated in each get_Extents call
	bool _volatile;

	//Flags for Spatial Indexing
	BOOL _useSpatialIndex;
	BOOL _hasSpatialIndex;
	IndexSearching::CSpatialIndexID _spatialIndexID;
	BOOL _spatialIndexLoaded;	
	DOUBLE _spatialIndexMaxAreaPercent;
	int _spatialIndexNodeCapacity;
	
	// drawing options
	tkSelectionAppearance _selectionAppearance;
	OLE_COLOR _selectionColor;
	unsigned char _selectionTransparency;

	QTree* _qtree;
	BOOL _useQTree;
	vector<int> _deleteElement;
	
	// during processing operations only
	QTree* _tempTree;
	
	bool _useValidationList;

private:
	// memory shapes
	BOOL ReleaseMemoryShapes();
	BOOL VerifyMemShapes(ICallback * cBack);
	void TrimMemShapes();

	// read/write
	BOOL ReadShx();
	BOOL WriteShx(FILE * _shxfile, ICallback * cBack);
	BOOL WriteShp(FILE * shpfile, ICallback * cBack);	

	// selection
	BOOL DefineShapePoints(long ShapeIndex, ShpfileType & ShapeType, std::vector<long> & parts, std::vector<double> & xPts, std::vector<double> & yPts);
	VARIANT_BOOL SelectShapesAlt(CExtents *BoundBox, double Tolerance, SelectMode SelectMode, VARIANT* arr);

	// initialization
	void put_ReferenceToLabels(bool bNullReference = false);
	void put_ReferenceToCategories(bool bNullReference = false);
	void put_ReferenceToCharts(bool bNullReference = false);

	// quad tree
	void GenerateQTree();
	QTree* GenerateQTreeCore(bool SelectedOnly);
	bool GenerateTempQTree(bool SelectedOnly);
	void ClearTempQTree();
	QTree* GetTempQTree();

	// geoprocessing
	void DoClipOperation(VARIANT_BOOL SelectedOnlySubject, CShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay, CShapefile** retval, tkClipOperation operation, ShpfileType returnType = SHP_NULLSHAPE);
	void DissolveClipper(long FieldIndex, VARIANT_BOOL SelectedOnly, CFieldStatOperations* operations, CShapefile* sf);
	void DissolveGEOS(long FieldIndex, VARIANT_BOOL SelectedOnly, CFieldStatOperations* operations, CShapefile* sf);
	void IntersectionGEOS(VARIANT_BOOL SelectedOnlySubject, CShapefile* sfClip, VARIANT_BOOL SelectedOnlyClip, CShapefile* sfResult, map<long, long>* fieldMap = NULL, std::set<int>* subjectShapesToSkip = NULL,  std::set<int>* clippingShapesToSkip = NULL );
	void IntersectionClipper(VARIANT_BOOL SelectedOnlySubject, CShapefile* sfClip, VARIANT_BOOL SelectedOnlyClip, CShapefile* sfResult, map<long, long>* fieldMap = NULL, std::set<int>* subjectShapesToSkip = NULL,  std::set<int>* clippingShapesToSkip = NULL );
	CShapefile* IntersectionClipperNoAttributes(VARIANT_BOOL SelectedOnlySubject, CShapefile* sfClip, VARIANT_BOOL SelectedOnlyClip );
	void DifferenceGEOS(CShapefile* sfSubject, VARIANT_BOOL SelectedOnlySubject, CShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay,  CShapefile* sfResult, map<long, long>* fieldMap = NULL, std::set<int>* shapesToSkip = NULL);
	void DifferenceClipper(CShapefile* sfSubject, VARIANT_BOOL SelectedOnlySubject, CShapefile* sfClip, VARIANT_BOOL SelectedOnlyClip, CShapefile* sfResult, map<long, long>* fieldMap = NULL, std::set<int>* shapesToSkip = NULL);
	void ClipGEOS(VARIANT_BOOL SelectedOnlySubject, CShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay, CShapefile* sfResult);
	void ClipClipper(VARIANT_BOOL SelectedOnlySubject, CShapefile* sfOverlay, VARIANT_BOOL SelectedOnlyOverlay, CShapefile* sfResult);
	void AggregateShapesCore(VARIANT_BOOL SelectedOnly, LONG FieldIndex, CFieldStatOperations* statOperations, CShapefile** retval);
	void DissolveCore(long FieldIndex, VARIANT_BOOL SelectedOnly, CFieldStatOperations* statOperations, CShapefile** sf);
	void CalculateFieldStats(map<int, vector<int>*>& indicesMap, CFieldStatOperations* operations, CShapefile* output);
	void InsertShapesVector(CShapefile* sf, vector<CShape* >& vShapes, CShapefile* sfSubject, long subjectId, std::map<long, long>* fieldMapSubject = NULL,	CShapefile* sfClip = NULL, long clipId = -1, std::map<long, long>* fieldMapClip = NULL);
	void GetRelatedShapeCore(CShape* referenceShape, long referenceIndex, tkSpatialRelation relation, VARIANT* resultArray, VARIANT_BOOL* retval);
	
public:
	// accessing shapes
	bool ShapeAvailable(int shapeIndex, VARIANT_BOOL selectedOnly);
	HRESULT GetValidatedShape(int shapeIndex, CShape** retVal);
	void ReadGeosGeometries(VARIANT_BOOL selectedOnly);
	GEOSGeometry* GetGeosGeometry(int shapeIndex);

	// geoprocessing
	Coloring::ColorGraph* GeneratePolygonColors();
	bool ReprojectCore(CGeoProjection* newProjection, LONG* reprojectedCount, CShapefile** retVal, bool reprojectInPlace);

	// errors
	void ErrorMessage(long ErrorCode);
	void ErrorMessage(long ErrorCode, ICallback* cBack);

	// underlying data
	std::vector<ShapeData*>* get_ShapeVector();
	IShapeWrapper* get_ShapeWrapper(int ShapeIndex);
	IShapeData* get_ShapeData(int ShapeIndex);
	FILE* get_File(){ return _shpfile; }
	
	// serialization
	bool DeserializeCore(VARIANT_BOOL LoadSelection, CPLXMLNode* node);
	CPLXMLNode* SerializeCore(VARIANT_BOOL SaveSelection, CString ElementName, bool serializeCategories);

	// charts
	void SetChartsPositions(tkLabelPositioning Method);
	void ClearChartFrames();
	
	// selection
	bool SelectShapesCore(Extent& extents, double Tolerance, SelectMode SelectMode, std::vector<long>& selectResult, bool renderedOnly);
	bool QuickExtentsCore(long ShapeIndex, Extent& result);
	bool QuickExtentsCore(long ShapeIndex, double* xMin, double* yMin, double* xMax, double* yMax);

	// editing
	bool OpenCore(CStringW tmp_shpfileName, ICallback* cBack);
	HRESULT CreateNewCore(BSTR ShapefileName, ShpfileType ShapefileType, bool applyRandomOptions, VARIANT_BOOL *retval);
	void RegisterNewShape(CShape* Shape, long ShapeIndex);
	void ReregisterShape(int shapeIndex);

	// validation
	void SetValidationInfo(CShapeValidationInfo* info, tkShapeValidationType validationType);
	void CreateValidationList(bool selectedOnly);
	void ClearValidationList();
	void SetValidatedShape(int shapeIndex, ShapeValidationStatus status, CShape* shape = NULL);
	bool ValidateInput(CShapefile* isf, CString methodName, CString parameterName, VARIANT_BOOL selectedOnly, CString className = "Shapefile");
	CShapeValidationInfo* ValidateInputCore(CShapefile* isf, CString methodName, CString parameterName,	VARIANT_BOOL selectedOnly, tkShapeValidationMode validationMode, CString className = "Shapefile", bool reportOnly = false);
	CShapeValidationInfo* ValidateOutput(CShapefile** isf, CString methodName, CString className = "Shapefile", bool abortIfEmpty = true);
	bool ValidateOutput(CShapefile* sf, CString methodName, CString className= "Shapefile", bool abortIfEmpty = true);
	void RestoreShapeRecordsMapping();
	
	// drawing 
	void MarkUndrawn();
	void GetLabelString(long fieldIndex, long shapeIndex, BSTR* text, CString floatNumberFormat);

};
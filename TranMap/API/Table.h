
#pragma once

#include "GlobalVars.h"
#include "ErrorCodes.h"
#include "Structures.h"
#include "TableRow.h"
#include "dbf.h"
#include "Expression.h"
#include "CallbackHelper.h"
#include "Field.h"
#include <set>
#include <vector>
using namespace std;
class TableRow;
class CField;
struct RecordWrapper;
struct CategoriesData;

//#include "_ITableEvents_CP.H 
class  CTableClass 
{
public:
	CTableClass()
	{
		_key = SysAllocString(L"");
		_globalCallback = NULL;
		_lastErrorCode = tkNO_ERROR;
		_isEditingTable = FALSE;
		_dbfHandle = NULL;
		m_maxRowId = -1;
		// lu gReferenceCounter.AddRef(tkInterface::idTable);
	}
	~CTableClass()
	{
		VARIANT_BOOL vbretval;
		this->Close(&vbretval);

		for (int i = 0; i < (int)_tempFiles.size(); i++ )
		{
			try
			{
				CString * a = _tempFiles[i];
				_unlink(a->GetBuffer());
				delete a;
			}
			catch(...)
			{
				ASSERT(FALSE);
			}
		}
		_tempFiles.clear();

		::SysFreeString(_key);

		if( _globalCallback )
			delete _globalCallback;
		_globalCallback = NULL;
		//lu gReferenceCounter.Release(tkInterface::idTable);
	}
// CTableClass*
public:
	void EditDeleteRow(/*[in]*/ long RowIndex, /*[out,retval]*/ VARIANT_BOOL * retval);
	void StopEditingTable(/*[in, optional, defaultvalue(TRUE)]*/ VARIANT_BOOL ApplyChanges, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void StartEditingTable(/*[in, optional]*/  ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void EditCellValue(/*[in]*/ long FieldIndex, /*[in]*/ long RowIndex, /*[in]*/ VARIANT newVal, /*[out, retval]*/ VARIANT_BOOL * retval);
	void EditInsertRow(/*[in,out]*/ long * RowIndex, /*[out, retval]*/ VARIANT_BOOL * retval);
	void EditDeleteField(/*[in]*/ long FieldIndex, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void EditReplaceField(/*[in]*/ long FieldIndex, /*[in]*/ CField * newField, /*[in, optional]*/ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void EditInsertField(/*[in]*/ CField * Field, /*[in, out]*/ long * FieldIndex, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void EditClear(/*[out, retval]*/ VARIANT_BOOL * retval);
	void Close(/*[out, retval]*/ VARIANT_BOOL * retval);
	void SaveAs(/*[in]*/ BSTR dbfFilename, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void CreateNew(/*[in]*/ BSTR dbfFilename, /*[out, retval]*/ VARIANT_BOOL * retval);
	void Open(/*[in]*/ BSTR dbfFilename, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_CdlgFilter(/*[out, retval]*/ BSTR *pVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_EditingTable(/*[out, retval]*/ VARIANT_BOOL *pVal);
	void get_CellValue(/*[in]*/ long FieldIndex, /*[in]*/ long RowIndex, /*[out, retval]*/ VARIANT *pVal);
	void get_Field(/*[in]*/ long FieldIndex, /*[out, retval]*/ CField * *pVal);
	void get_NumFields(/*[out, retval]*/ long *pVal);
	void get_NumRows(/*[out, retval]*/ long *pVal);
	void get_MinValue(long FieldIndex, VARIANT* retval);
	void get_MaxValue(long FieldIndex, VARIANT* retval);
	void get_MeanValue(long FieldIndex, double* retVal);
	void get_StandardDeviation(long FieldIndex, double* retVal);
	void ParseExpression(BSTR Expression, BSTR* ErrorString, VARIANT_BOOL* retVal);
	void Query(BSTR Expression, VARIANT* Result, BSTR* ErrorString, VARIANT_BOOL* retval);

	void Save(/*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);
	void get_FieldIndexByName(BSTR FieldName, long* retval);
	void TestExpression(BSTR Expression, tkValueType ReturnType, BSTR* ErrorString, VARIANT_BOOL* retVal);
	void Calculate(BSTR Expression, LONG RowIndex, VARIANT* Result, BSTR* ErrorString, VARIANT_BOOL* retVal);
	void EditAddField(BSTR name, FieldType type, int precision, int width, long* fieldIndex);

	void StopJoin(int joinIndex, VARIANT_BOOL* retVal);
	void StopAllJoins();

	void get_IsJoined(VARIANT_BOOL* retVal);
	void get_JoinCount(int* retVal);

	void get_JoinFilename(int joinIndex, BSTR* retVal);
	void get_JoinFromField(int joinIndex, BSTR* retVal);
	void get_JoinToField(int joinIndex, BSTR* retVal);

	void get_FieldIsJoined(int FieldIndex, VARIANT_BOOL* retVal);
	void get_FieldJoinIndex(int FieldIndex, int* retVal);

	void Serialize(BSTR* retVal);
	void Deserialize(BSTR newVal);

	void TryJoin(CTableClass* table2, BSTR fieldTo, BSTR fieldFrom, int* rowCount, int* joinRowCount, VARIANT_BOOL* retVal);

	void Join(CTableClass* table2, BSTR fieldTo, BSTR fieldFrom, VARIANT_BOOL* retVal);
	void Join2(CTableClass* table2, BSTR fieldTo, BSTR fieldFrom, BSTR filenameToReopen, BSTR joinOptions, VARIANT_BOOL* retVal);
	void Join3(CTableClass* table2, BSTR fieldTo, BSTR fieldFrom, BSTR filenameToReopen, BSTR joinOptions, SAFEARRAY* filedList, VARIANT_BOOL* retVal);

	void Dump(/*[in]*/ BSTR dbfFilename, /*[in, optional]*/ ICallback * cBack, /*[out, retval]*/ VARIANT_BOOL * retval);

private:
	struct JoinInfo
	{
		CStringW filename;
		CString fieldFrom;
		CString fieldTo;
		CString options;
		int joinId;
		CString fields; // comma separated list
	};

	struct FieldMapping
	{
		int srcIndex;
		int destIndex;
	};
	
private:
	DBFInfo * _dbfHandle;	// underlying data structure
	std::vector<FieldWrapper*> _fields;
	std::vector<RecordWrapper> _rows;
	std::deque<CString *> _tempFiles;
	BSTR _key;
	long _lastErrorCode;
	ICallback * _globalCallback;
	bool _isEditingTable;
	CStringW _filename;
	std::vector<JoinInfo*> _joins;
	int _lastJoinId;

public:
	bool m_needToSaveAsNewFile;
	int m_maxRowId;	// maximum value in the MWShapeId field

private:
	bool SaveToFile(const CStringW& dbfFilename, bool updateFileInPlace, ICallback* cBack);
	void LoadDefault_fields();
	void LoadDefault_rows();
	long RowCount();
	long FieldCount();
	bool ReadRecord(long RowIndex);
	bool WriteRecord(DBFInfo* dbfHandle, long fromRowIndex, long toRowIndex);
	void ClearRow(long rowIndex);
	FieldType GetFieldType(long fieldIndex);
	long GetFieldPrecision(long fieldIndex);
	inline void ErrorMessage(long ErrorCode);
	std::vector<CString>* get_FieldNames();

public:	
	bool DeserializeCore(CPLXMLNode* node);
	CPLXMLNode* SerializeCore(CString ElementName);
	void CloseUnderlyingFile(); 
	TableRow* CloneTableRow(int rowIndex);
	bool InsertTableRow(TableRow* row, long rowIndex);
	TableRow* SwapTableRow(TableRow* row, long rowIndex);
	bool GetUids(long fieldIndex, std::map<long, long>& resutls);
	bool UpdateTableRow(TableRow* newRow, long rowIndex);

	void ParseExpressionCore(BSTR Expression, tkValueType returnType, CString& ErrorString, VARIANT_BOOL* retVal);

	std::vector<CategoriesData>* GenerateCategories(long FieldIndex, tkClassificationType ClassificationType, long numClasses);
	std::vector<CategoriesData>* GenerateCategories(long FieldIndex, tkClassificationType ClassificationType, long numClasses,
		CComVariant minValue, CComVariant maxValue);
	void AnalyzeExpressions(std::vector<CString>& expressions, std::vector<int>& results);
	bool QueryCore(CString Expression, std::vector<long>& indices, CString& ErrorString);
	bool CalculateCore(CString Expression, std::vector<CString>& results, CString& ErrorString, CString floatFormat, int rowIndex = -1);

	bool get_FieldValuesDouble(int FieldIndex, std::vector<double>& values);
	bool get_FieldValuesInteger(int FieldIndex, std::vector<int>& values);
	bool get_FieldValuesString(int FieldIndex, std::vector<CString>& values);

	bool set_IndexValue(int rowIndex);

	bool MakeUniqueFieldNames();

	bool CheckJoinInput(CTableClass* table2, CString fieldTo, CString fieldFrom, long& index1, long& index2);

	bool JoinFields(CTableClass* table2, std::vector<FieldMapping*>& mapping, std::set<CString>& fieldList);
	bool JoinInternal(CTableClass* table2, CString fieldTo, CString fieldFrom, CStringW filenameToReopen, CString options, set<CString>& fieldList);
	void RemoveJoinedFields();
	bool HasFieldChanges();
	void MarkFieldsAsUnchanged();
};

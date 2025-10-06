#include "stdafx.h"
#include "Shapefile.h"
#include "Labels.h"
#include "Charts.h"
#include "Table.h"
#include "Shape.h"
#include "ShapeHelper.h"
#include "ShapefileHelper.h"
#include "LabelsHelper.h"

#pragma region Labels

// ************************************************************
//			GetLabelValue
// ************************************************************
void CShapefile::GetLabelString(long fieldIndex, long shapeIndex, BSTR* text, CString floatNumberFormat)
{
	if (fieldIndex != -1)
	{
		CComVariant val;
		get_CellValue(fieldIndex, shapeIndex, &val);
		*text = Utility::Variant2BSTR(&val, floatNumberFormat);
	}
	else
	{
		*text = A2BSTR("");
	}
}

// ************************************************************
//			GenerateLabels
// ************************************************************
// FieldIndex == -1: labels without text will be generated; 
// Method == lpNone: labels with (0.0,0.0) coordinates will be generated
void  CShapefile::GenerateLabels(long FieldIndex, tkLabelPositioning Method, VARIANT_BOOL LargestPartOnly, long* Count)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*Count = 0;
	
	long numFields;
	this->get_NumFields(&numFields);
	
	if( FieldIndex < -1 || FieldIndex >= numFields)
	{	
		ErrorMessage(tkINDEX_OUT_OF_BOUNDS); 
		return ;
	} 
	
	_labels->Clear();
	
	ShpfileType shpType = ShapefileHelper::GetShapeType2D(this);
	long numShapes = ShapefileHelper::GetNumShapes(this);
	
	tkLineLabelOrientation orientation;
	_labels->get_LineOrientation(&orientation);
	CString floatFormat = LabelsHelper::GetFloatNumberFormat(_labels);

	long percent = 0;

	for(int i = 0; i < numShapes; i++)
	{
		CallbackHelper::Progress(_globalCallback, i, numShapes, "Calculating label positions...", _key, percent);
		
		CComBSTR text;
		GetLabelString(FieldIndex, i, &text, floatFormat);
		
		CShape* shp = NULL;
		this->get_Shape(i, &shp);
		if (shp)
		{
			long numParts;
			shp->get_NumParts(&numParts);
			VARIANT_BOOL vb;
			double rotation = 0.0;

			if( numParts == 1)
			{
				ShapeHelper::AddLabelToShape(shp, _labels, text, Method, orientation);
				continue;
			}
			else if (numParts == 0)
			{
				if (shpType == SHP_POINT || shpType == SHP_MULTIPOINT)
				{
					ShapeHelper::AddLabelToShape(shp, _labels, text, Method, orientation);
					continue;
				}	
			}
			else
			{
				if (!LargestPartOnly) 
				{
					int partCount = 0;
					for (int j = 0; j < numParts; j++)
					{
						CShape* shpPart = NULL;
						shp->get_PartAsShape(j, &shpPart);
						if ( !shpPart ) continue;

						if (shpType == SHP_POLYGON)
						{
							shpPart->get_PartIsClockWise(0, &vb);		
							if(!vb) continue;		// holes of polygons must not be labeled
						}
					
						if (partCount == 0) 
						{
							ShapeHelper::AddLabelToShape(shpPart, _labels, text, Method, orientation);
							partCount++;
						}
						else		
						{
							double x = 0.0, y = 0.0;
							shpPart->get_LabelPosition(Method, x, y, rotation, orientation);
							_labels->AddPart(i, text, x, y, rotation);
						}
					}

					if (partCount > 0)
						continue;
				}
				else
				{
					// only the largest/longest part
					long maxPart = ShapeHelper::GetLargestPart(shp);
					if (maxPart >= 0)
					{
						CShape* shpPart = NULL;
						shp->get_PartAsShape(maxPart, &shpPart);
						if (shpPart)
						{
							ShapeHelper::AddLabelToShape(shpPart, _labels, text, Method, orientation);
							continue;
						}
					}
				}
			} 
		}
		((CLabels*)_labels)->AddEmptyLabel();
	}
	
	*Count = LabelsHelper::GetCount(_labels);
	_labels->put_Synchronized(VARIANT_TRUE);
	_labels->put_Positioning(Method);

	if (FieldIndex == -1)
	{
		(_labels)->ForceRecalculateExpression();
	}
	else
	{
		(_labels)->SaveSourceField(FieldIndex);	// save it for deserialization
	}

	CallbackHelper::ProgressCompleted(_globalCallback, _key);
	return ;
}

// ******************************************************************
//		get_Labels/putLabels()
// ******************************************************************
//  Returns reference to Labels class
void  CShapefile::get_Labels(CLabels** pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _labels;
	return ;
}
void  CShapefile::put_Labels(CLabels* newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!newVal)
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE); 
	}
	else
	{
		//ComHelper::SetRef(newVal, (IDispatch**)&_labels, false);
		this->put_ReferenceToLabels(false);
	}
	return ;
}

/***********************************************************************/
/*		put_ReferenceToLabels
/***********************************************************************/
void CShapefile::put_ReferenceToLabels(bool bNullReference)
{
	if (_labels == NULL) return;
	CLabels* coLabels = static_cast<CLabels*>(_labels);
	if (!bNullReference)
		coLabels->put_ParentShapefile(this);
	else
		coLabels->put_ParentShapefile(NULL);
};
#pragma endregion

#pragma region Charts
// *******************************************************************
//		get/put_Charts()
// *******************************************************************
void  CShapefile::get_Charts (CCharts** pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*pVal = _charts;
	return ;
}
void  CShapefile::put_Charts (CCharts* newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!newVal)
	{
		ErrorMessage(tkINVALID_PARAMETER_VALUE); 
	}
	else
	{
		if (newVal != _charts)
		{
			if (_charts != NULL) 
			{
				delete _charts;
				_charts = NULL;
			}
			_charts = newVal;
			delete _charts;
		}
	}
	return ;
}
/***********************************************************************/
/*		put_ReferenceToCharts
/***********************************************************************/
void CShapefile::put_ReferenceToCharts(bool bNullReference)
{
	if (!_charts) return;
	CCharts* coCharts = static_cast<CCharts*>(_charts);
	coCharts->put_ParentShapefile(!bNullReference ? this : NULL);
};

// ********************************************************************
//			SetChartsPositions
// ********************************************************************
void CShapefile::SetChartsPositions(tkLabelPositioning Method)
{
	USES_CONVERSION;
	double x,y;
	
	ShpfileType shpType;
	this->get_ShapefileType(&shpType);
	shpType = Utility::ShapeTypeConvert2D(shpType);
	
	long _numShapes;
	this->get_NumShapes(&_numShapes);
	
	// creating structures
	for (unsigned int i = 0; i < _shapeData.size(); i++)
	{
		if (_shapeData[i]->chart == NULL)
		{
			_shapeData[i]->chart = new CChartInfo();
		}
	}

	if (Method == lpNone)
	{
		// simply set 0 positions, the actual positions will be set externally; is needed for loading of previously saved labels
		((CCharts*)_charts)->_chartsExist = true;
		for (unsigned int i = 0; i < _shapeData.size(); i++)
		{
			_shapeData[i]->chart->x = 0.0;
			_shapeData[i]->chart->y = 0.0;
		}
		return;
	}
	else
	{
		long percent = 0;
		for(int i = 0; i < _numShapes; i++)
		{
			CallbackHelper::Progress(_globalCallback, i, _numShapes, "Calculating charts positions...", _key, percent);
			
			CShape* shp = NULL;
			this->get_Shape(i, &shp);
			if (shp == NULL) continue;
			
			long numParts;
			shp->get_NumParts(&numParts);
			VARIANT_BOOL vbretval;
			double rotation = 0.0;

			// ----------------------------------------------------
			//	Labeling the only part								
			// ----------------------------------------------------
			if( numParts == 1)
			{
				((CShape*)shp)->get_LabelPosition(Method, x, y, rotation, lorHorizontal);			
			}
			else if (numParts == 0)
			{
				if (shpType == SHP_POINT || shpType == SHP_MULTIPOINT)
				{
					shp->get_XY(0, &x, &y, &vbretval);
				}	
				else
				{
					// it's an invalid shape, we'll add a label but it is fictitious
					x = y = 0.0;
				}
			}
			else //if ( numParts > 1 )
			{
				long maxPart = -1; double maxValue = 0;
				for (int j = 0; j < numParts; j++)
				{
					CShape* shpPart = NULL;
					shp->get_PartAsShape(j, &shpPart);
					if (shpPart == NULL) continue;
					
					if (shpType == SHP_POLYGON)
					{
						// Holes of polygons must not be labeled
						VARIANT_BOOL vbretval;
						shpPart->get_PartIsClockWise(0, &vbretval);
						if(!vbretval) continue;
					}	
					
					// Seeking the larges part of shape
					double value = 0.0;
					if		(shpType == SHP_POLYGON)		shpPart->get_Area(&value);
					else if (shpType == SHP_POLYLINE)	shpPart->get_Length(&value);

					if ( value > maxValue )
					{
						maxValue = value;
						maxPart = j;
					}
					delete shpPart; shpPart = NULL;
				}

				CShape* shpPart = NULL;
				shp->get_PartAsShape(maxPart, &shpPart);
				if ( shpPart == NULL ) continue;
				
				((CShape*)shpPart)->get_LabelPosition(Method, x, y, rotation, lorHorizontal);
				delete shpPart; shpPart = NULL;
			} // numParts > 1
			
			_shapeData[i]->chart->x = x;
			_shapeData[i]->chart->y = y;
			
			delete shp; shp = NULL;
		}
		
		CallbackHelper::ProgressCompleted(_globalCallback, _key);
	}
	return;
}

// *************************************************************
//		ClearChartFrames()
// *************************************************************
void CShapefile::ClearChartFrames()
{
	for (unsigned int i = 0; i < _shapeData.size(); i++ ) 
	{
		CChartInfo* chart = _shapeData[i]->chart;
		if (chart)
		{
			if ( chart->frame != NULL )
			{
				delete chart->frame;
				chart->frame = NULL;
			}
		}
	}
}
#pragma endregion


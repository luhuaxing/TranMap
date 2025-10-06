/**************************************************************************************
 * File name: Measuring.h
 *
 * Project: MapWindow Open Source (MapWinGis ActiveX control) 
 * Description: Declaration of the CMeasuring
 *
 **************************************************************************************
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * (the "License"); you may not use this file except in compliance with 
 * the License. You may obtain a copy of the License at http://www.mozilla.org/mpl/ 
 * See the License for the specific language governing rights and limitations
 * under the License.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ************************************************************************************** 
 * Contributor(s): 
 * (Open source contributors should list themselves and their modifications here). */
 
#pragma once
#include "MeasuringBase.h"

class MeasuringBase;
// CMeasuring
class  CMeasuring
{
public:
	CMeasuring() {
		_measuring = new MeasuringBase();
		_lastErrorCode = tkNO_ERROR;
		_globalCallback = NULL;
		_key = SysAllocString(L"");
		_undoButton = usRightMouseButton;
	}

	~CMeasuring() {
		SysFreeString(_key);
		if(_measuring)
		delete _measuring;
	}

public:
	void get_LastErrorCode(/*[out, retval]*/ long *pVal);
	void get_ErrorMsg(/*[in]*/ long ErrorCode, /*[out, retval]*/ BSTR *pVal);
	void get_GlobalCallback(/*[out, retval]*/ ICallback * *pVal);
	void put_GlobalCallback(/*[in]*/ ICallback * newVal);
	void get_Key(/*[out, retval]*/ BSTR *pVal);
	void put_Key(/*[in]*/ BSTR newVal);
	void get_Length(double* retVal);
	void UndoPoint(VARIANT_BOOL* retVal);
	void get_PointCount(long* retVal);
	void get_PointXY(long pointIndex, double* x, double* y, VARIANT_BOOL* retVal);
	void get_AreaWithClosingVertex(double lastPointProjX, double lastPointProjY, double* retVal);
	void get_MeasuringType(tkMeasuringType* retVal);
	void put_MeasuringType(tkMeasuringType newVal);
	void FinishMeasuring();
	void get_Area(double* retVal);
	void get_IsStopped(VARIANT_BOOL* retVal);
	void Clear();
	void get_SegmentLength(int segmentIndex, double* retVal);
	void get_Persistent(VARIANT_BOOL* retVal);
	void put_Persistent(VARIANT_BOOL newVal);
	void get_ShowBearing(VARIANT_BOOL* retVal);
	void put_ShowBearing(VARIANT_BOOL newVal);
	void get_IsUsingEllipsoid(VARIANT_BOOL* retVal);
	void get_AreaUnits(tkAreaDisplayMode* retVal);
	void put_AreaUnits(tkAreaDisplayMode newVal);
	void get_IsEmpty(VARIANT_BOOL* pVal);
	void get_BearingType(tkBearingType* pVal);
	void put_BearingType(tkBearingType newVal);
	void get_ShowLength(VARIANT_BOOL* pVal);
	void put_ShowLength(VARIANT_BOOL newVal);
	void get_LengthUnits(tkLengthDisplayMode* pVal);
	void put_LengthUnits(tkLengthDisplayMode newVal);
	void get_AngleFormat(tkAngleFormat* pVal);
	void put_AngleFormat(tkAngleFormat newVal);
	void get_AnglePrecision(LONG* pVal);
	void put_AnglePrecision(LONG newVal);
	void get_AreaPrecision(LONG* pVal);
	void put_AreaPrecision(LONG newVal);
	void get_LengthPrecision(LONG* pVal);
	void put_LengthPrecision(LONG newVal);
	void get_PointsVisible(VARIANT_BOOL* pVal);
	void put_PointsVisible(VARIANT_BOOL newVal);
	void get_LineColor(OLE_COLOR* pVal);
	void put_LineColor(OLE_COLOR newVal);
	void get_FillColor(OLE_COLOR* pVal);
	void put_FillColor(OLE_COLOR newVal);
	void get_FillTransparency(BYTE* pVal);
	void put_FillTransparency(BYTE newVal);
	void get_LineWidth(FLOAT* pVal);
	void put_LineWidth(FLOAT newVal);
	void get_LineStyle(tkDashStyle* pVal);
	void put_LineStyle(tkDashStyle newVal);
	void get_PointLabelsVisible(VARIANT_BOOL* pVal);
	void put_PointLabelsVisible(VARIANT_BOOL newVal);
	void get_ShowTotalLength(VARIANT_BOOL* pVal);
	void put_ShowTotalLength(VARIANT_BOOL newVal);
	void get_UndoButton(tkUndoShortcut* pVal);
	void put_UndoButton(tkUndoShortcut newVal);
	void Serialize(BSTR* retVal);
	void Deserialize(BSTR state, VARIANT_BOOL* retVal);

private:
	long _lastErrorCode;
	ICallback * _globalCallback;
	BSTR _key;
	MeasuringBase* _measuring;
	tkUndoShortcut _undoButton;

public:
	MeasuringBase* GetBase() { return _measuring; }
	
};

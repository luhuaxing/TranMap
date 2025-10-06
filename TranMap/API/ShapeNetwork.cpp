#include "stdafx.h"
#include "ShapeNetwork.h"
#include "PointClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CShapeNetwork

bool dPOINT_EQUAL( void * one, void * two, void * exParam )
{	double Tolerance = *((double*)exParam);
	dPOINT * d1 = (dPOINT*)one;
	dPOINT * d2 = (dPOINT*)two;

	if( fabs( d1->x - d2->x ) <= Tolerance )
	{	if( fabs( d1->y - d2->y ) <= Tolerance )
			return true;
	}
	return false;
}

void dPRINT_DATA( ofstream & out, void * data )
{	out<<((dPOINT*)data)->x<<" "<<((dPOINT*)data)->y;
}

long CShapeNetwork::UpEnd( edge * e, dPOINT * downpoint, double tolerance )
{	
	if( dPOINT_EQUAL( e->one->data, (void *)downpoint, (void*)&tolerance ) )
		return e->twoIndex;
	else
		return e->oneIndex;
}

long CShapeNetwork::DownEnd( edge * e, dPOINT * downpoint, double tolerance )
{	
	if( dPOINT_EQUAL( e->one->data, (void *)downpoint, (void*)&tolerance ) )
		return e->oneIndex;
	else
		return e->twoIndex;
}

void CShapeNetwork::CopyShape( bool reversePoints, CShape * oldshape, CShape * newshape )
{	
	ShpfileType shptype;
	VARIANT_BOOL vbretval;
	oldshape->get_ShapeType(&shptype);
	newshape->Create(shptype,&vbretval);
	long pIndex = 0;
	newshape->InsertPart(0,&pIndex,&vbretval);
	
	long numPoints = 0;
	oldshape->get_NumPoints(&numPoints);
	
	CPointClass * oldpnt = NULL;
	CPointClass * newpnt = NULL;
	double x, y, z;
	long pos = 0;
	VARIANT_BOOL retval = FALSE;
	for( int i = 0; i < numPoints; i++ )
	{	
		pos = i;
		oldshape->get_Point(pos,&oldpnt);
		oldpnt->get_X(&x);
		oldpnt->get_Y(&y);
		oldpnt->get_Z(&z);
		delete oldpnt;
		oldpnt = NULL;
		newpnt = new CPointClass();
		newpnt->put_X(x);
		newpnt->put_Y(y);
		newpnt->put_Z(z);
		if( reversePoints == true )
			pos = 0;
		newshape->InsertPoint(newpnt,&pos,&retval);		
	}
}

void CShapeNetwork::CopyField( CField * oldfield, CField * newfield )
{	
	CComBSTR name;
	long precision = 0,width = 0;
	FieldType ftype;
	oldfield->get_Name(&name);
	oldfield->get_Precision(&precision);
	oldfield->get_Type(&ftype);
	oldfield->get_Width(&width);
	newfield->put_Name(name);
	newfield->put_Precision(precision);
	newfield->put_Type(ftype);
	newfield->put_Width(width);
}

void CShapeNetwork::recPrintShpNetwork(shpNetNode * allnodes, long index, ofstream & out)
{	out<<index<<" : ";
	int i = 0;
	for( i = 0; i < (int)allnodes[index].up.size(); i++ )
		out<<allnodes[index].up[i]<<" ";
	out<<endl;
	
	for( i = 0; i < (int)allnodes[index].up.size(); i++ )
		recPrintShpNetwork( allnodes, allnodes[index].up[i], out );
}

void CShapeNetwork::PrintShpNetwork(shpNetNode * allnodes, long outlet, const char * filename)
{	
	ofstream out(filename);
	recPrintShpNetwork( allnodes, outlet, out );
	out.close();
}

void  CShapeNetwork::Build(CShapefile *Shapefile, long ShapeIndex, long FinalPointIndex, double Tolerance, AmbiguityResolution ar, ICallback *cBack, long *retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	//Verify the Shapefile
	if( Shapefile == NULL )
	{	*retval = 0;
		_lastErrorCode = tkUNEXPECTED_NULL_PARAMETER;
		if( cBack != NULL )
			cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
		else if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));
		return ;
	}

	//Verify the Shapefile Type
	ShpfileType shpfiletype;
	Shapefile->get_ShapefileType(&shpfiletype);
	if( shpfiletype != SHP_POLYLINE && shpfiletype != SHP_POLYLINEZ && shpfiletype != SHP_POLYLINEM )
	{	*retval = 0;
		_lastErrorCode = tkINCOMPATIBLE_SHAPEFILE_TYPE;
		if( cBack != NULL )
			cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
		else if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));
		return ;
	}

	long numShapes = 0;
	Shapefile->get_NumShapes(&numShapes);

	//Verify the ShapeIndex
	if( ShapeIndex < 0 || ShapeIndex >= numShapes )
	{	*retval = 0;
		_lastErrorCode = tkINDEX_OUT_OF_BOUNDS;
		if( cBack != NULL )
			cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
		else if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));
		return ;
	}

	//Verify the PointIndex
	CShape * outshp = NULL;
	long numPoints = 0;
	Shapefile->get_Shape(ShapeIndex,&outshp);	
	outshp->get_ShapeType(&shpfiletype);
	outshp->get_NumPoints(&numPoints);
	delete outshp;
	outshp = NULL;

	if( shpfiletype != SHP_POLYLINE && shpfiletype != SHP_POLYLINEZ && shpfiletype != SHP_POLYLINEM )
	{	*retval = 0;
		_lastErrorCode = tkINCOMPATIBLE_SHAPE_TYPE;
		if( cBack != NULL )
			cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
		else if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));
		return ;
	}
	if( FinalPointIndex != 0 && FinalPointIndex != numPoints - 1 )
	{	*retval = 0;
		_lastErrorCode = tkINVALID_FINAL_POINT_INDEX;
		if( cBack != NULL )
			cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
		else if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));
		return ;
	}

	graph undir_graph;	

	long percent = 0, newpercent = 0;
	double total = numShapes;

	ICallback* callback = cBack ? cBack : _globalCallback;

	double x1, y1, x2, y2;
	//Build the Undirected Graph
	for( int i = 0; i < numShapes; i++ )
	{	CShape * shape = NULL;
		Shapefile->get_Shape(i,&shape);
		shape->get_ShapeType(&shpfiletype);

		if( shpfiletype == SHP_POLYLINE || shpfiletype == SHP_POLYLINEZ || shpfiletype == SHP_POLYLINEM )
		{	CPointClass * pnt1 = NULL, * pnt2 = NULL;

			shape->get_NumPoints(&numPoints);
			shape->get_Point(0,&pnt1);
			shape->get_Point(numPoints-1,&pnt2);

			pnt1->get_X(&x1);
			pnt1->get_Y(&y1);

			pnt2->get_X(&x2);
			pnt2->get_Y(&y2);

			delete pnt1;
			delete pnt2;
			
			dPOINT * p1 = new dPOINT;
			dPOINT * p2 = new dPOINT;
			p1->x = x1;
			p1->y = y1;
			p2->x = x2;
			p2->y = y2;

			double distance = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );

			if( distance > Tolerance )
			{
				graphnode * gnOne = new graphnode();
				graphnode * gnTwo = new graphnode();		
				gnOne->data = (void*)p1;
				gnTwo->data = (void*)p2;

				edge * newEdge = new edge();
				newEdge->one = gnOne;
				newEdge->two = gnTwo;
				
				undir_graph.Insert(newEdge,(void*)&Tolerance,dPOINT_EQUAL);
			}
			else
			{	
				//Add the shape as a blank to the graph
				undir_graph.InsertBlank();

				//Verify that this isn't the outlet shape
				if( i == ShapeIndex )
				{	
					delete shape;
					shape = NULL;	
					*retval = 0;			
					_lastErrorCode = tkTOLERANCE_TOO_LARGE;
					if( cBack != NULL )
						cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
					else if( _globalCallback != NULL )
						_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
					return ;			
				}
			}
		}
		else
		{	
			//Verify that this isn't the outlet shape
			if( i == ShapeIndex )
			{
				delete shape;
				shape = NULL;
				*retval = 0;			
				_lastErrorCode = tkINCOMPATIBLE_SHAPE_TYPE;
				if( cBack != NULL )
					cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
				else if( _globalCallback != NULL )
					_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
				return ;
			}
			else
				undir_graph.InsertBlank();
		}

		delete shape;
		shape = NULL;
	
		CallbackHelper::Progress(callback, i, total, "Building ShapeNetwork", _key, percent);
	}

	//undir_graph.Save("graph.txt",dPRINT_DATA);
	
	//Get the info on the outlet node
	double length = 0;
	double fx, fy;
	dPOINT dpnt;
	CShape * shape = NULL;
	CPointClass * pnt = NULL;
	CUtils * gen = new CUtils();
	for( int ls = 0; ls < numShapes; ls++ )
	{	if( undir_graph.edges[ls] != NULL )
		{	Shapefile->get_Shape(ls,&shape);		
			gen->get_Length(shape,&length);

			if( length <= Tolerance )
			{	
				delete gen;
				gen = NULL;
				delete shape;
				shape = NULL;
				
				*retval = 0;			
				_lastErrorCode = tkTOLERANCE_TOO_LARGE;
				if( cBack != NULL )
					cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
				else if( _globalCallback != NULL )
					_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
				return ;
			}

			undir_graph.edges[ls]->length = length;
			if( ShapeIndex == ls )
			{	shape->get_Point(FinalPointIndex,&pnt);	
				pnt->get_X(&fx);
				pnt->get_Y(&fy);
				dpnt.x = fx;
				dpnt.y = fy;	
				delete pnt;
				pnt = NULL;
			}

			delete shape;
			shape = NULL;	
		}			
	}

	//convert the undirected graph into a directed acyclic graph	
	shpNetNode * edgeNetwork = new shpNetNode[numShapes];

	//Find the up end of the outlet	
	long up = UpEnd( undir_graph.edges[ShapeIndex], &dpnt, Tolerance );
	long down = DownEnd( undir_graph.edges[ShapeIndex], &dpnt, Tolerance );		
	edgeNetwork[ShapeIndex].downIndex = down;
	edgeNetwork[ShapeIndex].upIndex = up;
	edgeNetwork[ShapeIndex].distanceToOutlet = 0;

	std::deque<long> tmp_ambigShapeIndex;
	
	heap minheap;
	heapnode hn;
	long parentIndex;
	double distance = 0;
	
	minheap.insert(ShapeIndex,0);
 	while( minheap.size() > 0 )
	{	
		hn = minheap.top();
		minheap.pop();
		parentIndex = hn.index;

		if( edgeNetwork[parentIndex].used == false )
		{
			edge * e = undir_graph.edges[parentIndex];

			_networkSize++;
			edgeNetwork[parentIndex].used = true;
			edgeNetwork[parentIndex].length = e->length;		

			//Set up the parent
			if( edgeNetwork[parentIndex].pbIndex.size() > 0 )
			{	
				long parentparent = edgeNetwork[parentIndex].pbIndex[0];
				edgeNetwork[parentIndex].parentIndex = parentparent;

				if( parentparent >= 0 )
					edgeNetwork[parentparent].up.push_back(parentIndex);
				
				//Mark an ambiguous path
				if( edgeNetwork[parentIndex].pbIndex.size() > 1 )
					tmp_ambigShapeIndex.push_back(parentIndex);				
			}

			up = edgeNetwork[parentIndex].upIndex;
			down = edgeNetwork[parentIndex].downIndex;			
			
			distance = edgeNetwork[parentIndex].distanceToOutlet + e->length;				
			graphnode * gn = undir_graph.graphnodes[up];

			for( int i = 0; i < (int)gn->edges.size(); i++ )
			{	
				long childIndex = gn->edges[i];

				if( edgeNetwork[childIndex].used == false )
				{
					if( edgeNetwork[childIndex].distanceToOutlet < 0 ||
						distance < edgeNetwork[childIndex].distanceToOutlet )
					{
						up = UpEnd( undir_graph.edges[childIndex],(dPOINT*)gn->data,Tolerance );
						down = DownEnd( undir_graph.edges[childIndex],(dPOINT*)gn->data,Tolerance );

						edgeNetwork[childIndex].pbIndex.clear();
						edgeNetwork[childIndex].upIndex = up;
						edgeNetwork[childIndex].downIndex = down;
						edgeNetwork[childIndex].pbIndex.push_back(parentIndex);
						edgeNetwork[childIndex].distanceToOutlet = distance;
						minheap.insert(childIndex,distance);
					}
					//Ambiguous Path
					else if( distance == edgeNetwork[gn->edges[i]].distanceToOutlet )
						edgeNetwork[childIndex].pbIndex.push_back(parentIndex);					
				}
			}
		}
	}
	
	//PrintShpNetwork(edgeNetwork,ShapeIndex,"Network.txt");

	//copy the network into a new shapefile
	if( _network != NULL )
		delete [] _network;
	_network = new shpNetNode[_networkSize];
	long * shapeMap = new long[numShapes];
	long shapepos = 0;
	VARIANT_BOOL vbretval = FALSE;
	
	CShape * newshape = NULL;
	CShape * oldshape = NULL;

	//_unlink("d:\\data\\netfile.shp");
	//_unlink("d:\\data\\netfile.shx");
	//_unlink("d:\\data\\netfile.dbf");

	if( _netshpfile != NULL )
		delete _netshpfile;
	_netshpfile = new CShapefile();
	Shapefile->get_ShapefileType(&shpfiletype);
	_netshpfile->CreateNew(m_globalSettings.emptyBstr,shpfiletype,&vbretval);

	//Copy all of the Fields
	long numFields = 0;
	Shapefile->get_NumFields(&numFields);
	CField * newfield = NULL;	
	CField * oldfield = NULL;
	long fieldpos =0;
	int f = 0;
	for( f = 0; f < numFields; f++ )
	{	
		newfield = new CField();
		Shapefile->get_Field(f,&oldfield);
		CopyField(oldfield,newfield);
		fieldpos = f;
		_netshpfile->EditInsertField(newfield,&fieldpos,cBack,&vbretval);
	}

	//Create two new fields
	CField * id = new CField();
	CField * did =new CField();
				
	CComBSTR bstrNetId("NET_ID");
	id->put_Name(bstrNetId);
	id->put_Precision(0);
	id->put_Type(INTEGER_FIELD);
	id->put_Width(5);

	CComBSTR bstrNetDid("NET_DID");
	did->put_Name(bstrNetDid);
	did->put_Precision(0);
	did->put_Type(INTEGER_FIELD);
	did->put_Width(5);

	fieldpos = 0;
	_netshpfile->EditInsertField(did,&fieldpos,cBack,&vbretval);
	_netshpfile->EditInsertField(id,&fieldpos,cBack,&vbretval);

	//Create the outlet shape
	newshape = new CShape();
	Shapefile->get_Shape(ShapeIndex,&oldshape);	

	dPOINT zeroPnt;
	CPointClass * izpnt = NULL;
	oldshape->get_Point(0,&izpnt);
	double izx,izy;
	izpnt->get_X(&izx);
	izpnt->get_Y(&izy);
	delete izpnt;
	izpnt = NULL;
	zeroPnt.x = izx;
	zeroPnt.y = izy;

	bool reversePoints = false;
	if( FinalPointIndex != 0 )
		reversePoints = true;
	else
		reversePoints = false;

	CopyShape(reversePoints,oldshape,newshape);	
	_netshpfile->EditInsertShape(newshape,&shapepos,&vbretval);
	delete oldshape;
	oldshape = NULL;
	delete newshape;
	newshape = NULL;

	VARIANT cID,cDID;
	VariantInit(&cID); 
	VariantInit(&cDID); 
	cID.vt = VT_I4;
	cDID.vt = VT_I4;

	cID.lVal = shapepos;
	cDID.lVal = -1;
	_netshpfile->EditCellValue(0,shapepos,cID,&vbretval);
	_netshpfile->EditCellValue(1,shapepos,cDID,&vbretval);

	
	VARIANT cVal;
	VariantInit(&cVal); 
	for( f = 0; f < numFields; f++ )
	{	Shapefile->get_CellValue(f,ShapeIndex,&cVal);
		//Adjust for the two new fields
		_netshpfile->EditCellValue(f + 2,shapepos,cVal,&vbretval);
	}
		
	shapeMap[ShapeIndex] = shapepos;	

	_network[shapepos].distanceToOutlet = 0;
	_network[shapepos].parentIndex = -1;
	_network[shapepos].length = edgeNetwork[ShapeIndex].length;
	
	std::deque<long> netPath;
	netPath.push_back(ShapeIndex);
	
	percent = 0, newpercent = 0;
	total = _networkSize;
	
	//add all other shapes by simulating a recursive search
	long shpcnt = 1;
	long insertedShapes = 1;
	while( netPath.size() > 0 )
	{			
		long parent = netPath[0];
		netPath.pop_front();
		
		shpcnt++;
		
		for( int i = 0; i < (int)edgeNetwork[parent].up.size(); i++ )
		{	
			shapepos = insertedShapes;

			long childIndex = edgeNetwork[parent].up[i];
			netPath.push_back(childIndex);			

			newshape = new CShape();
			Shapefile->get_Shape(childIndex,&oldshape);				
			
			oldshape->get_Point(0,&izpnt);
			izpnt->get_X(&izx);
			izpnt->get_Y(&izy);
			delete izpnt;
			izpnt = NULL;
			zeroPnt.x = izx;
			zeroPnt.y = izy;

			CShape * parentShape = NULL;
			Shapefile->get_Shape(parent,&parentShape);
			long pnumPts = 0;
			parentShape->get_NumPoints(&pnumPts);			
			CPointClass * parentPnt1 = NULL;
			CPointClass * parentPnt2 = NULL;
			parentShape->get_Point(0,&parentPnt1);
			parentShape->get_Point(pnumPts-1,&parentPnt2);
			double pp1x, pp1y, pp2x, pp2y;
			parentPnt1->get_X(&pp1x);
			parentPnt1->get_Y(&pp1y);
			parentPnt2->get_X(&pp2x);
			parentPnt2->get_Y(&pp2y);
			delete parentPnt1;
			parentPnt1 = NULL;
			delete parentPnt2;
			parentPnt2 = NULL;
			delete parentShape;
			parentShape = NULL;

			dPOINT dpp1;
			dpp1.x = pp1x;
			dpp1.y = pp1y;
			dPOINT dpp2;
			dpp2.x = pp2x;
			dpp2.y = pp2y;

			if( dPOINT_EQUAL((void*)&dpp1,(void*)&zeroPnt,(void*)&Tolerance) != FALSE ||
				dPOINT_EQUAL((void*)&dpp2,(void*)&zeroPnt,(void*)&Tolerance) != FALSE )
				reversePoints = false;
			else
				reversePoints = true;

			CopyShape(reversePoints,oldshape,newshape);				
			_netshpfile->EditInsertShape(newshape,&shapepos,&vbretval);
			delete oldshape;
			oldshape = NULL;
			delete newshape;
			newshape = NULL;

			cID.lVal = shapepos;
			cDID.lVal = shapeMap[parent];
			_netshpfile->EditCellValue(0,shapepos,cID,&vbretval);
			_netshpfile->EditCellValue(1,shapepos,cDID,&vbretval);

			for( f = 0; f < numFields; f++ )
			{	Shapefile->get_CellValue(f,childIndex,&cVal);
				//Adjust for the two new fields
				_netshpfile->EditCellValue(f + 2,shapepos,cVal,&vbretval);
			}
		
			distance = edgeNetwork[parent].distanceToOutlet + undir_graph.edges[parent]->length;							

			shapeMap[childIndex] = shapepos;
			_network[shapeMap[parent]].up.push_back(shapepos);
			_network[shapepos].distanceToOutlet = distance;
			_network[shapepos].parentIndex = shapeMap[parent];
			_network[shapepos].length = edgeNetwork[childIndex].length;

			insertedShapes++;			
		}

		CallbackHelper::Progress(callback, shpcnt, total, "ShpNetwork::Copying Shapefile", _key, percent);
	}
	_currentNode = 0;

	//ShapeMap the Ambiguous Shapes
	_ambigShapeIndex.clear();
	for( int ti = 0; ti < (int)tmp_ambigShapeIndex.size(); ti++ )
	{	long shpindex = tmp_ambigShapeIndex[ti];
	_ambigShapeIndex.push_back(shapeMap[shpindex]);
	}
		
	delete [] shapeMap;
	delete [] edgeNetwork;

	//_netshpfile->AddRef();
	*retval = _ambigShapeIndex.size() + 1; //ARA 02/03/06 Possible to have size() of 0 so needed to be incremented by 1 so it wasn't returning error code
	
	VariantClear(&cID); 
	VariantClear(&cDID); 
	VariantClear(&cVal); 

	return ;
}

void  CShapeNetwork::DeleteShape(long ShapeIndex, VARIANT_BOOL *retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if( IsAligned() == false )
	{	*retval = FALSE;
		_lastErrorCode = tkNOT_ALIGNED;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	if( ShapeIndex >= 0 && ShapeIndex < _networkSize )
	{	
		VARIANT_BOOL vbretval = FALSE;
		//Verify that the shapefile and dbf are in editing mode
		_netshpfile->get_EditingShapes(&vbretval);
		if( vbretval == FALSE )
		{	*retval = FALSE;
			_lastErrorCode = tkSHPFILE_NOT_IN_EDIT_MODE;
			if( _globalCallback != NULL )
				_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
			return ;
		}
		_netshpfile->get_EditingTable(&vbretval);
		if( vbretval == FALSE )
		{	*retval = FALSE;
			_lastErrorCode = tkDBF_NOT_IN_EDIT_MODE;
			if( _globalCallback != NULL )
				_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));						
			return ;
		}

		//Mark all nodes as not used
		for( int n = 0; n < _networkSize; n++ )
			_network[n].used = false;
		
		//Find the path along the nodes
		std::deque<long> netPath;
		netPath.push_back(ShapeIndex);
		long localNetSize = 0;
		while( netPath.size() > 0 )
		{			
			long parent = netPath[0];
			netPath.pop_front();

			localNetSize++;
			_network[parent].used = true;

			for( int i = 0; i < (int)_network[parent].up.size(); i++ )
			{	long index = _network[parent].up[i];
				netPath.push_back(index);
			}
		}

		if( _networkSize == localNetSize )
		{	Close(retval);
			*retval = TRUE;
			return ;
		}

		//Recopy the network
		long * shifts = new long[_networkSize];
		memset(shifts,0,sizeof(long)*_networkSize);

		long delcnt = 0;
		int m = 0;
		for( m = 0; m < _networkSize; m++ )
		{	if( _network[m].used == true )
			{	_netshpfile->EditDeleteShape(m - delcnt,&vbretval);					
				++delcnt;			

				for( int k = m + 1; k < _networkSize; k++ )
					shifts[k]++;				
			}			
		}

		//Change any indexes in the old network			
		for( m = 0; m < _networkSize; m++ )
		{	long parentIndex = _network[m].parentIndex;
			if( parentIndex >= 0 )
				_network[m].parentIndex -= shifts[parentIndex];
					
			int kp = 0;
			for( kp = 0; kp < (int)_network[m].up.size(); kp++ )
			{	long childIndex = _network[m].up[kp];
				if( _network[childIndex].used == true )
					_network[m].up.erase( _network[m].up.begin() + kp );				
			}

			for( kp = 0; kp < (int)_network[m].up.size(); kp++ )
				_network[m].up[kp] -= shifts[_network[m].up[kp]];
		}
		
		delete [] shifts;
		shifts = NULL;

		shpNetNode * newNetwork = new shpNetNode[_networkSize - localNetSize];				
		long indcnt = 0;
		for( m = 0; m < _networkSize; m++ )
		{	if( _network[m].used == false )			
				newNetwork[indcnt++] = _network[m];
		}
		
		delete [] _network;
		_network = newNetwork;
		_networkSize = _networkSize - localNetSize;

		//Reset the parents and id's in the dbf
		VARIANT cID, cDID;
		VariantInit(&cID); 
		VariantInit(&cDID); 
		cID.vt = VT_I4;
		cDID.vt = VT_I4;
		for( int fn = 0; fn < _networkSize; fn++ )
		{	cID.lVal = fn;
			cDID.lVal = _network[fn].parentIndex;
			_netshpfile->EditCellValue(0,fn,cID,&vbretval);
			_netshpfile->EditCellValue(1,fn,cDID,&vbretval);
		}		
		
		VariantClear(&cID); 
		VariantClear(&cDID); 
		
		*retval = TRUE;
	}
	else
	{	*retval = FALSE;
		_lastErrorCode = tkINDEX_OUT_OF_BOUNDS;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));						
		return ;			
	}
	

	return ;
}

void  CShapeNetwork::MoveUp(long UpIndex, VARIANT_BOOL *retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if( IsAligned() == false )
	{	*retval = FALSE;
		_lastErrorCode = tkNOT_ALIGNED;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	if( _currentNode >= 0 )
	{	long numUps = _network[_currentNode].up.size();
		if( UpIndex >= 0 && UpIndex < numUps )
		{	_currentNode = _network[_currentNode].up[UpIndex];
			*retval = TRUE;
		}
		else
		{	*retval = FALSE;
			_lastErrorCode = tkINDEX_OUT_OF_BOUNDS;
			if( _globalCallback != NULL )
				_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));									
			return ;
		}
	}
	else
	{	*retval = FALSE;
		_lastErrorCode = tkINVALID_NODE;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));									
		return ;
	}	

	return ;
}

void  CShapeNetwork::MoveDown(VARIANT_BOOL *retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if( IsAligned() == false )
	{	*retval = FALSE;
		_lastErrorCode = tkNOT_ALIGNED;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	if( _currentNode >= 0 )
	{	
		if( _network[_currentNode].parentIndex >= 0 )
		{	_currentNode = _network[_currentNode].parentIndex;
			*retval = TRUE;
		}
		else
		{	*retval = FALSE;
			_lastErrorCode = tkNODE_AT_OUTLET;
			if( _globalCallback != NULL )
				_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));									
			return ;
		}
	}
	else
	{	*retval = FALSE;
		_lastErrorCode = tkINVALID_NODE;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));									
		return ;
	}	

	return ;
}

void  CShapeNetwork::MoveTo(long ShapeIndex, VARIANT_BOOL *retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( IsAligned() == false )
	{	*retval = FALSE;
		_lastErrorCode = tkNOT_ALIGNED;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	if( ShapeIndex >= 0 && ShapeIndex < _networkSize )
	{	_currentNode = ShapeIndex;
		*retval = TRUE;
	}
	else
	{	*retval = FALSE;
		_lastErrorCode = tkINDEX_OUT_OF_BOUNDS;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	return ;
}

void  CShapeNetwork::MoveToOutlet(VARIANT_BOOL *retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if( IsAligned() == false )
	{	*retval = FALSE;
		_lastErrorCode = tkNOT_ALIGNED;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	if( _networkSize > 0 )
	{	_currentNode = 0;
		*retval = TRUE;
	}
	else
	{	*retval = FALSE;
		_lastErrorCode = tkINVALID_NODE;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	return ;
}

void  CShapeNetwork::get_Shapefile(CShapefile **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if( IsAligned() == false )
	{	*pVal = NULL;
		_lastErrorCode = tkNOT_ALIGNED;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	//if( _netshpfile != NULL )
	//	_netshpfile->AddRef();

	*pVal = _netshpfile;	
	return ;
}

void  CShapeNetwork::get_CurrentShape(CShape **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if( IsAligned() == false )
	{	*pVal = NULL;
		_lastErrorCode = tkNOT_ALIGNED;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	if( _currentNode >= 0 )
	{	if( _netshpfile != NULL )
			_netshpfile->get_Shape(_currentNode,pVal);
		else
		{	*pVal = NULL;
			_lastErrorCode = tkNO_NETWORK;
			if( _globalCallback != NULL )
				_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
			return ;
		}
	}
	else
	{	*pVal = NULL;
		_lastErrorCode = tkINVALID_NODE;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	return ;
}

void  CShapeNetwork::get_CurrentShapeIndex(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if( IsAligned() == false )
	{	*pVal = -1;
		_lastErrorCode = tkNOT_ALIGNED;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	if( _currentNode >= 0 )
		*pVal = _currentNode;
	else
	{	*pVal = -1;	
		_lastErrorCode = tkINVALID_NODE;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	return ;
}

void  CShapeNetwork::get_DistanceToOutlet(long PointIndex, double *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if( IsAligned() == false )
	{	*pVal = 0.0;
		_lastErrorCode = tkNOT_ALIGNED;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	if( _currentNode >= 0 )
	{	double distance = 0;
		
		CShape * shp = NULL;
		_netshpfile->get_Shape(_currentNode,&shp);
		long numPoints = 0;
		shp->get_NumPoints(&numPoints);

		if( PointIndex < 0 || PointIndex >= numPoints )
		{	*pVal = 0;
			_lastErrorCode = tkINDEX_OUT_OF_BOUNDS;
			if( _globalCallback != NULL )
				_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));						
			return ;
		}
		else
		{	distance = _network[_currentNode].distanceToOutlet;			
			CPointClass * pnt = NULL;
			double x1, y1, z1;
			double x2, y2, z2;

			shp->get_Point(0,&pnt);
			pnt->get_X(&x1);
			pnt->get_Y(&y1);
			pnt->get_Z(&z1);
			delete pnt;
			pnt = NULL;

			for( int i = 1; i < PointIndex; i++ )
			{	
				shp->get_Point(i,&pnt);
				pnt->get_X(&x2);
				pnt->get_Y(&y2);
				pnt->get_Z(&z2);
				delete pnt;
				pnt = NULL;

				distance += sqrt( pow( x2 - x1, 2 ) + pow( y2 - y1, 2 ) + pow( z2 - z1, 2 ) );

				x1 = x2;
				y1 = y2;
				z1 = z2;
			}
		}
		
		delete shp;
		shp = NULL;

		*pVal = distance;
	}
	else
	{	*pVal = 0.0;		
		_lastErrorCode = tkINVALID_NODE;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	return ;
}

void  CShapeNetwork::get_NumDirectUps(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( IsAligned() == false )
	{	*pVal = 0;
		_lastErrorCode = tkNOT_ALIGNED;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	if( _currentNode >= 0 )
		*pVal = _network[_currentNode].up.size();
	else
	{	*pVal = 0;
		_lastErrorCode = tkINVALID_NODE;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}	

	return ;
}

void  CShapeNetwork::get_NetworkSize(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if( IsAligned() == false )
	{	*pVal = 0;
		_lastErrorCode = tkNOT_ALIGNED;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	if( _currentNode >= 0 )
	{
		std::deque<long> netPath;
		netPath.push_back(_currentNode);

		long localNetSize = 0;
		while( netPath.size() > 0 )
		{			
			long parent = netPath[0];
			netPath.pop_front();
			localNetSize++;

			for( int i = 0; i < (int)_network[parent].up.size(); i++ )
			{	long index = _network[parent].up[i];
				netPath.push_back(index);
			}
		}
		*pVal = localNetSize;
	}
	else
	{	*pVal = 0;
		_lastErrorCode = tkINVALID_NODE;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	return ;
}

void  CShapeNetwork::get_AmbigShapeIndex(long Index, long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if( IsAligned() == false )
	{	*pVal = -1;
		_lastErrorCode = tkNOT_ALIGNED;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	if (Index >= 0 && Index < (long)_ambigShapeIndex.size())
		*pVal = _ambigShapeIndex[Index];
	else
	{	*pVal = -1;
		_lastErrorCode = tkINDEX_OUT_OF_BOUNDS;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	return ;
}

void  CShapeNetwork::get_LastErrorCode(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	*pVal = _lastErrorCode;
	_lastErrorCode = tkNO_ERROR;
	
	return ;
}

void  CShapeNetwork::get_ErrorMsg(long ErrorCode, BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	*pVal = A2BSTR(ErrorMsg(ErrorCode));

	return ;
}

void  CShapeNetwork::get_GlobalCallback(ICallback **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	*pVal = _globalCallback;
	//if( _globalCallback != NULL )
	//{	
	//	_globalCallback->AddRef();
	//}
	return ;
}

void  CShapeNetwork::put_GlobalCallback(ICallback *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_globalCallback = newVal;
	return ;
}

void  CShapeNetwork::get_Key(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;

	*pVal = OLE2BSTR(_key);

	return ;
}

void  CShapeNetwork::put_Key(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;

	::SysFreeString(_key);
	_key = OLE2BSTR(newVal);

	return ;
}

void  CShapeNetwork::get_ParentIndex(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if( IsAligned() == false )
	{	*pVal = -2;
		_lastErrorCode = tkNOT_ALIGNED;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	if( _currentNode >= 0 )
		*pVal = _network[_currentNode].parentIndex;	
	else
	{	*pVal = -2;
		_lastErrorCode = tkINVALID_NODE;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	return ;
}

void  CShapeNetwork::put_ParentIndex(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())	

	if( IsAligned() == false )
	{	_lastErrorCode = tkNOT_ALIGNED;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	if( _currentNode == 0 )
	{	_lastErrorCode = tkCANT_CHANGE_OUTLET_PARENT;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}
	else if(_currentNode > 0 )
	{	if( newVal >= 0 && newVal < _networkSize && newVal != _currentNode )
		{	
			//Verify that this shape can get to the outlet
			for( shpNetNode snn = _network[newVal]; snn.parentIndex > 0; snn = _network[snn.parentIndex] )
			{	if( snn.parentIndex == _currentNode )
				{	//There would create a loop
					_lastErrorCode = tkNET_LOOP;
					if( _globalCallback != NULL )
						_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
					return ;
				}
			}

			//Possibly flip the shape so that Point0 is down
			CShape * parentShape = NULL;
			_netshpfile->get_Shape(newVal,&parentShape);

			long numPoints = 0;
			parentShape->get_NumPoints(&numPoints);
			if( numPoints < 2 )
			{	_lastErrorCode = tkINVALID_SHP_FILE;
				if( _globalCallback != NULL )
					_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
				return ;
			}
			
			CPointClass * ppnt = NULL;
			parentShape->get_Point(numPoints-1,&ppnt);
			double ppx, ppy;
			ppnt->get_X(&ppx);
			ppnt->get_Y(&ppy);
			delete ppnt;
			ppnt = NULL;
			delete parentShape;
			parentShape = NULL;

			CShape * shp = NULL;
			_netshpfile->get_Shape(_currentNode,&shp);
			shp->get_NumPoints(&numPoints);
			if( numPoints < 2 )
			{	_lastErrorCode = tkINVALID_SHP_FILE;
				if( _globalCallback != NULL )
					_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
				return ;
			}

			CPointClass * pnt1 = NULL, * pnt2 = NULL;
			shp->get_Point(0,&pnt1);
			shp->get_Point(numPoints-1,&pnt2);
			double px1, px2, py1, py2;
			pnt1->get_X(&px1);
			pnt1->get_Y(&py1);
			pnt2->get_X(&px2);
			pnt2->get_Y(&py2);
			delete shp;
			shp = NULL;
			delete pnt1;
			pnt1 = NULL;
			delete pnt2;
			pnt2 = NULL;

			double distance1 = sqrt( fabs( pow( px1 - ppx, 2 ) + pow( py1 - ppy, 2 ) ) );
			double distance2 = sqrt( fabs( pow( px2 - ppx, 2 ) + pow( py2 - ppy, 2 ) ) );
				
			VARIANT_BOOL retval = FALSE;
			_netshpfile->get_EditingShapes(&retval);
			if( retval == FALSE )
			{	_lastErrorCode = tkSHPFILE_NOT_IN_EDIT_MODE;
				if( _globalCallback != NULL )
					_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
				return ;
			}

			_netshpfile->get_EditingTable(&retval);
			if( retval == FALSE )
			{	_lastErrorCode = tkDBF_NOT_IN_EDIT_MODE;
				if( _globalCallback != NULL )
					_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
				return ;
			}

			//Reverse the points
			if( distance2 < distance1 )
			{	_netshpfile->get_Shape(_currentNode,&shp);
				shp->get_NumPoints(&numPoints);			
				CPointClass * pnt = NULL;
				long pntcnt = numPoints;
				int i = 0;
				for( i = numPoints - 1; i >= 0; i-- )
				{	shp->get_Point(i,&pnt);
					long tmppntcnt = pntcnt;
					shp->InsertPoint(pnt,&tmppntcnt,&retval);
					delete pnt;
					pnt = NULL;
					pntcnt++;
				}
				for( i = 0; i < numPoints; i++ )
					shp->DeletePoint(0,&retval);
				//Rob Cairns 20-Dec-05
				delete shp;
			}

			retval = FALSE;
			VARIANT vnv;
			VariantInit(&vnv); 
			vnv.vt = VT_I4;
			vnv.lVal = newVal;
			_netshpfile->EditCellValue(1,_currentNode,vnv,&retval);

			_network[_currentNode].parentIndex = newVal;
			_network[newVal].up.push_back(_currentNode);

			//Push all of the nodes up's onto the parent
			for( int i = 0; i < (int)_network[_currentNode].up.size(); i++ )
			{	long childIndex = _network[_currentNode].up[i];
				_netshpfile->EditCellValue(1,childIndex,vnv,&retval);

				_network[childIndex].parentIndex = newVal;
				_network[newVal].up.push_back(childIndex);
			}
			_network[_currentNode].up.clear();
			VariantClear(&vnv); 
		}
		else
		{	if( newVal == _currentNode )
			{	_lastErrorCode = tkNET_LOOP;
				if( _globalCallback != NULL )
					_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
				return ;
			}
			else
			{	_lastErrorCode = tkINDEX_OUT_OF_BOUNDS;
				if( _globalCallback != NULL )
					_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
				return ;
			}
		}
	}
	else
	{	_lastErrorCode = tkINVALID_NODE;
		if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;	
	}

	return ;
}

void  CShapeNetwork::Open(CShapefile *sf, ICallback *cBack, VARIANT_BOOL *retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	ICallback* callback = cBack ? cBack : _globalCallback;

	if( sf == NULL )
	{	*retval = FALSE;
		_lastErrorCode = tkUNEXPECTED_NULL_PARAMETER;
		if( cBack != NULL )
			cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
		else if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}
	
	//Verify the number of shapes
	long numShapes = 0;
	sf->get_NumShapes(&numShapes);
	if( numShapes <= 0 )
	{	*retval = FALSE;
		_lastErrorCode = tkINVALID_SHP_FILE;
		if( cBack != NULL )
			cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
		else if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	//Verify the first two fields
	long numFields = 0;
	sf->get_NumFields(&numFields);
	if( numFields < 2 )
	{	*retval = FALSE;
		_lastErrorCode = tkMISSING_FIELD;
		if( cBack != NULL )
			cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
		else if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}	
	FieldType foneType;
	FieldType ftwoType;
	CField * fone = NULL;
	CField * ftwo = NULL;
	sf->get_Field(0,&fone);
	sf->get_Field(1,&ftwo);
	fone->get_Type(&foneType);
	ftwo->get_Type(&ftwoType);
	delete fone;
	delete ftwo;
	fone = NULL;
	ftwo = NULL;
	if( foneType != INTEGER_FIELD || ftwoType != INTEGER_FIELD )
	{	*retval = FALSE;
		_lastErrorCode = tkINVALID_FIELD;
		if( cBack != NULL )
			cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
		else if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;
	}

	Close(retval);
	
	_netshpfile = sf;
	//_netshpfile->AddRef();	
	
	_networkSize = numShapes;
	_currentNode = 0;
	_network = new shpNetNode[numShapes];
	ShpfileType shptype = SHP_NULLSHAPE;
	VARIANT cID, cDID;
	VariantInit(&cID); 
	VariantInit(&cDID); 
	long lval1 = 0, lval2 = 0;

	CUtils * gen = new CUtils();
	
	long percent = 0, newpercent = 0;
	double total = numShapes;

	double length = 0;
	int i = 0;
	for( i = 0; i < numShapes; i++ )
	{	
		CShape * shp = NULL;		
		_netshpfile->get_Shape(i,&shp);
		if( shp == NULL )
		{	
			delete _netshpfile;
			_netshpfile = NULL;
			*retval = FALSE;
			_lastErrorCode = tkINVALID_SHP_FILE;
			if( cBack != NULL )
				cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
			else if( _globalCallback != NULL )
				_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		
			VariantClear(&cID); 
			VariantClear(&cDID); 

			return ;
		}

		shp->get_ShapeType(&shptype);

		gen->get_Length(shp,&length);

		delete shp;
		shp = NULL;

		if( shptype != SHP_POLYLINE && shptype != SHP_POLYLINEZ && shptype != SHP_POLYLINEM )
		{	gen;
			Close(retval);
			*retval = FALSE;
			_lastErrorCode = tkINCOMPATIBLE_SHAPEFILE_TYPE;
			if( cBack != NULL )
				cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
			else if( _globalCallback != NULL )
				_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			

			VariantClear(&cID); 
			VariantClear(&cDID); 

			return ;
		}
		else
		{	_netshpfile->get_CellValue(0,i,&cID);
			_netshpfile->get_CellValue(1,i,&cDID);
			
			lVal(cID,lval1);

			//assert( lval1 == i && "The ID != ShapeIndex" );

			if( lval1 != i )
			{	gen;
				Close(retval);
				*retval = FALSE;
				_lastErrorCode = tkINVALID_FIELD_VALUE;
				if( cBack != NULL )
					cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
				else if( _globalCallback != NULL )
					_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
				
				VariantClear(&cID); 
				VariantClear(&cDID); 

				return ;
			}		

			lVal(cDID,lval2);		
			if( i == 0 )
			{
				//assert( lval2 == -1 && "The DID != -1" );

				if( lval2 != -1 )
				{	gen;
					Close(retval);
					*retval = FALSE;
					_lastErrorCode = tkINVALID_FIELD_VALUE;
					if( cBack != NULL )
						cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
					else if( _globalCallback != NULL )
						_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
				
					VariantClear(&cID); 
					VariantClear(&cDID); 

					return ;
				}	
			}
			else
			{	
				//assert( ( lval2 >= 0 && lval2 < numShapes ) && "The DID is out of range." );

				if( lval2 < 0 || lval2 >= numShapes )
				{	gen;
					Close(retval);
					*retval = FALSE;
					_lastErrorCode = tkINVALID_FIELD_VALUE;
					if( cBack != NULL )
						cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
					else if( _globalCallback != NULL )
						_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
					
					VariantClear(&cID); 
					VariantClear(&cDID); 

					return ;
				}	
			}
			
			_network[i].length = length;
			_network[i].used = false;
			if( lval2 != -1 )
				_network[lval2].up.push_back(i);
			_network[i].parentIndex = lval2;
		}
		
		CallbackHelper::Progress(callback, i, total, "ShpNetwork::Open", _key, percent);
	}
	gen;
	gen = NULL;
	
	//Verify that every end shape can get to the outlet
	std::deque<long> netEnds;
	for( i = 0; i < _networkSize; i++ )
	{	if( _network[i].up.size() == 0 )
			netEnds.push_back(i);
	}
	percent = 0, newpercent = 0;
	total = netEnds.size();
	for( i = 0; i < (int)netEnds.size(); i++ )
	{
		bool * used = new bool[_networkSize];
		memset(used,0,sizeof(bool)*_networkSize);

		for( shpNetNode snn = _network[i]; snn.parentIndex > 0; snn = _network[snn.parentIndex] )
		{	if( used[snn.parentIndex] == true )
			{
				delete [] used;

				//This would create a loop				
				Close(retval);
				*retval = FALSE;

				_lastErrorCode = tkNET_LOOP;
				if( cBack != NULL )
					cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
				else if( _globalCallback != NULL )
					_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
				
				VariantClear(&cID); 
				VariantClear(&cDID); 

				return ;				
			}
			used[snn.parentIndex] = true;
		}
		
		delete [] used;

		CallbackHelper::Progress(callback, i, total, "ShpNetwork::Verifying Integrity of Network", _key, percent);
	}

	//Compute the distance to outlet
	std::deque<long> travNet;
	travNet.push_back(0);
	_network[0].distanceToOutlet = 0;
	while( travNet.size() > 0 )
	{	long parent = travNet[0];
		travNet.pop_front();
		
		for( int i = 0; i < (int)_network[parent].up.size(); i++ )
		{	long index = _network[parent].up[i];
			_network[index].distanceToOutlet = _network[parent].distanceToOutlet + _network[parent].length;
			travNet.push_back(index);
		}		
	}

	*retval = TRUE;
	VariantClear(&cID); 
	VariantClear(&cDID); 
	return ;
}

void  CShapeNetwork::Close(VARIANT_BOOL *retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( _netshpfile != NULL )
		delete _netshpfile;
	_netshpfile = NULL;
	if( _network != NULL )
		delete [] _network;
	_network = NULL;
	_currentNode = -1;
	_networkSize = 0;

	return ;
}

bool CShapeNetwork::IsAligned()
{	if( _netshpfile == NULL )
	{	if( _currentNode == -1 )
			return true;
		else
			return false;
	}
	else
	{	long numShapes = 0;
		_netshpfile->get_NumShapes(&numShapes);
		if( numShapes == _networkSize )
			return true;
		else
			return false;
	}
}

long roundCustom( double d )
{	
	if( ceil(d) - d <= .5 )
		return (int)ceil(d);
	else
		return (int)floor(d);
}

void  CShapeNetwork::RasterizeD8(VARIANT_BOOL UseNetworkBounds, CGridHeader * Header, double cellsize, ICallback *cBack, CGrid **retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	ICallback * callback = _globalCallback ? _globalCallback : cBack;

	if( IsAligned() == false )
	{	*retval = NULL;
		_lastErrorCode = tkNOT_ALIGNED;
		if( cBack != NULL )
			cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
		else if( _globalCallback != NULL )
			_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
		return ;		
	}

	if( cBack == NULL )
		cBack = _globalCallback;

	short nodata = -1;
		
	if( UseNetworkBounds != VARIANT_FALSE )
	{	
		CExtents * box = NULL;
		_netshpfile->get_Extents(&box);
		double xllcenter = 0, yllcenter = 0;
		double xurcenter = 0, yurcenter = 0;
		box->get_xMin(&xllcenter);
		box->get_yMin(&yllcenter);
		box->get_xMax(&xurcenter);
		box->get_yMax(&yurcenter);
		delete box;
		
		if( cellsize <= 0 )
		{	*retval = NULL;
			_lastErrorCode = tkINVALID_PARAMETER_VALUE;
			if( cBack != NULL )
				cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
			else if( _globalCallback != NULL )
				_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
			return ;
		}

		long ncols = roundCustom((xurcenter - xllcenter) / cellsize) + 1;
		long nrows = roundCustom((yurcenter - yllcenter) / cellsize) + 1;

		CGridHeader * nbheader = new CGridHeader();
		nbheader->put_dX(cellsize);
		nbheader->put_dY(cellsize);
		VARIANT vndv;
		VariantInit(&vndv); 
		vndv.vt = VT_I4;
		vndv.lVal = nodata;
		nbheader->put_NodataValue(vndv);
		nbheader->put_NumberCols(ncols);
		nbheader->put_NumberRows(nrows);
		nbheader->put_XllCenter(xllcenter);
		nbheader->put_YllCenter(yllcenter);

		VARIANT_BOOL vbretval = FALSE;
		*retval = new CGrid();

		CallbackHelper::Progress(_globalCallback, 0, "ShpNetwork::RasterizeD8", _key);

		(*retval)->CreateNew(m_globalSettings.emptyBstr,nbheader,ShortDataType,vndv,VARIANT_TRUE,UseExtension,cBack,&vbretval);
		delete nbheader;
		VariantClear(&vndv); 
		if( vbretval == VARIANT_FALSE )
		{	
			delete (*retval);
			*retval = NULL;
			_lastErrorCode = tkGRID_NOT_INITIALIZED;
			if( cBack != NULL )
				cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
			else if( _globalCallback != NULL )
				_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
			return ;
		}
	}
	else
	{	if( Header == NULL )
		{	*retval = NULL;
			_lastErrorCode = tkUNEXPECTED_NULL_PARAMETER;
			if( cBack != NULL )
				cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
			else if( _globalCallback != NULL )
				_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
			return ;
		}	
			
		double dx = 0.0, dy = 0.0;
		Header->get_dX(&dx);
		Header->get_dY(&dy);
		cellsize = dx;
		if( dy < cellsize )
			cellsize = dy;

		if( cellsize <= 0 )
		{	*retval = NULL;
			
			if( cellsize == dx )
				_lastErrorCode = tkINVALID_DX;
			else
				_lastErrorCode = tkINVALID_DY;

			if( cBack != NULL )
				cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
			else if( _globalCallback != NULL )
				_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
			return ;
		}		

		//Reset the nodata value
		VARIANT vndv;
		VariantInit(&vndv); 
		vndv.vt = VT_I2;
		vndv.iVal = nodata;
		Header->put_NodataValue(vndv);

		VARIANT_BOOL vbretval = FALSE;
		*retval = new CGrid();

		CallbackHelper::Progress(_globalCallback, 0, "ShpNetwork::RasterizeD8", _key);

		(*retval)->CreateNew(m_globalSettings.emptyBstr, Header, ShortDataType, vndv, VARIANT_TRUE, UseExtension, cBack, &vbretval);
		VariantClear(&vndv); 
		if( vbretval == FALSE )
		{	
			delete (*retval);
			*retval = NULL;
			_lastErrorCode = tkGRID_NOT_INITIALIZED;
			if( cBack != NULL )
				cBack->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));		
			else if( _globalCallback != NULL )
				_globalCallback->Error(OLE2BSTR(_key),A2BSTR(ErrorMsg(_lastErrorCode)));			
			return ;
		}
	}	
	
	//Generate a stack that has the ups at elements 0 -
	std::deque<long> upsfirst;
	std::deque<long> netstack;
	netstack.push_back(0);

	while( netstack.size() > 0 )//&& upsfirst.size() < 30 )
	{	
		long parent = netstack[0];
		netstack.pop_front();
		upsfirst.push_front(parent);		

		for( int i = 0; i < (int)_network[parent].up.size(); i++ )
		{	long index = _network[parent].up[i];
			netstack.push_back(index);			
		}
	}

	//Rasterize each segment
	//	Point0 is always the outlet

	long percent = 0, newpercent = 0;
	double total = upsfirst.size();

	for( int s = 0; s < (int)upsfirst.size(); s++ )
	{	long shapeIndex = upsfirst[s];

		CShape * shp = NULL;
		_netshpfile->get_Shape(shapeIndex,&shp);

		long numPoints = 0;
		shp->get_NumPoints(&numPoints);
			
		std::deque<snraspnt> Rasterize;
		//Create a raster_cell that will be written
		CPointClass * downpnt = NULL;
		shp->get_Point(0,&downpnt);
		double dpx, dpy;
		downpnt->get_X(&dpx);
		downpnt->get_Y(&dpy);
		delete downpnt;
		
		snraspnt current_point;
		(*retval)->ProjToCell(dpx,dpy,&(current_point.column),&(current_point.row));
		current_point.length = cellsize;
		Rasterize.push_back( current_point );
		snraspnt temp_point;

		//Step through the line incrementing the length inside of the cell
		for( int i = 0; i < numPoints - 1; i++ )
		{		
			CPointClass * one = NULL;
			CPointClass * two = NULL;

			shp->get_Point(i,&one);
			shp->get_Point(i+1,&two);

			double x1, x2, y1, y2;
			one->get_X(&x1);
			one->get_Y(&y1);
			two->get_X(&x2);
			two->get_Y(&y2);

			delete one;
			delete two;

			double dx = x2 - x1;
			double dy = y2 - y1;

			//Step by 1/30th of the cellsize
			double increment = cellsize/30.0;

			double step_x = -increment;
			double step_y = -increment;
		
			if( dx > 0 )
				step_x = increment;
			if( dy > 0 )
				step_y = increment;		
					
			if( dx != 0.0 )
			{	
				double m = dy/dx;
				double b = y1 - m*x1;

				if( fabs(dx) > fabs(dy) )
				{	
					for( double i = 0; i < fabs(dx); i=i+increment )
					{	y1 = ( m*x1 + b );
						
						(*retval)->ProjToCell(x1,y1,&(temp_point.column),&(temp_point.row));						
						current_point.column = temp_point.column;
						current_point.row = temp_point.row;

						if( Rasterize[ Rasterize.size() - 1 ] == current_point )
						{	current_point.increment_length( increment );
							Rasterize[ Rasterize.size() - 1 ] = current_point;
						}
						else
						{	//Check to see if this raster has been used before
							bool prevUsed = false;
							for( int pu = 0; pu < (int)Rasterize.size(); pu++ )
							{	if( Rasterize[pu] == current_point )
								{	prevUsed = true;
									current_point = Rasterize[pu];
									Rasterize.erase( Rasterize.begin() + pu );
									current_point.increment_length( increment );
									Rasterize.push_back( current_point );
									break;
								}
							}
							if( !prevUsed )
							{
								current_point.length = increment;
								Rasterize.push_back( current_point );
							}
							
						}
																
						x1 = x1 + step_x;
					}
				}
				else
				{
					for( double j = 0; j < fabs(dy); j=j+increment )
					{	x1 = ( ( y1 - b )/m );						

						(*retval)->ProjToCell(x1,y1,&(temp_point.column),&(temp_point.row));						
						current_point.column = temp_point.column;
						current_point.row = temp_point.row;

						if( Rasterize[ Rasterize.size() - 1 ] == current_point )
						{	current_point.increment_length( increment );
							Rasterize[ Rasterize.size() - 1 ] = current_point;
						}
						else
						{	//Check to see if this raster has been used before
							bool prevUsed = false;
							for( int pu = 0; pu < (int)Rasterize.size(); pu++ )
							{	if( Rasterize[pu] == current_point )
								{	prevUsed = true;
									current_point = Rasterize[pu];
									Rasterize.erase( Rasterize.begin() + pu );
									current_point.increment_length( increment );
									Rasterize.push_back( current_point );
									break;
								}
							}
							if( !prevUsed )
							{
								current_point.length = increment;
								Rasterize.push_back( current_point );
							}
						}
										
						y1 = y1 + step_y;
					}
				}
			}
			else
			{	
				for( double j = 0; j < fabs(dy); j=j+increment )
				{	
					(*retval)->ProjToCell(x1,y1,&(temp_point.column),&(temp_point.row));						
					current_point.column = temp_point.column;
					current_point.row = temp_point.row;

					if( Rasterize[ Rasterize.size() - 1 ] == current_point )
					{	current_point.increment_length( increment );
						Rasterize[ Rasterize.size() - 1 ] = current_point;
					}
					else
					{	//Check to see if this raster has been used before
						bool prevUsed = false;
						for( int pu = 0; pu < (int)Rasterize.size(); pu++ )
						{	if( Rasterize[pu] == current_point )
							{	prevUsed = true;
								current_point = Rasterize[pu];
								Rasterize.erase( Rasterize.begin() + pu );
								current_point.increment_length( increment );
								Rasterize.push_back( current_point );
								break;
							}
						}
						if( !prevUsed )
						{
							current_point.length = increment;
							Rasterize.push_back( current_point );
						}
					}
					
					y1 = y1 + step_y;
				}
			}			
		}

		//Push the last point onto the deque
		CPointClass * uppnt = NULL;
		shp->get_Point(numPoints - 1,&uppnt);
		double uppx, uppy;
		uppnt->get_X(&uppx);
		uppnt->get_Y(&uppy);
		delete uppnt;
		(*retval)->ProjToCell(uppx,uppy,&(temp_point.column),&(temp_point.row));
		temp_point.length = cellsize;
		if( Rasterize[ Rasterize.size() - 1 ] == temp_point )
			Rasterize[ Rasterize.size() - 1 ] = temp_point;
		else
			Rasterize.push_back( temp_point );		

		delete shp;
		shp = NULL;

		//Write the raster_cells to the grid
		//	with directional information of network flow
		//
		//	Flow Diagram
		//
		//	4 3 2		
		//  5 X 1
		//  6 7 8
		
		short direction = 0;
		double half_csize = cellsize*.5 - cellsize/30.0;

		bool first_point = true;
		VARIANT vval;
		VariantInit(&vval); 
		short val;
			
		snraspnt last_raster = Rasterize[0];

		for( int k = 1; k < (int)Rasterize.size(); k++ )
		{	
			//Test if the Length is long enough to write in the final_grid
			double length = Rasterize[k].length;
			if( Rasterize[k].length >= half_csize )
			{	
				//Do not set the first point if it has already been set
				if( first_point == true )
				{	first_point = false;
					(*retval)->get_Value(Rasterize[k].column,Rasterize[k].row,&vval);
					sVal(vval,val);
					if( val != nodata )
					{	last_raster = Rasterize[k];
						continue;					
					}
				}

				//direction = length;
				direction = RasterDirection( Rasterize[k], last_raster );
				vval.vt = VT_I2;
				vval.iVal = direction;
				//vval.iVal = length;
				(*retval)->put_Value( Rasterize[k].column, Rasterize[k].row, vval );
				last_raster = Rasterize[k];
			}
		}				

		Rasterize.clear();	
		
		CallbackHelper::Progress(callback, s, total, "ShpNetwork::RasterizeD8", _key, percent);

		VariantClear(&vval); 
	}

	return ;
}

short CShapeNetwork::RasterDirection( snraspnt & source, snraspnt & sink )
{	
	long src_i = source.column; 
	long src_j = source.row;
	long snk_i = sink.column;
	long snk_j = sink.row;

	//Up or Down
	if( src_i == snk_i )
	{	if( src_j < snk_j )
			return 7;
		else if( src_j > snk_j )
			return 3;
	}
	//Left or Right
	else if( src_j == snk_j )
	{	if( src_i < snk_i )
			return 1;
		else if( src_i > snk_i )
			return 5;
	}	
	//UpRight or DownRight
	else if( src_i < snk_i )
	{	if( src_j < snk_j )
			return 8;
		else if( src_j > snk_j )
			return 2;
	}
	//UpLeft or DownLeft
	else if( src_i > snk_i )
	{	if( src_j < snk_j )
			return 6;
		else if( src_j > snk_j )
			return 4;
	}

	return 0;
}


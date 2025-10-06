# ifndef ENUMERATIONS_H
# define ENUMERATIONS_H

enum ShapeLayerFlags
{	sfVisible = 1,
	slfDrawFill = 2,
	slfDrawLine = 4,
	slfDrawPoint = 8,	
};

enum LayerType
{	ImageLayer,
	ShapefileLayer,
	UndefinedLayer,
	OgrLayerSource,
};
enum LayerFlags
{	Visible = 1
};
    
enum tkFontStyle
{
    fstRegular = 0,
	fstBold = 1,
    fstItalic = 2,
    fstUnderline = 4,
    fstStrikeout = 8,
};

enum tkSimpleShapeType
{
    shpNone = 0,
	shpPoint = 1,
    shpPolyline = 2,
    shpPolygon = 3,
    shpMultiPoint = 4,
};

enum tkExtentsRelation
{
	erNone = 0,
	erEqual = 1,
	erInclude = 2,
	erIsIncluded = 3,
	erIntersection = 4
};

enum tkImageDrawingMethod
{
	idmNewWithResampling = 1,	// new drawing procedures will be used
    idmGDIPlusDrawing = 2,		// GDIPlus rather then GDI drawing will be used
    idmGDIPlusResampling = 4,		// Resampling for GDAL based images will be made by GDI+, otherwise it'll be carried out in tkRaster.cpp (ImageRampling.cpp)
};

struct Point2D 
{
	double x, y;
	Point2D() : x(0.0), y(0.0) {}
	Point2D(double x, double y): x(x), y(y)
	{		
	}

	double GetDistance(Point2D& other) 
	{
		return sqrt(pow(x - other.x, 2.0) + pow(y - other.y, 2.0));
	}

	double GetDistance(double x2, double y2) 
	{
		return sqrt(pow(x - x2, 2.0) + pow(y - y2, 2.0));
	}
};

struct PointWithId: Point2D
{
	double x, y;
	int id;
	PointWithId() : x(0.0), y(0.0), id(0) {}
	PointWithId(double x, double y, int id) : x(x), y(y), id(id) {}
};

enum tkTransformationMode
{
	tmNotDefined = 0,
	tmWgs84Complied = 1,
	tmDoTransformation = 2,
};
enum CacheType 
{ 
	SqliteCache = 0, 
	DiskCache = 1, 
};

enum HandleImage
{ 
	asRGB=0,
	asGrid=1,
	asComplex=2
};

enum ShapeValidityCheck
{
	NoPoints = 0,
	NotEnoughPoints = 1,
	NoParts = 3,
	EmptyParts = 4,
	FirstAndLastPointOfPartMatch = 5,
	DirectionOfPolyRings = 6,
};

enum ShapeValidationStatus
{
	Original = 0,
	Fixed = 1,
	Skip = 2,
};

enum GdalSupport
{
	GdalSupportNone = 0,
	GdalSupportRgb = 1,
	GdalSupportGrid = 2
};

enum tkDrawingDataAvailable
{
	MeasuringData = 0,
	Coordinates = 1,
	TilesData = 2,
	LayersData = 3,
	HotTracking = 4,
	ZoomingAnimation = 5,
	PanningInertia = 6,
	ActShape = 7,
	ShapeDigitizing = 8,
	ZoomBox = 9,
	MovingShapes = 10,
};

enum SqliteOpenMode
{
	OpenIfExists = 0,
	OpenOrCreate = 1,
};

enum ZoombarPart
{
	ZoombarNone = 0,
	ZoombarPlus = 1,
	ZoombarMinus = 2,
	ZoombarHandle = 3,
	ZoombarBar = 4,
};

enum DraggingOperation
{
	DragNone = 0,
	DragPanning = 1,
	DragZoombarHandle = 2,
	DragZoombox = 3,
	DragMoveShape = 4,
	DragMoveVertex = 5,
	DragMovePart = 6,
	DragMoveShapes = 7,
	DragRotateShapes = 8,
	DragSelectionBox = 9
};

enum TileProjectionState
{
	ProjectionMatch = 0,
	ProjectionDoTransform = 1,
	ProjectionCompatible = 2,	// WGS84 map; GMercator server; transformation is still necessary but at least results are guaranteed
};

enum ShapeInputMode
{
	simMeasuring = 0,
	simEditing = 1,
};

enum LayerSelector
{
	slctShapefiles = 0,
	slctInMemorySf = 1,
	slctHotTracking = 2,
	slctInteractiveEditing = 3,
	slctIdentify = 4,
};

enum MouseTolerance
{
	ToleranceSelect = 0,
	ToleranceInsert = 1,
	ToleranceSnap = 2,
};

enum SubjectOperation
{
	SubjectAddPart = 0,
	SubjectClip = 1,
	SubjectSplit = 2,
};

enum PointPart
{
	PartNone = 0,
	PartBegin = 1,
	PartEnd = 2,
};

enum EditorDeleteResult
{
	DeleteNone = 0,
	DeleteShape = 1,
	Delete
};

enum HotTrackingResult
{
	NewShape = 0,
	SameShape = 1,
	NoShape = 2,
};

enum RedrawTarget
{
	rtShapeEditor = 0,
	rtVolatileLayer = 1,
};

enum DebugTarget
{
	DebugAny = 0,
	DebugOgrLoading = 1,
	DebugPanning = 2,
	DebugTiles = 3,
	DebugForceGdal = 4,
};

enum OpenFileDialogFilter
{
	FilterImage = 0, 
	FilterGrid = 1, 
	FilterOgr = 2,
	FilterShapefile = 3,
	FilterAll = 4,
};

enum GdalPath
{
	PathGdalData = 0,
	PathGdalPlugins =1,
};

//////////////////////////////////
enum ShpfileType
{
	SHP_NULLSHAPE = 0,
	SHP_POINT = 1,
	SHP_POLYLINE = 3,
	SHP_POLYGON = 5,
	SHP_MULTIPOINT = 8,
	SHP_POINTZ = 11,
	SHP_POLYLINEZ = 13,
	SHP_POLYGONZ = 15,
	SHP_MULTIPOINTZ = 18,
	SHP_POINTM = 21,
	SHP_POLYLINEM = 23,
	SHP_POLYGONM = 25,
	SHP_MULTIPOINTM = 28,
	SHP_MULTIPATCH = 31
};
enum tkSpatialRelation
{
	srContains = 0,
	srCrosses = 1,
	srDisjoint = 2,
	srEquals = 3,
	srIntersects = 4,
	srOverlaps = 5,
	srTouches = 6,
	srWithin = 7
};
enum tkClipOperation
{
	clDifference = 0,
	clIntersection = 1,
	clSymDifference = 2,
	clUnion = 3,
	clClip = 4
};
enum tkBufferCap
{
	bcROUND = 0,
	bcFLAT = 1,
	bcSQUARE = 2
};
enum tkBufferJoin
{
	bjROUND = 0,
	bjMITRE = 1,
	bjBEVEL = 2
};
enum tkVectorDrawingMode
{
	vdmGDI = 0,
	vdmGDIMixed = 1,
	vdmGDIPlus = 2
};
enum tkGDIPlusHatchStyle
{
	hsNone = -1,
	hsHorizontal = 0,
	hsVertical = 1,
	hsForwardDiagonal = 2,
	hsBackwardDiagonal = 3,
	hsCross = 4,
	hsDiagonalCross = 5,
	hsPercent05 = 6,
	hsPercent10 = 7,
	hsPercent20 = 8,
	hsPercent25 = 9,
	hsPercent30 = 10,
	hsPercent40 = 11,
	hsPercent50 = 12,
	hsPercent60 = 13,
	hsPercent70 = 14,
	hsPercent75 = 15,
	hsPercent80 = 16,
	hsPercent90 = 17,
	hsLightDownwardDiagonal = 18,
	hsLightUpwardDiagonal = 19,
	hsDarkDownwardDiagonal = 20,
	hsDarkUpwardDiagonal = 21,
	hsWideDownwardDiagonal = 22,
	hsWideUpwardDiagonal = 23,
	hsLightVertical = 24,
	hsLightHorizontal = 25,
	hsNarrowVertical = 26,
	hsNarrowHorizontal = 27,
	hsDarkVertical = 28,
	hsDarkHorizontal = 29,
	hsDashedDownwardDiagonal = 30,
	hsDashedUpwardDiagonal = 31,
	hsDashedHorizontal = 32,
	hsDashedVertical = 33,
	hsSmallConfetti = 34,
	hsLargeConfetti = 35,
	hsZigZag = 36,
	hsWave = 37,
	hsDiagonalBrick = 38,
	hsHorizontalBrick = 39,
	hsWeave = 40,
	hsPlaid = 41,
	hsDivot = 42,
	hsDottedGrid = 43,
	hsDottedDiamond = 44,
	hsShingle = 45,
	hsTrellis = 46,
	hsSphere = 47,
	hsSmallGrid = 48,
	hsSmallCheckerBoard = 49,
	hsLargeCheckerBoard = 50,
	hsOutlinedDiamond = 51,
	hsSolidDiamond = 52
};
enum tkDashStyle
{
	dsSolid = 0,
	dsDash = 1,
	dsDot = 2,
	dsDashDot = 3,
	dsDashDotDot = 4,
	dsCustom = 5
};
enum tkPointShapeType
{
	ptShapeRegular = 0,
	ptShapeCross = 1,
	ptShapeStar = 2,
	ptShapeCircle = 3,
	ptShapeArrow = 4,
	ptShapeFlag = 5
};
enum ImageType
{
	BITMAP_FILE = 0,
	GIF_FILE = 1,
	USE_FILE_EXTENSION = 2,
	TIFF_FILE = 3,
	JPEG_FILE = 4,
	PNG_FILE = 5,
	PPM_FILE = 7,
	ECW_FILE = 8,
	JPEG2000_FILE = 9,
	SID_FILE = 10,
	PNM_FILE = 11,
	PGM_FILE = 12,
	BIL_FILE = 13,
	ADF_FILE = 14,
	GRD_FILE = 15,
	IMG_FILE = 16,
	ASC_FILE = 17,
	BT_FILE = 18,
	MAP_FILE = 19,
	LF2_FILE = 20,
	KAP_FILE = 21,
	DEM_FILE = 22,
	NTF_FILE = 23,
	NETCDF_FILE = 24,
	VRT_FILE = 25
}; 
enum ColoringType
{
	Hillshade = 0,
	Gradient = 1,
	Random = 2
};
enum GradientModel
{
	Logorithmic = 0,
	Linear = 1,
	Exponential = 2
};
enum PredefinedColorScheme
{
	FallLeaves = 0,
	SummerMountains = 1,
	Desert = 2,
	Glaciers = 3,
	Meadow = 4,
	ValleyFires = 5,
	DeadSea = 6,
	Highway1 = 7,
	Rainbow = 8,
	ReversedRainbow = 9
};
enum tkGDALResamplingMethod
{
	grmNone = 0,
	grmNearest = 1,
	grmGauss = 2,
	grmBicubic = 3,
	grmAverage = 4
};
enum tkInterpolationMode
{
	imBilinear = 3,
	imBicubic = 4,
	imNone = 5,
	imHighQualityBilinear = 6,
	imHighQualityBicubic = 7
};
enum tkVerticalPosition
{
	vpAboveParentLayer = 0,
	vpAboveAllLayers = 1
};
enum tkLabelAlignment
{
	laTopLeft = 0,
	laTopCenter = 1,
	laTopRight = 2,
	laCenterLeft = 3,
	laCenter = 4,
	laCenterRight = 5,
	laBottomLeft = 6,
	laBottomCenter = 7,
	laBottomRight = 8
};
enum tkLineLabelOrientation
{
	lorHorizontal = 0,
	lorParallel = 1,
	lorPerpindicular = 2
};
enum tkLinearGradientMode
{
	gmHorizontal = 0,
	gmVertical = 1,
	gmForwardDiagonal = 2,
	gmBackwardDiagonal = 3,
	gmNone = 4
};
enum tkLabelFrameType
{
	lfRectangle = 0,
	lfRoundedRectangle = 1,
	lfPointedRectangle = 2
};
enum SelectMode
{
	INTERSECTION = 0,
	INCLUSION = 1
};
enum tkClassificationType
{
	ctNaturalBreaks = 0,
	ctUniqueValues = 1,
	ctEqualIntervals = 2,
	ctEqualCount = 3,
	ctStandardDeviation = 4,
	ctEqualSumOfValues = 5
};
enum tkColorSchemeType
{
	ctSchemeRandom = 0,
	ctSchemeGraduated = 1
};
enum tkMapColor
{
	AliceBlue = -984833,
	AntiqueWhite = -332841,
	Aqua = -16711681,
	Aquamarine = -8388652,
	Azure = -983041,
	Beige = -657956,
	Bisque = -6972,
	Black = -16777216,
	BlanchedAlmond = -5171,
	Blue = -16776961,
	BlueViolet = -7722014,
	Brown = -5952982,
	BurlyWood = -2180985,
	CadetBlue = -10510688,
	Chartreuse = -8388864,
	Chocolate = -2987746,
	Coral = -32944,
	CornflowerBlue = -10185235,
	Cornsilk = -1828,
	Crimson = -2354116,
	Cyan = -16711681,
	DarkBlue = -16777077,
	DarkCyan = -16741493,
	DarkGoldenrod = -4684277,
	DarkGray = -5658199,
	DarkGreen = -16751616,
	DarkKhaki = -4343957,
	DarkMagenta = -7667573,
	DarkOliveGreen = -11179217,
	DarkOrange = -29696,
	DarkOrchid = -6737204,
	DarkRed = -7667712,
	DarkSalmon = -1468806,
	DarkSeaGreen = -7357301,
	DarkSlateBlue = -12042869,
	DarkSlateGray = -13676721,
	DarkTurquoise = -16724271,
	DarkViolet = -7077677,
	DeepPink = -60269,
	DeepSkyBlue = -16728065,
	DimGray = -9868951,
	DodgerBlue = -14774017,
	Firebrick = -5103070,
	FloralWhite = -1296,
	ForestGreen = -14513374,
	Fuchsia = -65281,
	Gainsboro = -2302756,
	GhostWhite = -460545,
	Gold = -10496,
	Goldenrod = -2448096,
	Gray = -8355712,
	Green = -16744448,
	GreenYellow = -5374161,
	Honeydew = -983056,
	HotPink = -38476,
	IndianRed = -3318692,
	Indigo = -11861886,
	Ivory = -16,
	Khaki = -989556,
	Lavender = -1644806,
	LavenderBlush = -3851,
	LawnGreen = -8586240,
	LemonChiffon = -1331,
	LightBlue = -5383962,
	LightCoral = -1015680,
	LightCyan = -2031617,
	LightGoldenrodYellow = -329006,
	LightGray = -2894893,
	LightGreen = -7278960,
	LightPink = -18751,
	LightSalmon = -24454,
	LightSeaGreen = -14634326,
	LightSkyBlue = -7876870,
	LightSlateGray = -8943463,
	LightSteelBlue = -5192482,
	LightYellow = -32,
	Lime = -16711936,
	LimeGreen = -13447886,
	Linen = -331546,
	Magenta = -65281,
	Maroon = -8388608,
	MediumAquamarine = -10039894,
	MediumBlue = -16777011,
	MediumOrchid = -4565549,
	MediumPurple = -7114533,
	MediumSeaGreen = -12799119,
	MediumSlateBlue = -8689426,
	MediumSpringGreen = -16713062,
	MediumTurquoise = -12004916,
	MediumVioletRed = -3730043,
	MidnightBlue = -15132304,
	MintCream = -655366,
	MistyRose = -6943,
	Moccasin = -6987,
	NavajoWhite = -8531,
	Navy = -16777088,
	OldLace = -133658,
	Olive = -8355840,
	OliveDrab = -9728477,
	Orange = -23296,
	OrangeRed = -47872,
	Orchid = -2461482,
	PaleGoldenrod = -1120086,
	PaleGreen = -6751336,
	PaleTurquoise = -5247250,
	PaleVioletRed = -2396013,
	PapayaWhip = -4139,
	PeachPuff = -9543,
	Peru = -3308225,
	Pink = -16181,
	Plum = -2252579,
	PowderBlue = -5185306,
	Purple = -8388480,
	Red = -65536,
	RosyBrown = -4419697,
	RoyalBlue = -12490271,
	SaddleBrown = -7650029,
	Salmon = -360334,
	SandyBrown = -744352,
	SeaGreen = -13726889,
	SeaShell = -2578,
	Sienna = -6270419,
	Silver = -4144960,
	SkyBlue = -7876885,
	SlateBlue = -9807155,
	SlateGray = -9404272,
	Snow = -1286,
	SpringGreen = -16711809,
	SteelBlue = -12156236,
	Tan = -2968436,
	Teal = -16744320,
	Thistle = -2572328,
	Tomato = -40121,
	Turquoise = -12525360,
	Violet = -1146130,
	Wheat = -663885,
	White = -1,
	WhiteSmoke = -657931,
	Yellow = -256,
	YellowGreen = -6632142
};
enum tkLabelElements
{
	leFont = 0,
	leFontOutline = 1,
	leShadow = 2,
	leHalo = 3,
	leFrameBackground = 4,
	leFrameOutline = 5,
	leDefault = 6
};
enum tkLabelPositioning
{
	lpCenter = 0,
	lpCentroid = 1,
	lpInteriorPoint = 2,
	lpFirstSegment = 3,
	lpLastSegment = 4,
	lpMiddleSegment = 5,
	lpLongestSegement = 6,
	lpNone = 7
};
enum tkSavingMode
{
	modeNone = 0,
	modeStandard = 1,
	modeXML = 2,
	modeDBF = 3,
	modeXMLOverwrite = 4
};
enum tkTextRenderingHint
{
	SystemDefault = 0,
	SingleBitPerPixelGridFit = 1,
	SingleBitPerPixel = 2,
	AntiAliasGridFit = 3,
	HintAntiAlias = 4,
	ClearTypeGridFit = 5
};
enum tkImageSourceType
{
	istUninitialized = 0,
	istDiskBased = 1,
	istInMemory = 2,
	istGDALBased = 3,
	istGDIPlus = 4
};
enum tkGridRendering
{
	grForceForAllFormats = 0,
	grForGridsOnly = 1
};
enum tkCoordinateSystem
{
	csAbidjan_1987 = 4143,
	csAccra = 4168,
	csAdindan = 4201,
	csAfgooye = 4205,
	csAgadez = 4206,
	csAGD66 = 4202,
	csAGD84 = 4203,
	csAin_el_Abd = 4204,
	csAlbanian_1987 = 4191,
	csAmerican_Samoa_1962 = 4169,
	csAmersfoort = 4289,
	csAmmassalik_1958 = 4196,
	csAnguilla_1957 = 4600,
	csAntigua_1943 = 4601,
	csAratu = 4208,
	csArc_1950 = 4209,
	csArc_1960 = 4210,
	csAscension_Island_1958 = 4712,
	csATF_Paris = 4901,
	csATS77 = 4122,
	csAustralian_Antarctic = 4176,
	csAyabelle_Lighthouse = 4713,
	csAzores_Central_1948 = 4183,
	csAzores_Central_1995 = 4665,
	csAzores_Occidental_1939 = 4182,
	csAzores_Oriental_1940 = 4184,
	csAzores_Oriental_1995 = 4664,
	csBarbados_1938 = 4212,
	csBatavia = 4211,
	csBatavia_Jakarta = 4813,
	csBDA2000 = 4762,
	csBeduaram = 4213,
	csBeijing_1954 = 4214,
	csBelge_1950 = 4215,
	csBelge_1950_Brussels = 4809,
	csBelge_1972 = 4313,
	csBellevue = 4714,
	csBermuda_1957 = 4216,
	csBern_1898_Bern = 4801,
	csBern_1938 = 4306,
	csBissau = 4165,
	csBogota_1975 = 4218,
	csBogota_1975_Bogota = 4802,
	csBukit_Rimpah = 4219,
	csCadastre_1997 = 4475,
	csCamacupa = 4220,
	csCamp_Area_Astro = 4715,
	csCampo_Inchauspe = 4221,
	csCape = 4222,
	csCape_Canaveral = 4717,
	csCarthage = 4223,
	csCarthage_Paris = 4816,
	csCH1903 = 4149,
	csCH1903_plus = 4150,
	csChatham_Islands_1971 = 4672,
	csChatham_Islands_1979 = 4673,
	csChina_Geodetic_Coordinate_System_2000 = 4490,
	csChos_Malal_1914 = 4160,
	csCHTRF95 = 4151,
	csChua = 4224,
	csCocos_Islands_1965 = 4708,
	csCombani_1950 = 4632,
	csConakry_1905 = 4315,
	csCorrego_Alegre = 4225,
	csCSG67 = 4623,
	csDabola_1981 = 4155,
	csDatum_73 = 4274,
	csDealul_Piscului_1930 = 4316,
	csDeception_Island = 4736,
	csDeir_ez_Zor = 4227,
	csDGN95 = 4755,
	csDHDN = 4314,
	csDiego_Garcia_1969 = 4724,
	csDominica_1945 = 4602,
	csDouala_1948 = 4192,
	csDRUKREF_03 = 5264,
	csEaster_Island_1967 = 4719,
	csED50 = 4230,
	csED50_ED77 = 4154,
	csED79 = 4668,
	csED87 = 4231,
	csEgypt_1907 = 4229,
	csEgypt_1930 = 4199,
	csEgypt_Gulf_of_Suez_S_650_TL = 4706,
	csELD79 = 4159,
	csEST92 = 4133,
	csEST97 = 4180,
	csETRS89 = 4258,
	csFahud = 4232,
	csFatu_Iva_72 = 4688,
	csFD54 = 4741,
	csFD58 = 4132,
	csFiji_1956 = 4721,
	csFiji_1986 = 4720,
	csfk89 = 4753,
	csFort_Marigot = 4621,
	csGan_1970 = 4684,
	csGaroua = 4197,
	csGDA94 = 4283,
	csGDBD2009 = 5246,
	csGDM2000 = 4742,
	csGGRS87 = 4121,
	csGR96 = 4747,
	csGrand_Cayman_1959 = 4723,
	csGrand_Comoros = 4646,
	csGreek = 4120,
	csGreek_Athens = 4815,
	csGrenada_1953 = 4603,
	csGuadeloupe_1948 = 4622,
	csGuam_1963 = 4675,
	csGulshan_303 = 4682,
	csHanoi_1972 = 4147,
	csHartebeesthoek94 = 4148,
	csHD1909 = 3819,
	csHD72 = 4237,
	csHelle_1954 = 4660,
	csHerat_North = 4255,
	csHito_XVIII_1963 = 4254,
	csHjorsey_1955 = 4658,
	csHong_Kong_1963 = 4738,
	csHong_Kong_1963_67 = 4739,
	csHong_Kong_1980 = 4611,
	csHTRS96 = 4761,
	csHu_Tzu_Shan_1950 = 4236,
	csID74 = 4238,
	csIGC_1962_6th_Parallel_South = 4697,
	csIGCB_1955 = 4701,
	csIGM95 = 4670,
	csIGN_1962_Kerguelen = 4698,
	csIGN_Astro_1960 = 4700,
	csIGN53_Mare = 4641,
	csIGN56_Lifou = 4633,
	csIGN63_Hiva_Oa = 4689,
	csIGN72_Grande_Terre = 4662,
	csIGN72_Nuku_Hiva = 4630,
	csIGRS = 3889,
	csIKBD_92 = 4667,
	csIndian_1954 = 4239,
	csIndian_1960 = 4131,
	csIndian_1975 = 4240,
	csIRENET95 = 4173,
	csISN93 = 4659,
	csIsrael = 4141,
	csIwo_Jima_1945 = 4709,
	csJAD2001 = 4758,
	csJAD69 = 4242,
	csJamaica_1875 = 4241,
	csJGD2000 = 4612,
	csJohnston_Island_1961 = 4725,
	csJouik_1961 = 4679,
	csKalianpur_1880 = 4243,
	csKalianpur_1937 = 4144,
	csKalianpur_1962 = 4145,
	csKalianpur_1975 = 4146,
	csKandawala = 4244,
	csKarbala_1979 = 4743,
	csKasai_1953 = 4696,
	csKatanga_1955 = 4695,
	csKertau_RSO = 4751,
	csKertau_1968 = 4245,
	csKKJ = 4123,
	csKOC = 4246,
	csKorea_2000 = 4737,
	csKorean_1985 = 4162,
	csKorean_1995 = 4166,
	csKousseri = 4198,
	csKUDAMS = 4319,
	csKusaie_1951 = 4735,
	csLa_Canoa = 4247,
	csLake = 4249,
	csLao_1993 = 4677,
	csLao_1997 = 4678,
	csLe_Pouce_1934 = 4699,
	csLeigon = 4250,
	csLGD2006 = 4754,
	csLiberia_1964 = 4251,
	csLisbon = 4207,
	csLisbon_Lisbon = 4803,
	csLisbon_1890 = 4666,
	csLisbon_1890_Lisbon = 4904,
	csLittle_Cayman_1961 = 4726,
	csLKS92 = 4661,
	csLKS94 = 4669,
	csLocodjo_1965 = 4142,
	csLoma_Quintana = 4288,
	csLome = 4252,
	csLuxembourg_1930 = 4181,
	csLuzon_1911 = 4253,
	csMadrid_1870_Madrid = 4903,
	csMadzansua = 4128,
	csMAGNA_SIRGAS = 4686,
	csMahe_1971 = 4256,
	csMakassar = 4257,
	csMakassar_Jakarta = 4804,
	csMalongo_1987 = 4259,
	csManoca_1962 = 4193,
	csMarcus_Island_1952 = 4711,
	csMarshall_Islands_1960 = 4732,
	csMartinique_1938 = 4625,
	csMassawa = 4262,
	csMaupiti_83 = 4692,
	csMauritania_1999 = 4702,
	csMerchich = 4261,
	csMexican_Datum_of_1993 = 4483,
	csMGI = 4312,
	csMGI_Ferro = 4805,
	csMGI_1901 = 3906,
	csMhast_offshore = 4705,
	csMhast_onshore = 4704,
	csMhast_1951 = 4703,
	csMidway_1961 = 4727,
	csMinna = 4263,
	csMOLDREF99 = 4023,
	csMonte_Mario = 4265,
	csMonte_Mario_Rome = 4806,
	csMontserrat_1958 = 4604,
	csMoorea_87 = 4691,
	csMOP78 = 4639,
	csMount_Dillon = 4157,
	csMoznet = 4130,
	csMporaloko = 4266,
	csNAD27 = 4267,
	csNAD27_Michigan = 4268,
	csNAD27_76 = 4608,
	csNAD27_CGQ77 = 4609,
	csNAD83 = 4269,
	csNAD83_CSRS = 4617,
	csNAD83_HARN = 4152,
	csNAD83_NSRS2007 = 4759,
	csNahrwan_1934 = 4744,
	csNahrwan_1967 = 4270,
	csNakhl_e_Ghanem = 4693,
	csNaparima_1955 = 4158,
	csNaparima_1972 = 4271,
	csNEA74_Noumea = 4644,
	csNew_Beijing = 4555,
	csNGN = 4318,
	csNGO_1948 = 4273,
	csNGO_1948_Oslo = 4817,
	csNord_Sahara_1959 = 4307,
	csNouakchott_1965 = 4680,
	csNSWC_9Z_2 = 4276,
	csNTF = 4275,
	csNTF_Paris = 4807,
	csNZGD2000 = 4167,
	csNZGD49 = 4272,
	csObservatario = 4129,
	csOld_Hawaiian = 4135,
	csOS_SN80 = 4279,
	csOSGB_1936 = 4277,
	csOSGB70 = 4278,
	csOSNI_1952 = 4188,
	csPadang = 4280,
	csPadang_Jakarta = 4808,
	csPalestine_1923 = 4281,
	csPampa_del_Castillo = 4161,
	csPD_83 = 4746,
	csPerroud_1950 = 4637,
	csPetrels_1972 = 4636,
	csPhoenix_Islands_1966 = 4716,
	csPico_de_las_Nieves_1984 = 4728,
	csPitcairn_1967 = 4729,
	csPitcairn_2006 = 4763,
	csPoint_58 = 4620,
	csPointe_Noire = 4282,
	csPorto_Santo = 4615,
	csPorto_Santo_1995 = 4663,
	csPOSGAR_94 = 4694,
	csPOSGAR_98 = 4190,
	csPrincipe = 4824,
	csPRS92 = 4683,
	csPSAD56 = 4248,
	csPSD93 = 4134,
	csPTRA08 = 5013,
	csPuerto_Rico = 4139,
	csPulkovo_1942 = 4284,
	csPulkovo_1942_58 = 4179,
	csPulkovo_1942_83 = 4178,
	csPulkovo_1995 = 4200,
	csPZ_90 = 4740,
	csQatar_1948 = 4286,
	csQatar_1974 = 4285,
	csQND95 = 4614,
	csQornoq_1927 = 4194,
	csRassadiran = 4153,
	csRD_83 = 4745,
	csREGCAN95 = 4081,
	csREGVEN = 4189,
	csReunion_1947 = 4626,
	csReykjavik_1900 = 4657,
	csRGF93 = 4171,
	csRGFG95 = 4624,
	csRGM04 = 4470,
	csRGNC91_93 = 4749,
	csRGPF = 4687,
	csRGR92 = 4627,
	csRGRDC_2005 = 4046,
	csRGSPM06 = 4463,
	csRRAF_1991 = 4558,
	csRSRGD2000 = 4764,
	csRT38 = 4308,
	csRT38_Stockholm = 4814,
	csRT90 = 4124,
	csSAD69 = 4618,
	csSaint_Pierre_et_Miquelon_1950 = 4638,
	csSanto_1965 = 4730,
	csSao_Tome = 4823,
	csSapper_Hill_1943 = 4292,
	csSchwarzeck = 4293,
	csScoresbysund_1952 = 4195,
	csSegara = 4613,
	csSegara_Jakarta = 4820,
	csSelvagem_Grande = 4616,
	csSerindung = 4295,
	csSierra_Leone_1924 = 4174,
	csSierra_Leone_1968 = 4175,
	csSIRGAS_1995 = 4170,
	csSIRGAS_2000 = 4674,
	csS_JTSK = 4156,
	csS_JTSK_Ferro = 4818,
	csS_JTSK_05 = 5228,
	csS_JTSK_05_Ferro = 5229,
	csSLD99 = 5233,
	csSlovenia_1996 = 4765,
	csSolomon_1968 = 4718,
	csSouth_Georgia_1968 = 4722,
	csSouth_Yemen = 4164,
	csSREF98 = 4075,
	csSt_George_Island = 4138,
	csSt_Helena_1971 = 4710,
	csSt_Kitts_1955 = 4605,
	csSt_Lawrence_Island = 4136,
	csSt_Lucia_1955 = 4606,
	csSt_Paul_Island = 4137,
	csSt_Vincent_1945 = 4607,
	csST71_Belep = 4643,
	csST84_Ile_des_Pins = 4642,
	csST87_Ouvea = 4750,
	csSVY21 = 4757,
	csSWEREF99 = 4619,
	csTahaa_54 = 4629,
	csTahiti_52 = 4628,
	csTahiti_79 = 4690,
	csTananarive = 4297,
	csTananarive_Paris = 4810,
	csTC_1948 = 4303,
	csTern_Island_1961 = 4707,
	csTete = 4127,
	csTimbalai_1948 = 4298,
	csTM65 = 4299,
	csTM75 = 4300,
	csTokyo = 4301,
	csTokyo_1892 = 5132,
	csTrinidad_1903 = 4302,
	csTristan_1968 = 4734,
	csTUREF = 5252,
	csTWD67 = 3821,
	csTWD97 = 3824,
	csVanua_Levu_1915 = 4748,
	csVientiane_1982 = 4676,
	csViti_Levu_1912 = 4752,
	csVN_2000 = 4756,
	csVoirol_1875 = 4304,
	csVoirol_1875_Paris = 4811,
	csVoirol_1879 = 4671,
	csVoirol_1879_Paris = 4821,
	csWake_Island_1952 = 4733,
	csWGS_66 = 4760,
	csWGS_72 = 4322,
	csWGS_72BE = 4324,
	csWGS_84 = 4326,
	csXian_1980 = 4610,
	csYacare = 4309,
	csYemen_NGN96 = 4163,
	csYoff = 4310,
	csZanderij = 4311
};
enum tkProjectionParameter
{
	LatitudeOfOrigin = 0,
	CentralMeridian = 1,
	ScaleFactor = 2,
	FalseEasting = 3,
	FalseNorthing = 4,
	LongitudeOfOrigin = 5
};
enum tkGeogCSParameter
{
	SemiMajor = 0,
	SemiMinor = 1,
	InverseFlattening = 2,
	PrimeMeridian = 3,
	AngularUnit = 4
};
enum tkWgs84Projection
{
	Wgs84_World_Mercator = 3395,
	Wgs84_PDC_Mercator = 3832,
	Wgs84_Pseudo_Mercator = 3857,
	Wgs84_Mercator_41 = 3994,
	Wgs84_World_Equidistant_Cylindrical = 4087,
	Wgs84_UPS_North_EN = 5041,
	Wgs84_UPS_South_EN = 5042,
	Wgs84_UTM_grid_system_northern_hemisphere = 32600,
	Wgs84_UTM_zone_1N = 32601,
	Wgs84_UTM_zone_2N = 32602,
	Wgs84_UTM_zone_3N = 32603,
	Wgs84_UTM_zone_4N = 32604,
	Wgs84_UTM_zone_5N = 32605,
	Wgs84_UTM_zone_6N = 32606,
	Wgs84_UTM_zone_7N = 32607,
	Wgs84_UTM_zone_8N = 32608,
	Wgs84_UTM_zone_9N = 32609,
	Wgs84_UTM_zone_10N = 32610,
	Wgs84_UTM_zone_11N = 32611,
	Wgs84_UTM_zone_12N = 32612,
	Wgs84_UTM_zone_13N = 32613,
	Wgs84_UTM_zone_14N = 32614,
	Wgs84_UTM_zone_15N = 32615,
	Wgs84_UTM_zone_16N = 32616,
	Wgs84_UTM_zone_17N = 32617,
	Wgs84_UTM_zone_18N = 32618,
	Wgs84_UTM_zone_19N = 32619,
	Wgs84_UTM_zone_20N = 32620,
	Wgs84_UTM_zone_21N = 32621,
	Wgs84_UTM_zone_22N = 32622,
	Wgs84_UTM_zone_23N = 32623,
	Wgs84_UTM_zone_24N = 32624,
	Wgs84_UTM_zone_25N = 32625,
	Wgs84_UTM_zone_26N = 32626,
	Wgs84_UTM_zone_27N = 32627,
	Wgs84_UTM_zone_28N = 32628,
	Wgs84_UTM_zone_29N = 32629,
	Wgs84_UTM_zone_30N = 32630,
	Wgs84_UTM_zone_31N = 32631,
	Wgs84_UTM_zone_32N = 32632,
	Wgs84_UTM_zone_33N = 32633,
	Wgs84_UTM_zone_34N = 32634,
	Wgs84_UTM_zone_35N = 32635,
	Wgs84_UTM_zone_36N = 32636,
	Wgs84_UTM_zone_37N = 32637,
	Wgs84_UTM_zone_38N = 32638,
	Wgs84_UTM_zone_39N = 32639,
	Wgs84_UTM_zone_40N = 32640,
	Wgs84_UTM_zone_41N = 32641,
	Wgs84_UTM_zone_42N = 32642,
	Wgs84_UTM_zone_43N = 32643,
	Wgs84_UTM_zone_44N = 32644,
	Wgs84_UTM_zone_45N = 32645,
	Wgs84_UTM_zone_46N = 32646,
	Wgs84_UTM_zone_47N = 32647,
	Wgs84_UTM_zone_48N = 32648,
	Wgs84_UTM_zone_49N = 32649,
	Wgs84_UTM_zone_50N = 32650,
	Wgs84_UTM_zone_51N = 32651,
	Wgs84_UTM_zone_52N = 32652,
	Wgs84_UTM_zone_53N = 32653,
	Wgs84_UTM_zone_54N = 32654,
	Wgs84_UTM_zone_55N = 32655,
	Wgs84_UTM_zone_56N = 32656,
	Wgs84_UTM_zone_57N = 32657,
	Wgs84_UTM_zone_58N = 32658,
	Wgs84_UTM_zone_59N = 32659,
	Wgs84_UTM_zone_60N = 32660,
	Wgs84_UPS_North_NE = 32661,
	Wgs84_BLM_14N_ftUS = 32664,
	Wgs84_BLM_15N_ftUS = 32665,
	Wgs84_BLM_16N_ftUS = 32666,
	Wgs84_BLM_17N_ftUS = 32667,
	Wgs84_UTM_grid_system_southern_hemisphere = 32700,
	Wgs84_UTM_zone_1S = 32701,
	Wgs84_UTM_zone_2S = 32702,
	Wgs84_UTM_zone_3S = 32703,
	Wgs84_UTM_zone_4S = 32704,
	Wgs84_UTM_zone_5S = 32705,
	Wgs84_UTM_zone_6S = 32706,
	Wgs84_UTM_zone_7S = 32707,
	Wgs84_UTM_zone_8S = 32708,
	Wgs84_UTM_zone_9S = 32709,
	Wgs84_UTM_zone_10S = 32710,
	Wgs84_UTM_zone_11S = 32711,
	Wgs84_UTM_zone_12S = 32712,
	Wgs84_UTM_zone_13S = 32713,
	Wgs84_UTM_zone_14S = 32714,
	Wgs84_UTM_zone_15S = 32715,
	Wgs84_UTM_zone_16S = 32716,
	Wgs84_UTM_zone_17S = 32717,
	Wgs84_UTM_zone_18S = 32718,
	Wgs84_UTM_zone_19S = 32719,
	Wgs84_UTM_zone_20S = 32720,
	Wgs84_UTM_zone_21S = 32721,
	Wgs84_UTM_zone_22S = 32722,
	Wgs84_UTM_zone_23S = 32723,
	Wgs84_UTM_zone_24S = 32724,
	Wgs84_UTM_zone_25S = 32725,
	Wgs84_UTM_zone_26S = 32726,
	Wgs84_UTM_zone_27S = 32727,
	Wgs84_UTM_zone_28S = 32728,
	Wgs84_UTM_zone_29S = 32729,
	Wgs84_UTM_zone_30S = 32730,
	Wgs84_UTM_zone_31S = 32731,
	Wgs84_UTM_zone_32S = 32732,
	Wgs84_UTM_zone_33S = 32733,
	Wgs84_UTM_zone_34S = 32734,
	Wgs84_UTM_zone_35S = 32735,
	Wgs84_UTM_zone_36S = 32736,
	Wgs84_UTM_zone_37S = 32737,
	Wgs84_UTM_zone_38S = 32738,
	Wgs84_UTM_zone_39S = 32739,
	Wgs84_UTM_zone_40S = 32740,
	Wgs84_UTM_zone_41S = 32741,
	Wgs84_UTM_zone_42S = 32742,
	Wgs84_UTM_zone_43S = 32743,
	Wgs84_UTM_zone_44S = 32744,
	Wgs84_UTM_zone_45S = 32745,
	Wgs84_UTM_zone_46S = 32746,
	Wgs84_UTM_zone_47S = 32747,
	Wgs84_UTM_zone_48S = 32748,
	Wgs84_UTM_zone_49S = 32749,
	Wgs84_UTM_zone_50S = 32750,
	Wgs84_UTM_zone_51S = 32751,
	Wgs84_UTM_zone_52S = 32752,
	Wgs84_UTM_zone_53S = 32753,
	Wgs84_UTM_zone_54S = 32754,
	Wgs84_UTM_zone_55S = 32755,
	Wgs84_UTM_zone_56S = 32756,
	Wgs84_UTM_zone_57S = 32757,
	Wgs84_UTM_zone_58S = 32758,
	Wgs84_UTM_zone_59S = 32759,
	Wgs84_UTM_zone_60S = 32760,
	Wgs84_UPS_South_NE = 32761
};
enum tkNad83Projection
{
	Nad83_Kentucky_North = 2205,
	Nad83_Arizona_East_ft = 2222,
	Nad83_Arizona_Central_ft = 2223,
	Nad83_Arizona_West_ft = 2224,
	Nad83_California_zone_1_ftUS = 2225,
	Nad83_California_zone_2_ftUS = 2226,
	Nad83_California_zone_3_ftUS = 2227,
	Nad83_California_zone_4_ftUS = 2228,
	Nad83_California_zone_5_ftUS = 2229,
	Nad83_California_zone_6_ftUS = 2230,
	Nad83_Colorado_North_ftUS = 2231,
	Nad83_Colorado_Central_ftUS = 2232,
	Nad83_Colorado_South_ftUS = 2233,
	Nad83_Connecticut_ftUS = 2234,
	Nad83_Delaware_ftUS = 2235,
	Nad83_Florida_East_ftUS = 2236,
	Nad83_Florida_West_ftUS = 2237,
	Nad83_Florida_North_ftUS = 2238,
	Nad83_Georgia_East_ftUS = 2239,
	Nad83_Georgia_West_ftUS = 2240,
	Nad83_Idaho_East_ftUS = 2241,
	Nad83_Idaho_Central_ftUS = 2242,
	Nad83_Idaho_West_ftUS = 2243,
	Nad83_Kentucky_North_ftUS = 2246,
	Nad83_Kentucky_South_ftUS = 2247,
	Nad83_Maryland_ftUS = 2248,
	Nad83_Massachusetts_Mainland_ftUS = 2249,
	Nad83_Massachusetts_Island_ftUS = 2250,
	Nad83_Michigan_North_ft = 2251,
	Nad83_Michigan_Central_ft = 2252,
	Nad83_Michigan_South_ft = 2253,
	Nad83_Mississippi_East_ftUS = 2254,
	Nad83_Mississippi_West_ftUS = 2255,
	Nad83_Montana_ft = 2256,
	Nad83_New_Mexico_East_ftUS = 2257,
	Nad83_New_Mexico_Central_ftUS = 2258,
	Nad83_New_Mexico_West_ftUS = 2259,
	Nad83_New_York_East_ftUS = 2260,
	Nad83_New_York_Central_ftUS = 2261,
	Nad83_New_York_West_ftUS = 2262,
	Nad83_New_York_Long_Island_ftUS = 2263,
	Nad83_North_Carolina_ftUS = 2264,
	Nad83_North_Dakota_North_ft = 2265,
	Nad83_North_Dakota_South_ft = 2266,
	Nad83_Oklahoma_North_ftUS = 2267,
	Nad83_Oklahoma_South_ftUS = 2268,
	Nad83_Oregon_North_ft = 2269,
	Nad83_Oregon_South_ft = 2270,
	Nad83_Pennsylvania_North_ftUS = 2271,
	Nad83_Pennsylvania_South_ftUS = 2272,
	Nad83_South_Carolina_ft = 2273,
	Nad83_Tennessee_ftUS = 2274,
	Nad83_Texas_North_ftUS = 2275,
	Nad83_Texas_North_Central_ftUS = 2276,
	Nad83_Texas_Central_ftUS = 2277,
	Nad83_Texas_South_Central_ftUS = 2278,
	Nad83_Texas_South_ftUS = 2279,
	Nad83_Utah_North_ft = 2280,
	Nad83_Utah_Central_ft = 2281,
	Nad83_Utah_South_ft = 2282,
	Nad83_Virginia_North_ftUS = 2283,
	Nad83_Virginia_South_ftUS = 2284,
	Nad83_Washington_North_ftUS = 2285,
	Nad83_Washington_South_ftUS = 2286,
	Nad83_Wisconsin_North_ftUS = 2287,
	Nad83_Wisconsin_Central_ftUS = 2288,
	Nad83_Wisconsin_South_ftUS = 2289,
	Nad83_Indiana_East_ftUS = 2965,
	Nad83_Indiana_West_ftUS = 2966,
	Nad83_Oregon_Lambert = 2991,
	Nad83_Oregon_Lambert_ft = 2992,
	Nad83_BC_Albers = 3005,
	Nad83_Wisconsin_Transverse_Mercator = 3070,
	Nad83_Maine_CS2000_East = 3072,
	Nad83_Maine_CS2000_West = 3074,
	Nad83_Michigan_Oblique_Mercator = 3078,
	Nad83_Texas_State_Mapping_System = 3081,
	Nad83_Texas_Centric_Lambert_Conformal = 3082,
	Nad83_Texas_Centric_Albers_Equal_Area = 3083,
	Nad83_Florida_GDL_Albers = 3086,
	Nad83_Kentucky_Single_Zone = 3088,
	Nad83_Kentucky_Single_Zone_ftUS = 3089,
	Nad83_Ontario_MNR_Lambert = 3161,
	Nad83_Great_Lakes_Albers = 3174,
	Nad83_Great_Lakes_and_St_Lawrence_Albers = 3175,
	Nad83_California_Albers = 3310,
	Nad83_Alaska_Albers = 3338,
	Nad83_Statistics_Canada_Lambert = 3347,
	Nad83_Alberta_10_TM_Forest = 3400,
	Nad83_Alberta_10_TM_Resource = 3401,
	Nad83_Iowa_North_ft_US = 3417,
	Nad83_Iowa_South_ft_US = 3418,
	Nad83_Kansas_North_ft_US = 3419,
	Nad83_Kansas_South_ft_US = 3420,
	Nad83_Nevada_East_ft_US = 3421,
	Nad83_Nevada_Central_ft_US = 3422,
	Nad83_Nevada_West_ft_US = 3423,
	Nad83_New_Jersey_ft_US = 3424,
	Nad83_Arkansas_North_ftUS = 3433,
	Nad83_Arkansas_South_ftUS = 3434,
	Nad83_Illinois_East_ftUS = 3435,
	Nad83_Illinois_West_ftUS = 3436,
	Nad83_New_Hampshire_ftUS = 3437,
	Nad83_Rhode_Island_ftUS = 3438,
	Nad83_Louisiana_North_ftUS = 3451,
	Nad83_Louisiana_South_ftUS = 3452,
	Nad83_Louisiana_Offshore_ftUS = 3453,
	Nad83_South_Dakota_South_ftUS = 3455,
	Nad83_Maine_CS2000_Central = 3463,
	Nad83_Utah_North_ftUS = 3560,
	Nad83_Utah_Central_ftUS = 3566,
	Nad83_Utah_South_ftUS = 3567,
	Nad83_Yukon_Albers = 3578,
	Nad83_NWT_Lambert = 3580,
	Nad83_Ohio_North_ftUS = 3734,
	Nad83_Ohio_South_ftUS = 3735,
	Nad83_Wyoming_East_ftUS = 3736,
	Nad83_Wyoming_East_Central_ftUS = 3737,
	Nad83_Wyoming_West_Central_ftUS = 3738,
	Nad83_Wyoming_West_ftUS = 3739,
	Nad83_Hawaii_zone_3_ftUS = 3759,
	Nad83_Alberta_3TM_ref_merid_111_W = 3775,
	Nad83_Alberta_3TM_ref_merid_114_W = 3776,
	Nad83_Alberta_3TM_ref_merid_117_W = 3777,
	Nad83_MTQ_Lambert = 3798,
	Nad83_Alberta_3TM_ref_merid_120_W = 3801,
	Nad83_Mississippi_TM = 3814,
	Nad83_Virginia_Lambert = 3968,
	Nad83_Canada_Atlas_Lambert = 3978,
	Nad83_BLM_59N_ftUS = 4217,
	Nad83_BLM_60N_ftUS = 4420,
	Nad83_BLM_1N_ftUS = 4421,
	Nad83_BLM_2N_ftUS = 4422,
	Nad83_BLM_3N_ftUS = 4423,
	Nad83_BLM_4N_ftUS = 4424,
	Nad83_BLM_5N_ftUS = 4425,
	Nad83_BLM_6N_ftUS = 4426,
	Nad83_BLM_7N_ftUS = 4427,
	Nad83_BLM_8N_ftUS = 4428,
	Nad83_BLM_9N_ftUS = 4429,
	Nad83_BLM_10N_ftUS = 4430,
	Nad83_BLM_11N_ftUS = 4431,
	Nad83_BLM_12N_ftUS = 4432,
	Nad83_BLM_13N_ftUS = 4433,
	Nad83_BLM_18N_ftUS = 4438,
	Nad83_BLM_19N_ftUS = 4439,
	Nad83_South_Dakota_North_ftUS = 4457,
	Nad83_Conus_Albers = 5070,
	Nad83_Teranet_Ontario_Lambert = 5320,
	Nad83_Maine_East_ftUS = 26847,
	Nad83_Maine_West_ftUS = 26848,
	Nad83_Minnesota_North_ftUS = 26849,
	Nad83_Minnesota_Central_ftUS = 26850,
	Nad83_Minnesota_South_ftUS = 26851,
	Nad83_Nebraska_ftUS = 26852,
	Nad83_West_Virginia_North_ftUS = 26853,
	Nad83_West_Virginia_South_ftUS = 26854,
	Nad83_UTM_zone_59N = 3372,
	Nad83_UTM_zone_60N = 3373,
	Nad83_UTM_zone_1N = 26901,
	Nad83_UTM_zone_2N = 26902,
	Nad83_UTM_zone_3N = 26903,
	Nad83_UTM_zone_4N = 26904,
	Nad83_UTM_zone_5N = 26905,
	Nad83_UTM_zone_6N = 26906,
	Nad83_UTM_zone_7N = 26907,
	Nad83_UTM_zone_8N = 26908,
	Nad83_UTM_zone_9N = 26909,
	Nad83_UTM_zone_10N = 26910,
	Nad83_UTM_zone_11N = 26911,
	Nad83_UTM_zone_12N = 26912,
	Nad83_UTM_zone_13N = 26913,
	Nad83_UTM_zone_14N = 26914,
	Nad83_UTM_zone_15N = 26915,
	Nad83_UTM_zone_16N = 26916,
	Nad83_UTM_zone_17N = 26917,
	Nad83_UTM_zone_18N = 26918,
	Nad83_UTM_zone_19N = 26919,
	Nad83_UTM_zone_20N = 26920,
	Nad83_UTM_zone_21N = 26921,
	Nad83_UTM_zone_22N = 26922,
	Nad83_UTM_zone_23N = 26923,
	Nad83_BLM_14N_ftUS = 32164,
	Nad83_BLM_15N_ftUS = 32165,
	Nad83_BLM_16N_ftUS = 32166,
	Nad83_BLM_17N_ftUS = 32167,
	Nad83_Alabama_East = 26929,
	Nad83_Alabama_West = 26930,
	Nad83_Alaska_zone_1 = 26931,
	Nad83_Alaska_zone_2 = 26932,
	Nad83_Alaska_zone_3 = 26933,
	Nad83_Alaska_zone_4 = 26934,
	Nad83_Alaska_zone_5 = 26935,
	Nad83_Alaska_zone_6 = 26936,
	Nad83_Alaska_zone_7 = 26937,
	Nad83_Alaska_zone_8 = 26938,
	Nad83_Alaska_zone_9 = 26939,
	Nad83_Alaska_zone_10 = 26940,
	Nad83_California_zone_1 = 26941,
	Nad83_California_zone_2 = 26942,
	Nad83_California_zone_3 = 26943,
	Nad83_California_zone_4 = 26944,
	Nad83_California_zone_5 = 26945,
	Nad83_California_zone_6 = 26946,
	Nad83_Arizona_East = 26948,
	Nad83_Arizona_Central = 26949,
	Nad83_Arizona_West = 26950,
	Nad83_Arkansas_North = 26951,
	Nad83_Arkansas_South = 26952,
	Nad83_Colorado_North = 26953,
	Nad83_Colorado_Central = 26954,
	Nad83_Colorado_South = 26955,
	Nad83_Connecticut = 26956,
	Nad83_Delaware = 26957,
	Nad83_Florida_East = 26958,
	Nad83_Florida_West = 26959,
	Nad83_Florida_North = 26960,
	Nad83_Hawaii_zone_1 = 26961,
	Nad83_Hawaii_zone_2 = 26962,
	Nad83_Hawaii_zone_3 = 26963,
	Nad83_Hawaii_zone_4 = 26964,
	Nad83_Hawaii_zone_5 = 26965,
	Nad83_Georgia_East = 26966,
	Nad83_Georgia_West = 26967,
	Nad83_Idaho_East = 26968,
	Nad83_Idaho_Central = 26969,
	Nad83_Idaho_West = 26970,
	Nad83_Illinois_East = 26971,
	Nad83_Illinois_West = 26972,
	Nad83_Indiana_East = 26973,
	Nad83_Indiana_West = 26974,
	Nad83_Iowa_North = 26975,
	Nad83_Iowa_South = 26976,
	Nad83_Kansas_North = 26977,
	Nad83_Kansas_South = 26978,
	Nad83_Kentucky_South = 26980,
	Nad83_Louisiana_North = 26981,
	Nad83_Louisiana_South = 26982,
	Nad83_Maine_East = 26983,
	Nad83_Maine_West = 26984,
	Nad83_Maryland = 26985,
	Nad83_Massachusetts_Mainland = 26986,
	Nad83_Massachusetts_Island = 26987,
	Nad83_Michigan_North = 26988,
	Nad83_Michigan_Central = 26989,
	Nad83_Michigan_South = 26990,
	Nad83_Minnesota_North = 26991,
	Nad83_Minnesota_Central = 26992,
	Nad83_Minnesota_South = 26993,
	Nad83_Mississippi_East = 26994,
	Nad83_Mississippi_West = 26995,
	Nad83_Missouri_East = 26996,
	Nad83_Missouri_Central = 26997,
	Nad83_Missouri_West = 26998,
	Nad83_Montana = 32100,
	Nad83_Nebraska = 32104,
	Nad83_Nevada_East = 32107,
	Nad83_Nevada_Central = 32108,
	Nad83_Nevada_West = 32109,
	Nad83_New_Hampshire = 32110,
	Nad83_New_Jersey = 32111,
	Nad83_New_Mexico_East = 32112,
	Nad83_New_Mexico_Central = 32113,
	Nad83_New_Mexico_West = 32114,
	Nad83_New_York_East = 32115,
	Nad83_New_York_Central = 32116,
	Nad83_New_York_West = 32117,
	Nad83_New_York_Long_Island = 32118,
	Nad83_North_Carolina = 32119,
	Nad83_North_Dakota_North = 32120,
	Nad83_North_Dakota_South = 32121,
	Nad83_Ohio_North = 32122,
	Nad83_Ohio_South = 32123,
	Nad83_Oklahoma_North = 32124,
	Nad83_Oklahoma_South = 32125,
	Nad83_Oregon_North = 32126,
	Nad83_Oregon_South = 32127,
	Nad83_Pennsylvania_North = 32128,
	Nad83_Pennsylvania_South = 32129,
	Nad83_Rhode_Island = 32130,
	Nad83_South_Carolina = 32133,
	Nad83_South_Dakota_North = 32134,
	Nad83_South_Dakota_South = 32135,
	Nad83_Tennessee = 32136,
	Nad83_Texas_North = 32137,
	Nad83_Texas_North_Central = 32138,
	Nad83_Texas_Central = 32139,
	Nad83_Texas_South_Central = 32140,
	Nad83_Texas_South = 32141,
	Nad83_Utah_North = 32142,
	Nad83_Utah_Central = 32143,
	Nad83_Utah_South = 32144,
	Nad83_Vermont = 32145,
	Nad83_Virginia_North = 32146,
	Nad83_Virginia_South = 32147,
	Nad83_Washington_North = 32148,
	Nad83_Washington_South = 32149,
	Nad83_West_Virginia_North = 32150,
	Nad83_West_Virginia_South = 32151,
	Nad83_Wisconsin_North = 32152,
	Nad83_Wisconsin_Central = 32153,
	Nad83_Wisconsin_South = 32154,
	Nad83_Wyoming_East = 32155,
	Nad83_Wyoming_East_Central = 32156,
	Nad83_Wyoming_West_Central = 32157,
	Nad83_Wyoming_West = 32158,
	Nad83_Puerto_Rico_and_Virgin_Is = 32161,
	Nad83_SCoPQ_zone_2 = 32180,
	Nad83_MTM_zone_1 = 32181,
	Nad83_MTM_zone_2 = 32182,
	Nad83_MTM_zone_3 = 32183,
	Nad83_MTM_zone_4 = 32184,
	Nad83_MTM_zone_5 = 32185,
	Nad83_MTM_zone_6 = 32186,
	Nad83_MTM_zone_7 = 32187,
	Nad83_MTM_zone_8 = 32188,
	Nad83_MTM_zone_9 = 32189,
	Nad83_MTM_zone_10 = 32190,
	Nad83_MTM_zone_11 = 32191,
	Nad83_MTM_zone_12 = 32192,
	Nad83_MTM_zone_13 = 32193,
	Nad83_MTM_zone_14 = 32194,
	Nad83_MTM_zone_15 = 32195,
	Nad83_MTM_zone_16 = 32196,
	Nad83_MTM_zone_17 = 32197,
	Nad83_Quebec_Lambert = 32198,
	Nad83_Louisiana_Offshore = 32199
};
enum GridDataType
{
	ShortDataType = 0,
	LongDataType = 1,
	FloatDataType = 2,
	DoubleDataType = 3,
	InvalidDataType = -1,
	UnknownDataType = 4,
	ByteDataType = 5
};
enum GridFileType
{
	Ascii = 0,
	Binary = 1,
	Esri = 2,
	GeoTiff = 3,
	Sdts = 4,
	PAux = 5,
	PCIDsk = 6,
	DTed = 7,
	Bil = 8,
	Ecw = 9,
	MrSid = 10,
	Flt = 11,
	Dem = 12,
	UseExtension = 13,
	InvalidGridFileType = -1
};
enum tkGridSourceType
{
	gstUninitialized = 0,
	gstGDALBased = 1,
	gstNative = 2
};
enum tkGridProxyMode
{
	gpmAuto = 0,
	gpmUseProxy = 1,
	gpmNoProxy = 2
};
enum tkGridSchemeRetrieval
{
	gsrAuto = 0,
	gsrDiskBased = 1,
	gsrDiskBasedForProxy = 2,
	gsrGdalColorTable = 3
};
enum tkGridSchemeGeneration
{
	gsgGradient = 0,
	gsgUniqueValues = 1,
	gsgUniqueValuesOrGradient = 2
};
enum tkFillType
{
	ftStandard = 0,
	ftHatch = 1,
	ftGradient = 2,
	ftPicture = 3
};
enum tkGradientType
{
	gtLinear = 0,
	gtRectangular = 1,
	gtCircle = 2
};
enum tkPointSymbolType
{
	ptSymbolStandard = 0,
	ptSymbolFontCharacter = 1,
	ptSymbolPicture = 2
};
enum tkGradientBounds
{
	gbWholeLayer = 0,
	gbPerShape = 1
};
enum tkVertexType
{
	vtSquare = 0,
	vtCircle = 1
};
enum tkLineType
{
	lltSimple = 0,
	lltMarker = 1
};
enum tkDefaultPointSymbol
{
	dpsSquare = 0,
	dpsCircle = 1,
	dpsDiamond = 2,
	dpsTriangleUp = 3,
	dpsTriangleDown = 4,
	dpsTriangleLeft = 5,
	dpsTriangleRight = 6,
	dpsCross = 7,
	dpsXCross = 8,
	dpsStar = 9,
	dpsPentagon = 10,
	dpsArrowUp = 11,
	dpsArrowDown = 12,
	dpsArrowLeft = 13,
	dpsArrowRight = 14,
	dpsAsterisk = 15,
	dpsFlag = 16
};
enum tkAreaDisplayMode
{
	admMetric = 0,
	admHectars = 1,
	admAmerican = 2
};
enum tkBearingType
{
	btAbsolute = 0,
	btReducedNDE = 1,
	btReducedNED = 2,
	btRelative = 3,
	btLeftAngle = 4,
	btRightAngle = 5
};
enum tkLayerSelection
{
	lsAllLayers = 0,
	lsNoLayer = 1,
	lsActiveLayer = 2
}; 
enum tkEditorState
{
	esNone = 0,
	esDigitize = 1,
	esEdit = 2,
	esDigitizeUnbound = 3,
	esOverlay = 4
};
enum tkLengthDisplayMode
{
	ldmMetric = 0,
	ldmAmerican = 1
};
enum tkEditorValidation
{
	evBasic = 0,
	evCheckWithGeos = 1,
	evFixWithGeos = 2
};
enum tkEditorOverlay
{
	eoAddPart = 0,
	eoRemovePart = 1
};
enum tkEditorBehavior
{
	ebVertexEditor = 0,
	ebPartEditor = 1
};
enum tkAngleFormat
{
	afDegrees = 0,
	afMinutes = 1,
	afSeconds = 2,
	afRadians = 3
};
enum tkCursorMode
{
	cmZoomIn = 0,
	cmZoomOut = 1,
	cmPan = 2,
	cmSelection = 3,
	cmNone = 4,
	cmMeasure = 5,
	cmAddShape = 6,
	cmEditShape = 8,
	cmSplitByPolyline = 9,
	cmIdentify = 10,
	cmMoveShapes = 11,
	cmRotateShapes = 12,
	cmSelectByPolygon = 13,
	cmEraseByPolygon = 14,
	cmSplitByPolygon = 15,
	cmClipByPolygon = 16
};
enum tkCursor
{
	crsrMapDefault = 0,
	crsrAppStarting = 1,
	crsrArrow = 2,
	crsrCross = 3,
	crsrHelp = 4,
	crsrIBeam = 5,
	crsrNo = 6,
	crsrSizeAll = 7,
	crsrSizeNESW = 8,
	crsrSizeNS = 9,
	crsrSizeNWSE = 10,
	crsrSizeWE = 11,
	crsrUpArrow = 12,
	crsrWait = 13,
	crsrUserDefined = 14,
	crsrHand = 15
};
enum tkLockMode
{
	lmUnlock = 0,
	lmLock = 1
};
enum tkUnitsOfMeasure
{
	umDecimalDegrees = 0,
	umMiliMeters = 1,
	umCentimeters = 2,
	umInches = 3,
	umFeets = 4,
	umYards = 5,
	umMeters = 6,
	umMiles = 7,
	umKilometers = 8
};
enum tkScalebarUnits
{
	Metric = 0,
	American = 1,
	GoogleStyle = 2
};
enum tkTileProvider
{
	ProviderNone = -1,
	OpenStreetMap = 0,
	OpenCycleMap = 1,
	OpenTransportMap = 2,
	BingMaps = 3,
	BingSatellite = 4,
	BingHybrid = 5,
	GoogleMaps = 6,
	GoogleSatellite = 7,
	GoogleHybrid = 8,
	GoogleTerrain = 9,
	HereMaps = 10,
	HereSatellite = 11,
	HereHybrid = 12,
	HereTerrain = 13,
	Rosreestr = 21,
	OpenHumanitarianMap = 22,
	MapQuestAerial = 23,
	ProviderCustom = 1024
};
enum tkCacheType
{
	RAM = 0,
	Disk = 1,
	Both = 2
};
enum tkTileProjection
{
	SphericalMercator = 0,
	Amersfoort = 1
};
enum tkTilesProjectionStatus
{
	tpsNative = 0,
	tpsCompatible = 1,
	tpsEmptyOrInvalid = 2
};
enum tkProxyAuthentication
{
	asBasic = 0,
	asNtlm = 1
};
enum tkFileOpenStrategy
{
	fosNotSupported = -1,
	fosAutoDetect = 0,
	fosRgbImage = 1,
	fosDirectGrid = 2,
	fosProxyForGrid = 3,
	fosVectorLayer = 4,
	fosVectorDatasource = 5
};
enum FieldType
{
	STRING_FIELD = 0,
	INTEGER_FIELD = 1,
	DOUBLE_FIELD = 2
};
enum tkCategoryValue
{
	cvSingleValue = 0,
	cvRange = 1,
	cvExpression = 2
};
enum tkShapeElements
{
	shElementDefault = 0,
	shElementFill = 1,
	shElementFill2 = 2,
	shElementLines = 3,
	shElementFillBackground = 4
};
enum tkChartType
{
	chtBarChart = 0,
	chtPieChart = 1
};
enum tkChartValuesStyle
{
	vsHorizontal = 0,
	vsVertical = 1
};
enum tkValueType
{
	vtDouble = 0,
	vtString = 1,
	vtBoolean = 2,
	vtFloatArray = 3
};
enum tkShapefileSourceType
{
	sstUninitialized = 0,
	sstDiskBased = 1,
	sstInMemory = 2
};
enum tkGeometryEngine
{
	engineGeos = 0,
	engineClipper = 1
};
enum tkSelectionAppearance
{
	saSelectionColor = 0,
	saDrawingOptions = 1
};
enum tkCollisionMode
{
	AllowCollisions = 0,
	LocalList = 1,
	GlobalList = 2
};
enum tkShapeValidationType
{
	svtInput = 0,
	svtOutput = 1
};
enum tkShapeValidationMode
{
	NoValidation = 0,
	TryFixProceedOnFailure = 1,
	TryFixSkipOnFailure = 2,
	TryFixAbortOnFailure = 3,
	AbortOnErrors = 4
};
enum tkShapeValidationStatus
{
	WasntValidated = 0,
	Valid = 1,
	InvalidFixed = 2,
	InvalidSkipped = 3,
	InvalidReturned = 4,
	OperationAborted = 5
};
enum tkFieldStatOperation
{
	fsoSum = 0,
	fsoMin = 1,
	fsoMax = 2,
	fsoAvg = 3,
	fsoWeightedAvg = 4,
	fsoMode = 5
};
enum tkFieldOperationValidity
{
	fovValid = 0,
	fovFieldNotFound = 1,
	fovNotSupported = 2
};
enum tkSupportType
{
	stGdal = 0,
	stGdalOverviews = 1
};
enum tkOgrSaveType
{
	ostGeometryOnly = 0,
	ostAttributesOnly = 1,
	ostSaveAll = 2
};
enum tkOgrSaveResult
{
	osrNoChanges = 0,
	osrAllSaved = 1,
	osrSomeSaved = 2,
	osrNoneSaved = 3
};
enum tkOgrLayerCapability
{
	olcRandomRead = 0,
	olcSequentialWrite = 1,
	olcRandomWrite = 2,
	olcFastSpatialFilter = 3,
	olcFastFeatureCount = 4,
	olcFastGetExtent = 5,
	olcCreateField = 6,
	olcDeleteField = 7,
	olcReorderFields = 8,
	olcAlterFieldDefn = 9,
	olcTransactions = 10,
	olcDeleteFeature = 11,
	olcFastSetNextByIndex = 12,
	olcStringsAsUTF8 = 13,
	olcIgnoreFields = 14,
	olcCreateGeomField = 15
};
enum tkOgrSourceType
{
	ogrUninitialized = 0,
	ogrFile = 1,
	ogrDbTable = 2,
	ogrQuery = 3
};
enum tkOgrDSCapability
{
	odcCreateLayer = 0,
	odcDeleteLayer = 1,
	odcCreateGeomFieldAfterCreateLayer = 2,
	odcCreateDataSource = 3,
	odcDeleteDataSource = 4
};
enum tkGdalDriverMetadata
{
	dmdLONGNAME = 0,
	dmdHELPTOPIC = 1,
	dmdMIMETYPE = 2,
	dmdEXTENSION = 3,
	dmdEXTENSIONS = 4,
	dmdCREATIONOPTIONLIST = 5,
	dmdOPENOPTIONLIST = 6,
	dmdCREATIONDATATYPES = 7,
	dmdSUBDATASETS = 8,
	dmdOPEN = 9,
	dmdCREATE = 10,
	dmdCREATECOPY = 11,
	dmdVIRTUALIO = 12,
	dmdLAYER_CREATIONOPTIONLIST = 13,
	dmdOGR_DRIVER = 14
};
enum tkUndoOperation
{
	uoAddShape = 0,
	uoRemoveShape = 1,
	uoEditShape = 2,
	uoMoveShapes = 3,
	uoRotateShapes = 4
};
enum tkIdentifierMode
{
	imAllLayers = 0,
	imSingleLayer = 1
};
enum tkMeasuringType
{
	MeasureDistance = 0,
	MeasureArea = 1
};
enum tkUndoShortcut
{
	usNone = 0,
	usCtrlZ = 1,
	usRightMouseButton = 2
};
enum tkCustomDrawingFlags
{
	CustomDrawingNone = 0,
	OnDrawBackBufferHdc = 1,
	OnDrawBackBufferBitmapData = 2,
	BeforeAfterDrawing = 4
};
enum tkRedrawType
{
	RedrawAll = 1,
	RedrawSkipDataLayers = 2,
	RedrawMinimal = 3,
	RedrawSkipAllLayers = 4
};
enum tkMapProjection
{
	PROJECTION_CUSTOM = -1,
	PROJECTION_NONE = 0,
	PROJECTION_WGS84 = 1,
	PROJECTION_GOOGLE_MERCATOR = 2
};
enum tkKnownExtents
{
	keSiliconValley = -3,
	keGreenwich = -2,
	keNone = -1,
	keWorld = 0,
	keAfghanistan = 1,
	keAland_Islands = 2,
	keAlbania = 3,
	keAlgeria = 4,
	keAmerican_Samoa = 5,
	keAndorra = 6,
	keAngola = 7,
	keAnguilla = 8,
	keAntigua = 9,
	keArgentina = 10,
	keArmenia = 11,
	keAruba = 12,
	keAustralia = 13,
	keAustria = 14,
	keAzerbaijan = 15,
	keBahamas = 16,
	keBahrain = 17,
	keBangladesh = 18,
	keBarbados = 19,
	keBelarus = 20,
	keBelgium = 21,
	keBelize = 22,
	keBenin = 23,
	keBermuda = 24,
	keBhutan = 25,
	keBolivia = 26,
	keBosnia_and_Herzegovina = 27,
	keBotswana = 28,
	keBrazil = 29,
	keBritish_Virgin_Islands = 30,
	keBrunei = 31,
	keBulgaria = 32,
	keBurkina_Faso = 33,
	keBurundi = 34,
	keCambodia = 35,
	keCameroon = 36,
	keCanada = 37,
	keCape_Verde = 38,
	keCayman_Islands = 39,
	keCentral_African_Republic = 40,
	keChad = 41,
	keChile = 42,
	keChina = 43,
	keCocos_Islands = 44,
	keColombia = 45,
	keComoros = 46,
	keCongo = 47,
	keCook_Islands = 48,
	keCosta_Rica = 49,
	keCroatia = 50,
	keCuba = 51,
	keCyprus = 52,
	keCzech_Republic = 53,
	keDenmark = 54,
	keDjibouti = 55,
	keDominica = 56,
	keDominican_Republic = 57,
	keDR_Congo = 58,
	keEcuador = 59,
	keEgypt = 60,
	keEl_Salvador = 61,
	keEquatorial_Guinea = 62,
	keEritrea = 63,
	keEstonia = 64,
	keEthiopia = 65,
	keFaeroe_Islands = 66,
	keFalkland_Islands = 67,
	keFiji = 68,
	keFinland = 69,
	keFrance = 70,
	keFrench_Guiana = 71,
	keFrench_Polynesia = 72,
	keGabon = 73,
	keGambia = 74,
	keGeorgia = 75,
	keGermany = 76,
	keGhana = 77,
	keGibraltar = 78,
	keGreat_Britain = 79,
	keGreece = 80,
	keGreenland = 81,
	keGrenada = 82,
	keGuadeloupe = 83,
	keGuam = 84,
	keGuatemala = 85,
	keGuernsey = 86,
	keGuinea = 87,
	keGuinea_Bissau = 88,
	keGuyana = 89,
	keHaiti = 90,
	keHonduras = 91,
	keHungary = 92,
	keIceland = 93,
	keIndia = 94,
	keIndonesia = 95,
	keIran = 96,
	keIraq = 97,
	keIreland = 98,
	keIsle_of_Man = 99,
	keIsrael = 100,
	keItaly = 101,
	keIvory_Coast = 102,
	keJamaica = 103,
	keJapan = 104,
	keJersey = 105,
	keJordan = 106,
	keKazakhstan = 107,
	keKenya = 108,
	keKiribati = 109,
	keKuwait = 110,
	keKyrgyzstan = 111,
	keLaos = 112,
	keLatvia = 113,
	keLebanon = 114,
	keLesotho = 115,
	keLiberia = 116,
	keLibya = 117,
	keLiechtenstein = 118,
	keLithuania = 119,
	keLuxembourg = 120,
	keMacao = 121,
	keMacedonia = 122,
	keMadagascar = 123,
	keMalawi = 124,
	keMalaysia = 125,
	keMaldives = 126,
	keMali = 127,
	keMalta = 128,
	keMarshall_Islands = 129,
	keMartinique = 130,
	keMauritania = 131,
	keMauritius = 132,
	keMayotte = 133,
	keMexico = 134,
	keMicronesia = 135,
	keMoldova = 136,
	keMonaco = 137,
	keMongolia = 138,
	keMontenegro = 139,
	keMontserrat = 140,
	keMorocco = 141,
	keMozambique = 142,
	keNamibia = 143,
	keNauru = 144,
	keNepal = 145,
	keNetherlands = 146,
	keNew_Caledonia = 147,
	keNew_Zealand = 148,
	keNicaragua = 149,
	keNiger = 150,
	keNigeria = 151,
	keNiue = 152,
	keNorfolk_Island = 153,
	keNorth_Korea = 154,
	keNorthern_Mariana_Islands = 155,
	keNorway = 156,
	keOccupied_Palestinian_Territory = 157,
	keOman = 158,
	kePakistan = 159,
	kePalau = 160,
	kePanama = 161,
	kePapua_New_Guinea = 162,
	keParaguay = 163,
	kePeru = 164,
	kePhilippines = 165,
	kePitcairn = 166,
	kePoland = 167,
	kePortugal = 168,
	kePuerto_Rico = 169,
	keQatar = 170,
	keReunion_Island = 171,
	keRomania = 172,
	keRussia = 173,
	keRwanda = 174,
	keSaint_Barthelemy = 175,
	keSaint_Martin_French_part = 176,
	keSamoa = 177,
	keSan_Marino = 178,
	keSao_Tome_and_Principe = 179,
	keSaudi_Arabia = 180,
	keSenegal = 181,
	keSerbia = 182,
	keSeychelles = 183,
	keSierra_Leone = 184,
	keSingapore = 185,
	keSlovakia = 186,
	keSlovenia = 187,
	keSolomon_Islands = 188,
	keSomalia = 189,
	keSouth_Africa = 190,
	keSouth_Korea = 191,
	keSpain = 192,
	keSri_Lanka = 193,
	keSt_Helena = 194,
	keSt_Kitts_and_Nevis = 195,
	keSt_Lucia = 196,
	keSt_Pierre_and_Miquelon = 197,
	keSt_Vincent = 198,
	keSudan = 199,
	keSuriname = 200,
	keSvalbard_and_Jan_Mayen = 201,
	keSwaziland = 202,
	keSweden = 203,
	keSwitzerland = 204,
	keSyria = 205,
	keTajikistan = 206,
	keTanzania = 207,
	keThailand = 208,
	keTimor_Leste = 209,
	keTogo = 210,
	keTokelau = 211,
	keTonga = 212,
	keTrinidad_and_Tobago = 213,
	keTunisia = 214,
	keTurkey = 215,
	keTurkmenistan = 216,
	keTurks_and_Caicos_Islands = 217,
	keTuvalu = 218,
	keUganda = 219,
	keUkraine = 220,
	keUnited_Arab_Emirates = 221,
	keUruguay = 222,
	keUS_Virgin_Islands = 223,
	keUSA = 224,
	keUzbekistan = 225,
	keVanuatu = 226,
	keVenezuela = 227,
	keVietnam = 228,
	keWallis_and_Futuna = 229,
	keWestern_Sahara = 230,
	keYemen = 231,
	keZambia = 232,
	keZimbabwe = 233
};
enum tkDrawReferenceList
{
	dlScreenReferencedList = 0,
	dlSpatiallyReferencedList = 1
};
enum tkLineStipple
{
	lsNone = 0,
	lsDotted = 1,
	lsDashed = 2,
	lsDashDotDash = 3,
	lsDoubleSolid = 4,
	lsDoubleSolidPlusDash = 5,
	lsTrainTracks = 6,
	lsCustom = 7,
	lsDashDotDot = 8
};
enum tkFillStipple
{
	fsNone = 0,
	fsVerticalBars = 1,
	fsHorizontalBars = 2,
	fsDiagonalDownRight = 3,
	fsDiagonalDownLeft = 4,
	fsPolkaDot = 5,
	fsCustom = 6,
	fsCross = 7,
	fsRaster = 8
};
enum tkPointType
{
	ptSquare = 0,
	ptCircle = 1,
	ptDiamond = 2,
	ptTriangleUp = 3,
	ptTriangleDown = 4,
	ptTriangleLeft = 5,
	ptTriangleRight = 6,
	ptUserDefined = 7,
	ptImageList = 8,
	ptFontChar = 9
};
enum tkResizeBehavior
{
	rbClassic = 0,
	rbModern = 1,
	rbIntuitive = 2,
	rbWarp = 3,
	rbKeepScale = 4
};
enum tkCoordinatesDisplay
{
	cdmNone = 0,
	cdmAuto = 1,
	cdmDegrees = 2,
	cdmMapUnits = 3
};
enum tkCustomState
{
	csAuto = 0,
	csTrue = 1,
	csFalse = 2
};
enum tkZoomBarVerbosity
{
	zbvZoomOnly = 0,
	zbvFull = 1,
	zbvNone = 2
};
enum tkZoomBehavior
{
	zbDefault = 0,
	zbUseTileLevels = 1
};
enum tkZoomBoxStyle
{
	zbsRubberBand = 0,
	zbsGray = 1,
	zbsGrayInverted = 2,
	zbsOrange = 3,
	zbsBlue = 4
};
enum tkMwBoolean
{
	blnFalse = 0,
	blnTrue = 1
};
enum tkMeasuringAction
{
	PointAdded = 0,
	PointRemoved = 1,
	MesuringStopped = 2
};
enum tkDeleteTarget
{
	dtNone = 0,
	dtShape = 1,
	dtPart = 2,
	dtVertex = 3
};
enum AmbiguityResolution
{
	Z_VALUE = 0,
	DISTANCE_TO_OUTLET = 1,
	NO_RESOLUTION = 2
};
enum PolygonOperation
{
	DIFFERENCE_OPERATION = 0,
	INTERSECTION_OPERATION = 1,
	EXCLUSIVEOR_OPERATION = 2,
	UNION_OPERATION = 3
};
enum SplitMethod
{
	InscribedRadius = 0,
	AngleDeviation = 1
};
enum tkShapeDrawingMethod
{
	drawing1 = 0,
    drawing2 = 1
};
enum tkInterface
{
	idChart = 0,
	idChartField = 1,
	idCharts = 2,
	idColorScheme = 3,
	idESRIGridManager = 4,
	idExtents = 5,
	idField = 6,
	idGeoProjection = 7,
	idGlobalSettings = 8,
	idGrid = 9,
	idGridColorBreak = 10,
	idGridColorScheme = 11,
	idGridHeader = 12,
	idImage = 13,
	idLabelCategory = 14,
	idLabel = 15,
	idLabels = 16,
	idLinePattern = 17,
	idLineSegment = 18,
	idPoint = 19,
	idShape = 20,
	idShapeDrawingOptions = 21,
	idShapefile = 22,
	idShapefileCategories = 23,
	idShapefileCategory = 24,
	idShapefileColorBreak = 25,
	idShapefileColorScheme = 26,
	idShapeNetwork = 27,
	idTable = 28,
	idTileProviders = 29,
	idTiles = 30,
	idTin = 31,
	idUtils = 32,
	idVector = 33,
	idShapeValidationInfo = 34,
	idMeasuring = 35,
	idFileManager = 36,
	idFieldStatOperations = 37,
	idShapeEditor = 38,
	idOgrDatasource = 39,
	idOgrLayer = 40,
	idUndoList = 41,
	idIdentifier = 42
};
enum tkGridProxyFormat
{
	gpfBmpProxy = 0,
	gpfTiffProxy = 1
};
enum tkGdalError
{
	GdalErrorNone = 0,
	GdalErrorAppDefined = 1,
	GdalErrorOutOfMemory = 2,
	GdalErrorFileIO = 3,
	GdalErrorOpenFailed = 4,
	GdalErrorIllegalArg = 5,
	GdalErrorNotSupported = 6,
	GdalErrorAssertionFailed = 7,
	GdalErrorNoWriteAccess = 8,
	GdalErrorUserInterrupt = 9,
	GdalErrorObjectNull = 10,
	GdalErrorUndefined = 11
};
enum tkGdalErrorType
{
	GdalNone = 0,
	GdalDebug = 1,
	GdalWarning = 2,
	GdalFailure = 3,
	GdalFatal = 4,
	GdalUndefined = 5
};
enum tkSmoothingMode
{
	DefaultMode = 0,
	HighSpeedMode = 1,
	HighQualityMode = 2,
	None = 3,
	AntiAlias = 4
};
enum tkCompositingQuality
{
	Default = 0,
	HighSpeed = 1,
	HighQuality = 2,
	GammaCorrected = 3,
	AssumeLinear = 4
};
enum tkLocalizedStrings
{
	lsHectars = 0,
	lsMeters = 1,
	lsKilometers = 2,
	lsSquareKilometers = 3,
	lsSquareMeters = 4,
	lsMapUnits = 5,
	lsSquareMapUnits = 6,
	lsMiles = 7,
	lsFeet = 8,
	lsLatitude = 9,
	lsLongitude = 10,
	lsRadians = 11,
	lsNorthEast = 12,
	lsSouthEast = 13,
	lsSouthWest = 14,
	lsNorthWest = 15,
	lsNorth = 16,
	lsEast = 17,
	lsSouth = 18,
	lsWest = 19,
	lsAcres = 20,
	lsSquareFeet = 21,
	lsSquareMiles = 22
};
enum tkRasterOverviewCreation
{
	rocAuto = 0,
	rocYes = 1,
	rocNo = 2
};
enum tkTiffCompression
{
	tkmAUTO = -1,
	tkmJPEG = 0,
	tkmLZW = 1,
	tkmPACKBITS = 2,
	tkmDEFLATE = 3,
	tkmCCITTRLE = 4,
	tkmCCITTFAX3 = 5,
	tkmCCITTFAX4 = 6,
	tkmNONE = 7
};
enum tkOgrEncoding
{
	oseUtf8 = 0,
	oseAnsi = 1
};
enum tkPixelOffsetMode
{
	pomDefault = 0,
	pomHighPerformance = 1,
	pomHighQuality = 2
};

enum tkResamplingType
{
	rtNone = 0,
	rtLinear = 1,
	rtCubic = 2,
	rtLanczos = 3,
};



# endif

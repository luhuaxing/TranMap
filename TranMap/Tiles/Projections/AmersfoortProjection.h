
#pragma once
#include "CustomProjection.h"

// Amersfoort projection for Netherlands, EPSG 28992
class AmersfoortProjection: public CustomProjection
{
public:	
	// EPSG 28992
	AmersfoortProjection()
	{
		bool calculateDegrees = true;
		
		if (!calculateDegrees)
		{
			// let's just use precalculated values; it unlikely that proj4 calculation will ever change
			//Lat = -0.515879655998403; Lng = 48.0405185800458
			//Lat = -1.65729159493417; Lng = 55.9136415710084
			//Lat = 12.4317273490522; Lng = 55.9136222637686
			//Lat = 11.2902580269759; Lng = 48.0405018446845

			// there is some question however as to which values to take as we don't have 
			// a horizontally oriented rectangle; let's take the maximum bounds
			/*yMinLat = -0.515879655998403;
			yMaxLat = 12.4317273490522;
			xMinLng = 48.0405018446845;
			xMaxLng = 55.9136415710084;*/
		}
		else
		{
			// in meters 
			yMinLat = 22598.080;
			yMaxLat = 903401.920;
			xMinLng = -285401.920;
			xMaxLng = 595401.92;

			VARIANT_BOOL ret1, ret2;
			projWGS84->ImportFromEPSG(4326, &ret1);
			projCustom->ImportFromEPSG(28992, &ret2);

			if (!ret1 || !ret2)
				CallbackHelper::ErrorMsg("Failed to initialize projection for Amersfoort.");

			projWGS84->StartTransform(projCustom, &ret1);
			if (!ret1)
				CallbackHelper::ErrorMsg("Failed to initialize WGS84 to Amersfoort transformation.");

			projCustom->StartTransform(projWGS84, &ret2);
			if (!ret2)
				CallbackHelper::ErrorMsg("Failed to initialize Amersfoort to WGS84 transformation.");

			MinLatitude = yMinLat;
			MaxLatitude = yMaxLat;
			MinLongitude = xMinLng;
			MaxLongitude = xMaxLng;
			projCustom->Transform(&MinLongitude, &MinLatitude, &ret1);
			projCustom->Transform(&MaxLongitude, &MaxLatitude, &ret2);
		}

		yInverse = true;
		worldWide = false;

		serverProjection = tkTileProjection::Amersfoort;
	}
};
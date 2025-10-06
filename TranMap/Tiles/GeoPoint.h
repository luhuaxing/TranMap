
#pragma once

// Point on globe with longitude and latitude in decimal degrees
class PointLatLng
{
public:
	PointLatLng(double lat, double lng): Lat(lat), Lng(lng) {};
	PointLatLng(): Lat(0.0), Lng(0.0) {};
	double Lat;
	double Lng;
};

// Size specified of rectangle specified in termas of longitude and latitude in decimal degrees
class SizeLatLng
{
public:
	SizeLatLng(double widthLng, double heightLat): WidthLng(widthLng), HeightLat(heightLat) {};
	SizeLatLng(): WidthLng(0.0), HeightLat(0.0) {};
	double WidthLng;
	double HeightLat;
};
// A rectangle specified in terms of latitude and longitude in decimal degrees
struct RectLatLng
{
public:
	double xLng;
	double yLat;
	double WidthLng;
	double HeightLat;

	double MinLat() { return yLat - HeightLat; }
	double MaxLng() { return xLng + WidthLng; }

	void SetBounds(double lng, double lat, double widthLng, double heightLat)
	{
		xLng = lng;
		yLat = lat;
		WidthLng = widthLng;
		HeightLat = heightLat;
	}

	RectLatLng(double lng, double lat, double widthLng, double heightLat): xLng(lng), yLat(lat), WidthLng(widthLng), HeightLat(heightLat){};
	RectLatLng(): xLng(0.0), yLat(0.0), WidthLng(0.0), HeightLat(0.0){};
};

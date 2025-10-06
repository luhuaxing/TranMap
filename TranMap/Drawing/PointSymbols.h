
#pragma once

void get_DefaultSymbolOptions(tkDefaultPointSymbol symbol, tkPointShapeType& type, double& rotation, int& numSides, float& sidesRatio);
float* get_SimplePointShape(tkPointShapeType type, float& size, double& rotation, int& numSides, float& sidesRatio, int* numPoints);
float* get_RegularShape(float sizeX, float sizeY, int numSides, double rotation);
float* get_StarShape(float sizeX, float sizeY, int numSides, float shapeRatio, double rotation);
float* get_CrossShape(float size, int numSides, float shapeRatio, double rotation);
float* get_ArrowShape(float sizeX, float sizeY, double rotation);
float* get_FlagShape(float sizeX, float sizeY, double rotation);
float* get_SimplePointShape(tkDefaultPointSymbol symbol, float size, int* numPoints);


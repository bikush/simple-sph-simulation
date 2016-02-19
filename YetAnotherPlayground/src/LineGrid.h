#pragma once
#include "Object3D.h"

class LineGrid : public Object3D
{
public:
	LineGrid( int xCount, float deltaX, float offsetX, int yCount, float deltaY, float offsetY);
	~LineGrid();
};


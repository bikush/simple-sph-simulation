#pragma once
#include "Object3D.h"

class LineGrid : public Object3D
{
private:
	float lineWidth;

public:
	LineGrid( int xCount, float deltaX, float offsetX, int yCount, float deltaY, float offsetY);
	~LineGrid();

	void draw(const glm::mat4& viewProjection);
};


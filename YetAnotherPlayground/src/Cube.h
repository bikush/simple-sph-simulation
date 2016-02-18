#pragma once

#include "Object3D.h"


class Cube : public Object3D
{
public:
	Cube();
	~Cube();
	static Cube* CreateCube(const glm::vec3& startPosition, const glm::vec3& startScale);
};


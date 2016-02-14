#pragma once
#include <vector>
#include "Scene.h"
#include "Camera.h"

class Stage
{
public:
	Stage();
	virtual ~Stage();

protected:
	std::vector<Scene> scenes;
	std::vector<Camera> cameras;

public:
	void addCamera(const Camera& camera);
	void addScene(const Scene& scenes);
};


#include "LyingShapesScene.h"
#include "Cube.h"
#include "Camera.h"

LyingShapesScene::LyingShapesScene()
{
	float size = 100.0f;
	float halfSize = size * 0.5f;
	float deltaX = size / 10.0f;
	float scale = deltaX / 3.0f;
	int i = 0;
	for (float x = -halfSize; x < halfSize + deltaX * 0.5f ; x += deltaX)
	{
		for (float z = -halfSize; z < halfSize + deltaX * 0.5f; z += deltaX)
		{
			cubes.push_back(new Cube());
			cubes[i]->setPosition({ x, 0.0f, z });
			cubes[i]->setScale(scale);
			i++;
		}
	}
}


LyingShapesScene::~LyingShapesScene()
{
	for (auto shapeIt = cubes.begin(); shapeIt != cubes.end(); shapeIt++)
	{
		delete (*shapeIt);
	}
	cubes.clear();
}

void LyingShapesScene::eventReshape(int width, int height)
{
	
}

void LyingShapesScene::draw(const Camera & camera)
{
	auto viewProjection = camera.getViewProjection();
	for (unsigned int i = 0; i<cubes.size(); i++)
	{
		cubes[i]->draw(viewProjection);
	}
}

#include "LyingShapesScene.h"
#include "Cube.h"
#include "Camera.h"
#include "LineGrid.h"
#include "Utility.h"
#include "MarchingCubesShaded.h"

#include <iostream>

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
			//for (float y = -halfSize; y < halfSize + deltaX * 0.5f; y += deltaX)
			//{
				float y = 0.0f;
				cubes.push_back(new Cube());
				cubes[i]->transform.setPosition({ x, y, z });
				cubes[i]->transform.setScale(scale);
				i++;
			//}
		}
	}
	std::cout << "Number of cubes: " << i << std::endl;
	std::cout << "Number of triangles: " << i * 12 << std::endl;
	std::cout << "Size of data: " << i * 8 * 6 * sizeof(float) << " bytes" << std::endl;

	grid = new LineGrid(10,10.0f,10.0f,10,10.0f,10.0f);
	grid->transform.setPosition({ -100.0f,0.0f,-100.0f });
	grid->transform.setAngles(90.0f, 0.f, 0.0f);

	coords = new LineGrid(25.0f, 3.0f);

	/*mCubes = new MarchingCubesShaded("data/mCubesShaded.txt");
	mCubes->putSphere(5.0f, 5.0f, 5.0f, 3.0f);
	mCubes->putSphere(0.0f, 5.0f, 5.0f, 7.0f);
	mCubes->transform.setPosition({ -5.0f,0.0f,-20.0f });
	mCubes->transform.setScale(mCubes->getScale());*/
}


LyingShapesScene::~LyingShapesScene()
{
	for (auto shapeIt = cubes.begin(); shapeIt != cubes.end(); shapeIt++)
	{
		delete (*shapeIt);
	}
	cubes.clear();

	safeDelete(&grid);
	safeDelete(&coords);
	safeDelete(&mCubes);
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
	if (grid != nullptr)
	{
		grid->draw(viewProjection);
	}
	if (coords != nullptr)
	{
		coords->draw(viewProjection);
	}
	if (mCubes != nullptr)
	{
		mCubes->draw(camera);
	}
}

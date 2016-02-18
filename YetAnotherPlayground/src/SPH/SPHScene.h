#pragma once
#include "Scene.h"
#include "IntervalAverageTimer.h"
#include <gl\glew.h>

class SPHSystem3d;
class PointDataVisualiser;
class MarchingCubesShaded;

class SPHScene :
	public Scene
{
public:
	SPHScene(void);
	virtual ~SPHScene(void);
	
	void draw(const Camera& camera);
	void update(float dt);
	virtual void eventKeyboardUp(sf::Keyboard::Key keyPressed);

private:
	SPHSystem3d* sph3;
	PointDataVisualiser* pdv;
	MarchingCubesShaded *mcs;

	// Light data
	GLfloat lightPosition[4];
	GLfloat lightDifuse[4];
	GLfloat lightAmbient[4];
	GLfloat lightSpecular[4];

	IntervalAverageTimer sphTimer;
	IntervalAverageTimer marchingTimer;
	bool drawWithMC;
	bool drawPDVWithShader;
	float treshold;

	bool paused;

	void initData();
	void initLight();
};


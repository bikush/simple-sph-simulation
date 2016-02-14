#pragma once
#include "WindowManager.h"
#include <gl\glew.h>
#include "Camera.h"

class SPHSystem3d;
class PointDataVisualiser;
class MarchingCubesShaded;
class IntervalAverageTimer;

class SPHWindowManager :
	public WindowManager
{
public:
	SPHWindowManager(void) : WindowManager("windowSettings.txt"), 
		camera( {1024.0f,768.0f}, Camera::ProjectionType::PERSPECTIVE, {1.0f,10000.0f} )
	{}
	virtual ~SPHWindowManager(void);

	static void open();

protected:
	virtual void windowWillRun();
	virtual void windowWillClose();
	virtual void drawScene();
	virtual void eventKeyboardDown(sf::Keyboard::Key keyPressed);
	virtual void eventKeyboardUp(sf::Keyboard::Key keyPressed);

private:
	SPHSystem3d* sph3;
	PointDataVisualiser* pdv;
	MarchingCubesShaded *mcs;
	Camera camera;

	// Light data
	GLfloat lightPosition[4];
	GLfloat lightDifuse[4];
	GLfloat lightAmbient[4];
	GLfloat lightSpecular[4];

	IntervalAverageTimer* sac;
	IntervalAverageTimer* mac;
	IntervalAverageTimer* rac;
	bool drawWithMC;
	bool drawPDVWithShader;
	float treshold;

	void initData();
	void initLight();
};


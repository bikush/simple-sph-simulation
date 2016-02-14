#pragma once
#include "Scene.h"
#include <vector>

class Shape;

class DistributedShapesScene :
	public Scene
{
public:
	DistributedShapesScene(void);
	~DistributedShapesScene(void);

	void eventReshape( int width, int height);
	void draw( const Camera& camera );

private:	
	std::vector<Shape*> shapes;
};


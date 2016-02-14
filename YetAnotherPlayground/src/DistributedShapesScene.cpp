#include "DistributedShapesScene.h"
#include "Shape.h"
#include "Camera.h"

DistributedShapesScene::DistributedShapesScene(void)
{
}


DistributedShapesScene::~DistributedShapesScene(void)
{	
	for( auto shapeIt = shapes.begin(); shapeIt != shapes.end(); shapeIt++ )
	{
		delete (*shapeIt);
	}
	shapes.clear();
}

void DistributedShapesScene::eventReshape( int width, int height)
{
	float origOffset = 100.0;
	float repeats = 10.0;
	float offset = glm::min<float>( origOffset, glm::min( width / repeats, height / repeats ));
	float deltaX = (width - offset * 2 - 1)/repeats;
	float deltaY = (height - offset * 2 - 1)/repeats;
	float scale = 20.0f * offset/origOffset;
	unsigned int i = 0;
	for( float x = offset; x < width - offset; x+=deltaX )
	{
		for( float y = offset;  y < height - offset; y+=deltaY )
		{
			if( i >= shapes.size() )
			{
				shapes.push_back( new Shape() );
			}
			shapes[i]->setPosition( glm::vec2( x, y ) );
			shapes[i]->setScale( scale );
			i++;
		}
	}
}

void DistributedShapesScene::draw(const Camera & camera)
{
	auto viewProjection = camera.getViewProjection();
	for( unsigned int i = 0; i<shapes.size(); i++ )
	{
		shapes[i]->draw( viewProjection );
	}
}

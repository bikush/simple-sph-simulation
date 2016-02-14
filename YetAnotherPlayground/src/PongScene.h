#pragma once
#ifndef _H_PongScene
#define _H_PongScene

#include "Scene.h"

class Shape;

class PongScene : public Scene
{
private:
	Shape* leftPaddle;
	Shape* rightPaddle;
	Shape* ball;

public:
	PongScene();
	~PongScene();

	void eventReshape( int width, int height);
	void eventKeyboardDown( sf::Keyboard::Key keyPressed );
	void eventKeyboardUp( sf::Keyboard::Key keyPressed );

	void draw(const Camera& camera);
};


#endif
#pragma once

#include <SFML/Graphics.hpp>
#include <glm/mat4x4.hpp>

class Camera;

class Scene
{
public:
	Scene(void){}
	virtual ~Scene(void){}

	virtual void eventReshape		( int width, int height){}
	virtual void eventMousePressed	( sf::Event::MouseButtonEvent mEvent ){}
	virtual void eventMouseMotion	( sf::Event::MouseMoveEvent mEvent){}
	virtual void eventMouseWheel	( sf::Event::MouseWheelEvent mEvent){}
	virtual void eventKeyboardDown	( sf::Keyboard::Key keyPressed ){}
	virtual void eventKeyboardUp	( sf::Keyboard::Key keyPressed ){}

	virtual void draw( const Camera& camera ){}
	virtual void update(float dt) {}
};


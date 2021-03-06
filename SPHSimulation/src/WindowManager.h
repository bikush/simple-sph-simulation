/*
 *	13.2.2014.
 *  Bruno
 */

#pragma once
#ifndef _H_WindowManager
#define _H_WindowManager

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>

#include <GL/glew.h>

class WindowManager
{
// Window management
protected:
	sf::RenderWindow* window;

public:
	sf::Window* getWindow();

// Creator and init methods
public:
	WindowManager(const std::string& configFile);
	virtual ~WindowManager(void);
	void run();

	static void open();
	volatile bool shouldClose;

// To be overridden
protected:
	virtual void windowWillRun();
	virtual void windowWillClose();
	virtual void updateScene( float dt );
	virtual void drawGLScene();
	virtual void drawSFMLScene(sf::RenderWindow& window);

private:
	void drawCycle();

protected:
	bool mouseLeft;
	bool mouseRight;
	int mouseLastX;
	int mouseLastY;

protected:
	// SFML event control
	virtual void eventReshape		( int width, int height);
	virtual void eventMousePressed	( sf::Event::MouseButtonEvent mEvent );
	virtual void eventMouseMotion	( sf::Event::MouseMoveEvent mEvent);
	virtual void eventMouseWheel	( sf::Event::MouseWheelEvent mEvent);
	virtual void eventKeyboardDown	( sf::Keyboard::Key keyPressed );
	virtual void eventKeyboardUp	( sf::Keyboard::Key keyPressed );

private:
	void windowReshaped				( int width, int height );

};

#endif

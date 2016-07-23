/*
 *	13.2.2014.
 *  Bruno
 */

#include <iostream>
#include "WindowManager.h"
#include "MappedData.h"
#include "ShaderProgram.h"
/*
#include "Timer.h"

#include "globals.h"*/

#include <glm/gtc/matrix_transform.hpp>

using namespace std;

void loadGLEW()
{
	GLenum glewStatus = glewInit();
	if( glewStatus != GLEW_OK ) {
		cerr << "Error initializing GLEW: " << glewGetErrorString(glewStatus) << endl;
		exit(-1);
	}

	const GLubyte *renderer = glGetString( GL_RENDERER );
	const GLubyte *vendor = glGetString( GL_VENDOR );
	const GLubyte *version = glGetString( GL_VERSION );
	const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("GL Vendor    : %s\n", vendor);
	printf("GL Renderer  : %s\n", renderer);
	printf("GL Version (string)  : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version : %s\n", glslVersion);  
	
/*	if( GLEW_EXT_geometry_shader4 || GLEW_ARB_geometry_shader4 )
	{
		cout << "Geometry shader supported." << endl;
	}else
	{
		cout << "Geometry shader not supported." << endl;
	}*/
}


sf::Window* WindowManager::getWindow()
{
	return window;
}

WindowManager::WindowManager(const std::string& configFile) :
	shouldClose(false), mouseRight( false ), mouseLeft( false ), mouseLastX(0), mouseLastY(0)
{
	MappedData data(configFile.c_str());
	
	sf::ContextSettings settings;	
	settings.depthBits			= data.getData("window", "depthBits").get<int>( 24 );
	settings.stencilBits		= data.getData("window", "stencilBits").get<int>( 8 );
	settings.antialiasingLevel	= data.getData("window", "antialiasing").get<int>( 2 );
	int width = data.getData("window", "width").get<int>( 1024 );
	int height = data.getData("window", "height").get<int>( 600 );
	int bitsPerPixel = data.getData("window", "bitsPerPixel").get<int>( 32 );
	string title = data.getData("window", "title").getStringData("Playground");
	window = new sf::RenderWindow(sf::VideoMode(width, height, bitsPerPixel), title.c_str(), sf::Style::Default , settings);	

	loadGLEW();
}


WindowManager::~WindowManager(void)
{
	
}

void WindowManager::windowWillClose()
{
}

void WindowManager::windowWillRun()
{
}

void WindowManager::run()
{
	windowWillRun();
	window->setVerticalSyncEnabled(true);
	window->setKeyRepeatEnabled(false);

	sf::Clock windowClock;
	sf::Time lastDrawTime = windowClock.getElapsedTime();
	do
    {
        sf::Event Event;
        while (window->pollEvent(Event))
        {
            if (Event.type == sf::Event::Closed){
                window->close();
			}

            if (Event.type == sf::Event::KeyPressed){
				eventKeyboardDown( Event.key.code );
			}

			if( Event.type == sf::Event::KeyReleased ){
				eventKeyboardUp( Event.key.code );
			}

            if (Event.type == sf::Event::Resized){
               windowReshaped(Event.size.width, Event.size.height);
			}

			if (Event.type == sf::Event::MouseButtonPressed || Event.type == sf::Event::MouseButtonReleased ){
				eventMousePressed( Event.mouseButton );
			}	

			if (Event.type == sf::Event::MouseMoved){				
				eventMouseMotion( Event.mouseMove );
			}

			if (Event.type == sf::Event::MouseWheelMoved){
				eventMouseWheel( Event.mouseWheel );
			}
        }

		if( !window->isOpen() || shouldClose )
		{			
			windowWillClose();
			break; 
		}
			
		float dt = (windowClock.getElapsedTime().asMicroseconds() - lastDrawTime.asMicroseconds()) / 1000000.0f;		
		lastDrawTime = windowClock.getElapsedTime();
		updateScene( dt );

		drawCycle();

    }while( true );
}

void WindowManager::open()
{
	WindowManager window( "windowSettings.txt" );
	window.run();	
}

void WindowManager::windowReshaped( int width, int height )
{
	eventReshape(width, height);
}

void WindowManager::eventReshape(int width, int height)
{
}

void WindowManager::eventMousePressed( sf::Event::MouseButtonEvent mEvent )
{
	bool pressed = sf::Mouse::isButtonPressed( mEvent.button );
	bool left  = mEvent.button == sf::Mouse::Left;
	bool right = mEvent.button == sf::Mouse::Right;

	// If left btn pressed, update left state, else stays the same
	mouseLeft = left ? pressed : mouseLeft;
	mouseRight = right ? pressed : mouseRight;		
}

void WindowManager::eventMouseMotion( sf::Event::MouseMoveEvent mEvent )
{
	mouseLastX = mEvent.x;
	mouseLastY = mEvent.y;
}

void WindowManager::eventMouseWheel( sf::Event::MouseWheelEvent mEvent )
{
	// Incomplete implementation, unused
}

void WindowManager::eventKeyboardDown( sf::Keyboard::Key keyPressed )
{
}

void WindowManager::eventKeyboardUp( sf::Keyboard::Key keyPressed )
{
	if (keyPressed == sf::Keyboard::Escape) {
		shouldClose = true;
	}
}

void WindowManager::updateScene( float dt )
{
}

void WindowManager::drawGLScene()
{
}

void WindowManager::drawSFMLScene(sf::RenderWindow& window)
{
}

void WindowManager::drawCycle()
{
	window->setActive();
	
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClearDepth(1.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	drawGLScene();

	window->pushGLStates();
	drawSFMLScene(*window);
	window->popGLStates();	
        
    window->display();
}

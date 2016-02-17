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
	settings.depthBits			= data.getData("window", "depthBits").getInt( 24 );
	settings.stencilBits		= data.getData("window", "stencilBits").getInt( 8 );
	settings.antialiasingLevel	= data.getData("window", "antialiasing").getInt( 2 );
	int width = data.getData("window", "width").getInt( 1024 );
	int height = data.getData("window", "height").getInt( 600 );
	int bitsPerPixel = data.getData("window", "bitsPerPixel").getInt( 32 );
	string title = data.getData("window", "title").getStringData("Playground");
	window = new sf::Window(sf::VideoMode(width, height, bitsPerPixel), title.c_str(), sf::Style::Default , settings);	

	loadGLEW();
/*
	// TODO: can have multiple cameras, should make more camera types
	camera = CameraFocused();
	// TODO: is frustum extraction necessary
	camera.getFrustum().setWidthHeight( width, height );
	f = &camera.getFrustum();*/
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
			
		double dt = (windowClock.getElapsedTime().asMicroseconds() - lastDrawTime.asMicroseconds()) / 1000000.0;		
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
	
/*	camera.getFrustum().setWidthHeight( width, height );
	
	glViewport(0, 0, camera.getFrustum().width, camera.getFrustum().height);
		
	glColor3ub (0, 0, 255);
	float boja[] = {1,1,1,1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, boja);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();*/
}

void WindowManager::eventMousePressed( sf::Event::MouseButtonEvent mEvent )
{
	bool pressed = sf::Mouse::isButtonPressed( mEvent.button );
	bool left  = mEvent.button == sf::Mouse::Left;
	bool right = mEvent.button == sf::Mouse::Right;

	if (left && pressed)
	{
		mouseLeft = true;	
	}else if (left && !pressed)
	{
		mouseLeft = false;	
	}

	if (right && pressed)
	{
		mouseRight = true;			
	}else if (right && !pressed)
	{
		mouseRight = false;			
	}		
}

void WindowManager::eventMouseMotion( sf::Event::MouseMoveEvent mEvent ){
	int x = mEvent.x;
	int y = mEvent.y;
	//int dx = x - mouseLastX;
	//int dy = y - mouseLastY;
	
	if(mouseLeft){
//		camera.rotateY( 0.87266f*dx );
//		camera.rotateX( 0.87266f*dy );
	}else if(mouseRight){
		;
	}

	mouseLastX = x;
	mouseLastY = y;	
}

void WindowManager::eventMouseWheel( sf::Event::MouseWheelEvent mEvent )
{
//	camera.changeDistance( mEvent.delta * -0.8 );
}

void WindowManager::eventKeyboardDown( sf::Keyboard::Key keyPressed )
{

}

void WindowManager::eventKeyboardUp( sf::Keyboard::Key keyPressed )
{

}

void WindowManager::updateScene( double dt )
{

}

void WindowManager::drawScene()
{
}

void WindowManager::drawCycle()
{
	window->setActive();

	//glEnable(GL_NORMALIZE);
	//glEnable(GL_CULL_FACE);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_DEPTH_TEST);

	//Timer::calcTimeFlow();
	//camera.getFrustum().setupGLProjection();
	//camera.getFrustum().CalculateFrustum();
	//camera.setupCamera();


		
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClearDepth(1.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	drawScene();
        
    window->display();
}

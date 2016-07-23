
#include "LearningWindowManager.h"
#include "ShaderProgram.h"
#include "Scene.h"
#include "LyingShapesScene.h"
#include "SPHScene.h"

#include <sstream>
#include <glm/gtc/type_ptr.hpp>


LearningWindowManager::LearningWindowManager( const std::string& configFile )
	: WindowManager( configFile ) ,
	camera({ 1024.0f,768.0f }, Camera::ProjectionType::PERSPECTIVE, { 1.0f,1000.0f }),
	averageFrameTime( 60 )
{
	
}

void LearningWindowManager::open()
{
	LearningWindowManager window("data/windowSettings.txt");
	window.run();
}

void LearningWindowManager::addScene( Scene* scene )
{
	scenes.push_back( scene );
	auto size = window->getSize();
	scene->eventReshape( size.x, size.y );
}

void LearningWindowManager::windowWillClose()
{
	for( auto sceneIt = scenes.begin(); sceneIt != scenes.end(); sceneIt++ )
	{
		delete (*sceneIt);
	}

}

void LearningWindowManager::windowWillRun()
{
	auto size = window->getSize();
	eventReshape( size.x, size.y );

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	addScene(new SPHScene());
}

void LearningWindowManager::eventReshape( int width, int height)
{
	camera.windowDidResize( width, height );
	for( unsigned int i = 0; i<scenes.size(); i++)
	{
		scenes[i]->eventReshape(width, height);
	}
}

void LearningWindowManager::eventKeyboardDown( sf::Keyboard::Key keyPressed )
{
	for( unsigned int i = 0; i<scenes.size(); i++)
	{
		scenes[i]->eventKeyboardDown( keyPressed );
	}

	handleCameraMove(keyPressed, true);
}

void LearningWindowManager::eventKeyboardUp( sf::Keyboard::Key keyPressed )
{
	for( unsigned int i = 0; i<scenes.size(); i++)
	{
		scenes[i]->eventKeyboardUp( keyPressed );
	}
	
	if (keyPressed == sf::Keyboard::Num3)
	{
		addScene(new LyingShapesScene());
	}
	if (keyPressed == sf::Keyboard::Num4)
	{
		addScene(new SPHScene());
	}
	if( keyPressed == sf::Keyboard::Num0 )
	{
		if( scenes.size() > 0 )
		{
			auto lastOne = scenes.end() - 1;
			delete *lastOne;
			scenes.erase( lastOne );
		}
	}
	handleCameraMove(keyPressed, false);

	WindowManager::eventKeyboardUp(keyPressed);
}

void LearningWindowManager::eventMouseMotion(sf::Event::MouseMoveEvent mEvent)
{
	int dx = mEvent.x - mouseLastX;
	int dy = mEvent.y - mouseLastY;

	if (mouseLeft) 
	{
		camera.rotate(dx, dy);
	}

	WindowManager::eventMouseMotion(mEvent);
}

void LearningWindowManager::handleCameraMove(sf::Keyboard::Key key, bool pressed) 
{
	if (key == sf::Keyboard::Left) {
		camera.setMoveLeft(pressed);
	}
	if (key == sf::Keyboard::Right) {
		camera.setMoveRight(pressed);
	}
	if (key == sf::Keyboard::Up) {
		camera.setMoveForward(pressed);
	}
	if (key == sf::Keyboard::Down) {
		camera.setMoveBackward(pressed);
	}
}

void LearningWindowManager::drawGLScene()
{
	camera.loadViewport();
	for( unsigned int i = 0; i<scenes.size(); i++ )
	{
		scenes[i]->draw( camera );
	}

	WindowManager::drawGLScene();
}

void LearningWindowManager::drawSFMLScene(sf::RenderWindow & window)
{
	for (unsigned int i = 0; i<scenes.size(); i++)
	{
		scenes[i]->drawSFML(window);
	}

	WindowManager::drawSFMLScene(window);
}

void LearningWindowManager::updateScene( float dt )
{	
	camera.update(dt);

	for (unsigned int i = 0; i < scenes.size(); i++)
	{
		scenes[i]->update(dt);
	}

	averageFrameTime.addValue(1, dt);
	double perFrame = averageFrameTime.getAverage();
	std::stringstream titleStream;
	titleStream << "Time: " << perFrame << " FPS: " << 1.0/perFrame;
	std::string title = titleStream.str();
	window->setTitle( title.c_str() );
}

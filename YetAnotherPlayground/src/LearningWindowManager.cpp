
#include "LearningWindowManager.h"
#include "ShaderProgram.h"
#include "Scene.h"
#include "DistributedShapesScene.h"
#include "PongScene.h"
#include "LyingShapesScene.h"
#include "SPHWindowManager.h"

#include <sstream>
#include <glm/gtc/type_ptr.hpp>


LearningWindowManager::LearningWindowManager( const std::string& configFile )
	: WindowManager( configFile ) ,
	camera({ 1024.0f,768.0f }, Camera::ProjectionType::PERSPECTIVE, { 1.0f,1000.0f }),
	averageFrameTime( 60 )
{
	//setProjection( 2.0, 2.0 );
	//view = glm::mat4(1.0f);
	//view = glm::lookAt( glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0) );
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
}

void LearningWindowManager::eventReshape( int width, int height)
{
	setProjection( width, height );
	camera.windowDidResize(width, height);
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

	if( keyPressed == sf::Keyboard::Num1 )
	{
		addScene( new DistributedShapesScene() );
	}
	if ( keyPressed == sf::Keyboard::Num2 )
	{
		addScene( new PongScene() );
	}
	if (keyPressed == sf::Keyboard::Num3)
	{
		addScene(new LyingShapesScene());
	}
	if (keyPressed == sf::Keyboard::Num4)
	{
		addScene(new SPHWindowManager());
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
}

void LearningWindowManager::eventMouseMotion(sf::Event::MouseMoveEvent mEvent)
{
	int dx = mEvent.x - mouseLastX;
	int dy = mEvent.y - mouseLastY;
	//std::cout << dx << "  " << dy << std::endl;

	if (mouseLeft) {
		camera.rotate(dx, dy);
	}

	WindowManager::eventMouseMotion(mEvent);
}

void LearningWindowManager::handleCameraMove(sf::Keyboard::Key key, bool pressed) {
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

void LearningWindowManager::setProjection( double width, double height )
{
	std::cout << "Reshape projection: " << width << ", " << height << std::endl;
	//camera.setupOtrhographicProjection({ 0.0f, 0.0f }, { (float)width, (float)height }, 0.0f, 100.0f);
	//
	//double orthoHeight = height / 3;
	//double orthoWidth = orthoHeight * width / height;
	//
	////projection = glm::ortho( orthoWidth*-0.5, orthoWidth*0.5, orthoHeight*-0.5, orthoHeight*0.5 );
	//projection = glm::ortho(0.0, width, 0.0, height,0.0,100.0 );
	////projection = glm::ortho(-100.0, 100.0, -100.0, 100.0 );

	////view = glm::translate( glm::mat4(), glm::vec3(-orthoWidth * 0.5, -orthoHeight * 0.5,0.0) );
}

void LearningWindowManager::drawScene()
{
	//auto viewProjection = camera.getViewProjection(); // projection * view;
	
	camera.loadViewport();
	for( unsigned int i = 0; i<scenes.size(); i++ )
	{
		scenes[i]->draw( camera );
	}

	WindowManager::drawScene();
}

void LearningWindowManager::updateScene( double dt )
{	
	camera.update(dt);
	camera.updateCamera();

	for (unsigned int i = 0; i < scenes.size(); i++)
	{
		scenes[i]->update(dt);
	}

	averageFrameTime.addValue(1, dt);
	double perFrame = averageFrameTime.currentAverage();
	std::stringstream titleStream;
	titleStream << "Time: " << perFrame << " FPS: " << 1.0/perFrame;
	std::string title = titleStream.str();
	window->setTitle( title.c_str() );
}

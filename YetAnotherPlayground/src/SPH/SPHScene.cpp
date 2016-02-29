#include "SPHScene.h"
#include "MappedData.h"

#include "PointDataVisualiser.h"
#include "SPHSystem3d.h"
#include "MarchingCubesShaded.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "LineGrid.h"

using namespace std;

SPHScene::SPHScene(void) : Scene(),
	paused(false),
	sphTimer(3), marchingTimer(3),
	drawWithMC(false), drawPDVWithShader(true)
{
	// TODO: shared global settings
	MappedData settings("data/settings.txt");
	string imagesPath = settings.getData("folders", "images").getStringData();

	sph3 = new SPHSystem3d("data/sph3d.txt");
	cout << "SPH particle size: " << sizeof(SPHParticle3d) << endl;

	grid = new LineGrid(20, 10.0f, 10.0f, 20, 10.0f, 10.0f);
	grid->transform.setPosition({ -100.0f,0.0f,-100.0f });
	grid->transform.setAngles(90.0f, 0.f, 0.0f);

	coords = new LineGrid(25.0f, 3.0f);

	marchingCubes = new MarchingCubesShaded("data/mCubesShaded.txt");
	marchingCubes->transform.setPosition({ 0.0f,0.0f,0.0f });
	marchingCubes->transform.setScale(marchingCubes->getScale());

	pointVisualizer = new PointDataVisualiser((imagesPath + "point.jpg").c_str(), true);
	pointVisualizer->transform.setPosition({ 0.0f,0.5f,0.0f });
	pointVisualizer->transform.setScale({ 1.0f, 1.0f, -1.0f });
	pointVisualizer->transform.setAngles({ 0.0, 90.0f, 0.0f });
	pointVisualizer->setColor(0.03f, 0.2f, 0.5f);

	fontLoaded = anonPro.loadFromFile("data/fonts/anonymous-pro/AnonymousPro-Regular.ttf");	
	if(fontLoaded)
	{
		status.setFont(anonPro);
		status.setCharacterSize(18);
		status.setColor(sf::Color::White);
		status.setPosition(0, 0);
	}
}

SPHScene::~SPHScene(void)
{	
	safeDelete(&grid);
	safeDelete(&coords);
	safeDelete(&marchingCubes);
	safeDelete(&pointVisualizer);
	safeDelete(&sph3);
}

void SPHScene::eventKeyboardUp(sf::Keyboard::Key keyPressed)
{
	switch (keyPressed)
	{
		case sf::Keyboard::Add: 
					treshold = contain<float>( treshold+0.01, 0, 2 );
					marchingCubes->setTreshold( treshold );
					break;

		case sf::Keyboard::Subtract: 	
					treshold = contain<float>( treshold-0.01, 0, 2 );
					marchingCubes->setTreshold( treshold );
					break;

		case sf::Keyboard::P:
					paused = true;
					Timer::pauseToggle();	// TODO: Timer class is wtf?
					break;

		case sf::Keyboard::O:
					{
						float o = 0.0f;
						float d = 1.0f;
						sph3->addParticle( vec3f( 2, 5, 2 ), vec3f( 2,0,2 ) );
					}
					break;
							
		case sf::Keyboard::L:
					{
						float o =0;
						float d = 1;
						sph3->addParticle( vec3f( 3, 5, 2 ), vec3f( 0,-2,-2 ) );
						sph3->addParticle( vec3f( 3, 6, 2 ), vec3f( 0,-2,-2 ) );
						sph3->addParticle( vec3f( 2, 6, 2 ), vec3f( 0,-2,-2 ) );
						sph3->addParticle( vec3f( 2, 5, 2 ), vec3f( 0,-2,-2 ) );
						sph3->addParticle( vec3f( 3, 5, 3 ), vec3f( 0,-2,-2 ) );
						sph3->addParticle( vec3f( 3, 6, 3 ), vec3f( 0,-2,-2 ) );
						sph3->addParticle( vec3f( 2, 6, 3 ), vec3f( 0,-2,-2 ) );
						sph3->addParticle( vec3f( 2, 5, 3 ), vec3f( 0,-2,-2 ) );
					}
					break;

		/* SPH variable setting */
		case sf::Keyboard::E:	sph3->setRestDensity( sph3->getRestDensity() + 0.005f );					
					break;

		case sf::Keyboard::D:	sph3->setRestDensity( sph3->getRestDensity() - 0.005f );					
					break;

		case sf::Keyboard::R:	sph3->setK( sph3->getK() + 0.1f );
					break;

		case sf::Keyboard::F:	sph3->setK( sph3->getK() - 0.1f );
					break;
					/**/
		case sf::Keyboard::T:	sph3->setViscosity(sph3->getViscosity() + 0.005f );
					break;

		case sf::Keyboard::G:	sph3->setViscosity(sph3->getViscosity() - 0.005f );
					break;

		case sf::Keyboard::Z:	sph3->setSmoothingLength(sph3->getSmoothingLength()+0.05f);
					break;

		case sf::Keyboard::H:	sph3->setSmoothingLength(sph3->getSmoothingLength()-0.05f);
					break;

		case sf::Keyboard::U:	sph3->setColorFieldTreshold(sph3->getColorFieldTreshold()+0.01f);
					break;

		case sf::Keyboard::J:	sph3->setColorFieldTreshold(sph3->getColorFieldTreshold()-0.01f);
					break;

		case sf::Keyboard::I:	sph3->setSurfaceTension(sph3->getSurfaceTension()+0.01f);
					break;

		case sf::Keyboard::K:	sph3->setSurfaceTension(sph3->getSurfaceTension()-0.01f);
					break;
	

		case sf::Keyboard::M:
					drawPDVWithShader = !drawPDVWithShader;
					break;

		case sf::Keyboard::N:
					drawWithMC = !drawWithMC;
					break;

		case sf::Keyboard::Multiply:
					sph3->toggleSurface(1);
					break;

		case sf::Keyboard::Num1:
					sph3->setUseGravity( !sph3->usesGravity() );
					break;

		case sf::Keyboard::Num2:
					sph3->clearAllParticles();
					break;

		case sf::Keyboard::Num3:
					glDisable(GL_NORMALIZE);
					break;

		default:
			Scene::eventKeyboardUp(keyPressed);
			break;
	}
}

void SPHScene::update(float dt)
{
	if (!paused)
	{
		sphTimer.resume();
		sph3->animate(0.0125f);
		sphTimer.pause();
	}

	static char title[256];	
	sprintf_s(title,256, "Time: %.2f, Fps: %d, Particles: %d, SPH time: %1.8f, Draw time: %1.8f", 
		Timer::getTime(), Timer::getFPS(), sph3->getParticleCount(), 
		sphTimer.getAverage(), marchingTimer.getAverage());
	status.setString( title );	
}

void SPHScene::draw(const Camera & camera)
{
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	grid->draw(camera.getViewProjection());
	coords->draw(camera.getViewProjection());
			
	marchingTimer.resume();	
	if(drawWithMC)
	{
		marchingCubes->clear();
		sph3->draw( marchingCubes );		
		marchingCubes->draw( camera );		
	}else
	{	
		sph3->draw( pointVisualizer );			
		if( drawPDVWithShader )
		{
			pointVisualizer->draw( camera );
		}else{
			ShaderProgram::turnOff();
			pointVisualizer->draw( camera, true );
		}	
		
	}
	marchingTimer.pause();
}

void SPHScene::drawSFML(sf::RenderWindow & window)
{
	if (!fontLoaded) {
		return;
	}

	window.draw(status);
}

#include "SPHScene.h"
#include "MappedData.h"

#include "PointDataVisualiser.h"
#include "SPHSystem3d.h"
#include "SPHSystem3dClean.h"
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
	sph3 = new SPHSystem3d("data/sph3d.txt");
	cout << "SPH particle size: " << sizeof(SPHParticle3d) << endl;

	grid = new LineGrid(10, 5.0f, 5.0f, 10, 5.0f, 5.0f);
	grid->transform.setPosition({ -25.0f,0.0f,-25.0f });
	grid->transform.setAngles(90.0f, 0.f, 0.0f);

	coords = new LineGrid(25.0f, 3.0f);

	marchingCubes = new MarchingCubesShaded("data/mCubesShaded.txt");
	marchingCubes->transform.setPosition({ 0.0f,0.0f,0.0f });
	marchingCubes->transform.setScale(marchingCubes->getScale());

	pointVisualizer = new PointDataVisualiser("data/images/point.png", true);
	pointVisualizer->transform.setPosition({ 0.0f,0.5f,0.0f });
	pointVisualizer->transform.setScale({ 1.0f, 1.0f, -1.0f });
	pointVisualizer->transform.setAngles({ 0.0, 90.0f, 0.0f });
	pointVisualizer->setColor(0.03f, 0.2f, 0.5f);

	fontLoaded = anonPro.loadFromFile("data/fonts/anonymous-pro/AnonymousPro-Regular.ttf");
	if (fontLoaded)
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
					treshold = contain<float>( treshold+0.01f, 0, 2 );
					marchingCubes->setTreshold( treshold );
					break;

		case sf::Keyboard::Subtract: 	
					treshold = contain<float>( treshold-0.01f, 0, 2 );
					marchingCubes->setTreshold( treshold );
					break;

		case sf::Keyboard::P:
					Timer::pauseToggle();	// TODO: Timer class is wtf?
					paused = Timer::isPaused();
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
	// TODO: refactor timer, it is horrible
	Timer::calcTimeFlow();

	if (!paused)
	{
		sphTimer.resume();
		sph3->animate(0.0125f);
		sphTimer.pause();
	}

	stringstream infoText;

	infoText.precision(2);
	infoText << "Time: " << fixed << Timer::getTime();

	infoText.precision(8);
	infoText << ", Fps: " << Timer::getFPS() <<	", Draw time: " << marchingTimer.getAverage() << endl;

	infoText << "[SPH]" << endl;
	infoText.precision(8);
	infoText << "  Time: " << sphTimer.getAverage() << endl;
	infoText.precision(4);
	infoText << "  Particles (O/L): " << sph3->getParticleCount() << endl;
	infoText << "  Rest Density (E/D): " << sph3->getRestDensity() << endl;
	infoText << "  K (R/F): " << sph3->getK() << endl;
	infoText << "  Viscosity (T/G): " << sph3->getViscosity() << endl;
	infoText << "  Smoothing Length (Z/H): " << sph3->getSmoothingLength() << endl;
	infoText << "  Color Field treshold (U/J): " << sph3->getColorFieldTreshold() << endl;
	infoText << "  Surface Tension (I/K): " << sph3->getSurfaceTension() << endl;
	infoText << "  Gravity (1): " << (sph3->usesGravity() ? "ON" : "OFF") << endl;
	if (drawWithMC)
	{
		infoText << "[MarchingCubes]" << endl << "  Treshold (+/-): " << marchingCubes->getTreshold() << endl;
	}
	
	status.setString( infoText.str() );	
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

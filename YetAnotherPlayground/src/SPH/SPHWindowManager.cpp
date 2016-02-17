#include "SPHWindowManager.h"
#include "MappedData.h"

#include "PointDataVisualiser.h"
#include "SPHSystem3d.h"
#include "MarchingCubesShaded.h"
#include "ShaderProgram.h"
#include "Camera.h"

using namespace std;

SPHWindowManager::SPHWindowManager(void) : Scene(),
	paused(false),
	sphTimer(3), marchingTimer(3),
	drawWithMC(false), drawPDVWithShader(true)
{
	initData();
	initLight();
}

SPHWindowManager::~SPHWindowManager(void)
{	
}

void SPHWindowManager::initData()
{
	// TODO: shared global settings
	MappedData settings("settings.txt");
	string imagesPath = settings.getData("folders", "images").getStringData();

	sph3 = new SPHSystem3d( "sph3d.txt" );
	cout << "SPH particle size: " << sizeof( SPHParticle3d ) << endl;

	mcs = new MarchingCubesShaded( "data/mCubesShaded.txt" );

	pdv = new PointDataVisualiser( (imagesPath+"point.jpg").c_str(), true );	
	pdv->setPosition(-5,-5,0);
	pdv->setScale( 2, 2, 2 );
	pdv->setColor(0.03f,0.2f,0.5f);
}

void SPHWindowManager::initLight(){
	// TODO: Lights are part of the scene
	MappedData lightConf("lights.txt");

	lightConf.getData( "light1", "position" ).fillFloatArray( lightPosition, 3 );
	lightConf.getData( "light1", "ambient" ).fillFloatArray( lightAmbient, 4 );
	lightConf.getData( "light1", "difuse" ).fillFloatArray( lightDifuse, 4 );
	lightConf.getData( "light1", "specular" ).fillFloatArray( lightSpecular, 4 );
	float ca = lightConf.getData( "light1", "constAttenuation" ).getFloat();
	float la = lightConf.getData( "light1", "linearAttenuation" ).getFloat();
	float qa = lightConf.getData( "light1", "quadraticAttenuation" ).getFloat();	

	/*lightAmbient = light_Ambient;
	lightDifuse = light_Difuse;
	lightPosition = light_Position;
	lightSpecular = light_Specular;*/

	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifuse);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, ca);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, la);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, qa);
		
    glEnable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
}

void SPHWindowManager::eventKeyboardUp(sf::Keyboard::Key keyPressed)
{
	switch (keyPressed)
	{
		case sf::Keyboard::Add: 
					treshold = contain<float>( treshold+0.01, 0, 2 );
					mcs->setTreshold( treshold );
					break;

		case sf::Keyboard::Subtract: 	
					treshold = contain<float>( treshold-0.01, 0, 2 );
					mcs->setTreshold( treshold );
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

void SPHWindowManager::update(float dt)
{
	if (!paused)
	{
		sphTimer.resume();
		sph3->animate(0.0125f);
		sphTimer.pause();
	}
}

void SPHWindowManager::draw(const Camera & camera)
{

	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
			
	marchingTimer.resume();	
	if(drawWithMC)
	{
		mcs->clear();
		sph3->draw( mcs );		
		mcs->draw( camera );		
	}else
	{	
		sph3->draw( pdv );			
		if( drawPDVWithShader )
		{
			pdv->draw( camera );
		}else{
			ShaderProgram::turnOff();
			pdv->draw( camera, true );
		}	
		
	}
	marchingTimer.pause();
	
	// TODO: print this info somewhere
	//static char title[256];	
	//sprintf_s(title,256, "Time: %.2f, Fps: %d, Particles: %d, SPH time: %1.8f, Draw time: %1.8f, Rest time: %1.8f", Timer::getTime(), Timer::getFPS(), sph3->getParticleCount(), sphTimer->getAverage(), marchingTimer->getAverage(), restTimer->getAverage());
	//window->setTitle( title );	

}
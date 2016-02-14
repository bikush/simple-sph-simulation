
#include "PongScene.h"
#include "Shape.h"
#include "Utility.h"
#include "Camera.h"

PongScene::PongScene()
{
	leftPaddle = Shape::CreateShape( glm::vec2(0,0), glm::vec2(10,100) );
	rightPaddle = Shape::CreateShape( glm::vec2(0,0), glm::vec2(10,100) );
	ball = Shape::CreateShape( glm::vec2(0,0), glm::vec2(30,30) );
}

PongScene::~PongScene()
{
	safeDelete( &leftPaddle );
	safeDelete( &rightPaddle );
	safeDelete( &ball );
}

void PongScene::eventReshape( int width, int height)
{
	glm::vec2 paddleSize;
	paddleSize.x = (float)glm::min( width/5, 10 );
	paddleSize.y = (float)glm::min( height/5, 100 );

	leftPaddle->setScale(paddleSize);
	rightPaddle->setScale(paddleSize);

	glm::vec2 leftPosition = leftPaddle->getPosition();
	leftPosition.x = glm::min( 10.0f, width / 5.0f );
	leftPosition.y = glm::min( leftPosition.y + paddleSize.y, height - paddleSize.y );
	leftPaddle->setPosition(leftPosition);

	glm::vec2 rightPositon = rightPaddle->getPosition();
	rightPositon.x = glm::max( width - 10.0f - paddleSize.x, width * 4.0f / 5.0f);
	rightPositon.y = glm::min(rightPositon.y + paddleSize.y, height - paddleSize.y);
	rightPaddle->setPosition(rightPositon);

	ball->setPosition(glm::vec2(width*.5f, height*.5f));
	//leftPosition = glm::min( wi
}

void PongScene::eventKeyboardDown( sf::Keyboard::Key keyPressed )
{
}

void PongScene::eventKeyboardUp( sf::Keyboard::Key keyPressed )
{
	if (keyPressed == sf::Keyboard::Up) 
	{
		leftPaddle->setPosition(leftPaddle->getPosition() + glm::vec2(0, 10));
		rightPaddle->setPosition(rightPaddle->getPosition() + glm::vec2(0, 10));
	}
	if (keyPressed == sf::Keyboard::Down)
	{
		leftPaddle->setPosition(leftPaddle->getPosition() + glm::vec2(0, -10));
		rightPaddle->setPosition(rightPaddle->getPosition() + glm::vec2(0, -10));
	}
}

void PongScene::draw(const Camera& camera)
{
	const glm::mat4& viewProjection = camera.getViewProjection();
	leftPaddle->draw(viewProjection);
	rightPaddle->draw(viewProjection);
	ball->draw(viewProjection);
}
#include "Camera.h"
#include "Utility.h"
#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\quaternion.hpp>

Camera::Camera( glm::vec2 windowSize, ProjectionType type, glm::vec2 zLimits) :
	windowSize( windowSize ), projectionType( type ), projectionZLimit( zLimits ),
	position({ 0.0f, 5.0f, 0.0f }), forward({ 0.0f, 0.0f, -1.0f }), up({ 0.0f, 1.0f, 0.0f }), axis(glm::cross(forward, up))
{
	setupView();
	refreshProjection();
	refreshViewProjection();
}


Camera::~Camera(void)
{
}

// VIEW
void Camera::setupView()
{
	view = glm::lookAt(position, position + forward, -glm::cross(forward,axis));
	refreshViewProjection();
}

void Camera::applyOffset(glm::vec3 offset)
{
	position += offset;
	setupView();
}

void Camera::setMoveForward(bool movement) {
	moveForward = movement;
}

void Camera::setMoveBackward(bool movement) {
	moveBackward = movement;
}

void Camera::setMoveLeft(bool movement) {
	moveLeft = movement;
}

void Camera::setMoveRight(bool movement) {
	moveRight = movement;
}

void Camera::rotate(int idx, int idy)
{
	float dx = -100.f * idx / windowSize.x;
	float dy = -100.f * idy / windowSize.y;

	auto pitch = glm::angleAxis(dy, axis);
	auto heading = glm::angleAxis(dx, up);

	glm::quat temp = glm::cross(pitch, heading);
	temp = glm::normalize(temp);
	//update the direction from the quaternion
	forward = glm::rotate(temp, forward);
	axis = glm::rotate(heading, axis);
	// if up changes, camera becomes too free
	//up = glm::rotate(pitch, up);

	setupView();
}

void Camera::update(float dt)
{
	float speed = 30.0f;
	int forwardMovement = (moveForward ? 1 : 0) + (moveBackward ? -1 : 0);
	if (forwardMovement != 0) {
		applyOffset( forward * (forwardMovement * dt * speed) );
	}
	int sideMovement = (moveLeft ? -1 : 0) + (moveRight ? 1 : 0);
	if (sideMovement != 0) {
		applyOffset( axis * (sideMovement * dt * speed) );
	}
}

const glm::mat4 & Camera::getView() const
{
	return view;
}

// PROJECTION

void Camera::refreshProjection()
{
	if (projectionType == PERSPECTIVE) {
		setupPerspectiveProjection( 45.0f, windowSize.x/windowSize.y, projectionZLimit.x, projectionZLimit.y);
	}
	else {
		// TODO: projection start and size are dependant on the world and cameras relative eye offset
		// Ortho size could and should be open to outside interpretations, for now it is fixed
		setupOtrhographicProjection({ 0.0f,0.0f }, { windowSize.x,windowSize.y }, projectionZLimit.x, projectionZLimit.y);
	}
}

void Camera::setupOtrhographicProjection(glm::vec2 projectionStart, glm::vec2 projectionSize, float zNear, float zFar)
{
	projectionType = ProjectionType::OTRHOGRAPHIC;
	this->projectionStart = projectionStart;
	this->projectionSize = projectionSize;
	projection = glm::ortho(
		projectionStart.x, projectionStart.x + projectionSize.x, 
		projectionStart.y, projectionStart.y + projectionSize.y, 
		zNear, zFar);
	refreshViewProjection();
}

void Camera::setupPerspectiveProjection(float fovy, float aspectRatio, float zNear, float zFar)
{
	projectionType = ProjectionType::PERSPECTIVE;
	projection = glm::perspective(fovy, aspectRatio, zNear, zFar);
	refreshViewProjection();
}

const glm::mat4 & Camera::getProjection() const
{
	return projection;
}

// VIEW PROJECTION
void Camera::refreshViewProjection()
{
	viewProjection = projection * view;
}

const glm::mat4& Camera::getViewProjection() const
{
	return viewProjection;
}

void Camera::updateCamera()
{
	// OLD: 
	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -distance);

	glRotatef(angles.x, 1.0, 0.0, 0.0);
	glRotatef(angles.y, 0.0, 1.0, 0.0);

	glTranslatef(-center.x, -center.y, -center.z);*/
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOVy, aspectRatio, NCP, FCP); */
	frustum.setupGLProjection();
	frustum.CalculateFrustum();
}

// VIEWPORT
void Camera::windowDidResize(float width, float height)
{
	// TODO: viewport could stretch to fit, or it could be static
	windowSize.x = width;
	windowSize.y = height;
	setupViewport(0,0,width,height);
	refreshProjection();
}

void Camera::setupViewport(float x, float y, float width, float height)
{
	viewportPos.x = x;
	viewportPos.y = y;
	viewportSize.x = width;
	viewportSize.y = height;
}

void Camera::loadViewport()
{
	glViewport(viewportPos.x, viewportPos.y, viewportSize.x, viewportSize.y);
}

const Frustum & Camera::getFrustum() const
{
	return frustum;
}

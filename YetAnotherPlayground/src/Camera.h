#pragma once

#include <glm\mat4x4.hpp>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include "Frustum.h"

class Camera
{
public:
	enum ProjectionType {
		OTRHOGRAPHIC, PERSPECTIVE
	};

// CONSTRUCTION
public:
	Camera( glm::vec2 windowSize, ProjectionType type, glm::vec2 zLimits );
	virtual ~Camera(void);

// VIEW
protected:
	bool moveForward;
	bool moveBackward;
	bool moveLeft;
	bool moveRight;
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 axis;
	glm::mat4 view;
	void setupView();

public:
	void applyOffset( glm::vec3 offset );
	const glm::vec3& getPosition() const { return position; }
	const glm::vec3& getUp() const { return up;	}
	void setMoveForward(bool movement);
	void setMoveBackward(bool movement);
	void setMoveLeft(bool movement);
	void setMoveRight(bool movement);

	void rotate(int dx, int dy);

	void update(float dt);

	const glm::mat4& getView() const;
	
// PROJECTION
protected:
	ProjectionType projectionType;
	glm::vec2 projectionZLimit;
	glm::vec2 projectionStart;
	glm::vec2 projectionSize;
	glm::mat4 projection;
	void refreshProjection();

public:
	void setupOtrhographicProjection( glm::vec2 projectionStart, glm::vec2 projectionSize, float zNear, float zFar );
	void setupPerspectiveProjection( float fovy, float aspectRatio, float zNear, float zFar );

	const glm::mat4& getProjection() const;
	
// VIEW PROJECTION
protected:
	glm::mat4 viewProjection;
	void refreshViewProjection();

public:
	const glm::mat4& getViewProjection() const;
	void updateCamera();

// VIEWPORT
private:
	glm::vec2 windowSize;
	glm::vec2 viewportPos;
	glm::vec2 viewportSize;

public:
	void windowDidResize(int width, int height);
	void setupViewport(float x, float y, float widthm, float height);
	void loadViewport();

// FRUSTUM
private:
	Frustum frustum;
public:
	const Frustum& getFrustum() const;
};


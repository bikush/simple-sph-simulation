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
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
	glm::mat4 view;
	void setupView();

public:
	void applyOffset( glm::vec3 offset );
	const glm::vec3& getPosition() const { return position; }
	
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
	void windowDidResize(float width, float height);
	void setupViewport(float x, float y, float widthm, float height);
	void loadViewport();

// FRUSTUM
private:
	Frustum frustum;
public:
	const Frustum& getFrustum() const;
};


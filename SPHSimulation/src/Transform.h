#pragma once

#include "GlmVec.h"
#include <glm\mat4x4.hpp>

class Transform
{
public:
	Transform();
	Transform( const glm::vec3& position, const glm::vec3& scale, const glm::vec3& axisAngles );

	void setPosition(const glm::vec3& newPosition);
	void setScale(const glm::vec3& newScale);
	void setScale(const float newScale);
	void setAngles(const float xAxis, const float yAxis, const float zAxis);
	void setAngles(const glm::vec3& newAngles);

	glm::vec3 getPosition() const;
	glm::vec3 getScale() const;
	glm::vec3 getAxisAngles() const;

	const glm::mat4& getTransformMatrix();

private:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 axisAngles;

	bool recalculate;
	glm::mat4 modelMatrix;
};
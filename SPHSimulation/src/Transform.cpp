#define GLM_FORCE_RADIANS
#include "Transform.h"
#include <glm\gtc\matrix_transform.hpp>

Transform::Transform() : 
	position(0.0f), scale(1.0f), axisAngles(0.0f), modelMatrix(1.0f), recalculate(false)
{
}

Transform::Transform(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& axisAngles) :
	position(position), scale(scale), axisAngles(axisAngles), recalculate(true)
{
}

void Transform::setPosition(const glm::vec3 & newPosition)
{
	position = newPosition;
	recalculate = true;
}

void Transform::setScale(const glm::vec3 & newScale)
{
	scale = newScale;
	recalculate = true;
}

void Transform::setScale(const float newScale)
{
	scale = glm::vec3(newScale);
	recalculate = true;
}

void Transform::setAngles(const float xAxis, const float yAxis, const float zAxis)
{
	axisAngles = { xAxis, yAxis, zAxis };
	recalculate = true;
}

void Transform::setAngles(const glm::vec3& angles)
{
	axisAngles = angles;
	recalculate = true;
}

glm::vec3 Transform::getPosition() const
{
	return position;
}

glm::vec3 Transform::getScale() const
{
	return scale;
}

glm::vec3 Transform::getAxisAngles() const
{
	return axisAngles;
}

const glm::mat4& Transform::getTransformMatrix()
{
	if (recalculate) {
		glm::vec3 radAngles = glm::radians(axisAngles);

		modelMatrix =
			glm::rotate(
				glm::rotate(
					glm::rotate(
						glm::scale(glm::translate(glm::mat4(1.0f), position), scale),
						radAngles.x, { 1.0f,0.0f,0.0f }),
					radAngles.y, { 0.0f,1.0f,0.0f }),
				radAngles.z, { 0.0f,0.0f,1.0f });
		recalculate = false;
	}
	return modelMatrix;
}

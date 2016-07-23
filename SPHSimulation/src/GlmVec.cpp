#include "GlmVec.h"
#include <math.h>

bool isWithin(glm::vec3 value, glm::vec3 lower, glm::vec3 upper) {
	return
		value.x >= lower.x && value.x <= upper.x &&
		value.y >= lower.y && value.y <= upper.y &&
		value.z >= lower.z && value.z <= upper.z;
}

float minPart(const glm::vec3& value) {
	return fminf(value.x, fminf(value.y, value.z));
}

float maxPart(const glm::vec3& value) {
	return fmaxf(value.x, fmaxf(value.y, value.z));
}

glm::vec3 minDim(const glm::vec3& value) {
	float min = value.x;
	glm::vec3 out(min, 0, 0);
	if (min > value.y)
	{
		min = value.y;
		out.x = 0;
		out.y = value.y;
	}
	if (min > value.z)
	{
		out.x = 0;
		out.y = 0;
		out.z = value.z;
	}
	return out;
}

glm::vec3 minDir(const glm::vec3& value) {
	float min = value.x;
	glm::vec3 out(1, 0, 0);
	if (min > value.y)
	{
		min = value.y;
		out.x = 0;
		out.y = 1;
	}
	if (min > value.z)
	{
		out.x = 0;
		out.y = 0;
		out.z = 1;
	}
	return out;
}
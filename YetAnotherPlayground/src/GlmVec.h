#pragma once

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>

bool isWithin(glm::vec3 value, glm::vec3 lower, glm::vec3 upper);
float minPart(const glm::vec3& value);
float maxPart(const glm::vec3& value);
glm::vec3 minDim(const glm::vec3& value);
glm::vec3 minDir(const glm::vec3& value);
#pragma once
#include "Quaternion.h"
#include <glm\glm.hpp>


class Transform
{
public:
	Transform();
	Transform(glm::vec3 position, Quaternion rotation, glm::vec3 scale);
	~Transform();

	glm::vec3  getPosition();
	Quaternion getRotation();
	glm::vec3  getScale();

	void setPosition(glm::vec3 position);
	void setRotation(Quaternion rotation);
	void setScale(glm::vec3 scale);

private:
	glm::vec3  m_position;
	Quaternion m_rotation;
	glm::vec3  m_scale;

};


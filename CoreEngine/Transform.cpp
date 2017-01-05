#include "Transform.h"



Transform::Transform()
{
	Transform(glm::vec3(), Quaternion(), glm::vec3());
}

Transform::Transform(glm::vec3 position, Quaternion rotation, glm::vec3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale    = scale;
}


Transform::~Transform()
{
}

glm::vec3 Transform::getPosition()
{
	return m_position;
}

Quaternion Transform::getRotation()
{
	return m_rotation;
}

glm::vec3 Transform::getScale()
{
	return m_scale;
}

void Transform::setPosition(glm::vec3 position)
{
	m_position = position;
}

void Transform::setRotation(Quaternion rotation)
{
	m_rotation = rotation;
}

void Transform::setScale(glm::vec3 scale)
{
	m_scale = scale;
}

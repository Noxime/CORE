#include "Transform.h"



Transform::Transform()
{
	Transform(Vector3f(), Quaternion(), Vector3f());
}

Transform::Transform(Vector3f position, Quaternion rotation, Vector3f scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale    = scale;
}


Transform::~Transform()
{
}

Vector3f Transform::getPosition()
{
	return m_position;
}

Quaternion Transform::getRotation()
{
	return m_rotation;
}

Vector3f Transform::getScale()
{
	return m_scale;
}

void Transform::setPosition(Vector3f position)
{
	m_position = position;
}

void Transform::setRotation(Quaternion rotation)
{
	m_rotation = rotation;
}

void Transform::setScale(Vector3f scale)
{
	m_scale = scale;
}

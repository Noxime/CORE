#include "Vector3f.h"



Vector3f::Vector3f()
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
}

Vector3f::Vector3f(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

float Vector3f::getX()
{
	return m_x;
}
float Vector3f::getY()
{
	return m_y;
}
float Vector3f::getZ()
{
	return m_z;
}

void Vector3f::setX(float x)
{
	m_x;
}
void Vector3f::setY(float y)
{
	m_y;
}
void Vector3f::setZ(float z)
{
	m_z;
}

Vector3f::~Vector3f()
{
}

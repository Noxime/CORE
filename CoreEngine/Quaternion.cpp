#include "Quaternion.h"



Quaternion::Quaternion()
{
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;
}


Quaternion::~Quaternion()
{
}

float Quaternion::getX()
{
	return m_x;
}

float Quaternion::getY()
{
	return m_y;
}

float Quaternion::getZ()
{
	return m_z;
}

float Quaternion::getW()
{
	return m_w;
}

void Quaternion::setX(float x)
{
	m_x = x;
}

void Quaternion::setY(float y)
{
	m_y = y;
}

void Quaternion::setZ(float z)
{
	m_z = z;
}

void Quaternion::setW(float w)
{
	m_w = w;
}

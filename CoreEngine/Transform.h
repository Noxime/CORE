#pragma once
#include "Vector3f.h"
#include "Quaternion.h"
#include "Component.h"

class Transform: Component
{
public:
	Transform();
	Transform(Vector3f position, Quaternion rotation, Vector3f scale);
	~Transform();

	Vector3f   getPosition();
	Quaternion getRotation();
	Vector3f   getScale();

	void setPosition(Vector3f position);
	void setRotation(Quaternion rotation);
	void setScale(Vector3f scale);

private:
	Vector3f   m_position;
	Quaternion m_rotation;
	Vector3f   m_scale;

};


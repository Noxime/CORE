#pragma once
#include "Vector2f.h"
#include "Vector3f.h"

class Vertex
{
public:
	Vertex();
	Vertex(Vector3f position, Vector2f texcoord0, Vector2f texcoord1, Vector3f normal, Vector3f tangent);
	~Vertex();
private:
	Vector3f m_position;
	Vector2f m_texcoord_primary;
	Vector2f m_texcoord_secondary;
	Vector3f m_normal;
	Vector3f m_tangent;

};


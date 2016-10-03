#include "Vertex.h"



Vertex::Vertex()
{
	Vertex(Vector3f(), Vector2f(), Vector2f(), Vector3f(), Vector3f());
}

Vertex::Vertex(Vector3f position, Vector2f texcoord0, Vector2f texcoord1, Vector3f normal, Vector3f tangent)
{
	m_position           = position;
	m_texcoord_primary   = texcoord0;
	m_texcoord_secondary = texcoord1;
	m_normal             = normal;
	m_tangent            = tangent;
}


Vertex::~Vertex()
{
}

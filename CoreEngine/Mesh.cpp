#include "Mesh.h"



Mesh::Mesh()
{

}

Mesh::Mesh(uint32_t id, uint32_t eb, uint32_t vertCount)
{
	m_vbo = id;
	m_eb = eb;
	m_vertCount = vertCount;
}

uint32_t Mesh::getVBO()
{
	return m_vbo;
}

uint32_t Mesh::getEB()
{
	return m_eb;
}

uint32_t Mesh::getVertCount()
{
	return m_vertCount;
}


Mesh::~Mesh()
{

}

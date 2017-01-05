#include "Mesh.h"



Mesh::Mesh()
{

}

Mesh::Mesh(uint32_t id, uint32_t eb, uint32_t vertexCount, uint32_t indexCount)
{
	m_vbo = id;
	m_eb = eb;
	m_vertexCount = vertexCount;
	m_indexCount = indexCount;
}

uint32_t Mesh::getVBO()
{
	return m_vbo;
}

uint32_t Mesh::getEB()
{
	return m_eb;
}

uint32_t Mesh::getVertexCount()
{
	return m_vertexCount;
}

uint32_t Mesh::getIndexCount()
{
	return m_indexCount;
}



Mesh::~Mesh()
{

}

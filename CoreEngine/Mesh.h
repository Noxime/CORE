#pragma once
#include "Platform.h"
#include "Vertex.h"

class Mesh
{
public:
	Mesh();
	Mesh(uint32_t id, uint32_t eb, uint32_t vertexCount, uint32_t indexCount);
	
	uint32_t getVBO();
	uint32_t getEB();
	uint32_t getVertexCount();
	uint32_t getIndexCount();

	~Mesh();
private:

	uint32_t m_vbo;
	uint32_t m_eb;
	uint32_t m_vertexCount;
	uint32_t m_indexCount;

};


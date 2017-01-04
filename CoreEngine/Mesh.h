#pragma once
#include "Platform.h"
#include "Vertex.h"

class Mesh
{
public:
	Mesh();
	Mesh(uint32_t id, uint32_t eb, uint32_t vertCount);
	
	uint32_t getVBO();
	uint32_t getEB();
	uint32_t getVertCount();

	~Mesh();
private:

	uint32_t m_vbo;
	uint32_t m_eb;
	uint32_t m_vertCount;

};


#pragma once

#include "Mesh.h"
#include "Shader.h"

class MeshRenderer
{
public:
	MeshRenderer();
	//MeshRenderer(Mesh mesh, Shader shader);
	~MeshRenderer();
private:

	Mesh   m_mesh;
	//Shader m_shader;

};


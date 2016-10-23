#pragma once
#include "Component.h"
#include "Shader.h"
#include "Mesh.h"
#include "FileIO.h"

class MeshRender : Component
{
public:
	MeshRender(Mesh &mesh, Shader &shader, RenderingEngine *renderer);
	~MeshRender();

private:
	RenderingEngine *m_renderer;
	Mesh             m_mesh;
	Shader           m_shader;
};


#include "MeshRender.h"

MeshRender::MeshRender(Mesh &mesh, Shader &shader, RenderingEngine *renderer)
{
	m_renderer = renderer;
	m_mesh     = mesh;
	m_shader   = shader;
}


MeshRender::~MeshRender()
{

}

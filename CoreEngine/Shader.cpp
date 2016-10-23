#include "Shader.h"


Shader::Shader()
{
	std::cout << "fagget\n";
}

Shader::Shader(std::vector<char> vertsource, std::vector<char> fragsource, RenderingEngine *renderer)
{
	m_renderer = renderer;



	std::cout << "figget\n";
}


Shader::~Shader()
{

	std::cout << "fogget\n";
}

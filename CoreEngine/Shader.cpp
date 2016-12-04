#include "Shader.h"

Shader::Shader()
{
}

Shader::Shader(int program)
{
	m_program = program;
}


Shader::~Shader()
{

}

uint32_t Shader::getId()
{
	return m_program;
}


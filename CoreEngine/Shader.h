#pragma once

#include "Platform.h"
#include "RenderingEngine.h"

class Shader
{
public:
	Shader();
	Shader(int program);
	~Shader();


private:

	uint32_t m_program;
};
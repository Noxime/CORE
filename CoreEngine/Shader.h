#pragma once

#include "Platform.h"



class Shader
{
public:
	Shader();
	Shader(int program);
	~Shader();

	uint32_t getId();

private:

	uint32_t m_program;
};
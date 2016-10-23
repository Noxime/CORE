#pragma once
#include "Platform.h"
#include "RenderingEngine.h"

class Shader
{
public:
	Shader();
	Shader(std::vector<char> vertsource, std::vector<char> fragsource, RenderingEngine *renderer);
	~Shader();

private:

	RenderingEngine *m_renderer;
};


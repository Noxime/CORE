/*
Core engine

Aaro Perämaa
2016
*/

#include "RenderingEngine.h"


bool RenderingEngine::run()
{
	updateUniformBuffer(); //Hmm... might not be the right place to do this

	if (m_window != nullptr)
	{

		return m_window->update();
	}


	return true;
}

void RenderingEngine::setClearColor(glm::vec3 color)
{
	m_clearColor = color;
}


/*
Core engine

Aaro Perämaa
2016
*/

#include "RenderingEngine.h"


#if BUILD_WITH_RENDERING_BACKEND == RENDERING_BACKEND_OPENGL

RenderingEngine::RenderingEngine(uint32_t width, uint32_t height, std::string title, bool vsyncRequested)
{
	glfwInit();
	createWindow(width, height, title, vsyncRequested);
}

Window *RenderingEngine::createWindow(uint32_t width, uint32_t height, std::string title, bool vsyncRequested)
{
	m_window = new Window(this, width, height, title, vsyncRequested);

	return m_window;
}


void RenderingEngine::clearCurrentBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


RenderingEngine::~RenderingEngine()
{
	delete m_window; //Yeah no we dont want you
}

#endif
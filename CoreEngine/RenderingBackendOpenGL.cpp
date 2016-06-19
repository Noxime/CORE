#include "RenderingEngine.h"


#ifdef _WIN32
#include <Windows.h>
#endif

#if BUILD_WITH_RENDERING_BACKEND == RENDERING_BACKEND_OPENGL;

RenderingEngine::RenderingEngine()
{
	setupContext();
}

Window *RenderingEngine::createWindow(uint32_t width, uint32_t height, std::string title, bool vsyncRequested)
{
	m_window = new Window(this, width, height, title, vsyncRequested);

	return m_window;
}

bool RenderingEngine::run()
{
	if (m_window != nullptr)
	{
		return m_window->update();
	}
	return true;
}

void RenderingEngine::setupContext()
{

}

void RenderingEngine::destroyContext()
{

}

RenderingEngine::~RenderingEngine()
{
	delete m_window; //Yeah no we dont want you
	destroyContext();
}

#endif
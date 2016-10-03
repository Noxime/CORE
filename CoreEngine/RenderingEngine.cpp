/*
Core engine

Aaro Perämaa
2016
*/

#include "RenderingEngine.h"

Window *RenderingEngine::createWindow(uint32_t width, uint32_t height, std::string title, bool vsyncRequested)
{
	m_window = new Window(this, width, height, title, vsyncRequested);

	return m_window;
}

bool RenderingEngine::run()
{
	if (m_window != nullptr)
	{
		clearCurrentBuffer();
		return m_window->update();
	}
	return true;
}
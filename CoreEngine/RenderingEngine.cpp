/*
Core engine

Aaro Perämaa
2016
*/

#include "RenderingEngine.h"


bool RenderingEngine::run()
{
	if (m_window != nullptr)
	{
		drawFrame();

		return m_window->update();
	}
	return true;
}
/*
Core engine

Aaro Per�maa
2016
*/

#include "RenderingEngine.h"


bool RenderingEngine::run()
{
	if (m_window != nullptr)
	{

		return m_window->update();
	}
	return true;
}




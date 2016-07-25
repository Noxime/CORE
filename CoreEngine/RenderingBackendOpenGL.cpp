/*
Core engine

Aaro Perämaa
2016
*/

#include "RenderingEngine.h"


#ifdef _WIN32
#include <Windows.h>
#endif


#if BUILD_WITH_RENDERING_BACKEND == RENDERING_BACKEND_OPENGL;

RenderingEngine::RenderingEngine()
{

}



RenderingEngine::~RenderingEngine()
{
	delete m_window; //Yeah no we dont want you
}

#endif
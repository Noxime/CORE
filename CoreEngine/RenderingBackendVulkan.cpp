/*
Core engine

Aaro Perämaa
2016
*/

#include "RenderingEngine.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#if BUILD_WITH_RENDERING_BACKEND == RENDERING_BACKEND_VULKAN

RenderingEngine::RenderingEngine()
{

}


void RenderingEngine::clearCurrentBuffer()
{


}

void RenderingEngine::setupLayersAndExtensions()
{

}

void RenderingEngine::initInstance()
{
	
}

void RenderingEngine::initDevice()
{
	
}

void RenderingEngine::destroyInstance()
{

}

void RenderingEngine::destroyDevice()
{

}


RenderingEngine::~RenderingEngine()
{
	delete m_window; //Yeah no we dont want you


}

#endif
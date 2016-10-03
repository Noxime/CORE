/*
Core engine

Aaro Perämaa
2016
*/

#include "Window.h"
#include "RenderingEngine.h"

#include <assert.h>

Window::Window(RenderingEngine *renderer, uint32_t width, uint32_t height, std::string title, bool vsyncRequested)
{

}

void Window::initSwapchain()
{

}


void Window::initSurface()
{
	
}

void Window::initSwapchainImages()
{
	
}

void Window::close()
{
	m_windowShouldStayOpen = false; //Well we shouldnt 
}

bool Window::update()
{
	return m_windowShouldStayOpen;
}

uint32_t Window::getWidth()
{
	return m_surfaceSizeX;
}

uint32_t Window::getHeight()
{
	return m_surfaceSizeY;
}

void Window::destroySurface()
{

}

void Window::destroySwapchain()
{

}


void Window::destroySwapchainImages()
{

}


Window::~Window()
{

}
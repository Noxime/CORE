/*
Core engine

Aaro Perämaa
2016
*/

#include "Window.h"
#include "RenderingEngine.h"

#include <assert.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

Window::Window(RenderingEngine *renderer, uint32_t width, uint32_t height, std::string title, bool vsyncRequested)
{
	m_surfaceSizeX = width;
	m_surfaceSizeY = height;
	m_renderer     = renderer;

#ifdef BUILD_ENABLE_VULKAN_DEBUG
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //Dont create a gl context
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);   //Dont resize
#endif

	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
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

bool Window::update()
{
	glfwPollEvents();


	return !glfwWindowShouldClose(m_window);
}

uint32_t Window::getWidth()
{
	return m_surfaceSizeX;
}

uint32_t Window::getHeight()
{
	return m_surfaceSizeY;
}

bool Window::getVSync()
{
	return m_vsyncRequested;
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
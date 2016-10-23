#pragma once


#include "BUILD_OPTIONS.h"
#include "Platform.h"
#include "Window.h"

class RenderingEngine
{
public:
	RenderingEngine(uint32_t width, uint32_t height, std::string title, bool vsyncRequested);
	~RenderingEngine();


	bool run(); //While returns true game will run. Also updates the renderer class
	void clearCurrentBuffer();


	VkInstance               m_instance;                 //Vulkan instance
	VkDebugReportCallbackEXT m_callback;                 //Debug callback
	VkPhysicalDevice         m_pDevice = VK_NULL_HANDLE; //Handle to the physical device
	VkDevice                 m_device;                   //The logical device
	VkQueue                  m_graphicsQueue;            //Where we submit the calls
	VkQueue                  m_presentQueue;             //We need to present the images
	VkSurfaceKHR             m_surface;                  //Rendering surface

private:

	Window *createWindow(uint32_t width, uint32_t height, std::string title, bool vsyncRequested); //Window create and open

	void init();

	void initInstance();
	void initDebugCallback();
	void initSurface();
	void initPhysicalDevice();
	void initDevice();


	Window *m_window                              = nullptr;
};
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
	VkSwapchainKHR           m_swapchain;                //Framebuffer
	std::vector<VkImage>     m_scImages;                 //Swapchain images
	VkFormat                 m_scFormat;                 //Swapchain image format
	VkExtent2D               m_scExtent;                 //Swapchain size
	std::vector<VkImageView> m_scImageViews;             //We see the images through these

private:

	Window *createWindow(uint32_t width, uint32_t height, std::string title, bool vsyncRequested); //Window create and open

	void init();

	void initInstance();
	void initDebugCallback();
	void initSurface();
	void initPhysicalDevice();
	void initDevice();
	void initSwapchain();
	void initImageViews();


	Window *m_window                              = nullptr;
};
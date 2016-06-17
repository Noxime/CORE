#pragma once

#include "Platform.h"
#include <vector>

class VulkanEngine;

class Window
{
public:
	Window(VulkanEngine *renderer, uint32_t width, uint32_t height, std::string title, bool requestedVsync);
	~Window();

	void close(); //Close the window dude, someone is watching your screen over your shoulder
	bool update(); //Update the window aka make it responsive

private:

	void initOSWindow();
	void destroyOSWindow();
	void updateOSWindow();
	void initOSSurface();

	void initSurface();
	void destroySurface();

	void initSwapchain();
	void destroySwapchain();

	void initSwapchainImages();
	void destroySwapchainImages();

	VulkanEngine *m_renderer  = nullptr; //The rendering engine, duh
	VkSurfaceKHR     m_surface   = VK_NULL_HANDLE; //Graphgics surface
	VkSwapchainKHR   m_swapchain = VK_NULL_HANDLE; //Swapchain

	uint32_t	m_surfaceSizeX        = 512; //Width and height of the window
	uint32_t	m_surfaceSizeY        = 512;
	std::string m_title               = "CoreEngine"; //Title of the window
	uint32_t    m_swapchainImageCount = 2; //How-many-buffering?
	bool        m_vsyncRequested      = true; //Mailbox or Immediate? not guaranteed

	VkSurfaceFormatKHR       m_surfaceFormat       = {}; 
	VkSurfaceCapabilitiesKHR m_surfaceCapabilities = {}; //Name kinda says it all.

	std::vector<VkImage>     m_swapchainImages;
	std::vector<VkImageView> m_swapchainImageViews;

	bool m_windowShouldStayOpen = true; //Should this window close

#if VK_USE_PLATFORM_WIN32_KHR

	HINSTANCE       m_win32Instance        = NULL;
	HWND	        m_win32Window	       = NULL;
	std::string     m_win32ClassName	   = "CoreWin32";
	static uint64_t m_win32ClassIdCounter;


#endif

};


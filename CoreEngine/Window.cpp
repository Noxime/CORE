/*
Core engine

Aaro Perämaa
2016
*/

#include "Window.h"
#include "RenderingEngine.h"
#include "Shared.h"

#include <assert.h>

Window::Window(RenderingEngine *renderer, uint32_t width, uint32_t height, std::string title, bool vsyncRequested)
{
	m_renderer       = renderer;
	m_surfaceSizeX   = width;
	m_surfaceSizeY   = height;
	m_title		     = title;
	m_vsyncRequested = vsyncRequested;

	initOSWindow(); //Create the damn window
#if BUILD_WITH_RENDERING_BACKEND == RENDERING_BACKEND_VULKAN
	initSurface(); //And dont forget the graphics surface
	initSwapchain();
	initSwapchainImages();
#elif BUILD_WITH_RENDERING_BACKEND == RENDERING_BACKEND_OPENGL
	initOSSurface();
#endif
}

#if BUILD_WITH_RENDERING_BACKEND == RENDERING_BACKEND_VULKAN

void Window::initSwapchain()
{
	if (m_swapchainImageCount < m_surfaceCapabilities.minImageCount + 1) m_swapchainImageCount = m_surfaceCapabilities.minImageCount + 1; //Make sure implementation supports enough bufferings
	if (m_surfaceCapabilities.maxImageCount > 0)
	{
		if (m_swapchainImageCount > m_surfaceCapabilities.maxImageCount) m_swapchainImageCount = m_surfaceCapabilities.maxImageCount;
	}

	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR; //First-in first-out always supported
	{
		uint32_t presentModeCount = 0; //How many supported
		errorCheck(vkGetPhysicalDeviceSurfacePresentModesKHR(m_renderer->getVulkanPhysicalDevice(), m_surface, &presentModeCount, nullptr)); //Count supported modes
		std::vector<VkPresentModeKHR> presentModeList(presentModeCount); //All present modes
		errorCheck(vkGetPhysicalDeviceSurfacePresentModesKHR(m_renderer->getVulkanPhysicalDevice(), m_surface, &presentModeCount, presentModeList.data())); //populate array
		//We want MAILBOX, which is V-sync with little delay. Fifo is also V-sync but slower and immediate is instant (has tearing though)
		for (auto m : presentModeList)
		{
			if (m == VK_PRESENT_MODE_MAILBOX_KHR   &&  m_vsyncRequested) presentMode = VK_PRESENT_MODE_MAILBOX_KHR; //Look for wanted mode, mailbox if vsync wanted, otherwise immediate.
			if (m == VK_PRESENT_MODE_IMMEDIATE_KHR && !m_vsyncRequested) presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		}
	}

	VkSwapchainCreateInfoKHR swapchainInfo {};
	swapchainInfo.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainInfo.surface               = m_surface;
	swapchainInfo.minImageCount         = m_swapchainImageCount; //How-many-buffering?
	swapchainInfo.imageFormat           = m_surfaceFormat.format; //Same color format as surface
	swapchainInfo.imageColorSpace       = m_surfaceFormat.colorSpace; //Color space same as surface
	swapchainInfo.imageExtent.width     = m_surfaceSizeX; //Window size
	swapchainInfo.imageExtent.height    = m_surfaceSizeY;
	swapchainInfo.imageArrayLayers      = 1; //Stereorendering? TODO: IMPLEMENT VR SUPPORT
	swapchainInfo.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //Color rendering to the window directly
	swapchainInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE; //Share between queue families? nVidia + AMD co-existance?
	swapchainInfo.queueFamilyIndexCount = 0; //For exclusive mode 0
	swapchainInfo.pQueueFamilyIndices   = nullptr; //For exclusive mode 0
	swapchainInfo.preTransform          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR; //Is this rotated? For mobile?
	swapchainInfo.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; //Maybe for transparent windows (on OSX 4 example)
	swapchainInfo.presentMode           = presentMode; //V-Sync option
	swapchainInfo.clipped               = VK_TRUE; //Clip the edges of triangles, should always be active (no downside)
	swapchainInfo.oldSwapchain          = VK_NULL_HANDLE; //If we resized, we can rebuild the swapchain

	errorCheck(vkCreateSwapchainKHR(m_renderer->getVulkanDevice(), &swapchainInfo, nullptr, &m_swapchain));

	errorCheck(vkGetSwapchainImagesKHR(m_renderer->getVulkanDevice(), m_swapchain, &m_swapchainImageCount, nullptr));
}


void Window::initSurface()
{
	initOSSurface(); //Surface shall surface

	VkBool32 WSISupported = false; //Is Windows Surfrace interface supported?
	vkGetPhysicalDeviceSurfaceSupportKHR(m_renderer->getVulkanPhysicalDevice(), m_renderer->getVulkanGraphicsQueueFamilyIndex(), m_surface, &WSISupported);

	if (!WSISupported)
	{
		assert(0 && "WSI not supported");
		std::exit(-1);
	}

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_renderer->getVulkanPhysicalDevice(), m_surface, &m_surfaceCapabilities); //We get the capabilitittities of the surface (eg max size)
	if (m_surfaceCapabilities.currentExtent.width < UINT32_MAX) //Correct size?
	{
		m_surfaceSizeX = m_surfaceCapabilities.currentExtent.width;
		m_surfaceSizeY = m_surfaceCapabilities.currentExtent.height;
	}

	{
		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_renderer->getVulkanPhysicalDevice(), m_surface, &formatCount, nullptr); //How many surface formats?
		if (formatCount == 0) { //How come we dont have any surface formats?
			assert(0 && "Surface formats missing");
			std::exit(-1);
		}
		
		std::vector<VkSurfaceFormatKHR> formats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_renderer->getVulkanPhysicalDevice(), m_surface, &formatCount, formats.data()); //Populate format array
		if (formats[0].format == VK_FORMAT_UNDEFINED) //We dont care about the format
		{
			m_surfaceFormat.format     = VK_FORMAT_R8G8B8A8_UNORM; //RGB truecolor
			m_surfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		}
		else
		{
			m_surfaceFormat = formats[0]; //The optimal format for this system, (GTX 960 B8G8R8A8_UNORM aka BGRA 8bit)
		}
	}
}

void Window::initSwapchainImages()
{
	m_swapchainImages.resize(m_swapchainImageCount);
	m_swapchainImageViews.resize(m_swapchainImageCount);

	errorCheck(vkGetSwapchainImagesKHR(m_renderer->getVulkanDevice(), m_swapchain, &m_swapchainImageCount, m_swapchainImages.data()));

	for (uint32_t i = 0; i < m_swapchainImageCount; i++)
	{
		VkImageViewCreateInfo imageViewInfo = {};
		imageViewInfo.sType			                  = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image                           = m_swapchainImages[i];
		imageViewInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D; //2D images (not volumetric)
		imageViewInfo.format                          = m_surfaceFormat.format; //Same color format as everywhere else
		imageViewInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY; //Red maps to red
		imageViewInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY; //Green maps to green
		imageViewInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY; //Blue maps to blue
		imageViewInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY; //Alpha maps to alpha
		imageViewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT; //Color image
		imageViewInfo.subresourceRange.baseMipLevel   = 0; //The first one
		imageViewInfo.subresourceRange.levelCount     = 1; //Only one mip level
		imageViewInfo.subresourceRange.baseArrayLayer = 0; //the one and only
		imageViewInfo.subresourceRange.layerCount     = 1; //One layer of images

		errorCheck(vkCreateImageView(m_renderer->getVulkanDevice(), &imageViewInfo, nullptr, &m_swapchainImageViews[i]));
	}

}

#endif

void Window::close()
{
	m_windowShouldStayOpen = false; //Well we shouldnt 
}

bool Window::update()
{
	updateOSWindow(); //HEYOO WERE NOW RESPONSIVE

	return m_windowShouldStayOpen;
}

#if BUILD_WITH_RENDERING_BACKEND == RENDERING_BACKEND_VULKAN

void Window::destroySurface()
{
	vkDestroySurfaceKHR(m_renderer->getVulkanInstance(), m_surface, nullptr); //Get lost
}

void Window::destroySwapchain()
{
	vkDestroySwapchainKHR(m_renderer->getVulkanDevice(), m_swapchain, nullptr);
}


void Window::destroySwapchainImages()
{
	for (auto view : m_swapchainImageViews)
	{
		vkDestroyImageView(m_renderer->getVulkanDevice(), view, nullptr);
	}
}

#elif BUILD_WITH_RENDERING_BACKEND == RENDERING_BACKEND_OPENGL

#endif

Window::~Window()
{
#if BUILD_WITH_RENDERING_BACKEND == RENDERING_BACKEND_VULKAN
	destroySwapchainImages();
	destroySwapchain();
	destroySurface();
#endif
	destroyOSWindow(); //Now destroy the window
}
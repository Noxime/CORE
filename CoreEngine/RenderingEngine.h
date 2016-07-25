#pragma once


#include "BUILD_OPTIONS.h"
#include "Platform.h"
#include "Window.h"
#include "Shared.h"

class RenderingEngine
{
public:
	RenderingEngine();
	~RenderingEngine();

	Window *createWindow(uint32_t width, uint32_t height, std::string title, bool vsyncRequested); //Window create and open

	bool run(); //While returns true game will run. Also updates the renderer class


	//Getters
	const VkInstance                  getVulkanInstance()                 const;
	const VkPhysicalDevice            getVulkanPhysicalDevice()           const;
	const VkDevice                    getVulkanDevice()                   const;
	const VkQueue                     getVulkanQueue()                    const;
	const uint32_t                    getVulkanGraphicsQueueFamilyIndex() const;
	const VkPhysicalDeviceProperties &getVulkanPhysicalDeviceProperties() const;


private:

	void setupLayersAndExtensions();

	void initInstance();
	void destroyInstance();

	void initDevice();
	void destroyDevice();

	void setupDebug();

	void initDebug();
	void destroyDebug();

	VkInstance				   m_instance		  = VK_NULL_HANDLE; //Vulkan instance
	VkPhysicalDevice		   m_physicalDevice   = VK_NULL_HANDLE; //Physical device
	VkDevice				   m_device			  = VK_NULL_HANDLE; //Logical device
	VkQueue					   m_queue            = VK_NULL_HANDLE; //Vulkan queue
	VkPhysicalDeviceProperties m_deviceProperties = {}; //Properties for physical device

	uint32_t m_graphicsFamilyIndex	              = 0;

	std::vector<const char*> m_instanceLayers;     //List of instance debugging layers
	std::vector<const char*> m_instanceExtensions; //List of instance extensions
	std::vector<const char*> m_deviceLayers;       //List of device debugging layers
	std::vector<const char*> m_deviceExtensions;   //List of device extensions

	VkDebugReportCallbackEXT		   m_debugReport			 = VK_NULL_HANDLE;
	VkDebugReportCallbackCreateInfoEXT m_debugCallbackCreateInfo = {}; //Create callback info


	Window *m_window                              = nullptr;
};
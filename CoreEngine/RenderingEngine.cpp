#include "RenderingEngine.h"


#ifdef _WIN32
#include <Windows.h>
#endif

RenderingEngine::RenderingEngine()
{
	setupLayersAndExtensions();
	setupDebug();
	initInstance();
	initDebug();
	initDevice();
}

Window *RenderingEngine::createWindow(uint32_t width, uint32_t height, std::string title, bool vsyncRequested)
{
	m_window = new Window(this, width, height, title, vsyncRequested);

	return m_window;
}

bool RenderingEngine::run()
{
	if (m_window != nullptr)
	{
		return m_window->update();
	}
	return true;
}

const VkInstance RenderingEngine::getVulkanInstance() const
{
	return m_instance;
}

const VkPhysicalDevice RenderingEngine::getVulkanPhysicalDevice() const
{
	return m_physicalDevice;
}

const VkDevice RenderingEngine::getVulkanDevice() const
{
	return m_device;
}

const VkQueue RenderingEngine::getVulkanQueue() const
{
	return m_queue;
}

const uint32_t RenderingEngine::getVulkanGraphicsQueueFamilyIndex() const
{
	return m_graphicsFamilyIndex;
}

const VkPhysicalDeviceProperties & RenderingEngine::getVulkanPhysicalDeviceProperties() const
{
	return m_deviceProperties;
}


void RenderingEngine::setupLayersAndExtensions()
{
//	m_instanceExtensions.push_back(VK_KHR_DISPLAY_EXTENSION_NAME); //Not available in most systems
	m_instanceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME); //Load extensions
	m_instanceExtensions.push_back(PLATFORM_SURFACE_EXTENSION_NAME); //What if were on linux? Check platform

	m_deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME); //Enable swapchains
}

void RenderingEngine::initInstance()
{
	VkApplicationInfo appInfo{}; //Information about the application
	appInfo.sType			   = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.apiVersion		   = VK_MAKE_VERSION(1, 0, 3); //Lowest vulkan api we allow
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.pApplicationName   = "CoreEngine";

	VkInstanceCreateInfo instanceInfo{}; //Information for creating Vulkan Instance
	instanceInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo        = &appInfo;
	instanceInfo.enabledLayerCount       = m_instanceLayers.size();
	instanceInfo.ppEnabledLayerNames     = m_instanceLayers.data(); //Wowza thats a pretty curve
	instanceInfo.enabledExtensionCount   = m_instanceExtensions.size();
	instanceInfo.ppEnabledExtensionNames = m_instanceExtensions.data();
	instanceInfo.pNext                   = &m_debugCallbackCreateInfo;

	errorCheck(vkCreateInstance(&instanceInfo, nullptr, &m_instance)); //Create the instance
}

void RenderingEngine::initDevice()
{
	{
		uint32_t deviceCount = 0; //Amount of graphic chips in the system
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr); //Get the amout on GPU's in system
		std::vector<VkPhysicalDevice> deviceList(deviceCount); //Create list of GPU's of the appropriate size
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, deviceList.data()); //Populate the list of devices
		m_physicalDevice = deviceList[0]; //Get the first device. Might not be the best one!
		std::cout << "Physical devices: " << deviceCount << std::endl;
		vkGetPhysicalDeviceProperties(m_physicalDevice, &m_deviceProperties);
	}
	{
		uint32_t familyCount = 0; //Amount of families in this device
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &familyCount, nullptr); //Get the count of families
		std::vector<VkQueueFamilyProperties> familyList(familyCount); //Create a list of the families
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &familyCount, familyList.data()); //Now populate the list
		std::cout << "Queue families: " << familyCount << std::endl;

		bool foundGraphicsBit = false;
		for (uint32_t i = 0; i < familyCount; i++) //Iterate the families
		{
			if (familyList[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) //Do we have a graphics capable device?
			{
				foundGraphicsBit = true;
				m_graphicsFamilyIndex = i;
			}
		}

		if (!foundGraphicsBit) //We dont have a graphics device, so umm... kinda useless rendering engine right
		{
			assert(0 && "Vulkan error: No family with graphics found");
			std::exit(-1);
		}
	}
	{
		uint32_t layerCount = 0; //Vulkan layer count
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr); //Get layer property count
		std::vector<VkLayerProperties> layerList(layerCount); //list of layers
		vkEnumerateInstanceLayerProperties(&layerCount, layerList.data()); //Get the layers
		std::cout << "Instance layers: " << std::endl;
		for (auto &i : layerList)
		{
			std::cout << i.layerName << " " << i.description << std::endl;
		}
	}
	{
		uint32_t layerCount = 0; //Device layer count
		vkEnumerateDeviceLayerProperties(m_physicalDevice, &layerCount, nullptr); //Get layer property count
		std::vector<VkLayerProperties> layerList(layerCount); //list of layers
		vkEnumerateDeviceLayerProperties(m_physicalDevice, &layerCount, layerList.data()); //Get the layers
		std::cout << "Device layers: " << std::endl;
		for (auto &i : layerList)
		{
			std::cout << i.layerName << " " << i.description << std::endl;
		}
	}

	float queuePriorities[]{ 1.0f }; //The importance of queue families, we have one one so 100% importance for it.
	VkDeviceQueueCreateInfo createQueueInfo{}; //Information for creating the device queue
	createQueueInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	createQueueInfo.queueFamilyIndex = m_graphicsFamilyIndex; //We want graphics queue
	createQueueInfo.queueCount       = 1; //Only one type of family
	createQueueInfo.pQueuePriorities = queuePriorities;

	VkDeviceCreateInfo createDeviceInfo{}; //Information for creating the device
	createDeviceInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createDeviceInfo.queueCreateInfoCount    = 1; //We only want one queue family, the rendering one here
	createDeviceInfo.pQueueCreateInfos       = &createQueueInfo;
	createDeviceInfo.enabledLayerCount       = m_deviceLayers.size();
	createDeviceInfo.ppEnabledLayerNames     = m_deviceLayers.data();
	createDeviceInfo.enabledExtensionCount   = m_deviceExtensions.size();
	createDeviceInfo.ppEnabledExtensionNames = m_deviceExtensions.data();

	errorCheck(vkCreateDevice(m_physicalDevice, &createDeviceInfo, nullptr, &m_device)); //Create the logical device
	
	vkGetDeviceQueue(m_device, m_graphicsFamilyIndex, 0, &m_queue);
}

#if BUILD_ENABLE_VULKAN_DEBUG

VKAPI_ATTR VkBool32 VKAPI_CALL //???? Some kind of debug output function? idk what this is, im not experienced enough in C++
VulkanDebugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t srcObj,
	size_t location,
	int32_t msgCode,
	const char* layerPrefix,
	const char* msg,
	void* userData
	)
{

	std::ostringstream stream;
	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
	{
		stream << "Vulkan info: ";
	}
	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
	{
		stream << "Vulkan warning: ";
	}
	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
	{
		stream << "Vulkan performance warning: ";
	}
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		stream << "Vulkan error: ";
	}
	if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
	{
		stream << "Vulkan debug: ";
	}

	stream << "@[" << layerPrefix << "]: ";
	stream << msg;

	std::cout << stream.str() << std::endl;

#ifdef _WIN32 //Open a message box with the error if on windows
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		MessageBox(NULL, stream.str().c_str(), "Vulkan error!", 0);
	}
#endif

	return false;
}


void RenderingEngine::setupDebug()
{
	m_instanceLayers.push_back("VK_LAYER_LUNARG_standard_validation"); //Enable default validation layers
	m_deviceLayers.push_back("VK_LAYER_LUNARG_standard_validation");

	m_instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME); //Enable debug extension

	m_debugCallbackCreateInfo.sType		  = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	m_debugCallbackCreateInfo.pfnCallback = VulkanDebugCallback;
	m_debugCallbackCreateInfo.flags		  = VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
											VK_DEBUG_REPORT_WARNING_BIT_EXT |
											VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
											VK_DEBUG_REPORT_ERROR_BIT_EXT |
											VK_DEBUG_REPORT_DEBUG_BIT_EXT;
}

PFN_vkCreateDebugReportCallbackEXT fvkCreateDebugReportCallbackEXT = nullptr; //Fetch the debug reprot callback extension
PFN_vkDestroyDebugReportCallbackEXT fvkDestroyDebugReportCallbackEXT = nullptr; //Fetch the debug reprot callback extension

void RenderingEngine::initDebug()
{

	fvkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugReportCallbackEXT"); //Get the function pointers
	fvkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT");

	if (fvkCreateDebugReportCallbackEXT == nullptr || fvkDestroyDebugReportCallbackEXT == nullptr)
	{
		assert(0 && "Vulkan error: Cant fetch debug function pointers");
		std::exit(-1);
	}

	
	fvkCreateDebugReportCallbackEXT(m_instance, &m_debugCallbackCreateInfo, nullptr, &m_debugReport);
}

void RenderingEngine::destroyDebug()
{
	fvkDestroyDebugReportCallbackEXT(m_instance, m_debugReport, nullptr);
	m_debugReport = VK_NULL_HANDLE;
}

#else

void RenderingEngine::initDebug() {}
void RenderingEngine::setupDebug() {}
void RenderingEngine::destroyDebug() {}

#endif //BUILD_ENABLE_VULKAN_DEBUG

void RenderingEngine::destroyInstance()
{
	vkDestroyInstance(m_instance, nullptr);
	m_instance = VK_NULL_HANDLE;
}

void RenderingEngine::destroyDevice()
{
	vkDestroyDevice(m_device, nullptr);
	m_device = VK_NULL_HANDLE;
}


RenderingEngine::~RenderingEngine()
{
	delete m_window; //Yeah no we dont want you

	destroyDevice(); //Device has to be removed before instance

	destroyDebug();
	destroyInstance();
}


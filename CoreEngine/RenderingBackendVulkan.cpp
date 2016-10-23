/*
Core engine

Aaro Perämaa
2016
*/

#include "RenderingEngine.h"

#if BUILD_WITH_RENDERING_BACKEND == RENDERING_BACKEND_VULKAN

RenderingEngine::RenderingEngine(uint32_t width, uint32_t height, std::string title, bool vsyncRequested)
{
	glfwInit();
	createWindow(width, height, title, vsyncRequested);

	//init();

	initInstance();
	initDebugCallback();
	initSurface();
	initPhysicalDevice();
	initDevice();
}




void RenderingEngine::clearCurrentBuffer()
{


}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layerPrefix,
	const char* msg,
	void* userData) {

	std::cerr << "Validation layer: " << msg << std::endl;

	return VK_FALSE;
}

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {

	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

/*

unsigned int i = 0;

void RenderingEngine::init()
{
	// EXTENSIONS
	std::vector<const char*> extensions;
	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (i = 0; i < glfwExtensionCount; i++) {
		extensions.push_back(glfwExtensions[i]);
	}

#ifdef BUILD_ENABLE_VULKAN_DEBUG
	extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);


	VkDebugReportCallbackCreateInfoEXT debugInfo = {};
	debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	debugInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	debugInfo.pfnCallback = debugCallback;

#endif


	// LAYERS
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};


	// Istance
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan";                            //APP name
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Core Engine";                       //Plz dont change, i need da fame
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();

#ifdef BUILD_ENABLE_VULKAN_DEBUG
	createInfo.enabledLayerCount = validationLayers.size();
	createInfo.ppEnabledLayerNames = validationLayers.data();
#else
	createInfo.enabledLayerCount = 0;
#endif


	if (int e = vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
		std::cout << "errer boi " << e;
		throw std::runtime_error("VK: Failed to create instance! " + e);
	}

#ifdef BUILD_ENABLE_VULKAN_DEBUG
	if (CreateDebugReportCallbackEXT(m_instance, &debugInfo, nullptr, &m_callback) != VK_SUCCESS) {
		std::cout << "errer";
		throw std::runtime_error("VK: Failed to set up debug callback!");
	}
#endif // DEBUG


	// VULKAN DEVICE
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr); //Count devices
	if (deviceCount == 0) {
		std::cout << "errer";
		throw std::runtime_error("VK: Failed to find devices with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount); //All devices in the system
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

	m_pDevice = devices.at(0);

	uint32_t queueFamilyCount = 0; //Amount of queues on the device
	vkGetPhysicalDeviceQueueFamilyProperties(m_pDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount); //All the queue families
	vkGetPhysicalDeviceQueueFamilyProperties(m_pDevice, &queueFamilyCount, queueFamilies.data());

	//Find the graphics queue
	int graphicsFamily = -1;
	int i = 0;
	for (auto &queueFamily : queueFamilies)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphicsFamily = i;
		}

		if (graphicsFamily >= 0)
		{
			break;
		}

		i++;
	}

	if (graphicsFamily < 0)
	{
		std::cout << "errer";
		throw std::runtime_error("VK: Failed to find devices with graphics support!");
	}

	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = graphicsFamily;
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures = {}; //Device features, like geometry shaders. We dont need anything for now so just leave it alone

	VkDeviceCreateInfo deviceCreateInfo = {}; //NOW MAKE THE DEVICE
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	deviceCreateInfo.enabledExtensionCount = 0;

#ifdef BUILD_ENABLE_VULKAN_DEBUG
	deviceCreateInfo.enabledLayerCount = validationLayers.size();
	deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#else
	deviceCreateInfo.enabledLayerCount = 0;
#endif


	if (vkCreateDevice(m_pDevice, &deviceCreateInfo, nullptr, &m_device) != VK_SUCCESS) {
		std::cout << "errer";
		throw std::runtime_error("VK: Failed to create logical device!");
	}

	vkGetDeviceQueue(m_device, graphicsFamily, 0, &m_graphicsQueue); //We kinda want the queue


	// SURFACE

	if (glfwCreateWindowSurface(m_instance, m_window->m_window, nullptr, &m_surface) != VK_SUCCESS) {
		std::cout << "error";
		throw std::runtime_error("VK: Failed to create window surface!");
	}

	// PRESENTATION

	//PRESENT
	int presentFamily = -1;

	VkBool32 presentSupport = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(m_pDevice, i, m_surface, &presentSupport);

	if(presentSupport && )
	{
		presentFamily = i;
	}
}

//*/






struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};


const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};


bool checkValidationLayerSupport() {

	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

std::vector<const char*> getRequiredExtensions() {
	std::vector<const char*> extensions;

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (unsigned int i = 0; i < glfwExtensionCount; i++) {
		extensions.push_back(glfwExtensions[i]);
	}

#ifdef BUILD_ENABLE_VULKAN_DEBUG
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif // BUILD_ENABLE_VULKAN_DEBUG


	return extensions;
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR m_surface) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);

		if (queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR m_surface) {
	QueueFamilyIndices indices = findQueueFamilies(device, m_surface);

	return indices.isComplete();
}




void RenderingEngine::initInstance()
{
#ifdef BUILD_ENABLE_VULKAN_DEBUG
	if (!checkValidationLayerSupport()) {
		std::cout << "VK: Validation layers requested, but not available!";
	}
#endif // DEBUG


	VkApplicationInfo appInfo  = {};
	appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName   = "Sambro";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName        = "Core Engine";                  //Gimme some credit bby
	appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion         = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType                = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo     = &appInfo;

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount   = extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();

#ifdef BUILD_ENABLE_VULKAN_DEBUG
		createInfo.enabledLayerCount   = validationLayers.size();
		createInfo.ppEnabledLayerNames = validationLayers.data();
#else
		createInfo.enabledLayerCount = 0;
#endif

	if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
		std::cout << "VK: Failed to create instance!";
	}

}

void RenderingEngine::initDebugCallback()
{
#ifdef BUILD_ENABLE_VULKAN_DEBUG

	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType                              = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags                              = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback                        = debugCallback;

	if (CreateDebugReportCallbackEXT(m_instance, &createInfo, nullptr, &m_callback) != VK_SUCCESS) {
		std::cout << "VK: Failed to set up debug callback!";
	}
#endif
}

void RenderingEngine::initSurface()
{
	if (glfwCreateWindowSurface(m_instance, m_window->m_window, nullptr, &m_surface) != VK_SUCCESS) {
		std::cout << "VK: Failed to create window surface!";
	}
}

void RenderingEngine::initPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		std::cout << "VK: Failed to find GPUs with Vulkan support!";
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (isDeviceSuitable(device, m_surface)) {
			m_pDevice = device;
			break;
		}
	}

	if (m_pDevice == VK_NULL_HANDLE) {
		std::cout << "VK: Failed to find a suitable GPU!";
	}
}

void RenderingEngine::initDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(m_pDevice, m_surface);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

	float queuePriority = 1.0f;
	for (int queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex        = queueFamily;
		queueCreateInfo.queueCount              = 1;
		queueCreateInfo.pQueuePriorities        = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo   = {};
	createInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos    = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = 0;

#ifdef BUILD_ENABLE_VULKAN_DEBUG
		createInfo.enabledLayerCount   = validationLayers.size();
		createInfo.ppEnabledLayerNames = validationLayers.data();
#else
		createInfo.enabledLayerCount = 0;
#endif

	if (vkCreateDevice(m_pDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
		std::cout << "VK: Failed to create logical device!";
	}

	vkGetDeviceQueue(m_device, indices.graphicsFamily, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_device, indices.presentFamily, 0, &m_presentQueue);

}


Window *RenderingEngine::createWindow(uint32_t width, uint32_t height, std::string title, bool vsyncRequested)
{
	m_window = new Window(this, width, height, title, vsyncRequested);

	// SURFACE
	

	return m_window;
}


void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}

RenderingEngine::~RenderingEngine()
{
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	vkDestroyDevice(m_device, nullptr);
	DestroyDebugReportCallbackEXT(m_instance, m_callback, nullptr);
	vkDestroyInstance(m_instance, nullptr);

	delete m_window; //Yeah no we dont want you
}

#endif
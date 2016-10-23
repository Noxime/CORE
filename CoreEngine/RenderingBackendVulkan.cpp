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
	initSwapchain();

	std::cout << "VK: Init complete" << std::endl;
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

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};
const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};




// HELPER METHODS

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

bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR m_surface) {
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR m_surface) {
	QueueFamilyIndices indices = findQueueFamilies(device, m_surface);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, m_surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {

	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) { //The best possible format
		return{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& availableFormat : availableFormats) { //Other ones
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes, bool vsync) { //VSYNC mode

	if (vsync)
	{
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) { //Triple buffering
				return availablePresentMode;
			}
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height) { //Swapchain size
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		VkExtent2D actualExtent = { width, height };

		actualExtent.width  = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}







// INIT INIT INIT

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

	VkDeviceCreateInfo createInfo      = {};
	createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos       = queueCreateInfos.data();
	createInfo.queueCreateInfoCount    = (uint32_t)queueCreateInfos.size();
	createInfo.pEnabledFeatures        = &deviceFeatures;
	createInfo.enabledExtensionCount   = deviceExtensions.size();
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

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

void RenderingEngine::initSwapchain()
{
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_pDevice, m_surface);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode     = chooseSwapPresentMode(swapChainSupport.presentModes, m_window->getVSync());
	VkExtent2D extent                = chooseSwapExtent(swapChainSupport.capabilities, m_window->getWidth(), m_window->getHeight());


	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface                  = m_surface;
	createInfo.minImageCount            = imageCount;
	createInfo.imageFormat              = surfaceFormat.format;
	createInfo.imageColorSpace          = surfaceFormat.colorSpace;
	createInfo.imageExtent              = extent;
	createInfo.imageArrayLayers         = 1;   //TODO: VR support. Aka: geta job and get a vive
	createInfo.imageUsage               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //Directly render to me. TODO: Post process support soo... VK_IMAGE_USAGE_TRANSFER_DST_BIT


	// IF QUEUES ARE DIFFERENT, ENABLE SWAPCHAIN SHARING

	QueueFamilyIndices indices = findQueueFamilies(m_pDevice, m_surface);
	uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices   = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices   = nullptr; // Optional
	}

	createInfo.preTransform   = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; //We dont want to blend the window with the background
	createInfo.presentMode    = presentMode;
	createInfo.clipped        = VK_TRUE; //If hidden by other windows should we still render. MIGHT BE UNSAFE FOR RECORDING

	createInfo.oldSwapchain = VK_NULL_HANDLE; //TODO: Window resize.


	if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapchain) != VK_SUCCESS) {
		std::cout << "VK: Failed to create swap chain!";
	}


	// Swapchain images

	vkGetSwapchainImagesKHR(m_device, m_swapchain, &imageCount, nullptr);
	m_scImages.resize(imageCount);
	vkGetSwapchainImagesKHR(m_device, m_swapchain, &imageCount, m_scImages.data());

	m_scFormat = surfaceFormat.format;
	m_scExtent = extent;

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
	vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	vkDestroyDevice(m_device, nullptr);
	DestroyDebugReportCallbackEXT(m_instance, m_callback, nullptr);
	vkDestroyInstance(m_instance, nullptr);

	std::cout << "VK: Destruction complete." << std::endl;

	delete m_window; //Yeah no we dont want you
}

#endif
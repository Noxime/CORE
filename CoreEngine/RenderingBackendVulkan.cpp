/*
Core engine

Aaro Perämaa
2016
*/

#include "RenderingEngine.h"

#if BACKEND == BACKEND_VK

static uint32_t shaderID = 0;

Shader RenderingEngine::makeShader(std::vector<char> vert, std::vector<char> frag)
{
	VkShaderModule vertModule;
	VkShaderModule fragModule;

	VkShaderModuleCreateInfo vertCreateInfo = {};
	vertCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vertCreateInfo.codeSize = vert.size();
	vertCreateInfo.pCode = (uint32_t*)vert.data();

	if (vkCreateShaderModule(m_device, &vertCreateInfo, nullptr, &vertModule) != VK_SUCCESS) {
		std::cout << "VK: Failed to create vertex shader module!" << std::endl;
	}

	VkShaderModuleCreateInfo fragCreateInfo = {};
	fragCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	fragCreateInfo.codeSize = frag.size();
	fragCreateInfo.pCode = (uint32_t*)frag.data();

	if (vkCreateShaderModule(m_device, &fragCreateInfo, nullptr, &fragModule) != VK_SUCCESS) {
		std::cout << "VK: Failed to create fragment shader module!" << std::endl;
	}



	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragModule;
	fragShaderStageInfo.pName = "main";


	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };


	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();



	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)m_scExtent.width;
	viewport.height = (float)m_scExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = m_scExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE; //Could be useful for shadowmaps
	rasterizer.rasterizerDiscardEnable = VK_FALSE; //This disables the rasterizer

	rasterizer.polygonMode = VK_POLYGON_MODE_FILL; //Triangle mode
	rasterizer.lineWidth = 1.0f;

	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;   //Backface culling
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE; //Triangles are clockwise

	rasterizer.depthBiasEnable = VK_FALSE; //Could be used for shadow mapping

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; /// Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0; // Optional
	pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = 0; // Optional

	VkPipelineLayout layout;

	if (vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &layout) != VK_SUCCESS) {
		std::cout << "VK: Failed to create pipeline layout!" << std::endl;
	}


	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional
	pipelineInfo.layout = layout;
	pipelineInfo.renderPass = m_renderpass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1; // Optional

	VkPipeline pipeline;

	if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
		std::cout << "VK: Failed to create graphics pipeline!" << std::endl;
	}

	vkDestroyShaderModule(m_device, vertModule, nullptr);
	vkDestroyShaderModule(m_device, fragModule, nullptr);

	shaderID++;

	m_layouts[shaderID] = layout;
	m_shaders[shaderID] = pipeline;

	std::cout << "VK: Shader created" << std::endl;

	m_currentShader = shaderID;

	return Shader(shaderID);
}


uint32_t findMemoryType(VkPhysicalDevice pDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

static uint32_t vertexID = 0;

Mesh RenderingEngine::makeMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{	
	vertexID++;

	{ //Vertex buffer
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(m_device, stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffers[vertexID], m_vertexMemories[vertexID]);

		copyBuffer(stagingBuffer, m_vertexBuffers[vertexID], bufferSize);

		vkDestroyBuffer(m_device, stagingBuffer, nullptr); //Cleanup
		vkFreeMemory(m_device, stagingBufferMemory, nullptr);
	}

	{ //Index buffer
		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), (size_t)bufferSize);
		vkUnmapMemory(m_device, stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffers[vertexID], m_indexMemories[vertexID]); //Difference is here

		copyBuffer(stagingBuffer, m_indexBuffers[vertexID], bufferSize);

		vkDestroyBuffer(m_device, stagingBuffer, nullptr); //Cleanup
		vkFreeMemory(m_device, stagingBufferMemory, nullptr);
	}


	std::cout << "VK: Created mesh" << std::endl;

	return Mesh(vertexID, 0, vertices.size(), indices.size());

}





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
	initImageViews();
	initRenderpass();
	//initPipeline();

	initFramebuffers();
	initCommandPool();

	//initVertexBuffer();

	initCommandBuffers();
	initSemaphores();
	
	std::cout << "VK: Init complete" << std::endl;
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

	std::cerr << "VK: " << layerPrefix << ": " <<  msg << std::endl;

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

#ifdef VK_DEBUG
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

uint32_t findMemoryType(VkPhysicalDevice pDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) 
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(pDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}


	std::cout << "VK: Failed to find suitable memory type!" << std::endl;
	return 0;
}












// INIT INIT INIT
void RenderingEngine::initInstance()
{
#ifdef VK_DEBUG
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

#ifdef VK_DEBUG
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
#ifdef VK_DEBUG

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

#ifdef VK_DEBUG
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
	createInfo.oldSwapchain   = VK_NULL_HANDLE;


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

void RenderingEngine::initImageViews()
{
	m_scImageViews.resize(m_scImages.size(), VkImageView());

	for (uint32_t i = 0; i < m_scImages.size(); i++) {
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType                 = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image                 = m_scImages[i];
		createInfo.viewType              = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format                = m_scFormat;

		// Color swizzle
		createInfo.components.r          = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g          = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b          = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a          = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT; //Color images
		createInfo.subresourceRange.baseMipLevel   = 0;
		createInfo.subresourceRange.levelCount     = 1; //No mip mapping
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount     = 1; //More VR compatibility

		if (vkCreateImageView(m_device, &createInfo, nullptr, &m_scImageViews[i]) != VK_SUCCESS) {
			std::cout << "VK: Failed to create image views!";
		}
	}

}

void RenderingEngine::initRenderpass()
{
	

	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format                  = m_scFormat;
	colorAttachment.samples                 = VK_SAMPLE_COUNT_1_BIT;

	colorAttachment.loadOp                  = VK_ATTACHMENT_LOAD_OP_CLEAR; //Clear screen
	colorAttachment.storeOp                 = VK_ATTACHMENT_STORE_OP_STORE;

	colorAttachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	colorAttachment.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout             = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment            = 0;
	colorAttachmentRef.layout                = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subPass = {};
	subPass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subPass.colorAttachmentCount = 1;
	subPass.pColorAttachments    = &colorAttachmentRef;


	VkSubpassDependency dependency = {};
	dependency.srcSubpass          = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass          = 0;
	dependency.srcStageMask        = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependency.srcAccessMask       = VK_ACCESS_MEMORY_READ_BIT;
	dependency.dstStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount        = 1;
	renderPassInfo.pAttachments           = &colorAttachment;
	renderPassInfo.subpassCount           = 1;
	renderPassInfo.pSubpasses             = &subPass;
	renderPassInfo.dependencyCount        = 1;
	renderPassInfo.pDependencies          = &dependency;

	if (vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderpass) != VK_SUCCESS) {
		std::cout << "VK: Failed to create render pass!" << std::endl;
	}

}

/*
void RenderingEngine::initPipeline()
{
	auto vertsource = FileIO::loadFile("../resources/shaders/vert.spv");
	auto fragsource = FileIO::loadFile("../resources/shaders/frag.spv");

	VkShaderModule vertModule;
	VkShaderModule fragModule;

	VkShaderModuleCreateInfo vertCreateInfo = {};
	vertCreateInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vertCreateInfo.codeSize                 = vertsource.size();
	vertCreateInfo.pCode                    = (uint32_t*)vertsource.data();

	if (vkCreateShaderModule(m_device, &vertCreateInfo, nullptr, &vertModule) != VK_SUCCESS) {
		std::cout << "VK: Failed to create vertex shader module!" << std::endl;
	}

	VkShaderModuleCreateInfo fragCreateInfo = {};
	fragCreateInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	fragCreateInfo.codeSize                 = fragsource.size();
	fragCreateInfo.pCode                    = (uint32_t*)fragsource.data();

	if (vkCreateShaderModule(m_device, &fragCreateInfo, nullptr, &fragModule) != VK_SUCCESS) {
		std::cout << "VK: Failed to create fragment shader module!" << std::endl;
	}



	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage                           = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module                          = vertModule;
	vertShaderStageInfo.pName                           = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage                           = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module                          = fragModule;
	fragShaderStageInfo.pName                           = "main";


	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };


	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType                                = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount        = 0;
	vertexInputInfo.pVertexBindingDescriptions           = nullptr; // Optional
	vertexInputInfo.vertexAttributeDescriptionCount      = 0;
	vertexInputInfo.pVertexAttributeDescriptions         = nullptr; // Optional

	auto bindingDescription    = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount   = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
	vertexInputInfo.pVertexBindingDescriptions      = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions    = attributeDescriptions.data();



	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology                               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable                 = VK_FALSE;

	VkViewport viewport = {};
	viewport.x          = 0.0f;
	viewport.y          = 0.0f;
	viewport.width      = (float)m_scExtent.width;
	viewport.height     = (float)m_scExtent.height;
	viewport.minDepth   = 0.0f;
	viewport.maxDepth   = 1.0f;

	VkRect2D scissor = {};
	scissor.offset   = { 0, 0 };
	scissor.extent   = m_scExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType                             = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount                     = 1;
	viewportState.pViewports                        = &viewport;
	viewportState.scissorCount                      = 1;
	viewportState.pScissors                         = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable                       = VK_FALSE; //Could be useful for shadowmaps
	rasterizer.rasterizerDiscardEnable                = VK_FALSE; //This disables the rasterizer

	rasterizer.polygonMode = VK_POLYGON_MODE_FILL; //Triangle mode
	rasterizer.lineWidth   = 1.0f;

	rasterizer.cullMode    = VK_CULL_MODE_BACK_BIT;   //Backface culling
	rasterizer.frontFace   = VK_FRONT_FACE_CLOCKWISE; //Triangles are clockwise

	rasterizer.depthBiasEnable = VK_FALSE; //Could be used for shadow mapping

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType                                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable                  = VK_FALSE;
	multisampling.rasterizationSamples                 = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading                     = 1.0f; // Optional
	multisampling.pSampleMask                          = nullptr; /// Optional
	multisampling.alphaToCoverageEnable                = VK_FALSE; // Optional
	multisampling.alphaToOneEnable                     = VK_FALSE; // Optional

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask                      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable                         = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor                 = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor                 = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp                        = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor                 = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor                 = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp                        = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType                               = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable                       = VK_FALSE;
	colorBlending.logicOp                             = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount                     = 1;
	colorBlending.pAttachments                        = &colorBlendAttachment;
	colorBlending.blendConstants[0]                   = 0.0f; // Optional
	colorBlending.blendConstants[1]                   = 0.0f; // Optional
	colorBlending.blendConstants[2]                   = 0.0f; // Optional
	colorBlending.blendConstants[3]                   = 0.0f; // Optional

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount             = 0; // Optional
	pipelineLayoutInfo.pSetLayouts                = nullptr; // Optional
	pipelineLayoutInfo.pushConstantRangeCount     = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges        = 0; // Optional

	if (vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
		std::cout << "VK: Failed to create pipeline layout!" << std::endl;
	}


	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount                   = 2;
	pipelineInfo.pStages                      = shaderStages;
	pipelineInfo.pVertexInputState            = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState          = &inputAssembly;
	pipelineInfo.pViewportState               = &viewportState;
	pipelineInfo.pRasterizationState          = &rasterizer;
	pipelineInfo.pMultisampleState            = &multisampling;
	pipelineInfo.pDepthStencilState           = nullptr; // Optional
	pipelineInfo.pColorBlendState             = &colorBlending;
	pipelineInfo.pDynamicState                = nullptr; // Optional
	pipelineInfo.layout                       = m_pipelineLayout;
	pipelineInfo.renderPass                   = m_renderpass;
	pipelineInfo.subpass                      = 0;
	pipelineInfo.basePipelineHandle           = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex            = -1; // Optional

	if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS) {
		std::cout << "VK: Failed to create graphics pipeline!" << std::endl;
	}

	vkDestroyShaderModule(m_device, vertModule, nullptr);
	vkDestroyShaderModule(m_device, fragModule, nullptr);
}
*/


void RenderingEngine::initFramebuffers()
{
	m_framebuffers.resize(m_scImageViews.size(), VkFramebuffer());

	for (size_t i = 0; i < m_scImageViews.size(); i++) 
	{
		VkImageView attachments[] = {
			m_scImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass              = m_renderpass;
		framebufferInfo.attachmentCount         = 1;
		framebufferInfo.pAttachments            = attachments;
		framebufferInfo.width                   = m_scExtent.width;
		framebufferInfo.height                  = m_scExtent.height;
		framebufferInfo.layers                  = 1;

		if (vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &m_framebuffers[i]) != VK_SUCCESS) {
			std::cout << "VK: Failed to create framebuffer!" << std::endl;
		}
	}


}

void RenderingEngine::initCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(m_pDevice, m_surface);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex        = queueFamilyIndices.graphicsFamily;
	poolInfo.flags                   = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;// if rerecorded often

	if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
		std::cout << "VK: Failed to create command pool!" << std::endl;
	}

}

//nuevas sabor delicias
/*
void RenderingEngine::initVertexBuffer()
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size               = sizeof(exampleVertices[0]) * exampleVertices.size();

	bufferInfo.usage              = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode        = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(m_device, &bufferInfo, nullptr, &m_vertexBuffer) != VK_SUCCESS) {
		std::cout << "VK: Failed to create vertex buffer!" << std::endl;
	}


	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(m_device, m_vertexBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize       = memRequirements.size;
	allocInfo.memoryTypeIndex      = findMemoryType(m_pDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);


	if (vkAllocateMemory(m_device, &allocInfo, nullptr, &m_vertexMemory) != VK_SUCCESS) {
		std::cout << "VK: Failed to allocate vertex buffer memory!" << std::endl;
	}

	vkBindBufferMemory(m_device, m_vertexBuffer, m_vertexMemory, 0);

	void* data;
	vkMapMemory(m_device, m_vertexMemory, 0, bufferInfo.size, 0, &data);

	memcpy(data, exampleVertices.data(), (size_t)bufferInfo.size);
	vkUnmapMemory(m_device, m_vertexMemory);
}
*/

void RenderingEngine::initCommandBuffers()
{
	if (m_commandBuffers.size() > 0) {
		vkFreeCommandBuffers(m_device, m_commandPool, m_commandBuffers.size(), m_commandBuffers.data());
	}

	m_commandBuffers.resize(m_framebuffers.size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool                 = m_commandPool;
	allocInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount          = (uint32_t)m_commandBuffers.size();

	if (vkAllocateCommandBuffers(m_device, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
		std::cout << "VK: Failed to allocate command buffers!" << std::endl;
	}



	
	//reRecordCmdBuf();


}

void RenderingEngine::initSemaphores()
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType                 = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
		vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS) {

		std::cout << "VK: Failed to create semaphores!" << std::endl;
	}
}






















void threadedCmdRecord(VkCommandBuffer buf, VkRenderPass ren, VkFramebuffer fra, VkExtent2D ext, VkPipeline pip, VkBuffer vb, VkBuffer ib, uint32_t vCount, bool clear)
{
	
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags                    = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	beginInfo.pInheritanceInfo         = nullptr; // Optional

	vkBeginCommandBuffer(buf, &beginInfo);

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType                 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass            = ren;
	renderPassInfo.framebuffer           = fra;
	renderPassInfo.renderArea.offset     = { 0, 0 };
	renderPassInfo.renderArea.extent     = ext;

	//if (clear)
	{
		VkClearValue clearColor        = { 1.0f, 0.0f, 1.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues    = &clearColor;
	}
	vkCmdBeginRenderPass(buf, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


	vkCmdBindPipeline(buf, VK_PIPELINE_BIND_POINT_GRAPHICS, pip);

	VkBuffer vertexBuffers[] = { vb };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(buf, 0, 1, vertexBuffers, offsets); //Vertex buffers

	vkCmdBindIndexBuffer(buf, ib, 0, VK_INDEX_TYPE_UINT32); //Index buffers

	//vkCmdDraw(buf, vCount, 1, 0, 0);
	vkCmdDrawIndexed(buf, vCount, 1, 0, 0, 0);

//	vkCmdBindPipeline(buf, VK_PIPELINE_BIND_POINT_GRAPHICS, pip);// Pipeline binding!

//	vkCmdDraw(buf, 3, 1, 0, 0); //Draw the triangles, vert count, instance count, first vert, first inst.

	vkCmdEndRenderPass(buf); //STOP HAVING FUN

	if (vkEndCommandBuffer(buf) != VK_SUCCESS) { //Did everything go great?
		std::cout << "VK: Failed to record command buffer!" << std::endl;
	}

	


}


void RenderingEngine::reRecordCmdBuf(uint32_t indexCount, uint32_t vertexBuf)
{
	for (size_t i = 0; i < m_commandBuffers.size(); i++) {
		
#ifdef MULTITHREAD
		std::future<void> thread = std::async(threadedCmdRecord, m_commandBuffers[i], m_renderpass, m_framebuffers[i], m_scExtent, m_shaders[m_currentShader], m_vertexBuffers[vertexBuf], m_indexBuffers[vertexBuf], indexCount, m_clearQueued);
#else
		threadedCmdRecord(m_commandBuffers[i], m_renderpass, m_framebuffers[i], m_scExtent, m_shaders[m_currentShader], m_vertexBuffers[vertexBuf], m_indexBuffers[vertexBuf], indexCount, m_clearQueued);
#endif
	}

	if (m_clearQueued)
		m_clearQueued = false;
}

void RenderingEngine::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer & buffer, VkDeviceMemory & memory)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size               = size;
	bufferInfo.usage              = usage;
	bufferInfo.sharingMode        = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(m_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(m_device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(m_pDevice, memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(m_device, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(m_device, buffer, memory, 0);
}

void RenderingEngine::copyBuffer(VkBuffer source, VkBuffer destination, VkDeviceSize size)
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool                 = m_commandPool;
	allocInfo.commandBufferCount          = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

	//Record the buffer
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags                    = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset    = 0; // Optional
	copyRegion.dstOffset    = 0; // Optional
	copyRegion.size         = size;
	vkCmdCopyBuffer(commandBuffer, source, destination, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer); //Done recording, submit

	VkSubmitInfo submitInfo       = {};
	submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers    = &commandBuffer;

	vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_graphicsQueue);

	vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer); //cleanup

}

void RenderingEngine::clearFrame()
{

	for (size_t i = 0; i < m_commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr; // Optional

		vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo);

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_renderpass;
		renderPassInfo.framebuffer = m_framebuffers[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_scExtent;

		//if (clear)
		{
			VkClearValue clearColor = { 1.0f, 0.0f, 1.0f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;
		}
		vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		//	vkCmdBindPipeline(buf, VK_PIPELINE_BIND_POINT_GRAPHICS, pip);// Pipeline binding!

		//	vkCmdDraw(buf, 3, 1, 0, 0); //Draw the triangles, vert count, instance count, first vert, first inst.

		vkCmdEndRenderPass(m_commandBuffers[i]); //STOP HAVING FUN

		if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS) { //Did everything go great?
			std::cout << "VK: Failed to record command buffer!" << std::endl;
		}
	}

	uint32_t imageIndex; //Acquire next image
	vkAcquireNextImageKHR(m_device, m_swapchain, std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);


	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
		std::cout << "VK: Failed to submit draw command buffer!" << std::endl;
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { m_swapchain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	vkQueuePresentKHR(m_presentQueue, &presentInfo); //Show

	vkDeviceWaitIdle(m_device);
}

void RenderingEngine::drawMesh(Mesh mesh, Shader shader)
{
	m_currentShader = shader.getId();
	m_currentVB     = mesh.getVBO();
	reRecordCmdBuf(   mesh.getIndexCount(), mesh.getVBO());

	uint32_t imageIndex; //Acquire next image
	vkAcquireNextImageKHR(m_device, m_swapchain, std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);


	VkSubmitInfo submitInfo = {};
	submitInfo.sType        = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[]      = { m_imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount     = 1;
	submitInfo.pWaitSemaphores        = waitSemaphores;
	submitInfo.pWaitDstStageMask      = waitStages;
	submitInfo.commandBufferCount     = 1;
	submitInfo.pCommandBuffers        = &m_commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[]  = { m_renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores    = signalSemaphores;

	if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
		std::cout << "VK: Failed to submit draw command buffer!" << std::endl;
	}

	VkPresentInfoKHR presentInfo   = {};
	presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores    = signalSemaphores;

	VkSwapchainKHR swapChains[]    = { m_swapchain };
	presentInfo.swapchainCount     = 1;
	presentInfo.pSwapchains        = swapChains;
	presentInfo.pImageIndices      = &imageIndex;
	presentInfo.pResults           = nullptr; // Optional

	vkQueuePresentKHR(m_presentQueue, &presentInfo); //Show

	vkDeviceWaitIdle(m_device);
}

void RenderingEngine::setUniform2f(Shader target, std::string name, Vector2f vec)
{
	
}

void RenderingEngine::setUniform3f(Shader target, std::string name, Vector3f vec)
{
	
}


Window *RenderingEngine::createWindow(uint32_t width, uint32_t height, std::string title, bool vsyncRequested)
{
	m_window = new Window(width, height, title, vsyncRequested);

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
	vkDeviceWaitIdle(m_device);

	for each(auto mem in m_vertexMemories)
	{
		vkFreeMemory(m_device, mem.second, nullptr);
	}

	for each(auto mem in m_indexMemories)
	{
		vkFreeMemory(m_device, mem.second, nullptr);
	}

	for  each(auto buf in m_vertexBuffers)
	{
		vkDestroyBuffer(m_device, buf.second, nullptr);
	}
	for  each(auto buf in m_indexBuffers)
	{
		vkDestroyBuffer(m_device, buf.second, nullptr);
	}

	vkFreeCommandBuffers(m_device, m_commandPool, m_commandBuffers.size(), m_commandBuffers.data());

	vkDestroySemaphore(m_device, m_imageAvailableSemaphore, nullptr);
	vkDestroySemaphore(m_device, m_renderFinishedSemaphore, nullptr);
	vkDestroyCommandPool(m_device, m_commandPool, nullptr);


	for each (auto framebuffer in m_framebuffers)
	{
		vkDestroyFramebuffer(m_device, framebuffer, nullptr);
	}


	vkDestroyRenderPass(m_device, m_renderpass, nullptr);
	for (auto& layout : m_layouts)
	{
		vkDestroyPipelineLayout(m_device, layout.second, nullptr);
	}
	for (auto& pipeline : m_shaders)
	{
		vkDestroyPipeline(m_device, pipeline.second, nullptr);
	}


	for each (auto imageView in m_scImageViews)
	{
		vkDestroyImageView(m_device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	vkDestroyDevice(m_device, nullptr);
	DestroyDebugReportCallbackEXT(m_instance, m_callback, nullptr);
	vkDestroyInstance(m_instance, nullptr);

	std::cout << "VK: Destruction complete." << std::endl;

	delete m_window; //Yeah no we dont want you

	system("pause");
}

#endif
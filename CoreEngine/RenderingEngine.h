#pragma once


#include "BUILD_OPTIONS.h"
#include "Platform.h"
#include "Window.h"
#include "FileIO.h"
#include "Vertex.h"
#include "GameObject.h"
#include "Util.h"
#include "Shader.h"

class RenderingEngine
{
public:
	RenderingEngine(uint32_t width, uint32_t height, std::string title, bool vsyncRequested);
	~RenderingEngine();


	bool run(); //While returns true game will run. Also updates the renderer class
	Shader makeShader(std::vector<char> vert, std::vector<char> frag);
	Mesh   makeMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

	void setUniform2f(Shader target, std::string name, Vector2f vec);
	void setUniform3f(Shader target, std::string name, Vector3f vec);

	void clearFrame();
	void drawMesh(Mesh mesh, Shader shader);

private:

	VkInstance                                            m_instance;                 //Vulkan instance
	VkDebugReportCallbackEXT                              m_callback;                 //Debug callback
	VkPhysicalDevice                                      m_pDevice = VK_NULL_HANDLE; //Handle to the physical device
	VkDevice                                              m_device;                   //The logical device
	VkQueue                                               m_graphicsQueue;            //Where we submit the calls
	VkQueue                                               m_presentQueue;             //We need to present the images
	VkSurfaceKHR                                          m_surface;                  //Rendering surface
	VkSwapchainKHR                                        m_swapchain;                //Framebuffer
	std::vector<VkImage>                                  m_scImages;                 //Swapchain images
	VkFormat                                              m_scFormat;                 //Swapchain image format
	VkExtent2D                                            m_scExtent;                 //Swapchain size
	std::vector<VkImageView>                              m_scImageViews;             //We see the images through these
	VkRenderPass                                          m_renderpass;               //Renderpass
	//VkPipelineLayout                                      m_pipelineLayout;           //Will be used later
	//VkPipeline                                            m_pipeline;                 //The pipeline used
	std::vector<VkFramebuffer>                            m_framebuffers;             //Freimbaffers
	VkCommandPool                                         m_commandPool;              //This is why we are here
	std::vector<VkCommandBuffer>                          m_commandBuffers;           //Multithread this bitch
	VkSemaphore                                           m_imageAvailableSemaphore;  //We have got the image and were ready to render
	VkSemaphore                                           m_renderFinishedSemaphore;  //We have got the render and were ready to present

	bool m_clearQueued = false;

	

	uint32_t                             m_currentShader = 0;
	std::map<uint32_t, VkPipelineLayout> m_layouts;
	std::map<uint32_t, VkPipeline>       m_shaders;

	uint32_t                           m_currentVB = 0;
	std::map<uint32_t, VkBuffer>       m_vertexBuffers;
	std::map<uint32_t, VkDeviceMemory> m_vertexMemories;

	/*
	std::vector<Vertex> exampleVertices = {
		Vertex(Vector3f(-1, 0, 0), Vector2f( 0, 0), Vector2f( 0, 0), Vector3f( 0, 0, 1), Vector3f( 1, 0, 0)),
		Vertex(Vector3f( 1, 1, 0), Vector2f( 0, 0), Vector2f( 0, 0), Vector3f( 0, 0, 1), Vector3f( 1, 0, 0)),
		Vertex(Vector3f( 0, 1, 0), Vector2f( 0, 0), Vector2f( 0, 0), Vector3f( 0, 0, 1), Vector3f( 1, 0, 0)),
	};
	*/
	

	Window *createWindow(uint32_t width, uint32_t height, std::string title, bool vsyncRequested); //Window create and open

	//void init();

	
	void reRecordCmdBuf(uint32_t vc, uint32_t vertexBuf);

	void initInstance();
	void initDebugCallback();
	void initSurface();
	void initPhysicalDevice();
	void initDevice();
	void initSwapchain();
	void initImageViews();
	void initRenderpass();
	//void initPipeline();
	void initFramebuffers();
	void initCommandPool();

	//void initVertexBuffer();

	void initCommandBuffers();
	void initSemaphores();



	Window *m_window                              = nullptr;
};
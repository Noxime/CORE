#pragma once


#include "BUILD_OPTIONS.h"
#include "Platform.h"
#include "Window.h"
#include "Mesh.h"

class RenderingEngine
{
public:
	RenderingEngine();
	~RenderingEngine();

	Window *createWindow(uint32_t width, uint32_t height, std::string title, bool vsyncRequested); //Window create and open

	bool run(); //While returns true game will run. Also updates the renderer class
	void clearCurrentBuffer();

private:

	void setupLayersAndExtensions();

	void initInstance();
	void destroyInstance();

	void initDevice();
	void destroyDevice();

	void setupDebug();

	void initDebug();
	void destroyDebug();

	Window *m_window                              = nullptr;
};
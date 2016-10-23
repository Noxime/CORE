#pragma once

#include "Platform.h"
#include <vector>

#include <GLFW\glfw3.h>

class RenderingEngine;

class Window
{
public:
	Window(uint32_t width, uint32_t height, std::string title, bool requestedVsync);
	~Window();

	void close(); //Close the window dude, someone is watching your screen over your shoulder
	bool update(); //Update the window aka make it responsive

	uint32_t getWidth();
	uint32_t getHeight();
	bool     getVSync();

	GLFWwindow* m_window;


private:

	uint32_t	     m_surfaceSizeX         = 512; //Width and height of the window
	uint32_t	     m_surfaceSizeY         = 512;
	std::string		 m_title                = "CoreEngine"; //Title of the window
	bool			 m_vsyncRequested       = true; //Mailbox or Immediate? not guaranteed
	bool             m_windowShouldStayOpen = true; //Should this window close
};


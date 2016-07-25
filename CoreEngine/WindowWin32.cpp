/*
Core engine

Aaro Per�maa
2016
*/

#include "BUILD_OPTIONS.h"
#include "Platform.h"

#include "Window.h"

#include "RenderingEngine.h"

#include <assert.h>

#if VK_USE_PLATFORM_WIN32_KHR

// Microsoft Windows specific versions of window functions
LRESULT CALLBACK WindowsEventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window * window = reinterpret_cast<Window*>(
		GetWindowLongPtrW(hWnd, GWLP_USERDATA));

	switch (uMsg) {
	case WM_CLOSE:
		window->close();
		return 0;
	case WM_SIZE:
		// we get here if the window has changed size, we should rebuild most
		// of our window resources before rendering to this window again.
		// ( no need for this because our window sizing by hand is disabled )
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

uint64_t	Window::m_win32ClassIdCounter = 0;

void Window::initOSWindow()
{
	WNDCLASSEX winClass{};
	assert(m_surfaceSizeX > 0);
	assert(m_surfaceSizeY > 0);
	
	m_win32Instance = GetModuleHandle(nullptr);
	m_win32ClassName = m_title + "_" + std::to_string(m_win32ClassIdCounter);
	m_win32ClassIdCounter++;

	// Initialize the window class structure:
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = WindowsEventHandler;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = m_win32Instance; // hInstance
	winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winClass.lpszMenuName = NULL;
	winClass.lpszClassName = m_win32ClassName.c_str();
	winClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	// Register window class:
	if (!RegisterClassEx(&winClass)) {
		// It didn't work, so try to give a useful error:
		assert(0 && "Cannot create a window in which to draw!\n");
		fflush(stdout);
		std::exit(-1);
	}

	DWORD ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	// Create window with the registered class:
	RECT wr = { 0, 0, LONG(m_surfaceSizeX), LONG(m_surfaceSizeY) };
	AdjustWindowRectEx(&wr, style, FALSE, ex_style);
	m_win32Window = CreateWindowEx(0,
		m_win32ClassName.c_str(),		// class name
		m_title.c_str(),			// app name
		style,							// window style
		CW_USEDEFAULT, CW_USEDEFAULT,	// x/y coords
		wr.right - wr.left,				// width
		wr.bottom - wr.top,				// height
		NULL,							// handle to parent
		NULL,							// handle to menu
		m_win32Instance,				// hInstance
		NULL);							// no extra parameters
	if (!m_win32Window) {
		// It didn't work, so try to give a useful error:
		assert(1 && "Cannot create a window in which to draw!\n");
		fflush(stdout);
		std::exit(-1);
	}
	SetWindowLongPtr(m_win32Window, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow(m_win32Window, SW_SHOW);
	SetForegroundWindow(m_win32Window);
	SetFocus(m_win32Window);
}

void Window::destroyOSWindow()
{
	DestroyWindow(m_win32Window);
	UnregisterClass(m_win32ClassName.c_str(), m_win32Instance);
}

void Window::updateOSWindow() //Fetch messages
{
	MSG msg;
	if (PeekMessage(&msg, m_win32Window, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::initOSSurface()
{
#if BUILD_WITH_RENDERING_BACKEND == RENDERING_BACKEND_VULKAN

	VkWin32SurfaceCreateInfoKHR surfaceInfo {};
	surfaceInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.hinstance = m_win32Instance;
	surfaceInfo.hwnd      = m_win32Window;

	vkCreateWin32SurfaceKHR(m_renderer->getVulkanInstance(), &surfaceInfo, nullptr, &m_surface); //Create the graphics surface

#elif BUILD_WITH_RENDERING_BACKEND == RENDERING_BACKEND_OPENGL

#endif
}

#endif
#pragma once 

#include <cstdlib>
#include <assert.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

// WINDOWS
#if defined( _WIN32 )

#define VK_USE_PLATFORM_WIN32_KHR 1 //Heyy its me, windows. How bout some compatibiliy?
#define PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_WIN32_SURFACE_EXTENSION_NAME //Win32 surface

#include <Windows.h>

// LINUX ( Via XCB library )
#elif defined( __linux )

#define VK_USE_PLATFORM_XCB_KHR 1
#define PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_XCB_SURFACE_EXTENSION_NAME //Hehhey its linux! Freeware, fudge yea
#include <xcb/xcb.h>

#else // platform not yet supported

#error Platform not yet supported

#endif

#include <vulkan/vulkan.h> //Should be only vulkan include in the whole program
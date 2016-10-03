/*
Core engine

Aaro Perämaa
2016
*/

#pragma once

#define BUILD_ENABLE_VULKAN_DEBUG			1 //Debug messages
#define BUILD_ENABLE_VULKAN_RUNTIME_DEBUG   1 //Runtime vulkan error 

#define RENDERING_BACKEND_VULKAN 0
#define RENDERING_BACKEND_OPENGL 1

#define BUILD_WITH_RENDERING_BACKEND RENDERING_BACKEND_VULKAN
#pragma once

#include "Platform.h"

class Vertex
{
public:
	Vertex();
	Vertex(glm::vec3 position, glm::vec2 texcoord0, glm::vec2 texcoord1, glm::vec3 normal, glm::vec3 tangent);
	~Vertex();

	glm::vec3 getPos();
	glm::vec2 getTex0();
	glm::vec2 getTex1();
	glm::vec3 getNorm();
	glm::vec3 getTang();

	static VkVertexInputBindingDescription getBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 5> getAttributeDescriptions();

	const static uint32_t positionOffset  = 0;
	const static uint32_t texcoord0Offset = 1;
	const static uint32_t texcoord1Offset = 2;
	const static uint32_t normalOffset    = 3;
	const static uint32_t tangentOffset   = 4;

	const static uint32_t positionSize  = 3;
	const static uint32_t texcoord0Size = 2;
	const static uint32_t texcoord1Size = 2;
	const static uint32_t normalSize    = 3;
	const static uint32_t tangentSize   = 3;

private:
	glm::vec3 m_position;
	glm::vec2 m_texcoord_primary;
	glm::vec2 m_texcoord_secondary;
	glm::vec3 m_normal;
	glm::vec3 m_tangent;

};


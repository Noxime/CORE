#include "Vertex.h"



Vertex::Vertex()
{
	Vertex(glm::vec3(), glm::vec2(), glm::vec2(), glm::vec3(), glm::vec3());
}

Vertex::Vertex(glm::vec3 position, glm::vec2 texcoord0, glm::vec2 texcoord1, glm::vec3 normal, glm::vec3 tangent)
{
	m_position           = position;
	m_texcoord_primary   = texcoord0;
	m_texcoord_secondary = texcoord1;
	m_normal             = normal;
	m_tangent            = tangent;
}


Vertex::~Vertex()
{
}

glm::vec3 Vertex::getPos()
{
	return m_position;
}

glm::vec2 Vertex::getTex0()
{
	return m_texcoord_primary;
}

glm::vec2 Vertex::getTex1()
{
	return m_texcoord_secondary;
}

glm::vec3 Vertex::getNorm()
{
	return m_normal;
}

glm::vec3 Vertex::getTang()
{
	return m_tangent;
}

VkVertexInputBindingDescription Vertex::getBindingDescription()
{
	VkVertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; //TODO Instancing

	return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 5> Vertex::getAttributeDescriptions()
{
	std::array<VkVertexInputAttributeDescription, 5> attributeDescriptions = {};

	//Position
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, m_position);

	//Texcoord0
	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, m_texcoord_primary);

	//Texcoord1
	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(Vertex, m_texcoord_secondary);

	//Normal
	attributeDescriptions[3].binding = 0;
	attributeDescriptions[3].location = 3;
	attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[3].offset = offsetof(Vertex, m_normal);

	//Tangent
	attributeDescriptions[4].binding = 0;
	attributeDescriptions[4].location = 4;
	attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[4].offset = offsetof(Vertex, m_tangent);


	return attributeDescriptions;
}


#include "Vertex.h"



Vertex::Vertex()
{
	Vertex(Vector3f(), Vector2f(), Vector2f(), Vector3f(), Vector3f());
}

Vertex::Vertex(Vector3f position, Vector2f texcoord0, Vector2f texcoord1, Vector3f normal, Vector3f tangent)
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

Vector3f Vertex::getPos()
{
	return m_position;
}

Vector2f Vertex::getTex0()
{
	return m_texcoord_primary;
}

Vector2f Vertex::getTex1()
{
	return m_texcoord_secondary;
}

Vector3f Vertex::getNorm()
{
	return m_normal;
}

Vector3f Vertex::getTang()
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


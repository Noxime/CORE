#pragma once

#include "Platform.h"
#include "Vector2f.h"
#include "Vector3f.h"

class Vertex
{
public:
	Vertex();
	Vertex(Vector3f position, Vector2f texcoord0, Vector2f texcoord1, Vector3f normal, Vector3f tangent);
	~Vertex();

	Vector3f getPos();
	Vector2f getTex0();
	Vector2f getTex1();
	Vector3f getNorm();
	Vector3f getTang();

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
	Vector3f m_position;
	Vector2f m_texcoord_primary;
	Vector2f m_texcoord_secondary;
	Vector3f m_normal;
	Vector3f m_tangent;

};


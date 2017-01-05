#pragma once
#include "Transform.h"
#include "MeshRenderer.h"

#include "Platform.h"

class GameObject
{
public:
	GameObject();
	GameObject(Transform transform);
	~GameObject();

	void setEnabled(bool enabled);
	bool isEnabled();

	//void setRenderer(MeshRenderer renderer);

private:
	bool m_enabled = true;
	Transform m_transform;
	//MeshRenderer m_renderer;
};


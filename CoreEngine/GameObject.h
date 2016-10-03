#pragma once
#include "Vector3f.h"
#include "Transform.h"
#include "Component.h"

#include <vector>

class GameObject
{
public:
	GameObject();
	~GameObject();

	void setEnabled(bool enabled);
	bool isEnabled();

	void addComponent(Component component);
private:
	bool m_enabled = true;
	Transform m_transform;
	std::vector<Component> m_components;
};


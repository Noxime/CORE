#include "GameObject.h"





GameObject::GameObject(Transform transform)
{
	m_transform = transform;
}




GameObject::~GameObject()
{
}

void GameObject::setEnabled(bool enabled)
{
	m_enabled = enabled;
}

bool GameObject::isEnabled()
{
	return m_enabled;
}


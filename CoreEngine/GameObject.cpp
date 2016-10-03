#include "GameObject.h"



GameObject::GameObject()
{
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

void GameObject::addComponent(Component component)
{
	m_components.push_back(component);
}

#include "Component.h"



Component::Component()
{
}

void Component::onStart()
{
}

void Component::onUpdate()
{
}

void Component::onDraw()
{
}

void Component::setEnabled(bool enabled)
{
	m_enabled = enabled;
}

bool Component::isEnabled()
{
	return m_enabled;
}


Component::~Component()
{
}

#pragma once
class Component
{
public:
	Component();

	void onStart();
	void onUpdate();
	void onDraw();

	void setEnabled(bool enabled);
	bool isEnabled();

	~Component();

private:
	bool m_enabled = true;

};


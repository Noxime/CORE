#pragma once
class Vector2f
{
public:
	Vector2f();
	Vector2f(float x, float y);
	~Vector2f();

	void setX(float x);
	void setY(float y);

	float getX();
	float getY();
private:
	float m_x;
	float m_y;
};


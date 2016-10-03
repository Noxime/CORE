#pragma once
class Quaternion
{
public:
	Quaternion();
	Quaternion(float x, float y, float z, float w);
	~Quaternion();

	float getX();
	float getY();
	float getZ();
	float getW();

	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setW(float w);

private:
	float m_x;
	float m_y;
	float m_z;
	float m_w;
};


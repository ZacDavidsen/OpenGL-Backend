#pragma once

#include "Matrix.h"

class GLManager;

class Entity
{
	GLManager *manager;
	int model;
	int shader;

	Vec3 scale;
	Vec4 quaternion;
	Vec3 position;

	bool transformUpdated = true;
	Mat4 modelMatrix;

public:
	Entity(GLManager *manager, int model, int shader);
	~Entity();

	Vec3 getScale() const;
	void setScale(Vec3 newScale);
	void scaleBy(Vec3 deltaS);

	// gets rotation in roll pitch yaw, but it yaw is 90 both other angles return as 0
	Vec3 getRotationRPY() const;
	void setRotation(Vec3 rotation);
	void rotateBy(Vec3 rotation);
	void setRotation(float angle, Vec3 axis);
	void rotateAround(float angle, Vec3 axis);

	Vec3 getPosition() const;
	void setPosition(Vec3 newPosition);
	void translateBy(Vec3 deltaP);

	void draw();

private:
	static Vec4 getRotationQuaternion(float angle, Vec3 axis);
	static Vec4 multiplyQuaternions(Vec4 q1, Vec4 q2);
};


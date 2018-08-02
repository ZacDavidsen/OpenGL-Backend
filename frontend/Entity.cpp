#include "Entity.h"

#include <cmath>

#include "GLManager.h"


Entity::Entity(GLManager *manager, int model, int shader)
	: manager(manager), model(model), shader(shader), scale(Vec3(1)), quaternion(Vec4{ 0,0,0,1 }) , position(Vec3(0.0f))
{
}

Entity::~Entity()
{
}



Vec3 Entity::getScale() const 
{
	return this->scale;
}

void Entity::setScale(Vec3 newScale)
{
	this->scale = newScale;
	this->transformUpdated = true;
}

void Entity::scaleBy(Vec3 deltaS)
{
	for (int i = 0; i < 3; i++)
	{
		this->scale[i] *= deltaS[i];
	}
	this->transformUpdated = true;
}



Vec3 Entity::getRotationRPY() const
{
	//pitch and yaw are flipped because of the way the axes are set up...
	Vec3 ret;
	//roll
	ret[0] = atan2(2 * (quaternion[3] * quaternion[0] + quaternion[1] * quaternion[2]),
		1 - 2 * (quaternion[0] * quaternion[0] + quaternion[1] * quaternion[1]));

	//pitch
	ret[1] = atan2(2 * (quaternion[3] * quaternion[2] + quaternion[0] * quaternion[1]),
		1 - 2 * (quaternion[1] * quaternion[1] + quaternion[2] * quaternion[2]));

	//yaw
	ret[2] = asin(2 * (quaternion[3] * quaternion[1] - quaternion[2] * quaternion[0]));

	return ret;
}

void Entity::setRotation(Vec3 rotation)
{
	//pitch and yaw are flipped because of the way the axes are set up...
	this->quaternion[0] = sin(rotation[0] / 2) * cos(rotation[2] / 2) * cos(rotation[1] / 2) - 
						  cos(rotation[0] / 2) * sin(rotation[2] / 2) * sin(rotation[1] / 2);
	
	this->quaternion[1] = cos(rotation[0] / 2) * sin(rotation[2] / 2) * cos(rotation[1] / 2) +
						  sin(rotation[0] / 2) * cos(rotation[2] / 2) * sin(rotation[1] / 2);

	this->quaternion[2] = cos(rotation[0] / 2) * cos(rotation[2] / 2) * sin(rotation[1] / 2) -
						  sin(rotation[0] / 2) * sin(rotation[2] / 2) * cos(rotation[1] / 2);

	this->quaternion[3] = cos(rotation[0] / 2) * cos(rotation[2] / 2) * cos(rotation[1] / 2) +
						  sin(rotation[0] / 2) * sin(rotation[2] / 2) * sin(rotation[1] / 2);

	this->transformUpdated = true;
}

void Entity::rotateBy(Vec3 rotation)
{
	Vec4 q2;
	q2[0] = sin(rotation[0] / 2) * cos(rotation[2] / 2) * cos(rotation[1] / 2) -
			cos(rotation[0] / 2) * sin(rotation[2] / 2) * sin(rotation[1] / 2);

	q2[1] = cos(rotation[0] / 2) * sin(rotation[2] / 2) * cos(rotation[1] / 2) +
			sin(rotation[0] / 2) * cos(rotation[2] / 2) * sin(rotation[1] / 2);

	q2[2] = cos(rotation[0] / 2) * cos(rotation[2] / 2) * sin(rotation[1] / 2) -
			sin(rotation[0] / 2) * sin(rotation[2] / 2) * cos(rotation[1] / 2);

	q2[3] = cos(rotation[0] / 2) * cos(rotation[2] / 2) * cos(rotation[1] / 2) +
			sin(rotation[0] / 2) * sin(rotation[2] / 2) * sin(rotation[1] / 2);

	this->quaternion = multiplyQuaternions(q2, this->quaternion);
	this->transformUpdated = true;
}

void Entity::setRotation(float angle, Vec3 axis)
{
	this->quaternion = getRotationQuaternion(angle, axis);
	this->transformUpdated = true;
}

void Entity::rotateAround(float angle, Vec3 axis)
{
	Vec4 rotation = getRotationQuaternion(angle, axis);

	this->quaternion = multiplyQuaternions(rotation, this->quaternion);
	this->transformUpdated = true;
}



Vec3 Entity::getPosition() const
{
	return this->position;
}

void Entity::setPosition(Vec3 newPosition)
{
	this->position = newPosition;
	this->transformUpdated = true;
}

void Entity::translateBy(Vec3 deltaP)
{
	this->position = this->position + deltaP;
	this->transformUpdated = true;
}



void Entity::draw()
{
	if (this->transformUpdated)
	{
		this->modelMatrix = Mat4(1.0f);
		this->modelMatrix = Mat::scale(this->modelMatrix, this->scale);
		//this->modelMatrix = Mat::rotate(this->modelMatrix, this->rotation[0], Vec3{ 1,0,0 });
		//this->modelMatrix = Mat::rotate(this->modelMatrix, this->rotation[1], Vec3{ 0,1,0 });
		//this->modelMatrix = Mat::rotate(this->modelMatrix, this->rotation[2], Vec3{ 0,0,1 });

		Mat4 rot;
		rot[0][0] = 1 - 2 * (quaternion[1] * quaternion[1] + quaternion[2] * quaternion[2]);
		rot[0][1] = 2 * (quaternion[0] * quaternion[1] - quaternion[2] * quaternion[3]);
		rot[0][2] = 2 * (quaternion[0] * quaternion[2] + quaternion[1] * quaternion[3]);

		rot[1][0] = 2 * (quaternion[0] * quaternion[1] + quaternion[2] * quaternion[3]);
		rot[1][1] = 1 - 2 * (quaternion[0] * quaternion[0] + quaternion[2] * quaternion[2]);
		rot[1][2] = 2 * (quaternion[1] * quaternion[2] - quaternion[0] * quaternion[3]);

		rot[2][0] = 2 * (quaternion[0] * quaternion[2] - quaternion[1] * quaternion[3]);
		rot[2][1] = 2 * (quaternion[0] * quaternion[3] + quaternion[1] * quaternion[2]);
		rot[2][2] = 1 - 2 * (quaternion[0] * quaternion[0] + quaternion[1] * quaternion[1]);

		this->modelMatrix = rot * this->modelMatrix;

		this->modelMatrix = Mat::translate(this->modelMatrix, this->position);
		this->transformUpdated = false;
	}

	this->manager->loadUniform(this->shader, "model", this->modelMatrix);
	this->manager->drawItem(this->shader, this->model);
}



Vec4 Entity::getRotationQuaternion(float angle, Vec3 axis)
{
	Vec4 q2;
	axis = Mat::normalize(axis);
	q2[0] = axis[0] * sin(angle / 2);
	q2[1] = axis[1] * sin(angle / 2);
	q2[2] = axis[2] * sin(angle / 2);
	q2[3] = cos(angle / 2);
	return q2;
}

Vec4 Entity::multiplyQuaternions(Vec4 q1, Vec4 q2)
{
	Mat4 rot{ q1[3], -q1[2],  q1[1], q1[0],
			  q1[2],  q1[3], -q1[0], q1[1],
			 -q1[1],  q1[0],  q1[3], q1[2],
			 -q1[0], -q1[1], -q1[2], q1[3] };

	return rot * q2;
}

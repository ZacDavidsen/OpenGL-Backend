#pragma once
//#include "Matrix.h"
#include "MatrixTypes.h"

namespace Camera
{
	void setPosition(Vec3 newPos);
	Vec3 getPosition();

	void setDirection(Vec3 newDir);
	Vec3 getDirection();

	//In order for this to work correctly, you must first set the position
	//and then you can set the target
	void setTarget(Vec3 newTarget);

	Mat4 getCameraMatrix();

	void moveForward(float distance);
	void moveBack(float distance);
	void moveRight(float distance);
	void moveLeft(float distance);
	void moveUp(float distance);
	void moveDown(float distance);

	void rotateVertical(float angle);
	void rotateHorizontal(float angle);


	//this method defaults to an up vector of {0,1,0}
	Mat4 lookAt(Vec3 position, Vec3 direction, Vec3 up);
}
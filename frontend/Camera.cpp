#include "Camera.h"
#include "Matrix.h"

#define PI 3.14259f
#define MAX_VERTICAL_ANGLE 89.0f

Vec3 _position{0,0,0};
Vec3 _direction{0,0,1};
Vec3 _up{ 0,1,0 };

float _currYAngle = 0;

namespace Camera
{
	void setPosition(Vec3 newPos)
	{
		_position = newPos;
	}

	Vec3 getPosition()
	{
		return _position;
	}

	void setTarget(Vec3 newTarget)
	{
		_direction = Mat::normalize((Vec3)(_position - newTarget));
	}

	void setDirection(Vec3 newDir)
	{
		_direction = Mat::normalize((Vec3)(-newDir));
	}

	Vec3 getDirection()
	{
		return _direction;
	}

	Mat4 lookAt(Vec3 position, Vec3 direction, Vec3 up = _up)
	{
		Vec3 cameraRight = Mat::normalize(up.cross(direction));
		Vec3 cameraUp = Mat::normalize(direction.cross(cameraRight));

		Mat4 mat1;
		mat1[0][0] = cameraRight[0];
		mat1[0][1] = cameraRight[1];
		mat1[0][2] = cameraRight[2];

		mat1[1][0] = cameraUp[0];
		mat1[1][1] = cameraUp[1];
		mat1[1][2] = cameraUp[2];

		mat1[2][0] = direction[0];
		mat1[2][1] = direction[1];
		mat1[2][2] = direction[2];

		Mat4 mat2;
		mat2[0][3] = -position[0];
		mat2[1][3] = -position[1];
		mat2[2][3] = -position[2];

		return mat1*mat2;
	}

	Mat4 getCameraMatrix()
	{
		return lookAt(_position, _direction);
	}



	void moveForward(float distance)
	{
		Vec3 temp = _direction;
		temp[1] = 0;
		_position = _position - Mat::normalize(temp) * distance;
	}

	void moveBack(float distance)
	{
		Vec3 temp = _direction;
		temp[1] = 0;
		_position = _position + Mat::normalize(temp) * distance;
	}

	void moveRight(float distance)
	{
		_position = _position - Mat::normalize(_direction.cross(_up)) * distance;
	}

	void moveLeft(float distance)
	{
		_position = _position + Mat::normalize(_direction.cross(_up)) * distance;
	}

	void moveUp(float distance)
	{
		_position = _position + _up * distance;
	}

	void moveDown(float distance)
	{
		_position = _position - _up * distance;
	}

	void rotateVertical(float angle)
	{
		_currYAngle += angle;
		if (_currYAngle > (MAX_VERTICAL_ANGLE * PI / 180.0f))
		{
			angle = (MAX_VERTICAL_ANGLE * PI / 180.0f) - (_currYAngle - angle);
			_currYAngle = (MAX_VERTICAL_ANGLE * PI / 180.0f);
		}
		else if (_currYAngle < -(MAX_VERTICAL_ANGLE * PI / 180.0f))
		{
			angle = -(MAX_VERTICAL_ANGLE * PI / 180.0f) - (_currYAngle - angle);
			_currYAngle = -(MAX_VERTICAL_ANGLE * PI / 180.0f);
		}


		Mat4 trans;
		trans = Mat::rotate(trans, angle, _up.cross(Vec3{ _direction[0], 0, _direction[2] }));
		Vec4 temp{ _direction[0], _direction[1], _direction[2], 1 };
		temp = trans*temp;
		_direction[0] = temp[0];
		_direction[1] = temp[1];
		_direction[2] = temp[2];
	}

	void rotateHorizontal(float angle)
	{
		Mat4 trans;
		trans = Mat::rotate(trans, -angle, _up);
		Vec4 temp{_direction[0], _direction[1], _direction[2], 1};
		temp = trans*temp;
		_direction[0] = temp[0];
		_direction[1] = temp[1];
		_direction[2] = temp[2];
	}
}

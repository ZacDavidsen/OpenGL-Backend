#include "Camera.h"
#include "Matrix.h"

#define PI 3.14259f
#define MAX_VERTICAL_ANGLE 89.0f


Camera::Camera() {
	m_position = Vec3{ 0,0,0 };
	m_direction = Vec3{ 0,0,1 };
	m_up = Vec3{ 0,1,0 };
}

Camera::~Camera() {

}

void Camera::setPosition(Vec3 newPos)
{
	m_position = newPos;
}

Vec3 Camera::getPosition()
{
	return m_position;
}

void Camera::setTarget(Vec3 newTarget)
{
	m_direction = Mat::normalize((Vec3)(m_position - newTarget));
}

void Camera::setDirection(Vec3 newDir)
{
	m_direction = Mat::normalize((Vec3)(-newDir));
}

Vec3 Camera::getDirection()
{
	return m_direction;
}

Mat4 Camera::lookAt(Vec3 position, Vec3 direction, Vec3 up)
{
	//if (up) up = this->m_up;
	Vec3 cameraRight = Mat::normalize(Mat::cross(up, direction));
	Vec3 cameraUp = Mat::normalize(Mat::cross(direction, cameraRight));

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

Mat4 Camera::getCameraMatrix()
{
	return lookAt(m_position, m_direction, m_up);
}

void Camera::moveForward(float distance)
{
	Vec3 temp = m_direction;
	temp[1] = 0;
	m_position = m_position - Mat::normalize(temp) * distance;
}

void Camera::moveBack(float distance)
{
	Vec3 temp = m_direction;
	temp[1] = 0;
	m_position = m_position + Mat::normalize(temp) * distance;
}

void Camera::moveRight(float distance)
{
	m_position = m_position - Mat::normalize(Mat::cross(m_direction, m_up)) * distance;
}

void Camera::moveLeft(float distance)
{
	m_position = m_position + Mat::normalize(Mat::cross(m_direction, m_up)) * distance;
}

void Camera::moveUp(float distance)
{
	m_position = m_position + m_up * distance;
}

void Camera::moveDown(float distance)
{
	m_position = m_position - m_up * distance;
}

void Camera::rotateVertical(float angle)
{
	m_currYAngle += angle;
	if (m_currYAngle > (MAX_VERTICAL_ANGLE * PI / 180.0f))
	{
		angle = (MAX_VERTICAL_ANGLE * PI / 180.0f) - (m_currYAngle - angle);
		m_currYAngle = (MAX_VERTICAL_ANGLE * PI / 180.0f);
	}
	else if (m_currYAngle < -(MAX_VERTICAL_ANGLE * PI / 180.0f))
	{
		angle = -(MAX_VERTICAL_ANGLE * PI / 180.0f) - (m_currYAngle - angle);
		m_currYAngle = -(MAX_VERTICAL_ANGLE * PI / 180.0f);
	}


	Mat4 trans;
	trans = Mat::rotate(trans, angle, Mat::cross(m_up, Vec3{ m_direction[0], 0, m_direction[2] }));
	Vec4 temp{ m_direction[0], m_direction[1], m_direction[2], 1 };
	temp = trans*temp;
	m_direction[0] = temp[0];
	m_direction[1] = temp[1];
	m_direction[2] = temp[2];
}

void Camera::rotateHorizontal(float angle)
{
	Mat4 trans;
	trans = Mat::rotate(trans, -angle, m_up);
	Vec4 temp{m_direction[0], m_direction[1], m_direction[2], 1};
	temp = trans*temp;
	m_direction[0] = temp[0];
	m_direction[1] = temp[1];
	m_direction[2] = temp[2];
}

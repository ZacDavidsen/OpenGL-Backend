#pragma once

namespace Mat
{
	template<unsigned int height, unsigned int width = height, typename T = float>
	class Matrix;

	template<unsigned int length, typename T = float>
	class Vector;
}

typedef Mat::Matrix<2, 2, float> Mat2;
typedef Mat::Matrix<2, 2, double> dMat2;
typedef Mat::Matrix<2, 2, int> iMat2;
typedef Mat::Matrix<2, 2, unsigned int> uMat2;
typedef Mat::Matrix<2, 2, bool> bMat2;

typedef Mat::Matrix<3, 3, float> Mat3;
typedef Mat::Matrix<3, 3, double> dMat3;
typedef Mat::Matrix<3, 3, int> iMat3;
typedef Mat::Matrix<3, 3, unsigned int> uMat3;
typedef Mat::Matrix<3, 3, bool> bMat3;

typedef Mat::Matrix<4, 4, float> Mat4;
typedef Mat::Matrix<4, 4, double> dMat4;
typedef Mat::Matrix<4, 4, int> iMat4;
typedef Mat::Matrix<4, 4, unsigned int> uMat4;
typedef Mat::Matrix<4, 4, bool> bMat4;

typedef Mat::Vector<2, float> Vec2;
typedef Mat::Vector<2, double> dVec2;
typedef Mat::Vector<2, int> iVec2;
typedef Mat::Vector<2, unsigned int> uVec2;
typedef Mat::Vector<2, bool> bVec2;

typedef Mat::Vector<3, float> Vec3;
typedef Mat::Vector<3, double> dVec3;
typedef Mat::Vector<3, int> iVec3;
typedef Mat::Vector<3, unsigned int> uVec3;
typedef Mat::Vector<3, bool> bVec3;

typedef Mat::Vector<4, float> Vec4;
typedef Mat::Vector<4, double> dVec4;
typedef Mat::Vector<4, int> iVec4;
typedef Mat::Vector<4, unsigned int> uVec4;
typedef Mat::Vector<4, bool> bVec4;

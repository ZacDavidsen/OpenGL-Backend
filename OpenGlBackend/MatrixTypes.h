#pragma once

namespace Mat
{
	template<unsigned int height, unsigned int width = height>
	class Matrix;

	template<unsigned int length>
	class Vector;
}

typedef Mat::Matrix<2> Mat2;
typedef Mat::Matrix<4> Mat4;
typedef Mat::Vector<2> Vec2;
typedef Mat::Vector<3> Vec3;
typedef Mat::Vector<4> Vec4;
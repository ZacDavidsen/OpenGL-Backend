#pragma once

namespace MatrixHidden
{
	template<unsigned int height, unsigned int width = height>
	class Matrix;

	template<unsigned int length>
	class Vector;
}

typedef MatrixHidden::Matrix<2> Mat2;
typedef MatrixHidden::Matrix<4> Mat4;
typedef MatrixHidden::Vector<2> Vec2;
typedef MatrixHidden::Vector<3> Vec3;
typedef MatrixHidden::Vector<4> Vec4;
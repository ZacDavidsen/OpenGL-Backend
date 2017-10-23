#include "Matrix.h"

namespace Mat
{
	template<unsigned int length>
	MatrixHidden::Matrix<length, 1> normalize(const MatrixHidden::Matrix<length, 1> &vec)
	{
		float mag = 0;

		for (int i = 0; i < length; i++)
		{
			mag += vec.vals[i] * vec.vals[i];
		}

		mag = std::sqrt(mag);

		return vec / mag;
	}

	Mat4 translate(const Mat4& mat, const Vec3& transform)
	{
		MatrixHidden::Matrix<4> ret = MatrixHidden::Matrix<4>(mat);
		for (int i = 0; i < 3; i++)
		{
			ret.vals[(i + 1) * 4 - 1] += transform.vals[i];
		}
		return ret;
	}

	Mat4 scale(const Mat4& mat, const Vec3& transform)
	{
		MatrixHidden::Matrix<4> ret = MatrixHidden::Matrix<4>(mat);
		for (int i = 0; i < 3; i++)
		{
			ret.vals[i * 4 + i] *= transform.vals[i];
		}
		return ret;
	}

	Mat4 rotate(const Mat4 &m, float angle, const Vec3 &v)
	{
		//copied and repurposed from glm
		float const a = -angle;
		float const c = std::cos(a);
		float const s = std::sin(a);

		Vec3 axis(normalize(v));
		Vec3 temp((1.0f - c) * axis);

		Mat4 Rotate = Mat4();
		Rotate.vals[0] = c + temp[0] * axis[0];
		Rotate.vals[1] = temp[0] * axis[1] + s * axis[2];
		Rotate.vals[2] = temp[0] * axis[2] - s * axis[1];

		Rotate.vals[4] = temp[1] * axis[0] - s * axis[2];
		Rotate.vals[5] = c + temp[1] * axis[1];
		Rotate.vals[6] = temp[1] * axis[2] + s * axis[0];

		Rotate.vals[8] = temp[2] * axis[0] + s * axis[1];
		Rotate.vals[9] = temp[2] * axis[1] - s * axis[0];
		Rotate.vals[10] = c + temp[2] * axis[2];

		return m*Rotate;
	}

	float toRads(float in)
	{
		return in*3.14159265f / 180;
	}

	//https://stackoverflow.com/questions/8115352/glmperspective-explanation
	Mat4 perspective(float FOVangle, float aspectRatio, float nearz, float farz)
	{
		float vals[16];
		for (int i = 0; i < 16; i++)
			vals[i] = 0;

		float halfAngleTan = tan(FOVangle / 2);

		vals[0] = 1 / (aspectRatio*halfAngleTan);
		vals[5] = 1 / halfAngleTan;
		vals[10] = -(farz + nearz) / (farz - nearz);
		vals[11] = -(2 * farz * nearz) / (farz - nearz);
		vals[14] = -1;

		return Mat4(vals);
	}

	//http://www.songho.ca/opengl/gl_projectionmatrix.html
	//was used for how to create the following projection matrices


	Mat4 perspective(float left, float right, float top, float bottom, float nearz, float farz)
	{
		float vals[16];
		for (int i = 0; i < 16; i++)
			vals[i] = 0;
	
		vals[0] = 2*nearz / (right - left);
		vals[2] = right + left / (right - left);
		vals[5] = 2 * nearz / (top - bottom);
		vals[6] = top + bottom / (top - bottom);
		vals[10] = -(farz + nearz) / (farz - nearz);
		vals[11] = -(2 * farz * nearz) / (farz - nearz);
		vals[14] = -1;

		return Mat4(vals);
	}


	Mat4 perspective2(float width, float height, float nearz, float farz)
	{
		float vals[16];
		for (int i = 0; i < 16; i++)
			vals[i] = 0;

		vals[0] = 2 * nearz / (width);
		vals[5] = 2 * nearz / (height);
		vals[10] = -(farz + nearz) / (farz - nearz);
		vals[11] = -(2 * farz * nearz) / (farz - nearz);
		vals[14] = -1;

		return Mat4(vals);
	}

	Mat4 orthographic(float left, float right, float top, float bottom, float nearz, float farz)
	{
		float vals[16];
		for (int i = 0; i < 16; i++)
			vals[i] = 0;

		vals[0] = 2 / (right - left);
		vals[3] = -right - left / (right - left);
		vals[5] = 2 / (top - bottom);
		vals[7] = -top - bottom / (top - bottom);
		vals[10] = -2 / (farz - nearz);
		vals[11] = -(farz + nearz) / (farz - nearz);
		vals[15] = 1;

		return Mat4(vals);
	}

	Mat4 orthographic(float width, float height, float nearz, float farz)
	{
		float vals[16];
		for (int i = 0; i < 16; i++)
			vals[i] = 0;

		vals[0] = 2 / (width);
		vals[5] = 2 / (height);
		vals[10] = -2 / (farz - nearz);
		vals[11] = -(farz + nearz) / (farz - nearz);
		vals[15] = 1;

		return Mat4(vals);
	}

}


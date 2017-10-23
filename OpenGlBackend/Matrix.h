#pragma once
#include <iostream>
#include <cmath>

namespace MatrixHidden
{
	template<unsigned int height, unsigned int width = height>
	class Matrix;
}

typedef MatrixHidden::Matrix<2> Mat2;
typedef MatrixHidden::Matrix<4> Mat4;
typedef MatrixHidden::Matrix<2, 1> Vec2;
typedef MatrixHidden::Matrix<3, 1> Vec3;
typedef MatrixHidden::Matrix<4, 1> Vec4;

namespace Mat
{
	template<unsigned int length>
		MatrixHidden::Matrix<length, 1> normalize(const MatrixHidden::Matrix<length, 1> &vec);

	Mat4 translate(const Mat4 &mat, const Vec3 &transform);
	Mat4 scale(const Mat4 &mat, const Vec3 &transform);
	Mat4 rotate(const Mat4 &m, float angle, const Vec3 &v);

	float toRads(float in);
	Mat4 perspective(float FOVangle, float aspectRatio, float nearz, float farz);
	Mat4 perspective(float left, float right, float top, float bottom, float nearz, float farz);
	Mat4 perspective2(float width, float height, float nearz, float farz);

	Mat4 orthographic(float left, float right, float top, float bottom, float nearz, float farz);
	Mat4 orthographic(float width, float height, float nearz, float farz);

};

namespace MatrixHidden
{
	template<unsigned int height, unsigned int width>
	class Matrix
	{
		int rows;
		int cols;
		float vals[height*width];

		template<unsigned int height, unsigned int width> friend class Matrix;

		template<unsigned int length> friend
			Matrix<length, 1> Mat::normalize(const Matrix<length, 1> &vec);
		friend Mat4 Mat::translate(const Mat4 &mat, const Vec3 &transform);
		friend Mat4 Mat::scale(const Mat4 &mat, const Vec3 &transform);
		friend Mat4 Mat::rotate(const Mat4 &m, float angle, const Vec3 &v);

	public:
		Matrix(float diagVal = 1)
		{
			this->rows = height;
			this->cols = width;
			//this->vals = float[height*width];
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					if (i == j)
					{
						vals[i*width + j] = diagVal;
					}
					else
					{
						vals[i*width + j] = 0;
					}
				}
			}
		}

		Matrix(float values[])
		{
			this->rows = height;
			this->cols = width;
			//this->vals = float[height*width];
			for (int i = 0; i < height*width; i++)
			{
				this->vals[i] = values[i];
			}
		}

		Matrix(std::initializer_list<float> values)
		{
			if (values.size() < height*width)
			{
				throw "Mismatch of data length!";
			}

			this->rows = height;
			this->cols = width;
			//this->vals = float[height*width];

			const float *begin = values.begin();

			for (int i = 0; i < height*width; i++)
			{
				this->vals[i] = begin[i];
			}
		}

		Matrix(const Matrix &mat)
		{
			this->rows = height;
			this->cols = width;
			//this->vals = float[height*width];
			for (int i = 0; i < height*width; i++)
			{
				this->vals[i] = mat.vals[i];
			}
		}

		~Matrix()
		{
			//delete[] vals;
		}

		operator const float*() const
		{
			return this->vals;
		}

		Matrix operator+(const Matrix &right)
		{
			float newVals[height*width];
			for (int i = 0; i < height*width; i++)
			{
				newVals[i] = this->vals[i] + right.vals[i];
			}
			return Matrix(newVals);
		}

		Matrix operator-(const Matrix &right)
		{
			float newVals[height*width];
			for (int i = 0; i < height*width; i++)
			{
				newVals[i] = this->vals[i] - right.vals[i];
			}
			return Matrix(newVals);
		}

		template<unsigned int height, unsigned int width> friend
			Matrix<height, width> operator*(const Matrix<height, width> &mat, float num);

		template<unsigned int newWidth>
		Matrix<height, newWidth> operator*(const Matrix<width, newWidth> &right) const
		{
			float newVals[height*newWidth];

			for (int row = 0; row < height; row++)
			{
				for (int col = 0; col < newWidth; col++)
				{
					float *val = &newVals[row*newWidth + col];
					*val = 0;

					for (int i = 0; i < width; i++)
					{
						*val += this->vals[row*width + i] * right.vals[i*newWidth + col];
					}
				}
			}
			return Matrix<height,newWidth>(newVals);
		}

		Matrix operator/(float num) const
		{
			float newVals[height*width];
			for (int i = 0; i < height*width; i++)
			{
				newVals[i] = this->vals[i] / num;
			}
			return Matrix(newVals);
		}

		void print()
		{
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width-1; j++)
				{
					std::cout << vals[i*width + j] << ",";
				}
				std::cout << vals[(i+1)*width-1] << "\n";
			}
			std::cout << std::endl;
		}
	};

	template<unsigned int height, unsigned int width>
	Matrix<height, width> operator*(const Matrix<height, width> &mat, float num)
	{
		float newVals[height*width];
		for (int i = 0; i < height*width; i++)
		{
			newVals[i] = mat.vals[i] * num;
		}
		return Matrix<height, width>(newVals);
	}

	template<unsigned int height, unsigned int width>
	Matrix<height, width> operator*(float num, const Matrix<height, width> &mat)
	{
		return mat * num;
	}
}


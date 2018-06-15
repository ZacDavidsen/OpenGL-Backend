#pragma once
#include <initializer_list>
#include <string>
#include <cmath>
#include "MatrixTypes.h"

namespace Mat
{
	template<unsigned int length>
	MatrixHidden::Vector<length> normalize(const MatrixHidden::Vector<length> &vec);

	Vec3 cross(Vec3 left, Vec3 right);

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

		template<unsigned int height, unsigned int width> friend class Matrix;

	protected:

		float vals[height*width];

	public:
		Matrix(float diagVal = 1);
		Matrix(float values[]);
		Matrix(std::initializer_list<float> values);
		Matrix(const Matrix &mat);
		~Matrix();

		Matrix<height, width> operator+(const Matrix<height, width>& right) const;
		Matrix<height, width> operator-() const;
		Matrix<height, width> operator-(const Matrix<height, width>& right) const;
		Matrix<height, width> operator/(float num) const;
		Matrix<height, width> operator*(float num) const;
		template<unsigned int newWidth>
			Matrix<height, newWidth> operator*(const Matrix<width, newWidth> &right) const;

		float* operator[](int row);

		std::string toString() const;
		const float* getGLFormat() const;
	};

	//Totally not sure if this class is worth it, the only changed functionality is the 
	//subscript operator, and everything else needs overloaded to return the correct types...
	template<unsigned int length>
	class Vector : public Matrix<length, 1>
	{
	public:
		Vector(float initVal = 0)
		{
			for (int i = 0; i < length; i++)
			{
				vals[i] = initVal;
			}
		}

		Vector(float values[]): Matrix(values){}
		Vector(std::initializer_list<float> values): Matrix(values){}
		Vector(const Matrix &mat): Matrix(mat){}

		float& operator[](int index) 
		{ 
			return vals[index]; 
		}

		float operator[](int index) const
		{
			return vals[index];
		}

		float dotProduct(const Vector &right)
		{
			float total = 0;
			for (int i = 0; i < length; i++)
			{
				total += this->vals[i] * right.vals[i];
			}
			return total;
		}

	};



	template<unsigned int height, unsigned int width>
	Matrix<height, width>::Matrix(float diagVal)
	{
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

	template<unsigned int height, unsigned int width>
	Matrix<height, width>::Matrix(float values[])
	{
		for (int i = 0; i < height*width; i++)
		{
			this->vals[i] = values[i];
		}
	}

	template<unsigned int height, unsigned int width>
	Matrix<height, width>::Matrix(std::initializer_list<float> values)
	{
		if (values.size() != height * width)
		{
			throw "Mismatch of data length!";
		}

		const float *begin = values.begin();

		for (int i = 0; i < height*width; i++)
		{
			this->vals[i] = begin[i];
		}
	}

	template<unsigned int height, unsigned int width>
	Matrix<height, width>::Matrix(const Matrix &mat)
	{
		for (int i = 0; i < height*width; i++)
		{
			this->vals[i] = mat.vals[i];
		}
	}

	template<unsigned int height, unsigned int width>
	Matrix<height, width>::~Matrix()
	{
	}

	template<unsigned int height, unsigned int width>
	Matrix<height, width> Matrix<height, width>::operator+(const Matrix<height, width> &right) const
	{
		float newVals[height*width];
		for (int i = 0; i < height*width; i++)
		{
			newVals[i] = this->vals[i] + right.vals[i];
		}
		return Matrix(newVals);
	}

	template<unsigned int height, unsigned int width>
	Matrix<height, width> Matrix<height, width>::operator-() const
	{
		float newVals[height*width];
		for (int i = 0; i < height*width; i++)
		{
			newVals[i] = -this->vals[i];
		}
		return Matrix(newVals);
	}

	template<unsigned int height, unsigned int width>
	Matrix<height, width> Matrix<height, width>::operator-(const Matrix<height, width> &right) const
	{
		float newVals[height*width];
		for (int i = 0; i < height*width; i++)
		{
			newVals[i] = this->vals[i] - right.vals[i];
		}
		return Matrix(newVals);
	}

	template<unsigned int height, unsigned int width>
	template<unsigned int newWidth>
	Matrix<height, newWidth> Matrix<height, width>::operator*(const Matrix<width, newWidth> &right) const
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
		return Matrix<height, newWidth>(newVals);
	}

	template<unsigned int height, unsigned int width>
	Matrix<height, width> operator*(float num, const Matrix<height, width> &mat)
	{
		return mat * num;
	}

	template<unsigned int height, unsigned int width>
	Matrix<height, width> Matrix<height, width>::operator*(float num) const
	{
		float newVals[height*width];
		for (int i = 0; i < height*width; i++)
		{
			newVals[i] = this->vals[i] * num;
		}
		return Matrix<height, width>(newVals);
	}

	template<unsigned int height, unsigned int width>
	Matrix<height, width> Matrix<height, width>::operator/(float num) const
	{
		float newVals[height*width];
		for (int i = 0; i < height*width; i++)
		{
			newVals[i] = this->vals[i] / num;
		}
		return Matrix<height, width>(newVals);
	}

	template<unsigned int height, unsigned int width>
	float* Matrix<height, width>::operator[](int row)
	{
		return this->vals + row * width;
	}

	template<unsigned int height, unsigned int width>
	std::string Matrix<height, width>::toString() const
	{
		std::string ret;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width - 1; j++)
			{
				ret.append(std::to_string(vals[i*width + j])).append(",");
			}
			ret.append(std::to_string(vals[(i + 1)*width - 1])).append("\n");
		}
		return ret;
	}

	template<unsigned int height, unsigned int width>
	const float* Matrix<height, width>::getGLFormat() const {
		return this->vals;
	}
}


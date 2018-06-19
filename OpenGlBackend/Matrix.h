#pragma once
#include <initializer_list>
#include <string>
#include <cmath>
#include "MatrixTypes.h"

namespace Mat
{
	template<unsigned int length>
	Mat::Vector<length> normalize(const Mat::Vector<length> &vec);

	Vec3 cross(Vec3 left, Vec3 right);
	template<unsigned int length>
	Mat::Vector<length> dot(Mat::Vector<length> left, Mat::Vector<length> right);

	Mat4 translate(const Mat4 &mat, const Vec3 &transform);
	Mat4 scale(const Mat4 &mat, const Vec3 &transform);
	Mat4 rotate(const Mat4 &m, float angle, const Vec3 &v);

	float toRads(float in);
	Mat4 perspective(float FOVangle, float aspectRatio, float nearz, float farz);
	Mat4 perspective(float left, float right, float top, float bottom, float nearz, float farz);
	Mat4 perspective2(float width, float height, float nearz, float farz);

	Mat4 orthographic(float left, float right, float top, float bottom, float nearz, float farz);
	Mat4 orthographic(float width, float height, float nearz, float farz);



	template<unsigned int height, unsigned int width, typename T>
	class Matrix
	{
		static_assert(std::is_fundamental_v<T>, "Matrix and Vector can only use primitive types!");

		template<unsigned int height, unsigned int width, typename T> friend class Matrix;

	protected:

		T vals[height*width];

	public:
		Matrix(T diagVal = 1);
		Matrix(T values[]);
		Matrix(std::initializer_list<T> values);
		Matrix(const Matrix &mat);
		~Matrix();

		Matrix<height, width, T> operator+(const Matrix<height, width, T>& right) const;
		Matrix<height, width, T> operator-() const;
		Matrix<height, width, T> operator-(const Matrix<height, width, T>& right) const;
		Matrix<height, width, T> operator/(T num) const;
		Matrix<height, width, T> operator*(T num) const;
		template<unsigned int newWidth>
			Matrix<height, newWidth, T> operator*(const Matrix<width, newWidth, T> &right) const;

		T* operator[](int row);

		std::string toString() const;
		const T* getGLFormat() const;
	};

	//Totally not sure if this class is worth it, the only changed functionality is the 
	//subscript operator, and everything else needs overloaded to return the correct types...
	template<unsigned int length, typename T>
	class Vector : public Matrix<length, 1, T>
	{
	public:
		Vector(T initVal = 0) : Matrix(0)
		{
			for (int i = 0; i < length; i++)
			{
				vals[i] = initVal;
			}
		}

		Vector(T values[]): Matrix(values){}
		Vector(std::initializer_list<T> values): Matrix(values){}
		Vector(const Matrix &mat): Matrix(mat){}

		T& operator[](int index)
		{ 
			return vals[index]; 
		}

		T operator[](int index) const
		{
			return vals[index];
		}
	};



	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T>::Matrix() 
	{
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T>::Matrix(T diagVal)
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
	
	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T>::Matrix(T values[])
	{
		for (int i = 0; i < height*width; i++)
		{
			this->vals[i] = values[i];
		}
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T>::Matrix(std::initializer_list<T> values)
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

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T>::Matrix(const Matrix &mat)
	{
		for (int i = 0; i < height*width; i++)
		{
			this->vals[i] = mat.vals[i];
		}
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T>::~Matrix()
	{
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> Matrix<height, width, T>::operator+(const Matrix<height, width, T> &right) const
	{
		T newVals[height*width];
		for (int i = 0; i < height*width; i++)
		{
			newVals[i] = this->vals[i] + right.vals[i];
		}
		return Matrix(newVals);
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> Matrix<height, width, T>::operator-() const
	{
		T newVals[height*width];
		for (int i = 0; i < height*width; i++)
		{
			newVals[i] = -this->vals[i];
		}
		return Matrix(newVals);
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> Matrix<height, width, T>::operator-(const Matrix<height, width, T> &right) const
	{
		T newVals[height*width];
		for (int i = 0; i < height*width; i++)
		{
			newVals[i] = this->vals[i] - right.vals[i];
		}
		return Matrix(newVals);
	}

	template<unsigned int height, unsigned int width, typename T>
	template<unsigned int newWidth>
	Matrix<height, newWidth, T> Matrix<height, width, T>::operator*(const Matrix<width, newWidth, T> &right) const
	{
		T newVals[height*newWidth];

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
		return Matrix<height, newWidth, T>(newVals);
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> operator*(float num, const Matrix<height, width, T> &mat)
	{
		return mat * num;
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> Matrix<height, width, T>::operator*(T num) const
	{
		T newVals[height*width];
		for (int i = 0; i < height*width; i++)
		{
			newVals[i] = this->vals[i] * num;
		}
		return Matrix<height, width, T>(newVals);
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> Matrix<height, width, T>::operator/(T num) const
	{
		T newVals[height*width];
		for (int i = 0; i < height*width; i++)
		{
			newVals[i] = this->vals[i] / num;
		}
		return Matrix<height, width, T>(newVals);
	}

	template<unsigned int height, unsigned int width, typename T>
	T* Matrix<height, width, T>::operator[](int row)
	{
		return this->vals + row * width;
	}

	template<unsigned int height, unsigned int width, typename T>
	std::string Matrix<height, width, T>::toString() const
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

	template<unsigned int height, unsigned int width, typename T>
	const T* Matrix<height, width, T>::getGLFormat() const {
		return this->vals;
	}
}


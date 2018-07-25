#pragma once
#include <initializer_list>
#include <string>
#include <sstream>

#include "MatrixTypes.h"

namespace Mat
{
	//the Vector operations below could be defined as member functions but 
	//are instead defined externally to keep the interface (with cross product)
	//consistent, and also because they're really only needed for floats...

	template<unsigned int length>
	float magnitude(const Vector<length> &vec);

	template<unsigned int length>
	Vector<length> normalize(const Vector<length> &vec);

	template<unsigned int length>
	Vector<length> dot(const Vector<length> &left, const Vector<length> &right);

	Vec3 cross(Vec3 left, Vec3 right);

	Mat4 translate(const Mat4 &mat, const Vec3 &transform);
	Mat4 scale(const Mat4 &mat, const Vec3 &transform);
	Mat4 rotate(const Mat4 &m, float angle, const Vec3 &v);

	float toRads(float in);
	Mat4 perspectiveFOV(float FOVangle, float aspectRatio, float nearz, float farz);
	Mat4 perspective(float left, float right, float top, float bottom, float nearz, float farz);
	Mat4 perspective(float width, float height, float nearz, float farz);

	Mat4 orthographic(float left, float right, float bottom, float top, float nearz, float farz);
	Mat4 orthographic(float width, float height, float nearz, float farz);



	template<unsigned int height, unsigned int width, typename T>
	class Matrix
	{
		static_assert(std::is_fundamental_v<T>, "Matrix and Vector can only use primitive types!");

		template<unsigned int height, unsigned int width, typename T> friend class Matrix;

	protected:

		T vals[height*width];

	public:
		//TODO: passing 0 for diagVal looks the same as passing nullptr for values, resulting in ambiguous calls..
		explicit Matrix(T diagVal = 1);
		explicit Matrix(T values[]);
		explicit Matrix(std::initializer_list<T> values);
		Matrix(const Matrix &mat);
		~Matrix();

		T* operator[](int row);
		T const* operator[](int row) const;

		std::string toString() const;

		//Returns a T[] representing the matrix, in row-major order
		const T* getGLFormat() const;
	};


	//Totally not sure if this class is worth it, the only changed functionality is the 
	//subscript operator, but I haven't been able to find another way around it yet
	//Tried std::enable_if / SFINAE, but it errors before that since the functions differ
	//only by return type
	template<unsigned int length, typename T>
	class Vector : public Matrix<length, 1, T>
	{
	public:
		explicit Vector(T initVal = 0) : Matrix(initVal){}
		explicit Vector(T values[]) : Matrix(values){}
		explicit Vector(std::initializer_list<T> values) : Matrix(values){}
		Vector(const Matrix &mat) : Matrix(mat){}

		T& operator[](int index)
		{ 
			return vals[index]; 
		}

		const T operator[](int index) const
		{
			return vals[index];
		}
	};



	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T>::Matrix(T diagVal)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (i == j || width == 1)
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
			vals[i] = values[i];
		}
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T>::Matrix(std::initializer_list<T> values)
	{
		const T *begin = values.begin();

		for (int i = 0; i < height*width; i++)
		{
			vals[i] = begin[i];
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
	bool operator==(const Matrix<height, width, T> &left, const Matrix<height, width, T> &right)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (left[i][j] != right[i][j])
					return false;
			}
		}
		return true;
	}

	//The assignment operator shouldn't need to be overloaded since vals is stored in the class



	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> operator+(const Matrix<height, width, T> &left, const Matrix<height, width, T> &right)
	{
		Matrix<height, width, T> ret;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				ret[i][j] = left[i][j] + right[i][j];
			}
		}
		return ret;
	}

	//Not a fan of all this code duplication, but it's better than vec<x> + vec<x> = mat<x,1> followed by an implicit
	//conversion back to vec<x>
	template<unsigned int height, typename T>
	Vector<height, T> operator+(const Vector<height, T> &left, const Vector<height, T> &right)
	{
		Vector<height, T> ret;
		for (int i = 0; i < height; i++)
		{
			ret[i] = left[i] + right[i];
		}
		return ret;
	}



	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> operator-(const Matrix<height, width, T> &right)
	{
		Matrix<height, width, T> ret;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				ret[i][j] = -right[i][j];
			}
		}
		return ret;
	}

	template<unsigned int height, typename T>
	Vector<height, T> operator-(const Vector<height, T> &right)
	{
		Vector<height, T> ret;
		for (int i = 0; i < height; i++)
		{
			ret[i] = -right[i];

		}
		return ret;
	}



	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> operator-(const Matrix<height, width, T> &left, const Matrix<height, width, T> &right)
	{
		Matrix<height, width, T> ret;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				ret[i][j] = left[i][j] - right[i][j];
			}
		}
		return ret;
	}

	template<unsigned int height, typename T>
	Vector<height, T> operator-(const Vector<height, T> &left, const Vector<height, T> &right)
	{
		Vector<height, T> ret;
		for (int i = 0; i < height; i++)
		{
			ret[i] = left[i] - right[i];
		}
		return ret;
	}



	template<unsigned int height, unsigned int common, unsigned int width, typename T>
	Matrix<height, width, T> operator*(const Matrix<height, common, T> &left, const Matrix<common, width, T> &right)
	{
		Matrix<height, width, T> ret;

		for (int row = 0; row < height; row++)
		{
			for (int col = 0; col < width; col++)
			{
				ret[row][col] = 0;
				for (int i = 0; i < common; i++)
				{
					ret[row][col] += left[row][i] * right[i][col];
				}
			}
		}
		return ret;
	}

	template<unsigned int height, unsigned int common, typename T>
	Vector<height, T> operator*(const Matrix<height, common, T> &left, const Vector<common, T> &right)
	{
		Vector<height, T> ret;

		for (int row = 0; row < height; row++)
		{
			ret[row] = 0;
			for (int i = 0; i < common; i++)
			{
				ret[row] += left[row][i] * right[i];
			}
		}
		return ret;
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> operator*(T num, const Matrix<height, width, T> &mat)
	{
		return mat * num;
	}

	template<unsigned int height, typename T>
	Vector<height, T> operator*(T num, const Vector<height, T> &vec)
	{
		return vec * num;
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> operator*(const Matrix<height, width, T> &left, T num)
	{
		Matrix<height, width, T> ret;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				ret[i][j] = left[i][j] * num;
			}
		}
		return ret;
	}

	template<unsigned int height, typename T>
	Vector<height, T> operator*(const Vector<height, T> &left, T num)
	{
		Vector<height, T> ret;
		for (int i = 0; i < height; i++)
		{
			ret[i] = left[i] * num;
		}
		return ret;
	}



	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> operator/(const Matrix<height, width, T> &left, T num)
	{
		Matrix<height, width, T> ret;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				ret[i][j] = left[i][j] / num;
			}
		}
		return ret;
	}

	template<unsigned int height, typename T>
	Vector<height, T> operator/(const Vector<height, T> &left, T num)
	{
		Vector<height, T> ret;
		for (int i = 0; i < height; i++)
		{
			ret[i] = left[i] / num;
		}
		return ret;
	}



	template<unsigned int height, unsigned int width, typename T>
	T* Matrix<height, width, T>::operator[](int row)
	{
		return this->vals + row * width;
	}

	template<unsigned int height, unsigned int width, typename T>
	T const* Matrix<height, width, T>::operator[](int row) const
	{
		return this->vals + row * width;
	}



	template<unsigned int height, unsigned int width, typename T>
	std::string Matrix<height, width, T>::toString() const
	{
		std::stringstream ret = std::stringstream();

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width - 1; j++)
			{
				ret << (vals[i*width + j]) << (",");
			}
			ret << (vals[(i + 1)*width - 1]) << ("\n");
		}
		return ret.str();
	}

	template<unsigned int height, unsigned int width, typename T>
	const T* Matrix<height, width, T>::getGLFormat() const {
		return this->vals;
	}
}


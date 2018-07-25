#include "CppUnitTest.h"
#include "Matrix.h"

#define NO_MESSAGE NULL

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft{ namespace VisualStudio {namespace CppUnitTestFramework
{
template<unsigned int height, unsigned int width, typename T>
std::wstring ToString(const Mat::Matrix<height, width, T>& q) 
{
	RETURN_WIDE_STRING(q.toString().c_str());
}
template<unsigned int height, typename T>
std::wstring ToString(const Mat::Vector<height, T>& q)
{
	RETURN_WIDE_STRING(q.toString().c_str());
}
}}}

namespace MatrixTests
{		
	TEST_CLASS(MatrixOperations)
	{
	public:
		
		TEST_METHOD(Addition)
		{
			Mat4 matrix1{11,12,13,14,  
						 21,22,23,24,  
						 31,32,33,34,  
						 41,42,43,44};

			Mat4 matrix2{ 3,0,0,0,
						  0,3,0,0,
						  0,0,3,0,
						  0,0,0,3};

			Mat4 matrix3{1,1,1,1,
						 1,0,0,1,
						 1,0,0,1,
						 1,1,1,1};

			Mat4 onePlusTwoM = matrix1 + matrix2;
			Mat4 twoPlusOneM = matrix2 + matrix1;
			Mat4 onePlusTwoMExpected{ 14,12,13,14, 
									 21,25,23,24,  
									 31,32,36,34,  
									 41,42,43,47 };

			Assert::AreEqual(onePlusTwoMExpected, onePlusTwoM, NO_MESSAGE, LINE_INFO());
			Assert::AreEqual(onePlusTwoM, twoPlusOneM, NO_MESSAGE, LINE_INFO());

			Mat4 twoPlusThreeM = matrix2 + matrix3;
			Mat4 twoPlusThreeMExpected{ 4,1,1,1,
									   1,3,0,1,
									   1,0,3,1,
									   1,1,1,4 };

			Assert::AreEqual(twoPlusThreeMExpected, twoPlusThreeM, NO_MESSAGE, LINE_INFO());
		}


		TEST_METHOD(Subraction)
		{
			Mat4 matrix1{ 11,12,13,14,
						  21,22,23,24,
						  31,32,33,34,
						  41,42,43,44 };

			Mat4 matrix2{ 3,0,0,0,
						  0,3,0,0,
						  0,0,3,0,
						  0,0,0,3 };

			Mat4 matrix3{ 1,1,1,1,
						  1,0,0,1,
						  1,0,0,1,
						  1,1,1,1 };

			Mat4 oneMinusTwoM = matrix1 - matrix2;
			Mat4 twoMinusOneM = matrix2 - matrix1;
			Mat4 oneMinusTwoMExpected{ 8, 12,13,14,
									   21,19,23,24,
									   31,32,30,34,
									   41,42,43,41 };

			Assert::AreEqual(oneMinusTwoMExpected, oneMinusTwoM, NO_MESSAGE, LINE_INFO());
			Assert::AreEqual(-oneMinusTwoM, twoMinusOneM, NO_MESSAGE, LINE_INFO());

			Mat4 twoMinusThreeM = matrix2 - matrix3;
			Mat4 twoMinusThreeMExpected{  2,-1,-1,-1,
										 -1, 3, 0,-1,
										 -1, 0, 3,-1,
										 -1,-1,-1, 2 };

			Assert::AreEqual(twoMinusThreeMExpected, twoMinusThreeM, NO_MESSAGE, LINE_INFO());
		}


		TEST_METHOD(Scalar_Multiplication) 
		{

		}


		TEST_METHOD(Matrix_Multiplication)
		{
			Mat4 matrix1{11,12,13,14,  
						 21,22,23,24,  
						 31,32,33,34,  
						 41,42,43,44};

			Mat4 matrix2(3);

			Mat4 matrix3{1,1,1,1,
						 1,0,0,1,
						 1,0,0,1,
						 1,1,1,1};

			Mat4 oneTimesTwo = matrix1 * matrix2;
			Mat4 twoTimesOne = matrix2 * matrix1;
			Mat4 oneTimesTwoExpected{ 33,  36,  39,  42,
									  63,  66,  69,  72,
									  93,  96,  99,  102,
									  123, 126, 129, 132 };

			Assert::AreEqual(oneTimesTwoExpected, oneTimesTwo, NO_MESSAGE, LINE_INFO());
			Assert::AreEqual(oneTimesTwoExpected, twoTimesOne, NO_MESSAGE, LINE_INFO());

			Mat4 oneTimesThree = matrix1 * matrix3;
			Mat4 oneTimesThreeExpected{ 50,  25, 25, 50,
										90,  45, 45, 90,
										130, 65, 65, 130,
										170, 85, 85, 170 };

			Mat4 threeTimesOne = matrix3 * matrix1;
			Mat4 threeTimesOneExpected{ 104, 108, 112, 116,
										52,  54,  56,  58,
										52,  54,  56,  58,
										104, 108, 112, 116};

			Assert::AreEqual(oneTimesThreeExpected, oneTimesThree, NO_MESSAGE, LINE_INFO());
			Assert::AreEqual(threeTimesOneExpected, threeTimesOne, NO_MESSAGE, LINE_INFO());
		}


		TEST_METHOD(Scalar_Division)
		{

		}

	};


	TEST_CLASS(VectorOperations)
	{
	public:

		TEST_METHOD(Addition)
		{
			Vec3 vec1{ 1,2,3 };
			Vec3 vec2{ 3,2,1 };

			Vec3 onePlusTwoV = vec1 + vec2;
			Vec3 onePlusTwoVExpected{ 4,4,4 };

			Assert::AreEqual(onePlusTwoVExpected, onePlusTwoV, NO_MESSAGE, LINE_INFO());
		}


		TEST_METHOD(Subtraction)
		{
			Vec3 vec1{ 1,2,3 };
			Vec3 vec2{ 3,2,1 };

			Vec3 oneMinusTwoV = vec1 - vec2;
			Vec3 oneMinusTwoVExpected{ -2,0,2 };

			Assert::AreEqual(oneMinusTwoVExpected, oneMinusTwoV, NO_MESSAGE, LINE_INFO());
		}

		
		TEST_METHOD(Scalar_Multiplication)
		{
			
		}


		TEST_METHOD(Scalar_Division)
		{

		}


		TEST_METHOD(Magnitude)
		{

		}


		TEST_METHOD(Normalization)
		{

		}


		TEST_METHOD(Dot)
		{

		}


		TEST_METHOD(Cross)
		{

		}
	};


	TEST_CLASS(Transforms) {
	public:

		TEST_METHOD(Translation)
		{
			Vec4 vec{ 0,0,0,1 };

			Mat4 transPosX;
			transPosX = Mat::translate(transPosX, Vec3{1,0,0});

			Vec4 transXExpected{ 1,0,0,1 };
			Assert::AreEqual(transXExpected, transPosX * vec, NO_MESSAGE, LINE_INFO());

			Mat4 transPosY;
			transPosY = Mat::translate(transPosY, Vec3{ 0,1,0 });

			Vec4 transYExpected{ 0,1,0,1 };
			Assert::AreEqual(transYExpected, transPosY * vec, NO_MESSAGE, LINE_INFO());

			Mat4 transPosZ;
			transPosZ = Mat::translate(transPosZ, Vec3{ 0,0,1 });

			Vec4 transZExpected{ 0,0,1,1 };
			Assert::AreEqual(transZExpected, transPosZ * vec, NO_MESSAGE, LINE_INFO());

			Vec4 transXYZExpected{ 1,1,1,1 };

			Assert::AreEqual(transXYZExpected, transPosX * transPosY * transPosZ * vec, NO_MESSAGE, LINE_INFO());
		}


		TEST_METHOD(Scale)
		{

		}


		TEST_METHOD(Rotate)
		{

		}


		TEST_METHOD(Multiple_Tranforms)
		{

		}
	};
}
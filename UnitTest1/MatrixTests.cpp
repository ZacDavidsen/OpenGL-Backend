#include "CppUnitTest.h"
#include "Matrix.h"

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
	TEST_CLASS(MatrixTests)
	{
	public:
		
		TEST_METHOD(Matrix_Addition)
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

			Assert::AreEqual(onePlusTwoM, onePlusTwoMExpected);
			Assert::AreEqual(twoPlusOneM, onePlusTwoM);

			Mat4 twoPlusThreeM = matrix2 + matrix3;
			Mat4 twoPlusThreeMExpected{ 4,1,1,1,
									   1,3,0,1,
									   1,0,3,1,
									   1,1,1,4 };

			Assert::AreEqual(twoPlusThreeM, twoPlusThreeMExpected);

			Vec3 vec1{ 1,2,3 };
			Vec3 vec2{ 3,2,1 };

			Vec3 onePlusTwoV = vec1 + vec2;
			Vec3 onePlusTwoVExpected{ 4,4,4 };

			Assert::AreEqual(onePlusTwoV, onePlusTwoVExpected);
		}


		TEST_METHOD(Matrix_Subraction)
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

			Assert::AreEqual(oneMinusTwoM, oneMinusTwoMExpected);
			Assert::AreEqual(twoMinusOneM, -oneMinusTwoM);

			Mat4 twoMinusThreeM = matrix2 - matrix3;
			Mat4 twoMinusThreeMExpected{  2,-1,-1,-1,
										 -1, 3, 0,-1,
										 -1, 0, 3,-1,
										 -1,-1,-1, 2 };

			Assert::AreEqual(twoMinusThreeM, twoMinusThreeMExpected);

			Vec3 vec1{ 1,2,3 };
			Vec3 vec2{ 3,2,1 };

			Vec3 oneMinusTwoV = vec1 - vec2;
			Vec3 oneMinusTwoVExpected{ -2,0,2 };

			//Assert::AreEqual(oneMinusTwoV, oneMinusTwoVExpected);
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

			Assert::AreEqual(oneTimesTwo, oneTimesTwoExpected);
			Assert::AreEqual(twoTimesOne, oneTimesTwoExpected);

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

			Assert::AreEqual(oneTimesThree, oneTimesThreeExpected);
			Assert::AreEqual(threeTimesOne, threeTimesOneExpected);
		}

	};
}
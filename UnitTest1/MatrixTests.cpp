#include "CppUnitTest.h"
#include "Matrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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

			Mat4 matrix2(3);

			Mat4 matrix3{1,1,1,1,
						 1,0,0,1,
						 1,0,0,1,
						 1,1,1,1};

			Mat4 onePlusTwo = matrix1 + matrix2;
			Mat4 onePlusTwoExpected{ 14,12,13,14, 
									 21,25,23,24,  
									 31,32,36,34,  
									 41,42,43,47 };

			Mat4 twoPlusThree = matrix2 + matrix3;
			Mat4 twoPlusThreeExpected{ 4,1,1,1,
									   1,3,0,1,
									   1,0,3,1,
									   1,1,1,4 };

			Assert::AreEqual(onePlusTwo.toString(), onePlusTwoExpected.toString());
			Assert::AreEqual(twoPlusThree.toString(), twoPlusThreeExpected.toString());
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

			Assert::AreEqual(oneTimesTwo.toString(), oneTimesTwoExpected.toString());
			Assert::AreEqual(twoTimesOne.toString(), oneTimesTwoExpected.toString());

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

			Assert::AreEqual(oneTimesThree.toString(), oneTimesThreeExpected.toString());
			Assert::AreEqual(threeTimesOne.toString(), threeTimesOneExpected.toString());
		}

	};
}
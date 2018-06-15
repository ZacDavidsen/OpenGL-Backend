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
			Mat4 matrix1{11,12,13,14,  21,22,23,24,  31,32,33,34,  41,42,43,44};
			Mat4 matrix2(3);

			Mat4 matrixRes = matrix1 + matrix2;
			Mat4 expected{ 14,12,13,14,  21,25,23,24,  31,32,36,34,  41,42,43,47 };

			Assert::AreEqual(matrixRes.toString(), expected.toString());
		}

	};
}
#include <gtest\gtest.h>
#include <dr_matrixnxm.h>

using driderSDK::MatrixNxM;
using driderSDK::VectorN;
using driderSDK::SizeT;

template<SizeT _rows, SizeT _cols> void 
elementsEqualToValue(const MatrixNxM<_rows, _cols>& matrix, float value) {
  for (SizeT iRow = 0; iRow < _rows; ++iRow) {
    for (SizeT iCol = 0; iCol < _cols; ++iCol) {
       EXPECT_FLOAT_EQ(matrix[iRow][iCol], value);
    }
  }
}

template<SizeT _rows, SizeT _cols> void
rowsEqualToVector(const MatrixNxM<_rows, _cols>& matrix, 
                  const VectorN<_cols>& vector) {
  for (SizeT iRow = 0; iRow < _rows; iRow++) {
    for (SizeT iCol = 0; iCol < _cols; ++iCol) {
       EXPECT_FLOAT_EQ(matrix[iRow][iCol], vector[iCol]);
    }
  }
}

TEST(MatrixNxM, constructorOneFloat) {
  MatrixNxM<4, 4> matrix(1.44f);

  elementsEqualToValue(matrix, 1.44f);
}

TEST(MatrixNxM, constructorOneVector) {
  VectorN<4> vector;
  vector[0] = 1.3f;
  vector[1] = 2.3f;
  vector[2] = 3.3f;
  vector[3] = 4.3f;
  
  MatrixNxM<4,4> matrix(vector);
  
  rowsEqualToVector(matrix, vector);
}

TEST(MatrixNxM, operatorBrackets) {
  const MatrixNxM<3,3> constMatrix(12.f);
  MatrixNxM<3,3> matrix(9.f);
  
  EXPECT_FLOAT_EQ(matrix[0][2], 9.f);
  EXPECT_FLOAT_EQ(constMatrix[1][1], 12.f);
}

TEST(MatrixNxM, rows) {
  MatrixNxM<10, 2> matrix;

  EXPECT_TRUE(matrix.rows() == 10);
}

TEST(MatrixNxM, cols) {
  MatrixNxM<10, 22> matrix;

  EXPECT_TRUE(matrix.cols() == 22);
}

TEST(MatrixNxM, transpose) {
  MatrixNxM<3,3> matrix;

  matrix[0][0] = 1.f; matrix[0][1] = 2.f; matrix[0][2] = 3.f;
  matrix[1][0] = 4.f; matrix[1][1] = 5.f; matrix[1][2] = 6.f;
  matrix[2][0] = 7.f; matrix[2][1] = 8.f; matrix[2][2] = 9.f;

  matrix = matrix.transpose();

  EXPECT_FLOAT_EQ(matrix[0][0], 1.f);
  EXPECT_FLOAT_EQ(matrix[0][1], 4.f);
  EXPECT_FLOAT_EQ(matrix[0][2], 7.f);
  EXPECT_FLOAT_EQ(matrix[1][0], 2.f);
  EXPECT_FLOAT_EQ(matrix[1][1], 5.f);
  EXPECT_FLOAT_EQ(matrix[1][2], 8.f);
  EXPECT_FLOAT_EQ(matrix[2][0], 3.f);
  EXPECT_FLOAT_EQ(matrix[2][1], 6.f);
  EXPECT_FLOAT_EQ(matrix[2][2], 9.f);
}

TEST(MatrixNxM, ptr) {
  MatrixNxM<2,13> matrix;
  const MatrixNxM<2,4> constMatrix;

  EXPECT_TRUE(&matrix[0][0] == matrix.ptr());
  EXPECT_TRUE(&constMatrix[0][0] == constMatrix.ptr());
}

TEST(MatrixNxM, assignByMultiplicationFloat) {
  MatrixNxM<2, 3> matrix(5.f);

  matrix *= 10.f;

  elementsEqualToValue(matrix, 50.f);
}

TEST(MatrixNxM, assignByDivisionFloat) {
  MatrixNxM<2, 5> matrix(25.f);

  matrix /= 10.f;

  elementsEqualToValue(matrix, 2.5f);
}

TEST(MatrixNxM, operatorEquality) {
  MatrixNxM<2,2> matrixA(10.f);
  matrixA[1][1] = 22.f;
  MatrixNxM<2,2> matrixB(10.f);
  matrixB[1][1] = 22.f;

  EXPECT_TRUE(matrixA == matrixB);

  matrixA[0][0] = 2.f;

  EXPECT_FALSE(matrixA == matrixB);
}

TEST(MatrixNxM, operatorInequality) {
  MatrixNxM<2,2> matrixA(2.f);
  matrixA[1][1] = 22.f;
  MatrixNxM<2,2> matrixB(2.f);
  
  EXPECT_TRUE(matrixA != matrixB);

  matrixB[1][1] = 22.f;

  EXPECT_FALSE(matrixA != matrixB);
}

TEST(MatrixtNxM,  operatorMultiplicationVectorLeft) {
  VectorN<3> vector;
  vector[0] = 1.f; vector[1] = 2.f; vector[2] = 3.f;
  MatrixNxM<3, 2> matrix; 
  matrix[0][0] = 10.f;  matrix[0][1] = 7.f;
  matrix[1][0] = 2.f;   matrix[1][1] = 4.f; 
  matrix[2][0] = 4.f;   matrix[2][1] = 6.f;

  VectorN<2> result;
  result = vector * matrix;

  EXPECT_FLOAT_EQ(result[0], 26.f);
  EXPECT_FLOAT_EQ(result[1], 33.f);
}

TEST(MatrixNxM, operatorMultiplicationVectorRight) {
  MatrixNxM<2, 3> matrix;
  VectorN<3> vector;
  VectorN<2> result;

  matrix[0][0] = 2.f; matrix[0][1] = 2.f; matrix[0][2] = 2.f; 
  matrix[1][0] = 3.f; matrix[1][1] = 4.f; matrix[1][2] = 5.f; 

  vector[0] = 1.f;
  vector[1] = 2.f;
  vector[2] = 3.f;

  result = matrix * vector;

  EXPECT_FLOAT_EQ(result[0], 12.f);
  EXPECT_FLOAT_EQ(result[1], 26.f);
}

TEST(MatrixNxM, opeartorMultiplicationScalarRight) {
  MatrixNxM<3,3> matrix(11.5f);

  matrix = matrix * 2.f;

  elementsEqualToValue(matrix, 23.f);
}

TEST(MatrixNxM, opeartorMultiplicationScalarLeft) {
  MatrixNxM<3,3> matrix(11.5f);

  matrix = 4.f * matrix;

  elementsEqualToValue(matrix, 46.f);}

TEST(MatrixNxM, operatorDivisionScalar) {
  MatrixNxM<3,3> matrix(125.f);

  matrix = matrix / 25.f;

  elementsEqualToValue(matrix, 5.f);
}

TEST(MatrixNxM, operatorMultiplicationMatrix) {
  MatrixNxM<2,2> matrixA(2.2f);
  MatrixNxM<2,2> matrixB(3.3f);

  MatrixNxM<2, 2> result = matrixA * matrixB;

  elementsEqualToValue(result, 14.52f);
}
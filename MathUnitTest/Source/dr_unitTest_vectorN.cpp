#include <dr_vectorn.h>
#include <gtest\gtest.h>

using driderSDK::VectorN;

template<driderSDK::SizeT _elements>
void elementsEqualToValue(const VectorN<_elements>& vector, float value) {
  for(int i = 0; i < _elements; ++i) {
      EXPECT_FLOAT_EQ(vector[i], value); 
  }
}

TEST(VectorN, constructorOneFloat) {
  VectorN<4> vector(20.5f);

  for(int i = 0; i < 4; ++i) {
      EXPECT_FLOAT_EQ(vector[i], 20.5f); 
  }
}

TEST(VectorN, elements) {
  VectorN<223> vector;

  EXPECT_TRUE(vector.elements() == 223);
}

TEST(VectorN, operatorBrakets){
  VectorN<33> vector(1.44f);
  const VectorN<33> vectorConst(3.88f);
  vector[32] = 2.4f;
  EXPECT_FLOAT_EQ(vector[32], 2.4f);
  EXPECT_FLOAT_EQ(vectorConst[32], 3.88f);
}

TEST(VectorN, dot) {
  VectorN<5> vector;
  vector[0] = 1;
  vector[1] = 2;
  vector[2] = 3;
  vector[3] = 4;
  vector[4] = 5;

  VectorN<5> vector2;

  vector2[0] = 1.5f;
  vector2[1] = 2.5f;
  vector2[2] = 3.5f;
  vector2[3] = 4.5f;
  vector2[4] = 5.5f;

  EXPECT_FLOAT_EQ(vector.dot(vector2), 62.5f);
}

TEST(VectorN, length) {
  VectorN<4> vector;
  vector[0] = 2.2f;
  vector[1] = 4.4f;
  vector[2] = 5.5f;
  vector[3] = 6.1f;

  EXPECT_FLOAT_EQ(vector.length(), 9.57392f);
}

TEST(VectorN, lengthSqr) {
  VectorN<4> vector;
  vector[0] = 2.2f;
  vector[1] = 4.4f;
  vector[2] = 5.5f;
  vector[3] = 6.1f;

  EXPECT_FLOAT_EQ(vector.lengthSqr(), 91.66f);
}

TEST(VectorN, normalize) {
  VectorN<4> vector;
  vector[0] = 2.2f;
  vector[1] = 4.4f;
  vector[2] = 5.5f;
  vector[3] = 6.1f;
  vector.normalize();
  EXPECT_FLOAT_EQ(vector[0], 0.22979087f);
  EXPECT_FLOAT_EQ(vector[1], 0.45958173f);
  EXPECT_FLOAT_EQ(vector[2], 0.57447714f);
  EXPECT_FLOAT_EQ(vector[3], 0.63714737f);
}

TEST(VectorN, ptr) {
  VectorN<3> vector;
  const VectorN<3> vectorConst;
  EXPECT_EQ(vector.ptr(), &vector[0]);
  EXPECT_EQ(vectorConst.ptr(), &vectorConst[0]);
}

TEST(VectorN, assignmentByMultply) {
  VectorN<3> vector(3);

  vector *= 4.5f;

  elementsEqualToValue(vector, 13.5f);
}

TEST(VectorN, assignmentByDivision) {
  VectorN<3> vector(10.5f);

  vector /= 2.1f;

  elementsEqualToValue(vector, 5.f);
}

TEST(VectorN, assignmentByAddition) {
  VectorN<3> vector(4.4f);
  VectorN<3> vector2(2.6f);

  vector += vector2;

  elementsEqualToValue(vector, 7.0f);
}

TEST(VectorN, assignmentBySubtraction) {
  VectorN<3> vector(4.4f);
  VectorN<3> vector2(2.6f);

  vector -= vector2;

  elementsEqualToValue(vector, 1.8f);
}

TEST(VectorN, unaryMinus) {
  VectorN<4> vector(-19.f);
  VectorN<4> vector2(20.f);

  vector = -vector;
  vector2 = -vector2;

  elementsEqualToValue(vector, 19.f);
  elementsEqualToValue(vector2, -20.f);
}

TEST(VectorN, equality) {
  VectorN<3> vector(4.4f);
  VectorN<3> vector2(2.6f);

  EXPECT_TRUE(vector == vector);
  EXPECT_FALSE(vector == vector2);
}

TEST(VectorN, inequality) {
  VectorN<3> vector(4.4f);
  VectorN<3> vector2(2.6f);

  EXPECT_FALSE(vector != vector);
  EXPECT_TRUE(vector != vector2);
}

TEST(VectorN, multiplyScalarRight) {
  VectorN<4> vector(3.f);
  
  vector = vector * 4.f;

  elementsEqualToValue(vector, 12.f);
}

TEST(VectorN, multiplyScalarLeft) {
  VectorN<4> vector(3.f);
  
  vector = 4.f * vector;

  elementsEqualToValue(vector, 12.f);
}

TEST(VectorN, divisionScalarRight) {
  VectorN<4> vector(100.f);
  
  vector = vector / 5.f;

  elementsEqualToValue(vector, 20.f);
}

TEST(VectorN, additionVector) {
  VectorN<4> vector(100.f);
  VectorN<4> vector2(50.f);

  vector = vector + vector2;

  elementsEqualToValue(vector, 150.f);  
}

TEST(VectorN, subtractionVector) {
  VectorN<4> vector(100.f);
  VectorN<4> vector2(50.f);

  vector = vector - vector2;

  elementsEqualToValue(vector, 50.f);  
}
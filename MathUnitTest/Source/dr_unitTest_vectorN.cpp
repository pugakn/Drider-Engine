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
  VectorN<4> testVector(20.5f);

  for(int i = 0; i < 4; ++i) {
      EXPECT_FLOAT_EQ(testVector[i], 20.5f); 
  }
}

TEST(VectorN, elements) {
  VectorN<223> test;

  EXPECT_TRUE(test.elements() == 223);
}

TEST(VectorN, operatorBrakets){
  VectorN<33> test(1.44f);
  const VectorN<33> testConst(3.88f);
  test[32] = 2.4f;
  EXPECT_FLOAT_EQ(test[32], 2.4f);
  EXPECT_FLOAT_EQ(testConst[32], 3.88f);
}

TEST(VectorN, dot) {
  VectorN<5> test;
  test[0] = 1;
  test[1] = 2;
  test[2] = 3;
  test[3] = 4;
  test[4] = 5;

  VectorN<5> test2;

  test2[0] = 1.5f;
  test2[1] = 2.5f;
  test2[2] = 3.5f;
  test2[3] = 4.5f;
  test2[4] = 5.5f;

  EXPECT_FLOAT_EQ(test.dot(test2), 62.5f);
}

TEST(VectorN, length) {
  VectorN<4> test;
  test[0] = 2.2f;
  test[1] = 4.4f;
  test[2] = 5.5f;
  test[3] = 6.1f;

  EXPECT_FLOAT_EQ(test.length(), 9.57392f);
}

TEST(VectorN, lengthSqr) {
  VectorN<4> test;
  test[0] = 2.2f;
  test[1] = 4.4f;
  test[2] = 5.5f;
  test[3] = 6.1f;

  EXPECT_FLOAT_EQ(test.lengthSqr(), 91.66f);
}

TEST(VectorN, normalize) {
  VectorN<4> test;
  test[0] = 2.2f;
  test[1] = 4.4f;
  test[2] = 5.5f;
  test[3] = 6.1f;
  test.normalize();
  EXPECT_FLOAT_EQ(test[0], 0.22979087f);
  EXPECT_FLOAT_EQ(test[1], 0.45958173f);
  EXPECT_FLOAT_EQ(test[2], 0.57447714f);
  EXPECT_FLOAT_EQ(test[3], 0.63714737f);
}

TEST(VectorN, ptr) {
  VectorN<3> test;
  const VectorN<3> testConst;
  EXPECT_EQ(test.ptr(), &test[0]);
  EXPECT_EQ(testConst.ptr(), &testConst[0]);
}

TEST(VectorN, assignmentByMultply) {
  VectorN<3> test(3);

  test *= 4.5f;

  elementsEqualToValue(test, 13.5f);
}

TEST(VectorN, assignmentByDivision) {
  VectorN<3> test(10.5f);

  test /= 2.1f;

  elementsEqualToValue(test, 5.f);
}

TEST(VectorN, assignmentByAddition) {
  VectorN<3> test(4.4f);
  VectorN<3> test2(2.6f);

  test += test2;

  elementsEqualToValue(test, 7.0f);
}

TEST(VectorN, assignmentBySubtraction) {
  VectorN<3> test(4.4f);
  VectorN<3> test2(2.6f);

  test -= test2;

  elementsEqualToValue(test, 1.8f);
}

TEST(VectorN, unaryMinus) {
  VectorN<4> test(-19.f);
  VectorN<4> test2(20.f);

  test = -test;
  test2 = -test2;

  elementsEqualToValue(test, 19.f);
  elementsEqualToValue(test2, -20.f);
}

TEST(VectorN, equality) {
  VectorN<3> test(4.4f);
  VectorN<3> test2(2.6f);

  EXPECT_TRUE(test == test);
  EXPECT_FALSE(test == test2);
}

TEST(VectorN, inequality) {
  VectorN<3> test(4.4f);
  VectorN<3> test2(2.6f);

  EXPECT_FALSE(test != test);
  EXPECT_TRUE(test != test2);
}

TEST(VectorN, multiplyScalarRight) {
  VectorN<4> test(3.f);
  
  test = test * 4.f;

  elementsEqualToValue(test, 12.f);
}

TEST(VectorN, multiplyScalarLeft) {
  VectorN<4> test(3.f);
  
  test = 4.f * test;

  elementsEqualToValue(test, 12.f);
}

TEST(VectorN, divisionScalarRight) {
  VectorN<4> test(100.f);
  
  test = test / 5.f;

  elementsEqualToValue(test, 20.f);
}

TEST(VectorN, additionVector) {
  VectorN<4> test(100.f);
  VectorN<4> test2(50.f);

  test = test + test2;

  elementsEqualToValue(test, 150.f);  
}

TEST(VectorN, subtractionVector) {
  VectorN<4> test(100.f);
  VectorN<4> test2(50.f);

  test = test - test2;

  elementsEqualToValue(test, 50.f);  
}
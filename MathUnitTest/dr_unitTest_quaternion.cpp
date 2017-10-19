#include <gtest\gtest.h>
#include <dr_quaternion.h>

void checkQuaternionValues(driderSDK::Quaternion& TestQuaternion,
													 float x,
													 float y,
													 float z,
													 float w)
{
	EXPECT_EQ(TestQuaternion.x, x);
	EXPECT_EQ(TestQuaternion.y, y);
	EXPECT_EQ(TestQuaternion.z, z);
	EXPECT_EQ(TestQuaternion.w, w);
}

TEST(forceInitZero, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(driderSDK::Math::kZero);
	checkQuaternionValues(TestQuaternion, 0.f, 0.f, 0.f, 0.f);
}

TEST(forceInitIdentity, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(driderSDK::Math::kIdentity);
	checkQuaternionValues(TestQuaternion, 0.f, 0.f, 0.f, 1.f);
}

TEST(constructFromQuaternion, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(driderSDK::Math::kIdentity);
	driderSDK::Quaternion Quaternion2(TestQuaternion);
	checkQuaternionValues(Quaternion2, 0.f, 0.f, 0.f, 1.f);
}

TEST(constructFrom4floats, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(1.f, 2.f, 3.f, 5.f);
	checkQuaternionValues(TestQuaternion, 1.f, 2.f, 3.f, 5.f);
}

TEST(constructFromVector4D, Quaternion)
{
	driderSDK::Vector4D TestVector(5.f, 3.f, 2.f, 1.f);
	driderSDK::Quaternion TestQuaternion(TestVector);
	checkQuaternionValues(TestQuaternion, 5.f, 3.f, 2.f, 1.f);
}

TEST(measure, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(1.f, 2.f, 3.f, 5.f);
	EXPECT_EQ(TestQuaternion.measure(), 6.224997998);
}

TEST(conjugate, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(60.f, 22.f, 44.f, 52.f);
	checkQuaternionValues(TestQuaternion.conjugate(), -60.f, -22.f, -44.f, 52.f);
}

TEST(normalize, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(5.f, 3.f, 1.f, 2.f);
	TestQuaternion.normalize();
	EXPECT_EQ(TestQuaternion.measure(), 1);
}

//TEST(rotation, Quaternion)
//{
//	
//}
//
//TEST(matrixFromQuaternion4x4, Quaternion)
//{
//
//}
//
//TEST(matrixFromQuaternion3x3, Quaternion)
//{
//
//}

TEST(operatorBrackets, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(1.f, 6.f, 8.f, 5.f);
	float Testfloat = TestQuaternion[4];
	EXPECT_EQ(Testfloat, 5.f);
}

TEST(operatorAdd, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(1.f, 2.f, 3.f, 5.f);
	driderSDK::Quaternion OtherQuaternion(5.f, 15.f, 5.f, 20.f);
	checkQuaternionValues(TestQuaternion + OtherQuaternion, 6.f, 17.f, 8.f, 25.f);
}

TEST(operatorAddEquals, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(5.f, 5.f, 5.f, 5.f);
	driderSDK::Quaternion OtherQuaternion(10.f, 20.f, 10.f, 20.f);
	TestQuaternion += OtherQuaternion;
	checkQuaternionValues(TestQuaternion, 15.f, 25.f, 15.f, 25.f);
}

TEST(operatorMinus, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(3.f, 1.f, 5.f, 2.f);
	driderSDK::Quaternion OtherQuaternion(15.f, 10.f, 5.f, 12.f);
	checkQuaternionValues(OtherQuaternion - TestQuaternion, 12.f, 9.f, 0.f, 10.f);
}

TEST(operatorMinusEquals, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(2.f, 1.f, 2.f, 1.f);
	driderSDK::Quaternion OtherQuaternion(5.f, 7.f, 8.f, 2.f);
	OtherQuaternion -= TestQuaternion;
	checkQuaternionValues(OtherQuaternion, 3.f, 6.f, 6.f, 1.f);
}

TEST(operatorMultiplyByQuaternion, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(6.f, 1.f, 2.f, 5.f);
	driderSDK::Quaternion OtherQuaternion(3.f, 1.f, 6.f, 2.f);
	checkQuaternionValues(TestQuaternion*OtherQuaternion, 31.f, -23.f, 37.f, -21.f);
}

TEST(operatorMultiplyByScalar, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(2.f, 5.f, 3.f, 4.f);
	float TestScalar = 5;
	checkQuaternionValues(TestQuaternion*TestScalar, 10.f, 25.f, 15.f, 20.f);
}

TEST(operatorMultiplyByQuaternionEquals, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(6.f, 3.f, 4.f, 8.f);
	driderSDK::Quaternion OtherQuaternion(2.f, 5.f, 1.f, 7.f);
	TestQuaternion *= OtherQuaternion;
	checkQuaternionValues(TestQuaternion, 41.f, 63.f, 60.f, 25.f);
}

TEST(operatorMultiplyByScalarEquals, Quaternion)
{
	driderSDK::Quaternion TestQuaternion(12.f, 8.f, 4.f, 2.f);
	float TestScalar = 2;
	TestQuaternion *= TestScalar;
	checkQuaternionValues(TestQuaternion, 24.f, 16.f, 8.f, 4.f);
}
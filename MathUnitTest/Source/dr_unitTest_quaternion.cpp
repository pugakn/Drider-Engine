#include <gtest\gtest.h>
#include <dr_quaternion.h>
#include <dr_matrix4x4.h>
#include <dr_matrix3x3.h>


void checkQuaternionValues(driderSDK::Quaternion& TestQuaternion,
													 float x,
													 float y,
													 float z,
													 float w)
{
	EXPECT_FLOAT_EQ(TestQuaternion.x, x);
	EXPECT_FLOAT_EQ(TestQuaternion.y, y);
	EXPECT_FLOAT_EQ(TestQuaternion.z, z);
	EXPECT_FLOAT_EQ(TestQuaternion.w, w);
}

TEST(Quaternion, defaultConstructor)
{
	driderSDK::Quaternion* pQuaternion;
	pQuaternion = new driderSDK::Quaternion;
	EXPECT_FALSE(nullptr == pQuaternion);
	delete pQuaternion;
}

TEST(Quaternion, forceInitZero) {
	driderSDK::Quaternion TestQuaternion(driderSDK::Math::kZero);
	checkQuaternionValues(TestQuaternion, 0.f, 0.f, 0.f, 0.f);
}

TEST(Quaternion, forceInitIdentity) {
	driderSDK::Quaternion TestQuaternion(driderSDK::Math::kIdentity);
	checkQuaternionValues(TestQuaternion, 0.f, 0.f, 0.f, 1.f);
}

TEST(Quaternion, moveConstructor)
{
	driderSDK::Quaternion TestQuaternion(1.f, 1.f, 1.f, 2.f);
	driderSDK::Quaternion OtherQuaternion(std::move(TestQuaternion));
	checkQuaternionValues(OtherQuaternion, 1.f, 1.f, 1.f, 2.f);
}

TEST(Quaternion, constructFromQuaternion) {
	driderSDK::Quaternion TestQuaternion(driderSDK::Math::kIdentity);
	driderSDK::Quaternion Quaternion2(TestQuaternion);
	checkQuaternionValues(Quaternion2, 0.f, 0.f, 0.f, 1.f);
}

TEST(Quaternion, constructFrom4floats) {
	driderSDK::Quaternion TestQuaternion(1.f, 2.f, 3.f, 5.f);
	checkQuaternionValues(TestQuaternion, 1.f, 2.f, 3.f, 5.f);
}

TEST(Quaternion, constructFromVector4D) {
	driderSDK::Vector4D TestVector(5.f, 3.f, 2.f, 1.f);
	driderSDK::Quaternion TestQuaternion(TestVector);
	checkQuaternionValues(TestQuaternion, 5.f, 3.f, 2.f, 1.f);
}

TEST(Quaternion, destructor)
{
	driderSDK::Quaternion TestQuaternion(1.f, 3.f, 0.f, 5.f);
	TestQuaternion.~Quaternion();
	checkQuaternionValues(TestQuaternion, 1.f, 3.f, 0.f, 5.f);
}

TEST(Quaternion, measure) {
	driderSDK::Quaternion TestQuaternion(1.f, 2.f, 3.f, 5.f);
	float TestResult = driderSDK::Math::sqrt((1.f*1.f) + (2.f*2.f) + (3.f*3.f) + (5.f*5.f));
	EXPECT_FLOAT_EQ(TestQuaternion.measure(), TestResult);
}

TEST(Quaternion, conjugate) {
	driderSDK::Quaternion TestQuaternion(60.f, 22.f, 44.f, 52.f);
	checkQuaternionValues(TestQuaternion.conjugate(), -60.f, -22.f, -44.f, 52.f);
}

TEST(Quaternion, normalize) {
	driderSDK::Quaternion TestQuaternion(5.f, 3.f, 1.f, 2.f);
	TestQuaternion.normalize();
	EXPECT_FLOAT_EQ(TestQuaternion.measure(), 1);
}

TEST(Quaternion, rotation) {
	driderSDK::Quaternion TestQuaternion(0.f, 0.7071203316249954f, 0.f, 0.7071203316249954f);
	driderSDK::Vector3D TestVector(1,0,0);

	driderSDK::Vector3D Result = TestQuaternion.rotation(TestVector);
	EXPECT_FLOAT_EQ(Result.x, 0);
	EXPECT_FLOAT_EQ(Result.y, 0);
	EXPECT_FLOAT_EQ(Result.z, -1.f);

	
}

TEST(Quaternion, matrixFromQuaternion4x4)
{
	driderSDK::Quaternion TestQuaternion(3.f, 3.f, 2.f, 5.f);
	driderSDK::Matrix4x4 TestMatrix4;
	driderSDK::Matrix4x4 Result(21.f/47.f, -0.042553157f, 42.f/47.f, 0.f,
															38.f/47.f, 21.f/47.f, -18.f/47.f, 0.f,
															-18.f/47.f, 42.f/47.f, 11.f/47.f, 0.f, 
															0.f, 0.f, 0.f, 1.f);
	TestQuaternion.matrixFromQuaternion(TestMatrix4);
	EXPECT_FLOAT_EQ(Result[0][0], TestMatrix4[0][0]);
	EXPECT_FLOAT_EQ(Result[0][1], TestMatrix4[0][1]);
	EXPECT_FLOAT_EQ(Result[0][2], TestMatrix4[0][2]);
	EXPECT_FLOAT_EQ(Result[0][3], TestMatrix4[0][3]);

	EXPECT_FLOAT_EQ(Result[1][0], TestMatrix4[1][0]);
	EXPECT_FLOAT_EQ(Result[1][1], TestMatrix4[1][1]);
	EXPECT_FLOAT_EQ(Result[1][2], TestMatrix4[1][2]);
	EXPECT_FLOAT_EQ(Result[1][3], TestMatrix4[1][3]);

	EXPECT_FLOAT_EQ(Result[2][0], TestMatrix4[2][0]);
	EXPECT_FLOAT_EQ(Result[2][1], TestMatrix4[2][1]);
	EXPECT_FLOAT_EQ(Result[2][2], TestMatrix4[2][2]);
	EXPECT_FLOAT_EQ(Result[2][3], TestMatrix4[2][3]);

	EXPECT_FLOAT_EQ(Result[3][0], TestMatrix4[3][0]);
	EXPECT_FLOAT_EQ(Result[3][1], TestMatrix4[3][1]);
	EXPECT_FLOAT_EQ(Result[3][2], TestMatrix4[3][2]);
	EXPECT_FLOAT_EQ(Result[3][3], TestMatrix4[3][3]);
}

TEST(Quaternion, matrixFromQuaternion3x3)
{
	driderSDK::Quaternion TestQuaternion(6.f, 3.f, 4.f, 8.f);
	driderSDK::Matrix3x3 TestMatrix3;
	driderSDK::Matrix3x3 Result(3.f/5.f, -28.f/125.f, 96.f/125.f,
															4.f/5.f, 21.f/125.f, -72.f/125.f, 
															0, 24.f/25.f, 7.f/25.f);
	TestQuaternion.matrixFromQuaternion(TestMatrix3);

	EXPECT_FLOAT_EQ(Result[0][0], TestMatrix3[0][0]);
	EXPECT_FLOAT_EQ(Result[0][1], TestMatrix3[0][1]);
	EXPECT_FLOAT_EQ(Result[0][2], TestMatrix3[0][2]);
	EXPECT_FLOAT_EQ(Result[1][0], TestMatrix3[1][0]);
	EXPECT_FLOAT_EQ(Result[1][1], TestMatrix3[1][1]);
	EXPECT_FLOAT_EQ(Result[1][2], TestMatrix3[1][2]);
	EXPECT_FLOAT_EQ(Result[2][0], TestMatrix3[2][0]);
	EXPECT_FLOAT_EQ(Result[2][1], TestMatrix3[2][1]);
	EXPECT_FLOAT_EQ(Result[2][2], TestMatrix3[2][2]);

}

TEST(Quaternion, pointer)
{
	driderSDK::Quaternion TestQuaternion(25.f, 0.f, 1.f, 5.f);
	EXPECT_FLOAT_EQ(25.f, *TestQuaternion.ptr());
}

TEST(Quaternion, constPointer)
{
	driderSDK::Quaternion TestQuaternion(27.f, 0.f, 1.f, 5.f);
	EXPECT_FLOAT_EQ(27.f, *TestQuaternion.ptr());
}

TEST(Quaternion, operatorBrackets) {
	driderSDK::Quaternion TestQuaternion(1.f, 6.f, 8.f, 5.f);
	float Testfloat = TestQuaternion[3];
	EXPECT_FLOAT_EQ(Testfloat, 5.f);
}

TEST(Quaternion, constOperatorBrackets)
{
	driderSDK::Quaternion TestQuaternion(1.f, 6.f, 8.f, 5.f);
	float Testfloat = TestQuaternion[3];
	EXPECT_FLOAT_EQ(Testfloat, 5.f);
}

TEST(Quaternion, operatorAdd) {
	driderSDK::Quaternion TestQuaternion(1.f, 2.f, 3.f, 5.f);
	driderSDK::Quaternion OtherQuaternion(5.f, 15.f, 5.f, 20.f);
	checkQuaternionValues(TestQuaternion + OtherQuaternion, 6.f, 17.f, 8.f, 25.f);
}

TEST(Quaternion, operatorAddEquals) {
	driderSDK::Quaternion TestQuaternion(5.f, 5.f, 5.f, 5.f);
	driderSDK::Quaternion OtherQuaternion(10.f, 20.f, 10.f, 20.f);
	TestQuaternion += OtherQuaternion;
	checkQuaternionValues(TestQuaternion, 15.f, 25.f, 15.f, 25.f);
}

TEST(Quaternion, operatorMinus) {
	driderSDK::Quaternion TestQuaternion(3.f, 1.f, 5.f, 2.f);
	driderSDK::Quaternion OtherQuaternion(15.f, 10.f, 5.f, 12.f);
	checkQuaternionValues(OtherQuaternion - TestQuaternion, 12.f, 9.f, 0.f, 10.f);
}

TEST(Quaternion, operatorMinusEquals) {
	driderSDK::Quaternion TestQuaternion(2.f, 1.f, 2.f, 1.f);
	driderSDK::Quaternion OtherQuaternion(5.f, 7.f, 8.f, 2.f);
	OtherQuaternion -= TestQuaternion;
	checkQuaternionValues(OtherQuaternion, 3.f, 6.f, 6.f, 1.f);
}

TEST(Quaternion, operatorMultiplyByQuaternion) {
	driderSDK::Quaternion TestQuaternion(6.f, 1.f, 2.f, 5.f);
	driderSDK::Quaternion OtherQuaternion(3.f, 1.f, 6.f, 2.f);
	checkQuaternionValues(TestQuaternion*OtherQuaternion, 31.f, -23.f, 37.f, -21.f);
}

TEST(Quaternion, operatorMultiplyByScalar) {
	driderSDK::Quaternion TestQuaternion(2.f, 5.f, 3.f, 4.f);
	float TestScalar = 5;
	checkQuaternionValues(TestQuaternion*TestScalar, 10.f, 25.f, 15.f, 20.f);
}

TEST(Quaternion, operatorMultiplyByQuaternionEquals) {
	driderSDK::Quaternion TestQuaternion(6.f, 3.f, 4.f, 8.f);
	driderSDK::Quaternion OtherQuaternion(2.f, 5.f, 1.f, 7.f);
	TestQuaternion *= OtherQuaternion;
	checkQuaternionValues(TestQuaternion, 41.f, 63.f, 60.f, 25.f);
}

TEST(Quaternion, operatorMultiplyByScalarEquals) {
	driderSDK::Quaternion TestQuaternion(12.f, 8.f, 4.f, 2.f);
	float TestScalar = 2;
	TestQuaternion *= TestScalar;
	checkQuaternionValues(TestQuaternion, 24.f, 16.f, 8.f, 4.f);
}

TEST(Quaternion, operatorDivide) {
	driderSDK::Quaternion TestQuaternion(0.f, 1.f, 0.f, 1.f);
	driderSDK::Quaternion OtherQuaternion(0.5f, 0.5f, 0.75f, 1.f);
	checkQuaternionValues(TestQuaternion/ OtherQuaternion,
												0.12121212f, 0.24242425f, -0.60606062f, 0.72727275f);
}

TEST(Quaternion, operatorDivideEquals) {
	driderSDK::Quaternion TestQuaternion(0.f, 1.f, 0.f, 1.f);
	driderSDK::Quaternion OtherQuaternion(0.5f, 0.5f, 0.75f, 1.f);

	TestQuaternion /= OtherQuaternion;
	checkQuaternionValues(TestQuaternion,
												0.12121212f, 0.24242425f, -0.60606062f, 0.72727275f);
}

TEST(Quaternion, operatorAssign) {
	driderSDK::Quaternion TestQuaternion(driderSDK::Math::kZero);
	driderSDK::Quaternion OtherQuaternion(9.f, 8.f, 7.f, 6.f);
	TestQuaternion = OtherQuaternion;
	checkQuaternionValues(TestQuaternion, 9.f, 8.f, 7.f, 6.f);
}

TEST(Quaternion, operatorEqual) {
	driderSDK::Quaternion TestQuaternion(18.f, 17.f, 16.f, 15.f);
	driderSDK::Quaternion TrueQuaternion(18.f, 17.f, 16.f, 15.f);
	driderSDK::Quaternion FalseQuaternion(7.f, 45.f, 15.f, 48.f);
	EXPECT_TRUE(TestQuaternion == TrueQuaternion);
	EXPECT_FALSE(TestQuaternion == FalseQuaternion);
}

TEST(Quaternion, operatorNotEqual) {
	driderSDK::Quaternion TestQuaternion(75.f, 12.f, 8.f, 1.f);
	driderSDK::Quaternion TrueQuaternion(85.f, 1.f, 96.f, 2.f);
	driderSDK::Quaternion FalseQuaternion(75.f, 12.f, 8.f, 1.f);
	EXPECT_TRUE(TestQuaternion != TrueQuaternion);
	EXPECT_FALSE(TestQuaternion != FalseQuaternion);
}

TEST(Quaternion, operatorNot) {
	driderSDK::Quaternion TestQuaternion(1.f, 2.f, 3.f, 4.f);
	checkQuaternionValues(-TestQuaternion, -1.f, -2.f, -3.f, -4.f);
}
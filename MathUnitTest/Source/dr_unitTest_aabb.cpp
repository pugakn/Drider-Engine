#include <dr_aabb.h>
#include <dr_ray.h>
#include <dr_plane.h>
#include <dr_sphere.h>
#include <dr_frustrum.h>
#include <dr_matrix4x4.h>
#include <gtest\gtest.h>

void checkForAABBValues(driderSDK::AABB& aabb, 
												float _width, 
												float _height, 
												float _depth,
												float _x,
												float _y,
												float _z)
{
	EXPECT_FLOAT_EQ(aabb.width, _width);
	EXPECT_FLOAT_EQ(aabb.height, _height);
	EXPECT_FLOAT_EQ(aabb.depth, _depth);
	EXPECT_FLOAT_EQ(aabb.center.x, _x);
	EXPECT_FLOAT_EQ(aabb.center.y, _y);
	EXPECT_FLOAT_EQ(aabb.center.z, _z);
}

TEST(AABB, defaultConstructor) 
{
	driderSDK::AABB* TestAABB;
	TestAABB = new driderSDK::AABB;
	EXPECT_FALSE(nullptr == TestAABB);
	delete TestAABB;
}

TEST(AABB, constructFromValues)
{
	driderSDK::AABB TestAABB(3.f, 2.f, 3.f, driderSDK::Vector3D(0.f, -1.f, -2.f));
	checkForAABBValues(TestAABB, 3.f, 2.f, 3.f, 0.f, -1.f, -2.f);
}

TEST(AABB, moveConstructor)
{
	driderSDK::AABB TestAABB(5.f, 10.f, 5.f, driderSDK::Vector3D(0.f, 1.f, 2.f));
	driderSDK::AABB OtherAABB(std::move(TestAABB));

	checkForAABBValues(OtherAABB, 5.f, 10.f, 5.f, 0.f, 1.f, 2.f);
}

TEST(AABB, constructFromAABB)
{
	driderSDK::AABB TestAABB(1.f, 2.5f, 1.f, driderSDK::Vector3D(10.f, -2.f, 1.f));
	driderSDK::AABB OtherAABB(TestAABB);

	checkForAABBValues(OtherAABB, 1.f, 2.5f, 1.f, 10.f, -2.f, 1.f);
}

TEST(AABB, destructor)
{
	driderSDK::AABB TestAABB(7.f, 14.f, 7.f, driderSDK::Vector3D(6.0f, 6.f, 6.f));
	TestAABB.~AABB();
	checkForAABBValues(TestAABB, 7.f, 14.f, 7.f, 6.f, 6.f, 6.f);
}

TEST(AABB, getMaxPoint)
{
	driderSDK::AABB TestAABB(1.f, 1.f, 1.f, driderSDK::Vector3D(0.f, 0.f, 0.f));
	driderSDK::Vector3D max = TestAABB.getMaxPoint();
	EXPECT_FLOAT_EQ(max.x, 0.5f);
	EXPECT_FLOAT_EQ(max.y, 0.5f);
	EXPECT_FLOAT_EQ(max.z, 0.5f);
}

TEST(AABB, getMinPoint)
{
	driderSDK::AABB TestAABB(1.f, 1.f, 1.f, driderSDK::Vector3D(0.f, 0.f, 0.f));
	driderSDK::Vector3D min = TestAABB.getMinPoint();
	EXPECT_FLOAT_EQ(min.x, -0.5f);
	EXPECT_FLOAT_EQ(min.y, -0.5f);
	EXPECT_FLOAT_EQ(min.z, -0.5f);
}

TEST(AABB, intersectAABB) {
	driderSDK::AABB TestAABB(1.f, 1.f, 1.f, driderSDK::Vector3D(0.f, 0.f, 0.f));
	driderSDK::AABB TrueAABB(1.f, 1.f, 1.f, driderSDK::Vector3D(0.1f, 0.1f, 0.1f));
	driderSDK::AABB FalseAABB(1.f, 1.f, 1.f, driderSDK::Vector3D(5.f, 5.f, 5.f));
	EXPECT_TRUE(TestAABB.intersect(TrueAABB));
	EXPECT_FALSE(TestAABB.intersect(FalseAABB));
}

TEST(AABB, intersectPoint) {
	driderSDK::AABB TestAABB(1.f, 1.f, 1.f, driderSDK::Vector3D(0.f, 0.f, 0.f));
	driderSDK::Vector3D TruePoint(0.f, 0.f, 0.1f);
	driderSDK::Vector3D FalsePoint(5.f, 5.f, 5.f);
	EXPECT_TRUE(TestAABB.intersect(TruePoint));
	EXPECT_FALSE(TestAABB.intersect(FalsePoint));
}

TEST(AABB, intersectPlane) {
	driderSDK::AABB TestAABB(0.5f, 0.5f, 0.5f, driderSDK::Vector3D(0.f, 0.f, 0.f));
	driderSDK::Plane TruePlane(driderSDK::Vector3D(1.f, 0.f, 1.f), 
														 driderSDK::Vector3D(1.f, 0.f, 0.f), 
														 driderSDK::Vector3D(0.f, 0.f, 0.f));

	driderSDK::Plane FalsePlane(driderSDK::Vector3D(1.f, 0.f, 1.f),
														 driderSDK::Vector3D(1.f, 0.f, 0.f),
														 driderSDK::Vector3D(0.f, 1.f, 0.f));
	EXPECT_TRUE(TestAABB.intersect(TruePlane));
	EXPECT_FALSE(TestAABB.intersect(FalsePlane));

}

TEST(AABB, intersectFrustrum) {
  driderSDK::Matrix4x4 mat;
  mat.Orthogonal(1280, 980, 0.1f, 800);
  driderSDK::Frustrum f(mat);

  driderSDK::AABB aabb(10, 10, 10, driderSDK::Vector3D(0, 0, 0));
  aabb.intersect(f);
  EXPECT_TRUE(aabb.intersect(f));

  driderSDK::AABB aabb2(10, 10, 10, driderSDK::Vector3D(0, 490, 0));
  EXPECT_TRUE(aabb2.intersect(f));

  driderSDK::AABB aabb3(10, 10, 10, driderSDK::Vector3D(0, 510, 0));
  EXPECT_FALSE(aabb3.intersect(f));
}

TEST(AABB, intersectSphere)
{
	driderSDK::Sphere sphere(driderSDK::Vector3D(0.f, 0.f, 0.f), 0.5f);
	driderSDK::Sphere FalseSphere(driderSDK::Vector3D(-10.f, -10.f, -10.f), 0.5f);
	driderSDK::AABB TrueAabb(0.6f, 1.f, 0.6f, driderSDK::Vector3D(0.f, 0.f, 0.f));
	driderSDK::AABB FalseAabb(1.f, 1.f, 1.f, driderSDK::Vector3D(0.f, 0.f, 0.f));
	EXPECT_TRUE(TrueAabb.intersect(sphere));
	EXPECT_FALSE(FalseAabb.intersect(FalseSphere));
}

TEST(AABB, intersectRay) {
	driderSDK::AABB TestAABB(1.f, 1.f, 1.f, driderSDK::Vector3D(0.f, 0.f, 0.f));
	driderSDK::Ray TrueRay(driderSDK::Vector3D(0.f, 0.f, -10.f), driderSDK::Vector3D(0.01f, 0.01f, 1.f));
	driderSDK::Ray FalseRay(driderSDK::Vector3D(0.f, 0.f, -10.f), driderSDK::Vector3D(0.1f, 1.f, 0.1f));
	EXPECT_TRUE(TestAABB.intersect(TrueRay));
	EXPECT_FALSE(TestAABB.intersect(FalseRay));
}

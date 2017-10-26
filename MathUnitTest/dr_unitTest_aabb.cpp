#include <dr_aabb.h>
#include <dr_ray.h>
#include <dr_plane.h>
#include <dr_sphere.h>
#include <gtest\gtest.h>

void checkForAABBValues(driderSDK::AABB& aabb, 
												float _width, 
												float _height, 
												float _x,
												float _y,
												float _z)
{
	EXPECT_FLOAT_EQ(aabb.width, _width);
	EXPECT_FLOAT_EQ(aabb.height, _height);
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
	driderSDK::AABB TestAABB(3.f, 2.f, driderSDK::Vector3D(0.f, -1.f, -2.f));
	checkForAABBValues(TestAABB, 3.f, 2.f, 0.f, -1.f, -2.f);
}

TEST(AABB, moveConstructor)
{
	driderSDK::AABB TestAABB(5.f, 10.f, driderSDK::Vector3D(0.f, 1.f, 2.f));
	driderSDK::AABB OtherAABB(std::move(TestAABB));

	checkForAABBValues(OtherAABB, 5.f, 10.f, 0.f, 1.f, 2.f);
}

TEST(AABB, constructFromAABB)
{
	driderSDK::AABB TestAABB(1.f, 2.5f, driderSDK::Vector3D(10.f, -2.f, 1.f));
	driderSDK::AABB OtherAABB(TestAABB);

	checkForAABBValues(OtherAABB, 1.f, 2.5f, 10.f, -2.f, 1.f);
}

TEST(AABB, destructor)
{
	driderSDK::AABB TestAABB(7.f, 14.f, driderSDK::Vector3D(6.0f, 6.f, 6.f));
	TestAABB.~AABB();
	checkForAABBValues(TestAABB, 7.f, 14.f, 6.f, 6.f, 6.f);
}

TEST(AABB, intersectAABB) {
	driderSDK::AABB TestAABB(1.f, 1.f, driderSDK::Vector3D(0.f, 0.f, 0.f));
	driderSDK::AABB TrueAABB(1.f, 1.f, driderSDK::Vector3D(0.1f, 0.1f, 0.1f));
	driderSDK::AABB FalseAABB(1.f, 1.f, driderSDK::Vector3D(5.f, 5.f, 5.f));
	EXPECT_TRUE(TestAABB.intersect(TrueAABB));
	EXPECT_FALSE(TestAABB.intersect(FalseAABB));
}

TEST(AABB, intersectPoint) {
	driderSDK::AABB TestAABB(1.f, 1.f, driderSDK::Vector3D(0.f, 0.f, 0.f));
	driderSDK::Vector3D TruePoint(0.f, 0.f, 0.1f);
	driderSDK::Vector3D FalsePoint(5.f, 5.f, 5.f);
	EXPECT_TRUE(TestAABB.intersect(TruePoint));
	EXPECT_FALSE(TestAABB.intersect(FalsePoint));
}

TEST(AABB, intersectPlane) {
	driderSDK::AABB TestAABB(0.5f, 0.5f, driderSDK::Vector3D(0.f, 0.f, 0.f));
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
	
}

TEST(AABB, DISABLED_intersectSphere)
{
	driderSDK::Sphere sphere;
	sphere.center = driderSDK::Vector3D(0.0f, 0.0f, 0.0f);
	sphere.radius = 0.5f;

	driderSDK::AABB Aabb;
	Aabb.center = driderSDK::Vector3D(0.5f, 0.0f, 0.0f);
	Aabb.height = 1.0f;
	Aabb.width = 0.6f;

	EXPECT_TRUE(sphere.intersects(Aabb));
}

TEST(AABB, intersectRay) {
	driderSDK::AABB TestAABB(1, 1, driderSDK::Vector3D(0.f, 0.f, 0.f));
	driderSDK::Ray TrueRay(driderSDK::Vector3D(0.f, 0.f, -10.f), driderSDK::Vector3D(0.01f, 0.01f, 1.f));
	driderSDK::Ray FalseRay(driderSDK::Vector3D(0.f, 0.f, -10.f), driderSDK::Vector3D(0.1f, 1.f, 0.1f));
	EXPECT_TRUE(TestAABB.intersect(TrueRay));
	EXPECT_FALSE(TestAABB.intersect(FalseRay));
}

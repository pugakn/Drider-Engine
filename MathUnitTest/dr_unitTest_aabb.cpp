#include <dr_aabb.h>
#include <dr_ray.h>
#include <dr_plane.h>
#include <gtest\gtest.h>

TEST(aabb, intersectAABB) {
	driderSDK::AABB TestAABB(1,1, driderSDK::Vector3D(0,0,0));
	driderSDK::AABB TrueAABB(1,1, driderSDK::Vector3D(0.1,0.1,0.1));
	driderSDK::AABB FalseAABB(1, 1, driderSDK::Vector3D(5, 5, 5));
	EXPECT_TRUE(TestAABB.intersect(TrueAABB));
	EXPECT_FALSE(TestAABB.intersect(FalseAABB));
}

TEST(aabb, intersectPoint) {
	driderSDK::AABB TestAABB(1, 1, driderSDK::Vector3D(0, 0, 0));
	driderSDK::Vector3D TruePoint(0, 0, 0.1);
	driderSDK::Vector3D FalsePoint(5, 5, 5);
	EXPECT_TRUE(TestAABB.intersect(TruePoint));
	EXPECT_FALSE(TestAABB.intersect(FalsePoint));
}

TEST(aabb, intersectPlane) {
	driderSDK::AABB TestAABB(0.5, 0.5, driderSDK::Vector3D(0, 0, 0));
	driderSDK::Plane TruePlane(driderSDK::Vector3D(1, 0, 1), 
														 driderSDK::Vector3D(1, 0, 0), 
														 driderSDK::Vector3D(0, 0, 0));

	driderSDK::Plane FalsePlane(driderSDK::Vector3D(1, 0, 1),
														 driderSDK::Vector3D(1, 0, 0),
														 driderSDK::Vector3D(0, 1, 0));
	EXPECT_TRUE(TestAABB.intersect(TruePlane));
	EXPECT_FALSE(TestAABB.intersect(FalsePlane));

}

TEST(aabb, intersectFrustrum) {
	
}

TEST(aabb, intersectRay) {
	driderSDK::AABB TestAABB(1, 1, driderSDK::Vector3D(0, 0, 0));
	driderSDK::Ray TrueRay(driderSDK::Vector3D(0, 0, -10), driderSDK::Vector3D(0.01, 0.01, 1));
	driderSDK::Ray FalseRay(driderSDK::Vector3D(0, 0, -10), driderSDK::Vector3D(0.1, 1, 0.1));
	EXPECT_TRUE(TestAABB.intersect(TrueRay));
	EXPECT_FALSE(TestAABB.intersect(FalseRay));
}

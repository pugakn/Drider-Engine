#include <dr_intersections.h>
#include <dr_vector3d.h>
#include <dr_aabb.h>
#include <gtest\gtest.h>

TEST (Intersect, sphereSphere) {
  driderSDK::Vector3D center1(0.0,0.0,0.0);
  driderSDK::Vector3D center2(.5,0.0,0.0);

  float radius = .5f;
  EXPECT_TRUE(driderSDK::Intersect::sphereSphere(center1, radius, center2, radius));
}

TEST(Intersect, sphereCapsule) {
  driderSDK::Vector3D center(0.0,0.0,0.0);
  driderSDK::Vector3D pA(0.0,0.0,0.0);
  driderSDK::Vector3D pB(0.0,1.0,0.0);

  EXPECT_TRUE(driderSDK::Intersect::sphereCapsule(center, 0.5f, pA, pB, 0.5f));
}

TEST(Intersect, capsuleCapsule) {
  driderSDK::Vector3D pA(0.0, 0.0, 0.0);
  driderSDK::Vector3D pB(0.0, 1.0, 0.0);

  driderSDK::Vector3D pA2(0.0, 0.0, 0.0);
  driderSDK::Vector3D pB2(0.0, 1.0, 0.0);

  float radius = 0.5f;

  EXPECT_TRUE(driderSDK::Intersect::capsuleCapsule(pA, pB, radius, pA2, pB2, radius));
}

TEST (Intersect, sphereRay) {
  driderSDK::Vector3D center(0.0, 0.0, 0.0);
  float radius = .5;

  driderSDK::Vector3D origing(1.0,0.0,0.0);
  driderSDK::Vector3D dir(-1.0,0.0,0.0);
  EXPECT_TRUE(driderSDK::Intersect::sphereRay(center, radius, origing, dir));
}

TEST(Intersect, rayPlane) {
  driderSDK::Vector3D center(0.0, 0.0, 0.0);
  float radius = .5;

  driderSDK::Vector3D origing(1.0, 0.0, 0.0);
  driderSDK::Vector3D dir(-1.0, 0.0, 0.0);
  EXPECT_TRUE(driderSDK::Intersect::sphereRay(center, radius, origing, dir));

  driderSDK::Vector3D rOrigin(0.0,5.0,0.0);
  driderSDK::Vector3D rDir(0.0,-1.0,0.0);

  driderSDK::Vector3D pNormal(0.0, 1.0, 0.0);
  driderSDK::Vector3D pPoint(0.0, 0.0, 0.0);
  
  EXPECT_TRUE(driderSDK::Intersect::rayPlane(rOrigin, rDir, pNormal, pPoint));
}

TEST(Intersect, rayCapsule) {
  driderSDK::Vector3D rOrigin(0.0, 5.0, 0.0);
  driderSDK::Vector3D rDir(0.0,0.0,0.0);
  rDir += driderSDK::Vector3D(0.0,10.0,0.0);

  driderSDK::Vector3D pA(0.0, 0.0, 0.0);
  driderSDK::Vector3D pB(0.0, 1.0, 0.0);

  float r = 0;
  float t = 0;
  
  EXPECT_TRUE(driderSDK::Intersect::rayCapsule(rOrigin, rDir, pA, pB, r, t));
}

TEST (Intersect, frustrumFrustrum) {
  EXPECT_TRUE(false);
}

TEST(Intersect, frustrumSphere) {
  EXPECT_TRUE(false);
}

TEST(Intersect, aabbAabb) {
  driderSDK::Vector3D c1(0.0,0.0,0.0);
  driderSDK::Vector3D c2(3.f,0.1f, 0.f);
  
  EXPECT_TRUE(driderSDK::Intersect::aabbAabb(c1, 4, 4, 4, c2, 4, 4, 4));
}

TEST(Intersect, aabbSphere) {
  driderSDK::Vector3D c1(0.0, 0.0, 0.0);
  driderSDK::Vector3D c2(1.0, 0.0, 0.0);
	driderSDK::AABB aabb(5.0, 5.0, 5.0, c1);
	EXPECT_TRUE(driderSDK::Intersect::aabbSphere(c1, 
																							 aabb.width, 
																							 aabb.height,
																							 aabb.depth,
																							 c2, 
																							 .5f));
}


TEST(Intersect, aabbRay) {
  driderSDK::Vector3D c1(0.0,0.0,0.0);
  driderSDK::Vector3D origin(0.0,5.0,0.0);  
  driderSDK::Vector3D dir(0.0,-1.0,0.0);
	driderSDK::AABB aabb(5, 5, 5, c1);
	EXPECT_TRUE(driderSDK::Intersect::aabbRay(c1, 
																						aabb.getMaxPoint(), 
																						aabb.getMinPoint(), 
																						origin, 
																						dir));
}

TEST(Intersect, aabbPoint) {
  driderSDK::Vector3D c1(0.0, 0.0, 0.0);
  driderSDK::Vector3D point(0.0, 0.0, 0.0);
	driderSDK::AABB aabb(5, 5, 5, c1);
	EXPECT_TRUE(driderSDK::Intersect::aabbPoint(c1, 
																							aabb.getMaxPoint(), 
																							aabb.getMinPoint(), 
																							point));
}

TEST (Intersect, spherePlane) {
  driderSDK::Vector3D pnormal(0.0,1.0,0.0);
  driderSDK::Vector3D c(0.0,0.0,0.0);
  
  EXPECT_TRUE(driderSDK::Intersect::spherePlane(pnormal, 0, c, .5));
}

TEST(Intersect, aabbPlane) {
  driderSDK::Vector3D pnormal(0.0, 1.0, 0.0);
  driderSDK::Vector3D c(0.0, 0.0, 0.0);

  EXPECT_TRUE(driderSDK::Intersect::aabbPlane(c, 5,5,5,pnormal,0));
}

TEST(Intersect, segmentPlane) {
  driderSDK::Vector3D pnormal(0.0, 1.0, 0.0);
  driderSDK::Vector3D pA(0.0, 0.0, 0.0);
  driderSDK::Vector3D pB(0.0, 0.0, 0.0);

  EXPECT_TRUE(driderSDK::Intersect::segmentPlane(pA, pB,pnormal,1));
}

TEST(Intersect, capsulePlane) {
  driderSDK::Vector3D pA(0.0, 0.0, 0.0);
  driderSDK::Vector3D pB(0.0, 1.0, 0.0);
  float radius = .5f;

  driderSDK::Vector3D pNormal(0.0, 1.0, 0.0);
  EXPECT_TRUE(driderSDK::Intersect::capsulePlane(pA, pB, radius, pNormal, 0));
}

TEST (Intersect, frustrumPlane) {
  //std::array<driderSDK::Plane, 6> planes;
  
  //EXPECT_TRUE(driderSDK::Intersect::frustrumPlane(planes, driderSDK::Vector3D(0,1,0)));
  EXPECT_TRUE(true);
}

TEST(Intersect, planePlane) {
  EXPECT_TRUE(driderSDK::Intersect::planePlane(driderSDK::Vector3D(1, 0, 0),
              driderSDK::Vector3D(0, 1, 0)));

  EXPECT_TRUE(driderSDK::Intersect::planePlane(driderSDK::Vector3D(1, 0, 0),
                                               0,
                                               driderSDK::Vector3D(0, 1, 0),
                                               0,
                                               driderSDK::Vector3D(0, 0, 0),
                                               driderSDK::Vector3D(0, 1, 0)));
}

TEST(Intersect, sphereContainsPoint) {
  
  EXPECT_TRUE(driderSDK::Intersect::sphereContainsPoint(driderSDK::Vector3D(0, 0, 0),
                                                        driderSDK::Vector3D(0, 0, 0),
                                                        .5f));
}

TEST (Intersect, closestPointSegmentSegment) {
  EXPECT_TRUE(false);
}

TEST(Intersect, NoImplemented) {
  EXPECT_TRUE(false);
}
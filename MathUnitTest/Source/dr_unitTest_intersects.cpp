#include <dr_intersections.h>
#include <dr_vector3d.h>

#include <gtest\gtest.h>

TEST (Intersects, sphereSphere) {
  driderSDK::Vector3D center1(0.0,0.0,0.0);
  driderSDK::Vector3D center2(.5,0.0,0.0);

  float radius = .5f;
  EXPECT_TRUE(driderSDK::Intersect::sphereSphere(center1, radius, center2, radius));
}

TEST(Intersects, sphereCapsule) {
  driderSDK::Vector3D center(0.0,0.0,0.0);
  driderSDK::Vector3D pA(0.0,0.0,0.0);
  driderSDK::Vector3D pB(0.0,1.0,0.0);

  EXPECT_TRUE(driderSDK::Intersect::sphereCapsule(center, 0.5f, pA, pB, 0.5f));
}

TEST(Intersects, capsuleCapsule) {
  driderSDK::Vector3D pA(0.0, 0.0, 0.0);
  driderSDK::Vector3D pB(0.0, 1.0, 0.0);

  driderSDK::Vector3D pA2(0.0, 0.0, 0.0);
  driderSDK::Vector3D pB2(0.0, 1.0, 0.0);

  float radius = 0.5f;

  EXPECT_TRUE(driderSDK::Intersect::capsuleCapsule(pA, pB, radius, pA2, pB2, radius));
}

TEST (Intersects, sphereRay) {
  driderSDK::Vector3D center(0.0, 0.0, 0.0);
  float radius = .5;

  driderSDK::Vector3D origing(1.0,0.0,0.0);
  driderSDK::Vector3D dir(-1.0,0.0,0.0);
  EXPECT_TRUE(driderSDK::Intersect::sphereRay(center, radius, origing, dir));
}

TEST(Intersects, rayPlane) {
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

TEST(Intersects, rayCapsule) {
  driderSDK::Vector3D rOrigin(0.0, 5.0, 0.0);
  driderSDK::Vector3D rDir(0.0,0.0,0.0);
  rDir += driderSDK::Vector3D(0.0,10.0,0.0);

  driderSDK::Vector3D pA(0.0, 0.0, 0.0);
  driderSDK::Vector3D pB(0.0, 1.0, 0.0);

  float r = 0;
  float t = 0;
  
  EXPECT_TRUE(driderSDK::Intersect::rayCapsule(rOrigin, rDir, pA, pB, r, t));
}

TEST (Intersects, frustrumFrustrum) {
  EXPECT_TRUE(false);
}

TEST(Intersects, frustrumSphere) {
  EXPECT_TRUE(false);
}

TEST(Intersects, aabbAabb) {
  driderSDK::Vector3D c1(0.0,0.0,0.0);
  driderSDK::Vector3D c2(3.0,0.1, 0.0);
  
  EXPECT_TRUE(driderSDK::Intersect::aabbAabb(c1, 4, 4, c2, 4, 4));
}

TEST(Intersects, aabbSphere) {
  driderSDK::Vector3D c1(0.0, 0.0, 0.0);
  driderSDK::Vector3D c2(1.0, 0.0, 0.0);

  EXPECT_TRUE(driderSDK::Intersect::aabbSphere(c1, 5.0,5.0, c2,.5f));
}


TEST(Intersects, aabbRay) {
  driderSDK::Vector3D c1(0.0,0.0,0.0);
  driderSDK::Vector3D origin(0.0,5.0,0.0);  
  driderSDK::Vector3D dir(0.0,-1.0,0.0);

  EXPECT_TRUE(driderSDK::Intersect::aabbRay(c1, 5,5,origin, dir));
}

TEST(Intersects, aabbPoint) {
  driderSDK::Vector3D c1(0.0, 0.0, 0.0);
  driderSDK::Vector3D point(0.0, 0.0, 0.0);
  EXPECT_TRUE(driderSDK::Intersect::aabbPoint(c1,5,5,point));
}

TEST (Intersects, spherePlane) {
  driderSDK::Vector3D pnormal(0.0,1.0,0.0);
  driderSDK::Vector3D c(0.0,0.0,0.0);
  
  EXPECT_TRUE(driderSDK::Intersect::spherePlane(pnormal, 0, c, .5));
}

TEST(Intersects, aabbPlane) {
  driderSDK::Vector3D pnormal(0.0, 1.0, 0.0);
  driderSDK::Vector3D c(0.0, 0.0, 0.0);

  EXPECT_TRUE(driderSDK::Intersect::aabbPlane(c, 5,5,pnormal,0));
}

TEST(Intersects, segmentPlane) {
  driderSDK::Vector3D pnormal(0.0, 1.0, 0.0);
  driderSDK::Vector3D pA(0.0, 0.0, 0.0);
  driderSDK::Vector3D pB(0.0, 0.0, 0.0);

  EXPECT_TRUE(driderSDK::Intersect::segmentPlane(pA, pB,pnormal,1));
}

TEST(Intersects, capsulePlane) {
  driderSDK::Vector3D pA(0.0, 0.0, 0.0);
  driderSDK::Vector3D pB(0.0, 1.0, 0.0);
  float radius = .5f;

  driderSDK::Vector3D pNormal(0.0, 1.0, 0.0);
  EXPECT_TRUE(driderSDK::Intersect::capsulePlane(pA, pB, radius, pNormal, 0));
}

TEST (Intersects, frustrumPlane) {
  std::array<driderSDK::Plane, 6> planes;
  
  EXPECT_TRUE(driderSDK::Intersect::frustrumPlane(planes, driderSDK::Vector3D(0,1,0)));
}

TEST(Intersects, frustrumPlane) {
  std::array<driderSDK::Plane, 6> planes;

  EXPECT_TRUE(driderSDK::Intersect::frustrumPlane(planes, driderSDK::Vector3D(0, 1, 0)));
}

TEST(Intersects, planePlane) {
  EXPECT_TRUE(driderSDK::Intersect::planePlane(driderSDK::Vector3D(1, 0, 0),
              driderSDK::Vector3D(0, 1, 0)));

  EXPECT_TRUE(driderSDK::Intersect::planePlane(driderSDK::Vector3D(1, 0, 0),
                                               0,
                                               driderSDK::Vector3D(0, 1, 0),
                                               0,
                                               driderSDK::Vector3D(0, 0, 0),
                                               driderSDK::Vector3D(0, 1, 0)));
}

TEST(Intersects, sphereContainsPoint) {
  
  EXPECT_TRUE(driderSDK::Intersect::sphereContainsPoint(driderSDK::Vector3D(0, 0, 0),
                                                        driderSDK::Vector3D(0, 0, 0),
                                                        .5f));
}

TEST (Intersects, closestPointSegmentSegment) {
  EXPECT_TRUE(true);
}



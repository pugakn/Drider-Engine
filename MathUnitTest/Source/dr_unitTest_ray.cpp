#include "dr_ray.h"
#include "dr_plane.h"
#include "dr_aabb.h"
#include <gtest\gtest.h>

TEST(Ray, defaultConstructor) {
  driderSDK::Ray r;
  r.origin = driderSDK::Vector3D(1.0,0.0,0.0);

  EXPECT_TRUE(r.origin == driderSDK::Vector3D(1.0, 0.0, 0.0));
}

TEST(Ray, origingDirectionCons) {
  driderSDK::Ray r(driderSDK::Vector3D(1.0, 0.0, 0.0), 
                   driderSDK::Vector3D(1.0, 0.0, 0.0));

  EXPECT_TRUE(r.origin == driderSDK::Vector3D(1.0, 0.0, 0.0));
  EXPECT_TRUE(r.direction == driderSDK::Vector3D(1.0, 0.0, 0.0));
}

TEST(Ray, defaultDestructor) {
  driderSDK::Ray r(driderSDK::Vector3D(1.0, 0.0, 0.0),
                   driderSDK::Vector3D(1.0, 0.0, 0.0));

  EXPECT_TRUE(true);
}

//intersectPlane
TEST(Ray, intersectPlane) {
  driderSDK::Plane p(driderSDK::Vector3D(0,-1,0), 0);
  driderSDK::Ray r(driderSDK::Vector3D(-2,2,0), driderSDK::Vector3D(-1,-1,0));
  EXPECT_TRUE(r.intersects(p));
}

TEST(Ray, intersectPlaneT) {
  driderSDK::Plane p(driderSDK::Vector3D(0, -1, 0), 0);
  driderSDK::Ray r(driderSDK::Vector3D(-2, 2, 0), driderSDK::Vector3D(1, -1, 0).normalize());
  float distance;
  EXPECT_TRUE(r.intersects(p, &distance));

  driderSDK::Vector3D pointIntersect(r.origin + r.direction * distance);
  EXPECT_TRUE(pointIntersect.equals(driderSDK::Vector3D(0, 0, 0), 0.000001f));
}

TEST(Ray, intersectAABB) {
  driderSDK::Vector3D point1(0, 0, 0);
  driderSDK::Ray rayo(driderSDK::Vector3D(4, 0, 0), driderSDK::Vector3D(-1, 0, 0));
  driderSDK::AABB temp(1, 1, 1, point1);
  driderSDK::Vector3D pointIntersect;

  EXPECT_TRUE(rayo.intersects(temp, &pointIntersect));
  EXPECT_FLOAT_EQ(0.5f, pointIntersect[0]);
  EXPECT_FLOAT_EQ(0, pointIntersect[1]);
  EXPECT_FLOAT_EQ(0, pointIntersect[2]);
  rayo.origin.y = 4;
  EXPECT_FALSE(rayo.intersects(temp, &pointIntersect));

  rayo.origin.z = 4;
  rayo.direction = driderSDK::Vector3D(-1, -1, -1);
  EXPECT_TRUE(rayo.intersects(temp, &pointIntersect));
  EXPECT_FLOAT_EQ(0.5f, pointIntersect[0]);
  EXPECT_FLOAT_EQ(0.5f, pointIntersect[1]);
  EXPECT_FLOAT_EQ(0.5f, pointIntersect[2]);
}

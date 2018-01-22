#include <dr_matrix4x4.h>
#include <dr_frustrum.h>
#include <dr_ray.h>
#include <dr_plane.h>
#include <dr_aabb.h>
#include <dr_sphere.h>
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
  driderSDK::Plane p(driderSDK::Vector3D(-1, 0,0), 0);
  driderSDK::Ray r(driderSDK::Vector3D(-2,2,0), driderSDK::Vector3D(1,-1,0).normalize());
  EXPECT_TRUE(r.intersects(p));
  r.direction= driderSDK::Vector3D(-1, 1, 0);
  EXPECT_FALSE(r.intersects(p));
  r.direction = driderSDK::Vector3D(0, 1, 1);
  EXPECT_FALSE(r.intersects(p));
}

TEST(Ray, intersectPlaneT) {
  driderSDK::Plane p(driderSDK::Vector3D(-1, 0, 0), 0);
  driderSDK::Ray r(driderSDK::Vector3D(-2, 2, 0), driderSDK::Vector3D(1, -1, 0).normalize());
  float distance;
  EXPECT_TRUE(r.intersects(p, &distance));

  driderSDK::Vector3D pointIntersect(r.origin + r.direction * distance);
  EXPECT_TRUE(pointIntersect.equals(driderSDK::Vector3D(0, 0, 0), 0.000001f));
}

TEST(Ray, intersectSphere) {
  driderSDK::Sphere sphere(driderSDK::Vector3D(0, 0, 0), 1);
  driderSDK::Ray ray(driderSDK::Vector3D(4, 0, 0), driderSDK::Vector3D(-1, 0, 0));
  driderSDK::Vector3D point;

  EXPECT_TRUE(ray.intersects(sphere, &point));
  ray.direction = driderSDK::Vector3D(1, 0, 0);
  EXPECT_FALSE(ray.intersects(sphere, &point));

  driderSDK::Sphere sphere2(driderSDK::Vector3D(1, 1, 1), 1);
  driderSDK::Ray ray2(driderSDK::Vector3D(0, -2, 1), driderSDK::Vector3D(0, 1, 0));
  EXPECT_TRUE(ray2.intersects(sphere2, &point));
  ray2.direction = driderSDK::Vector3D(0, -1, 0);
  EXPECT_FALSE(ray2.intersects(sphere2, &point));


  driderSDK::Sphere sphere3(driderSDK::Vector3D(0, 0, 0), 1);
  driderSDK::Ray ray3(driderSDK::Vector3D(2, .5f, -1), driderSDK::Vector3D(-2, 0, 1).normalize());
  EXPECT_TRUE(ray3.intersects(sphere3, &point));
  ray3.direction = driderSDK::Vector3D(2, 0, -1).normalize();
  EXPECT_FALSE(ray3.intersects(sphere3, &point));

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

TEST (Ray, intersectRay) {
  driderSDK::Ray rayo1(driderSDK::Vector3D(-1, 0, 0), driderSDK::Vector3D(1, 1, 0).normalize());
  driderSDK::Ray rayo2(driderSDK::Vector3D(1, 0, 0), driderSDK::Vector3D(0, 1, 0).normalize());
  driderSDK::Vector3D point;

  EXPECT_TRUE(rayo1.intersects(rayo2, &point));
  EXPECT_TRUE(rayo2.intersects(rayo1, &point));

  rayo1.origin = driderSDK::Vector3D(-10, 5, -2);
  rayo2.origin = driderSDK::Vector3D(-10, 5, 2);
  rayo1.direction = driderSDK::Vector3D(0.0f, 0.0f, 1.0f);
  rayo2.direction = driderSDK::Vector3D(0.0f, 0.0f, -1.0f);
  EXPECT_TRUE(rayo1.intersects(rayo2, &point));
  EXPECT_TRUE(rayo2.intersects(rayo1, &point));

  rayo1.origin = driderSDK::Vector3D(-10, 5, -2);
  rayo2.origin = driderSDK::Vector3D(-10, 5, 2);
  rayo1.direction = driderSDK::Vector3D(0.0f, 0.0f, 1.0f);
  rayo2.direction = driderSDK::Vector3D(0.0f, 0.0f, 1.0f);
  EXPECT_TRUE(rayo1.intersects(rayo2, &point));
  EXPECT_FALSE(rayo2.intersects(rayo1, &point));

  rayo1.origin = driderSDK::Vector3D(-10, 5, -2);
  rayo2.origin = driderSDK::Vector3D(-10, 6, 2);
  rayo1.direction = driderSDK::Vector3D(0.0f, 0.0f, 1.0f);
  rayo2.direction = driderSDK::Vector3D(0.0f, 0.0f, -1.0f);
  EXPECT_FALSE(rayo1.intersects(rayo2, &point));
  EXPECT_FALSE(rayo2.intersects(rayo1, &point));
}

TEST(Ray, frustrumRay) {
  driderSDK::Matrix4x4 mat;
  mat.Orthogonal(1280, 980, 0.1f, 800);
  driderSDK::Frustrum f(mat);

  driderSDK::Ray ray(driderSDK::Vector3D(-2000, 0, 0), driderSDK::Vector3D(1, 0, 0));

  ray.intersects(f);
}

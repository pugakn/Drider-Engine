#include <dr_plane.h>
#include <dr_ray.h>
#include <dr_sphere.h>
#include <dr_frustrum.h>
#include <dr_aabb.h>
#include <dr_capsule.h>
#include <dr_intersections.h>
#include <gtest\gtest.h>

using driderSDK::Vector3D;

TEST(Plane, constructorDefault) {
  driderSDK::Plane plane;
  EXPECT_TRUE(true);
}

TEST(Plane, destructor) {
  {
    driderSDK::Plane plane;
  }
  EXPECT_FALSE(false);
}

TEST(Plane, constructorNormalGap) {
 Vector3D normal(1.f, 0.f, 0.f);
  driderSDK::Plane plane(normal, 10.f);

  EXPECT_TRUE(normal == plane);
  EXPECT_FLOAT_EQ(plane.d, 10.f);
}

TEST(Plane, constructorNormalPoint) {
  Vector3D normal(-1.f, 0.f, 0.f);
  Vector3D point(10.f, 2.f, 3.f);
  driderSDK::Plane plane(normal, point);

  EXPECT_TRUE(normal == plane);
  EXPECT_FLOAT_EQ(plane.d, -10.f);
}

TEST(Plane, constructorThreePoints) {
  
  Vector3D pointA{10.f, 5.f, 2.f};
  Vector3D pointB{-10.f, 3.f, 4.f};
  Vector3D pointC{2.2f, 3.f, -3.f};

  driderSDK::Plane plane(pointA, 
                         pointB, 
                         pointC);
  
  Vector3D normal = (pointB - pointA).cross(pointC - pointA);
  normal.normalize();

  float d = normal.dot(pointB);

  EXPECT_FLOAT_EQ(plane.x, normal.x);
  EXPECT_FLOAT_EQ(plane.y, normal.y);
  EXPECT_FLOAT_EQ(plane.z, normal.z);
  EXPECT_FLOAT_EQ(plane.d, d);
}

TEST(Plane, constructorCopy) {
  driderSDK::Plane plane({1.f, 0.f, 0.f}, 10.f);
  driderSDK::Plane planeCopy(plane);

  EXPECT_FLOAT_EQ(plane.x, planeCopy.x);
  EXPECT_FLOAT_EQ(plane.y, planeCopy.y);
  EXPECT_FLOAT_EQ(plane.z, planeCopy.z);
  EXPECT_FLOAT_EQ(plane.d, planeCopy.d);
}

TEST(Plane, distanceToPoint) {
  driderSDK::Plane plane({0.f, -1.0f, 0.f}, 4);
  Vector3D point(10.f, 14.f, -20.f);

  float distance = plane.distanceToPoint(point);

  EXPECT_FLOAT_EQ(distance, 18.f);
}

TEST(Plane, intersectsPoint) {
  driderSDK::Plane plane({0.f, -1.f, 0.f}, 0.f);
  Vector3D pointA(10.f, 0.f, 20.f);
  Vector3D pointB(2.f, 4.f, 20.f);
  Vector3D pointC(5.f, -3.f, 9.f);

  EXPECT_TRUE(plane.intersects(pointA) == driderSDK::PLANE_INTERSECT::kIn);
  EXPECT_TRUE(plane.intersects(pointB) == driderSDK::PLANE_INTERSECT::kBehind);
  EXPECT_TRUE(plane.intersects(pointC) == driderSDK::PLANE_INTERSECT::kFront);
}

TEST(Plane, intersectsPlane)  {
  driderSDK::Plane planeA({0.f, 1.f, 0.f}, 10.f);
  driderSDK::Plane planeB({0.f, -1.f, 0.f}, 10.f);
  driderSDK::Plane planeC({-1.f, 0.f, 1.f}, 0.f);

  EXPECT_TRUE(planeA.intersects(planeC));
  EXPECT_FALSE(planeA.intersects(planeB));
}

TEST(Plane, intersectsSphere) {
  driderSDK::Plane plane({0.f, 1.f, 0.f}, 10.f);
  driderSDK::Sphere sphereA({20.f, 15.f, 0.f}, 10.f);
  driderSDK::Sphere sphereB({10.f, 0.f, 10.f}, 4.f);

  float dist = plane.distanceToPoint(sphereB.center);

  EXPECT_TRUE(plane.intersects(sphereA));
  EXPECT_FALSE(plane.intersects(sphereB));
}

TEST(Plane, intersectsAABB) {
  driderSDK::Plane plane({1.f, 0.f, 0.f}, 7.f);
  driderSDK::AABB aabbA;
  driderSDK::AABB aabbB;
  //falta añadir profundidad 
  EXPECT_FALSE(true);
}

TEST(Plane, intersectsCapsule) {
  driderSDK::Capsule capsule(Vector3D(0.f,0.f,0.f), 
                             Vector3D(0.f, 1.f, 0.f), 0.5f);

  driderSDK::Vector3D abc(0.f, 0.7071f, 0.7071f);
  abc.normalize();
  driderSDK::Plane plane(abc, 0.f);
  
  EXPECT_TRUE(capsule.intersects(plane));
  plane.d = 10; 
  EXPECT_FALSE(capsule.intersects(plane));
}

TEST(Plane, intersectsFrustrum) {
  driderSDK::Frustrum frustrum;

  //No se ha implementado la matriz de proyeccion
  EXPECT_FALSE(true);
}

TEST(Plane, intersectsRay) {
  driderSDK::Plane plane({-1, 0.f, 0.f}, 10);
  driderSDK::Ray rayA({-20, 0.f, 0.f}, {1.f, 0.f, 0.f});
  driderSDK::Ray rayB({-20, 0.f, 0.f}, {0.f, 1.f, 0.f});

  EXPECT_TRUE(plane.intersects(rayA));
  EXPECT_FALSE(plane.intersects(rayB));
}

TEST(Plane, normalize) {
  driderSDK::Plane plane({-10.f, -10.f, 0.f}, 10.f);
  plane.normalize();

  EXPECT_FLOAT_EQ(plane.x, -0.70710678f);
  EXPECT_FLOAT_EQ(plane.y, -0.70710678f);
  EXPECT_FLOAT_EQ(plane.z, 0.f);
  EXPECT_FLOAT_EQ(plane.d, 0.70710678f);
}

TEST(Plane, operatorCopyAssignment){
  driderSDK::Plane plane({-10.f,1.f, 0.f}, 3.f);
  driderSDK::Plane plane2;

  plane2 = plane;

  EXPECT_FLOAT_EQ(plane.x, plane2.x);
  EXPECT_FLOAT_EQ(plane.y, plane2.y);
  EXPECT_FLOAT_EQ(plane.z, plane2.z);
  EXPECT_FLOAT_EQ(plane.d, plane2.d);
}

TEST(Plane, operatorEquality) {
  driderSDK::Plane plane({-10.f,1.f, 0.f}, 3.f);
  driderSDK::Plane plane2({-10.f,1.f, 0.f}, 3.f);

  EXPECT_TRUE(plane == plane2);

  plane2.x += 20.f;

  EXPECT_FALSE(plane == plane2);
}

TEST(Plane, operatorInequality){

  driderSDK::Plane plane({-10.f,1.f, 0.f}, 3.f);
  driderSDK::Plane plane2({-10.f,1.f, 0.f}, 3.f);

  EXPECT_FALSE(plane != plane2);

  plane2.x += 20.f;

  EXPECT_TRUE(plane != plane2);
}
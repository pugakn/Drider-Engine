#pragma once

namespace driderSDK {

class Sphere;
class Capsule;
class Ray;
class Vector3D;

//Uncommented
bool PointInSphere(const Vector3D& vec, const Sphere& s1);
//Uncommented
bool CollSphereSphere(const Sphere& s1, const Sphere& s2);
//Uncommented
bool CollSphereCapsule(const Sphere& s1, const Capsule& c1);
//Uncommented
bool CollSphereRay(const Sphere& s1, const Ray& r1);
}
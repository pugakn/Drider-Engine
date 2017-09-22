#pragma once

namespace driderSDK {

class Sphere;
class Capsule;
class Ray;

bool CollSphereSphere(Sphere s1, Sphere s2);
bool CollSphereCapsule(Sphere s1, Capsule c1);
bool CollSphereRay(Sphere s1, Ray r1);
}
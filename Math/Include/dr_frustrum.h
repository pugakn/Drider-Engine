#pragma once
#include <array>
#include "dr_math_prerequisites.h"
#include <dr_plane.h>
#include <dr_matrix4x4.h>
#include <dr_vector3d.h>
#include "dr_intersections.h"


namespace driderSDK {

class Sphere;
class Capsule;
class Ray;
class AABB;

/**
* Frustrum class
*/

class DR_MATH_EXPORT Frustrum
{
 public:
	enum PLANES { 
		kLeft, 
		kRight,
		kTop,
		kBottom,
		kNear,
		kFar
	}; 
  
  /**
  * TEST::frustrumConstructor
  * Default constructor
  */
  Frustrum () {}

	/**
  * TEST::fromVP
	* Constructor using a View Projection matrix
	*
	* @param VP
	*	 The View Projection matrix.
	*
	*/
	Frustrum(const Matrix4x4& view, const Matrix4x4& projection);

	/**
  * TEST::fromVP
	* Create the frustrum planes with the View Projection matrix
	*
	* @param VP
	*	 The View Projection matrix.
	*
	*/
	void 
	createFromVP(Matrix4x4 view, Matrix4x4 projection);

	/**
  * TEST::intersectsRay
	* Check if the frustrum intersects other ray
	*
	*	@param bRay
	*	 The ray to check the intersecton.
	*
	* @return
	*	  True if the frustrum intersects with the other ray
	*
	*/
	bool
	intersects(const Ray& bRay) const;

	/*
	* Check if the frustrum intersects with a plane
	*
	*	@param plane
	*	 The plane to check the intersecton.
	*
	* @return
	*	  True if the frustrum intersects with the plane
	*
	*/
	/*bool
	intersects(const Plane& plane) const;*/

	/**
  * TEST::frustrumSphere
	* Check if the frustrum intersects with a sphere
	*
	*	@param sphere
	*	 The sphere to check the intersecton.
	*
	* @return
	*	  True if the frustrum intersects with the sphere
	*
	*/
	bool
	intersects(const Sphere& sphere) const;

	/*
	* Check if the frustrum intersects with a capsule
	*
	*	@param capsule
	*	 The capsule to check the intersecton.
	*
	* @return
	*	  True if the frustrum intersects with the capsule
	*
	*/
	/*bool
	intersects(const Capsule& capsule) const;*/

	/*
	* Check if the frustrum intersects with a frustrum
	*
	*	@param frustrum
	*	 The frustrum to check the intersecton.
	*
	* @return
	*	  True if the frustrum intersects with the frustrum
	*
	/*
	bool
	intersects(const Frustrum& frustrum) const;*/

  /**
  * TEST::AABBFrustrum
  * Check if the frustrum intersects with a AABB
  *
  *	@param AABB
  *	 The AABB to check the intersecton.
  *
  * @return
  *	  True if the frustrum intersects with the AABB
  *
  */
  FRUSTRUM_INTERSECT::E
  intersects(const AABB& frustrum) const;

  std::array<Plane,6> planes;

};


}
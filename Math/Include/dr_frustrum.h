#pragma once
#include <dr_prerequisites.h>
#include <dr_plane.h>
#include <dr_matrix4x4.h>


namespace driderSDK {
using namespace math;
class Sphere;
class Capsule;
class Ray;
class Vector3D;
class Frustrum
{
 public:
	/**
	* Constructor using a View Projection matrix
	*
	* @param VP
	*	 The View Projection matrix.
	*
	*/
	Frustrum(const Matrix4x4& VP);

	//Uncommented
	void 
	setVP(const Matrix4x4& VP);

	//Uncommented
	FORCEINLINE Matrix4x4 
	getVP() {
		return VP;
	}

	/**
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

	/**
	* Check if the frustrum intersects with a plane
	*
	*	@param plane
	*	 The plane to check the intersecton.
	*
	* @return
	*	  True if the frustrum intersects with the plane
	*
	*/
	bool
	intersects(const Plane& plane) const;

	/**
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

	/**
	* Check if the frustrum intersects with a capsule
	*
	*	@param capsule
	*	 The capsule to check the intersecton.
	*
	* @return
	*	  True if the frustrum intersects with the capsule
	*
	*/
	bool
	intersects(const Capsule& capsule) const;

	/**
	* Check if the frustrum intersects with a frustrum
	*
	*	@param frustrum
	*	 The frustrum to check the intersecton.
	*
	* @return
	*	  True if the frustrum intersects with the frustrum
	*
	*/
	bool
	intersects(const Frustrum& frustrum) const;


	/**
	* Check if the frustrum intersects contains a point
	*
	*	@param point
	*	 The point to check.
	*
	* @return
	*	  True if the frustrum contains the point
	*
	*/
	bool
	contains(const Vector3D& point) const;

	/**
	* Check if the frustrum intersects contains a plane
	*
	*	@param plane
	*	 The plane to check.
	*
	* @return
	*	  True if the frustrum contains the plane
	*
	*/
	bool
	contains(const Plane& plane) const;

	/**
	* Check if the frustrum intersects contains a sphere
	*
	*	@param sphere
	*	 The sphere to check.
	*
	* @return
	*	  True if the frustrum contains the sphere
	*
	*/
	bool
	contains(const Sphere& sphere) const;

	/**
	* Check if the frustrum intersects contains a capsule
	*
	*	@param capsule
	*	 The capsule to check.
	*
	* @return
	*	  True if the frustrum contains the capsule
	*
	*/
	bool
	contains(const Capsule& capsule) const;

	/**
	* Check if the frustrum intersects contains a frustrum
	*
	*	@param frustrum
	*	 The frustrum to check.
	*
	* @return
	*	  True if the frustrum contains the frustrum
	*
	*/
	bool
	contains(const Frustrum& frustrum) const;

 private:
	Plane m_planes[6];
	Matrix4x4 VP;
};


}
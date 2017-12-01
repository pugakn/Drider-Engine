#pragma once
#include <dr_matrix4x4.h>
#include <dr_vector3d.h>
#include "dr_core_prerequisites.h"


namespace driderSDK {

/**
*  Camera class.
*
* Sample usage:
*	X;
*/
class DR_CORE_EXPORT Camera
{
public:

	/**
	* Default constructor.
	*
	*/
	Camera();

	/**
	* Initialize the camera with the given values.
	*
	*/
	Camera(const Vector3D& pos,
				 const Vector3D& look,
				 float fov, 
				 float nearPlane, 
				 float farPlane);

	/**
	* Default destructor.
	*
	*/
	~Camera();

	/**
	* Updates the camera.
	*
	* @param delta
	*   Time value to update the camera.
	*/
	void 
	update(float delta);

	/**
	* 
	*
	*/
	void 
	move();

	/**
	*
	*
	*/
	void 
	pan();

	/**
	* Set the camera's target
	*
	* @param target
	*   X
	*/
	void 
	setTarget(const Vector3D& target);

	/**
	* Rotate the camera.
	*
	* @param rotation
	*   Quaternion to rotate the camera.
	*/
	void 
	rotate(const Quaternion& rotation);

	/**
	*
	*
	*/
	void 
	orbit();

private:

	Vector3D m_pos;
	Vector3D m_target;
	Vector3D m_up;
	Matrix4x4 m_vp;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;
};

}


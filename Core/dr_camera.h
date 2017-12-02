#pragma once
#include <dr_matrix4x4.h>
#include <dr_vector3d.h>
#include "dr_core_prerequisites.h"


namespace driderSDK {

/**
*  Camera class.
*
* Sample usage:
*	Camera cam(Vector3D(0,0,0), Vector3D(1,0,0), 90, 0.1, 1000);
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
	* @param pos
	*   Position to initialize the camera.
	*
	* @param target
	*   Point to look at.
	*
	* @param fov
	*   Vision angle in degrees.
	*
	* @param nearPlane
	*   Value of the closest plane of the proyection.
	*
	* @param farPlane
	*   Value of the farthest plane of the proyection.
	*/
	Camera(const Vector3D& pos,
				 const Vector3D& target,
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
	* Moves the camera.
	*
	* @param forward
	*   Value to move the camera forward or backward.
	*
	* @param strafe
	*   Value to strafe the camera left or right.
	*/
	void 
	move(float forward, float strafe);

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
	*   Point to look at.
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


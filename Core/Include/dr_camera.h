#pragma once
#include <dr_matrix4x4.h>
#include <dr_vector3d.h>
#include <string>
#include "dr_core_prerequisites.h"
#include "dr_gameObject.h"
#include "dr_viewport.h"


namespace driderSDK {

/**
*  Camera class.
*
* Sample usage:
*	Camera cam("myCamera",Vector3D(0,0,0), Vector3D(1,0,0), myViewport, 90, 0.1, 1000);
*/
class DR_CORE_EXPORT Camera : public GameObject
{
public:

	/**
	* TEST::defaultConstructor
	* Default constructor.
	*
	*/
	Camera();

	/**
	* TEST::constructor
	* Initialize the camera with the given values.
	*
	* @param name
	*   Name of the camera.
	*
	* @param pos
	*   Position to initialize the camera.
	*
	* @param target
	*   Point to look at.
	*
	* @param viewport
	*   Viewport of the camera.
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
	Camera(const TString& name,
				 const Viewport& viewport,
				 float fov, 
				 float nearPlane, 
				 float farPlane);

	/**
	* TEST::destructor
	* Default destructor.
	*
	*/
	~Camera();

	/**
	* TEST::update
	* Updates the camera.
	*
	* @param delta
	*   Time value to update the camera.
	*/
	virtual void 
	updateImpl() override;

  void
  virtual draw(){}

	/**
	* TEST::move
	* Moves the camera.
	*
	* @param forward
	*   Value to move the camera, forward or backward.
	*
	* @param strafe
	*   Value to strafe the camera, left or right.
	*/
	void 
	move(float forward, float strafe);

	/**
	* TEST::pan
	* Moves the camera and the target
	*
	* @param forward
	*   Value to move the camera and target, forward or backward.
	*
	* @param strafe
	*   Value to strafe the camera and target, left or right.
	*/
	void 
	pan(float forward, float strafe);

	/**
	* TEST::createProyection
	* Creates the camera's proyection matrix
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
	void
	createProyection(float fov,
									 float nearPlane,
									 float farPlane);

	/**
	* TEST::setTarget
	* Set the camera's target
	*
	* @param target
	*   Point to look at.
	*/
	void 
	setTarget(const Vector3D& target);

	/**
	* TEST::setViewport
	* Set the camera's viewport
	*
	* @param _viewport
	*   Viewport to set.
	*/
	void
	setViewport(const Viewport& viewport);

	/**
	* TEST::rotate
	* Rotate the camera.
	*
	* @param rotation
	*   Quaternion to rotate the camera.
	*/
	void 
	rotate(const Quaternion& rotation);

	/**
	* TEST::orbit
	* Rotates de camera around the target
	*
	* @param pitch
	*   Angle in radians to rotate the horizontal coordinate.
	*
	* @param yaw
	*   Angle in radians to rotate the vertical coordinate.
	*/
	void 
	orbit(float pitch, float yaw);

	/**
	* TEST::getVP
	* Gets the view proyection matrix
	*
	* @return
	*  The view proyection of the camera.
	*/
  const Matrix4x4&
  getVP() const;

 private:
	Vector3D m_target;
	Vector3D m_up;
	Vector3D m_look;
	Matrix4x4 m_vp;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;
	Viewport m_viewport;
};

}


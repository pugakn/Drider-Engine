#pragma once
#include <dr_matrix4x4.h>
#include <dr_vector3d.h>
#include <string>
#include "dr_core_prerequisites.h"
#include "dr_gameObject.h"
#include "dr_viewport.h"

#include <dr_export_script.h>
#include <..\..\Script\Include\dr_script_engine.h>


namespace driderSDK {

CONSTRUCT_DESTRUCT_DECL(Camera)

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
  Camera() {}
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
				 const Viewport& viewport);

	/**
	* TEST::destructor
	* Default destructor.
	*
	*/
	~Camera();
  
  virtual SharedGameObj
  createInstance();

  virtual void
  copyData(SharedGameObj);

	/**
	* TEST::update
	* Updates the camera.
	*
	* @param delta
	*   Time value to update the camera.
	*/
	virtual void 
	updateImpl();
  
	/**
	* TEST::move
	* Moves the camera.
	*
	* @param forward
	*   Value to move the camera, forward or backward.
	*
	* @param strafe
	*   Value to strafe the camera, left or right.
  *
  * @param lockY
  *  Tells if the movement will be locked on the Y axis when 
  *  strafing or forwarding (it can't move upwards or downwards).
	*/
	void 
	move(float forwardVelocity, 
       float strafeVelocity, 
       float upVelocity, 
       bool lockY = false);

  void
  move(const Vector3D& direction);

	/**
	* TEST::pan
	* Moves the camera and the target
	*
	* @param forward
	*   Value to move the camera and target, forward or backward.
	*
	* @param strafe
	*   Value to strafe the camera and target, left or right.
  * 
  * @param lockY
  *  Tells if the movement will be locked on the Y axis when 
  *  strafing or forwarding (it can't move upwards or downwards).
	*/
	void 
	pan(float forward, float strafe, float upVelocity, bool lockY = false);

  void 
  pan(const Vector3D& direction);

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

  void
  setUp(const Vector3D& up);

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

  void
  rotate(float yaw, float pitch);
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

  /**
  * Gets the view matrix.
  *
  * @return
  *   View matrix.
  */
  const Matrix4x4&
  getView() const;

  /**
  * Gets the projection matrix.
  *
  * @return
  *   Projection matrix.
  */
  const Matrix4x4&
  getProjection() const;

  float
  getFarPlane() const;

  float
  getNearPlane() const;

  float
  getFOV() const;

  BEGINING_REGISTER(Camera, sizeof(Camera), VALUE_FLAGS)
  
  REGISTER_CONSTRUC_DESTRUCT(Camera)
  
  result = REGISTER_FOO_1P(Camera, move, const Vector3D&, void, "void", in)

  /*REGISTER_FOO_0P(Camera, getFarPlane, float)
  REGISTER_FOO_0P(Camera, getNearPlane, float)
  REGISTER_FOO_0P(Camera, getFOV, float)*/

  END_REGISTER

 private:


	Vector3D m_target;
	Vector3D m_up;
	Matrix4x4 m_vp;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;
	Viewport m_viewport;
  float m_nearPlane;
  float m_farPlane;
  float m_fov;
};

}


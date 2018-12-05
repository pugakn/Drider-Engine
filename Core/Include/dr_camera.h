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
*	Camera cam(Vector3D(0,0,0), Vector3D(1,0,0), myViewport, 90, 0.1, 1000);
*/
class DR_CORE_EXPORT Camera
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
  * @param _position
	*   Position to initialize the camera.
	*
	* @param _target
	*   Point to look at.
	*/
	Camera(const Vector3D& _position, 
         const Vector3D& _target,
         const TString& _name = _T(""));

	/**
	* TEST::destructor
	* Default destructor.
	*
	*/
	~Camera();
  
  void
  setPosition(const Vector3D& position);

  Vector3D
  getPosition() const;
	  
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
	* Creates the camera's perspective proyection matrix
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
	* TEST::createProyection
	* Creates the camera's orthogonal proyection matrix
	*
	* @param width
	*   Width vision.
  *
  * @param height
  *   Height hision.
	*
	* @param nearPlane
	*   Value of the closest plane of the proyection.
	*
	* @param farPlane
	*   Value of the farthest plane of the proyection.
	*/
	void
	createProyection(float width,
                   float height,
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
	* TEST::getViewport
	* Returns the camera's viewport
	*
	* @return
	*   Copy of the camera viewport.
	*/
  Viewport
	getViewport();

	/**
	* TEST::setViewport
	* Set the camera's viewport
	*
	* @param viewport
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

  /**
  * Returns the camera far plane.
  *
  * @return
  *  The camera far plane.
  */
  float
  getFarPlane() const;

  /**
  * Sets the camera far plane.
  *
  * @param farPlane
  *  The new camera far plane.
  */
  void
  setFarPlane(float farPlane);

  /**
  * Returns the camera near plane.
  *
  * @return
  *  The camera near plane.
  */
  float
  getNearPlane() const;

  /**
  * Sets the camera near plane.
  *
  * @param nearPlane
  *  The new camera near plane.
  */
  void
  setNearPlane(float nearPlane);

  /**
  * Returns the camera fov.
  *
  * @return
  *  The camera fov plane.
  */
  float
  getFOV() const;

  /**
  * Sets the camera FOV.
  *
  * @param FOV
  *  The new camera FOV.
  */
  void
  setFOV(float fov);

  TString
  getName() const;

  UInt32
  getViewportWidth() const;

  UInt32
  getViewportHeight() const;

  Vector3D
  getDirection() const;
  
  Vector3D
  getLocalRight() const;

  Vector3D
  getLocalUp() const;

  //BEGINING_REGISTER(Camera)
  BEGINING_REGISTER(Camera, sizeof(Camera), VALUE_FLAGS)
  
  REGISTER_CONSTRUC_DESTRUCT(Camera)
  
  result = REGISTER_FOO_1P(Camera, move, const Vector3D&, void, "void", in)

  /*REGISTER_FOO_0P(Camera, getFarPlane, float)
  REGISTER_FOO_0P(Camera, getNearPlane, float)
  REGISTER_FOO_0P(Camera, getFOV, float)*/

  END_REGISTER

 private:
   
  void 
  invalidateView();

  TString m_name;
	mutable Matrix4x4 m_vp;
	mutable Matrix4x4 m_view;
  mutable bool m_outdateView;
  Vector3D m_position;
	Vector3D m_target;
	Vector3D m_up;
	Matrix4x4 m_projection;
	Viewport m_viewport;
  float m_nearPlane;
  float m_farPlane;
  float m_fov;
};

}
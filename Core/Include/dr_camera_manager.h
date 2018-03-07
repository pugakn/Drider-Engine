#pragma once
#include "dr_core_prerequisites.h"
#include <list>
#include <string>

namespace driderSDK {

class Camera;
class Vector3D;
struct Viewport;
/**
*  Manages all the cameras.
*
* Sample usage:
*	CameraManager manager;
*/
class DR_CORE_EXPORT CameraManager 
{
 public:

	/**
	* TEST::constructor
	* Default constructor.
	*
	*/
	CameraManager();

	/**
	* TEST::destructor
	* Default destructor.
	*
	*/
	~CameraManager();

	/**
	* TEST::createCamera
	* Creates a camera with it's given values and stores it.
	*
	* @param name
	*   Name of the camera to create.
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
	void 
	createCamera(const TString& cameraName,
							 const Vector3D& pos,
							 const Vector3D& target,
							 const Viewport& viewport,
							 float fov,
							 float nearPlane,
							 float farPlane);

	/**
	* TEST::deleteCamera
	* Delete a camera by searching it's name.
	*
	* @param cameraName
	*   The name of the camera to delete.
	*/
	void
	deleteCamera(const TString& cameraName);


	/**
	* TEST::setViewportToCamera
	* Set a camera's viewport
	*
	* @param cameraName
	*   The name of the camera to set.
	*
	* @param viewport
	*   The viewport to set to the camera.   
	*/
	void
	setViewportToCamera(const TString& cameraName, const Viewport& viewport);

	/**
	* TEST::getActiveCamera
	* Get the active camera and returns it.
	*
	* @return;
	*   The name of the camera to delete.
	*/
	Camera& 
	getActiveCamera();

	/**
	* TEST::setActiveCamera
	* Sets the Manager's active camera
	*
	* @param cameraName
	*   The name of the camera.
	*
	*/
	void
	setActiveCamera(const TString& cameraName);

 private:

	std::list<Camera*> m_cameras;
	Camera* m_activeCamera;
};

}

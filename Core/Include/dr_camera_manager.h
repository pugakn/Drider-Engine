#pragma once

#include <unordered_map>

#include <dr_memory.h>
#include <dr_module.h>

#include "dr_core_prerequisites.h"

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
class DR_CORE_EXPORT CameraManager : public Module<CameraManager>
{
 public:

   using SharedCamera = std::shared_ptr<Camera>;
   using CamerasMap = std::unordered_map<TString, SharedCamera>;

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
	static void 
	createCamera(const TString& cameraName,
							 const Vector3D& pos,
							 const Vector3D& target,
							 const Viewport& viewport,
							 float fov,
							 float nearPlane,
							 float farPlane);

  
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
	* @param width
	*   Ortoghonal width.
	*
	* @param height
	*   Ortoghonal height.
	*
	* @param nearPlane
	*   Value of the closest plane of the proyection.
	*
	* @param farPlane
	*   Value of the farthest plane of the proyection.
	*/
	static void 
	createCamera(const TString& cameraName,
							 const Vector3D& pos,
							 const Vector3D& target,
							 const Viewport& viewport,
               float width,
               float height,
							 float nearPlane,
							 float farPlane);

  static void
  setViewportToAll(const Viewport& viewport);

	/**
	* TEST::deleteCamera
	* Delete a camera by searching it's name.
	*
	* @param cameraName
	*   The name of the camera to delete.
	*/
	static void
	deleteCamera(const TString& cameraName);

  /**
  * Gets a camera from the cameras list.
  * 
  * @param cameraName
  *  Name of the camera to search.
  * 
  * @return
  *   The camera if it's found on the cameras list, nullptr otherwise.
  */
  static SharedCamera
  getCamera(const TString& cameraName);

	/**
	* TEST::getActiveCamera
	* Get the active camera and returns it.
	*
	* @return;
	*   The name of the camera to delete.
	*/
	static SharedCamera
	getActiveCamera();

	/**
	* TEST::setActiveCamera
	* Sets the Manager's active camera
	*
	* @param cameraName
	*   The name of the camera.
	*
	*/
	static void
	setActiveCamera(const TString& cameraName);

  static void
  setActiveCamera(SharedCamera camera);

 private:

   void
   onStartUp();
   
   SharedCamera m_activeCam;
   SharedCamera m_dummyCam;
   CamerasMap m_cameras;
};

}

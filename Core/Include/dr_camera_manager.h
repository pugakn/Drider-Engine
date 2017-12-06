#pragma once
#include "dr_core_prerequisites.h"

namespace driderSDK {

class Camera;

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
	* Default constructor.
	*
	*/
	CameraManager();

	/**
	* Default destructor.
	*
	*/
	~CameraManager();

	/**
	* 
	*
	*/
	void 
	createCamera();

	/**
	*
	*
	*/
	void
	createViewport();

	/**
	*
	*
	*/
	void
	deleteCamera();

	/**
	*
	*
	*/
	void
	deleteViewport();

	/**
	*
	*
	*/
	void
	setViewportToCamera();

	/**
	*
	*
	*/
	void 
	getActiveCamera();

private:
	
};

}

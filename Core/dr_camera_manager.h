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
	addCamera();

	/**
	*
	*
	*/
	void 
	getActiveCamera();

private:
	
};

}

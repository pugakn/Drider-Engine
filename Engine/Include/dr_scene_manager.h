#pragma once
#include "dr_core_prerequisites.h"

#include <dr_gameObject.h>

namespace driderSDK {

	class File; 
  class Scene;

	/**
	*  Class that manages scenes.
	*
	* Sample usage:
	*	SceneManager Mgr;
	*	Mgr.saveSceneInFile(L"MAP30");
	*/
	class DR_CORE_EXPORT SceneManager
	{
	public:
		using SharedGameObj = std::shared_ptr<GameObject>;

	/**
		* Default constructor
		*
		*/
		SceneManager();

		/**
		* Default destructor
		*
		*/
		~SceneManager();

		/**
		* Loads a scene from a file
		*
		*  @param fileName
		*   Name of the scene's file to save.
		*
		*  @return 
		*   True if success.
		*/
		bool 
		loadProjectFromFile(const TString& fileName);

		/**
		* Saves the current scene into a file
		*
		*  @param fileName
		*   Name of the scene's file to open.
		*
		*  @return
		*   True if success.
		*/
		bool
		saveProjectInFile(const TString& fileName);

	private:

  public:
    Int32 m_numScenes;
    Int32 m_defaultScene;
    std::vector<std::shared_ptr<Scene>> m_scenes;

	};

}
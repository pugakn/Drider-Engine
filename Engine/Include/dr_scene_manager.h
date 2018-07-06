#pragma once
#include "dr_core_prerequisites.h"

namespace driderSDK {

	/**
	*  Class that managaes scenes.
	*
	* Sample usage:
	*	Don't
	*/
	class DR_CORE_EXPORT SceneManager 
	{
	public:

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
		loadSceneFromFile(const TString& fileName);

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
		saveSceneInFile(const TString& fileName);

		void
		setActiveScene();


	private:

		void 
		openSceneFile();

	};

}
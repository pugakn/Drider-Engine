#pragma once
#include "dr_core_prerequisites.h"

#include <dr_gameObject.h>

namespace driderSDK {

	class File;

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

	private:

		//void
		//setActiveScene();

		void
		interpretInput(const TString& input);

		void 
		interpretOutput(TString& output);

		TString 
		objectToStr(SharedGameObj obj);

		void
		strToObject();

	};

}
#include "dr_scene_manager.h"

#include "dr_resource_manager.h"

#include <dr_graph.h>
#include <dr_file_system.h>
#include <dr_string_utils.h>

namespace driderSDK {

	SceneManager::SceneManager() {}

	SceneManager::~SceneManager() {}

	bool
	SceneManager::loadSceneFromFile(const TString& fileName) {



		ResourceManager::loadResource(fileName);

		auto fileRef = ResourceManager::getReference(fileName);





		return false;
	}

	bool
	SceneManager::saveSceneInFile(const TString& fileName) {

		if (!SceneGraph::isStarted() || !ResourceManager::isStarted()) {
			Logger::instancePtr()->addError(__FILE__,
																			__LINE__, 
																			L"[SceneManager] Error: Modules not active");
			return false;
		}

		TString data;
		auto pNode = SceneGraph::getRoot();

		while (true) {

		}

		FileSystem fileMgr;
		File scene;

		//
		String a = StringUtils::toString(data);

		scene.Write(data.size(), a.c_str());

		fileMgr.CreateAndOpen(fileName, scene);

		return true;
	}

	void SceneManager::openSceneFile() {

	}

}
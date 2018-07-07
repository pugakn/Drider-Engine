#include "dr_scene_manager.h"

#include "dr_resource_manager.h"

#include <dr_graph.h>
#include <dr_file_system.h>
#include <dr_string_utils.h>
#include <dr_scene_core.h>

namespace driderSDK {

	SceneManager::SceneManager() {}

	SceneManager::~SceneManager() {}

	bool
	SceneManager::loadSceneFromFile(const TString& fileName) {

		if (!SceneGraph::isStarted() || !ResourceManager::isStarted()) {
			Logger::instancePtr()->addError(__FILE__,
																			__LINE__,
																			L"[SceneManager] Modules not active");
			return false;
		}

		ResourceManager::loadResource(fileName);

		auto rSceneFile = ResourceManager::getReference(fileName);
		auto sceneFile = std::dynamic_pointer_cast<SceneCore>(rSceneFile);

		File scene;

		//get the file

		if (scene.Size()) {
			interpretInput(scene);
			return true;
		}
		Logger::instancePtr()->addError(__FILE__,
																		__LINE__,
																		L"[SceneManager] The scene is corrupt");
		return false;
	}

	bool
	SceneManager::saveSceneInFile(const TString& fileName) {

		if (!SceneGraph::isStarted()) {
			Logger::instancePtr()->addError(__FILE__,
																			__LINE__, 
																			L"[SceneManager] Scenegraph not active");
			return false;
		}

		TString data;
		FileSystem fileMgr;
		File scene;

		//open file

		if (true) {
			interpretOutput(data);
		}

		String OutputData = StringUtils::toString(data);
		scene.Write(data.size(), OutputData.c_str());

		if (scene.Size()) {
			fileMgr.CreateAndOpen(fileName, scene);
			return true;
		}
		Logger::instancePtr()->addError(__FILE__,
																		__LINE__,
																		L"[SceneManager] Couldn't save the scene");
		return false;
	}

	void 
	SceneManager::interpretInput(File& input) {

		ANSIChar* data;
		data = new ANSIChar[input.Size()];
		input.Seek(0);
		input.Read(input.Size(), data);

		String strData = StringUtils::toString(data);

		while (true) {

		}
	}

	void 
	SceneManager::interpretOutput(TString& output) {

		auto pRoot = SceneGraph::getRoot();

		while (true) {

			//iterate through scenegraph

			//objectToStr();
		}

	}

	String 
	SceneManager::objectToStr(void* obj) {
	
		return String();
	}

	void 
	SceneManager::strToObject() {

	}

}
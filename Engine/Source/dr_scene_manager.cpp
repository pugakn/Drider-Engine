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
		ResourceManager::loadResource(fileName + L".scn");

		auto rSceneFile = ResourceManager::getReference(fileName);
		auto sceneFile = std::dynamic_pointer_cast<SceneCore>(rSceneFile);

		if (sceneFile->getData().size()) {
			interpretInput(sceneFile->getData());
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

		if (SceneGraph::getRoot()->getChildren().size()) {
			interpretOutput(data);
		}

		String OutputData = StringUtils::toString(data);
		scene.Write(data.size(), OutputData.c_str());

		if (scene.Size()) {
			fileMgr.CreateAndOpen(fileName + L".scn", scene);
			return true;
		}
		Logger::instancePtr()->addError(__FILE__,
																		__LINE__,
																		L"[SceneManager] Couldn't save the scene");
		return false;
	}

	void 
	SceneManager::interpretInput(const TString& input) {
		TString name;

		while (false/*iterate thriugh each object*/) {
			auto obj = std::make_shared<GameObject>();
			obj->setName(name);
			for (/*each component*/;;) {
				//obj->addComponent();
			}

			SceneGraph::addObject(obj);
		}
	}

	void 
	SceneManager::interpretOutput(TString& output) {

		//Retrieve aditional info here

		//iterate and save through root's children
		for (auto &rootChildren : SceneGraph::getRoot()->getChildren()) {
			output += objectToStr(rootChildren);
		}

	}

	TString 
	SceneManager::objectToStr(SharedGameObj obj) {

		/*
		name {
		component: val;
		component: val;
		component: val;
		child:
			name2 {
			component: val;
			}
		}
		*/

		TString result;
		result += obj->getName();
		result += L" {\n";
	
    auto componentList = obj->getComponents<GameComponent>();

		for (auto &component : componentList) {
		  //result += component->
		}

		for (auto &it : obj->getChildren()) {
			result += L"child:" + objectToStr(it) + L"\n";
		}

		result += L"}\n";
		return result;
	}

	void 
	SceneManager::strToObject() {

	}

}
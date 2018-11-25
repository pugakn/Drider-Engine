#include "dr_scene_manager.h"

#include "dr_resource_manager.h"

#include <dr_graph.h>
#include <dr_file_system.h>
#include <dr_string_utils.h>
#include <dr_scene_core.h>

namespace driderSDK {

	SceneManager::SceneManager() {
    m_numScenes = 1;
    m_defaultScene = 0;
  }

	SceneManager::~SceneManager() {}

	bool
	SceneManager::loadProjectFromFile(const TString& fileName) {

		if (!SceneGraph::isStarted() || !ResourceManager::isStarted()) {
			 Logger::instancePtr()->addError(__FILE__,
																			__LINE__,
																			L"[SceneManager] Modules not active");
			return false;
		}
		ResourceManager::loadResource(fileName + L".scn");

		auto rSceneFile = ResourceManager::getReference(fileName);
		auto sceneFile = std::dynamic_pointer_cast<SceneCore>(rSceneFile);
    
    
    
		return false;
	}

	bool
	SceneManager::saveProjectInFile(const TString& fileName) {

		if (!SceneGraph::isStarted()) {
			Logger::instancePtr()->addError(__FILE__,
																			__LINE__, 
																			L"[SceneManager] Scenegraph not active");
			return false;
		}

		FileSystem fileMgr;
    File projectFile;
    
    if(!fileMgr.Exists(fileName + L".txt")) {
      fileMgr.CreateAndOpen(fileName + L".txt", projectFile);
    } else {
      Logger::instancePtr()->addError(__FILE__,
                                      __LINE__,
                                      L"[SceneManager] The file already exist");
    }

    projectFile.m_file << m_numScenes;
    projectFile.m_file << m_defaultScene; 
    
    projectFile.Close();
  
		return false;
	}

}
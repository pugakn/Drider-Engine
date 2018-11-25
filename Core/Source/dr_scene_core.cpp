#include "dr_scene_core.h"
#include "dr_scene_info.h"

#include "dr_file.h"

namespace driderSDK {

	SceneCore::SceneCore() {}

	SceneCore::~SceneCore() {}

	void 
	SceneCore::init(void* pData) {
		SceneInfo* info = static_cast<SceneInfo*>(pData);

    m_name = info->sceneName;
		
	}

	void 
	SceneCore::init(void*, void*) {

	}
}
#include "dr_scene_core.h"
#include "dr_scene_info.h"

namespace driderSDK {

	SceneCore::SceneCore() {}

	SceneCore::~SceneCore() {}

	void 
	SceneCore::init(void* pData) {
		SceneInfo* info = static_cast<SceneInfo*>(pData);
		info->sceneName.erase(info->sceneName.length() - 3,
											 info->sceneName.length());
		m_data = info->data;
	}

	void 
	SceneCore::init(void*, void*) {

	}

	const TString& 
	SceneCore::getData() {
		return m_data;
	}
}
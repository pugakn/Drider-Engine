#include "dr_codec_scene.h"

#include <dr_scene_info.h>

#include <dr_file_system.h>
#include <dr_file.h>
#include <dr_string_utils.h>
#include <dr_memory.h>

#include <dr_logger.h>

namespace driderSDK {

	CodecScene::CodecScene() {}

	CodecScene::~CodecScene() {}

	Codec::UniqueVoidPtr 
	CodecScene::decode(TString pathName) {

		SceneInfo* scene = new SceneInfo;
    FileSystem fileSystem;
    TString sceneName = pathName + L".txt";
    if(fileSystem.Exists(sceneName)) {
      scene->sceneName = sceneName;
    } else {
      Logger::instance().addError(__FILE__,
                                  __LINE__,
                                  L"[Codec Scene] Scene file not found!");
    }

		return UniqueVoidPtr(scene, &dr_void_deleter<SceneInfo>);

	}

	bool 
	CodecScene::encode(TString pathName) {
		return false;
	}

	bool 
	CodecScene::isCompatible(TString resourceName) {
		return resourceName == L"txt";
	}

	CompatibleType::E 
	CodecScene::getType() {
		return CompatibleType::SCENE;
	}


}


#include "dr_codec_scene.h"

#include <dr_scene_info.h>
#include <dr_file.h>
#include <dr_string_utils.h>
#include <dr_memory.h>

namespace driderSDK {

	CodecScene::CodecScene() {}

	CodecScene::~CodecScene() {}

	Codec::UniqueVoidPtr 
	CodecScene::decode(TString pathName) {

		SceneInfo* scene = new SceneInfo;
		File sceneFile;

		if (sceneFile.Open(pathName)) {
			SizeT fileLength = sceneFile.Size();
			scene->data = sceneFile.GetAsString(fileLength);
			scene->sceneName = pathName;
		}
		sceneFile.Close();
		return UniqueVoidPtr(scene, &dr_void_deleter<SceneInfo>);

	}

	bool 
	CodecScene::encode(TString pathName) {
		return false;
	}

	bool 
	CodecScene::isCompatible(TString resourceName) {
		return resourceName == L"scn";
	}

	CompatibleType::E 
	CodecScene::getType() {
		return CompatibleType::SCENE;
	}


}


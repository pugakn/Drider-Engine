#include "dr_codec_script.h"

#include <dr_file.h>
#include <dr_string_utils.h>
#include <dr_memory.h>
#include <dr_script_info.h>

namespace driderSDK {

	Codec::UniqueVoidPtr
	CodecScript::decode(TString pathName) {
		
		ScriptInfo *script = new ScriptInfo;
		File scriptFile;

    TString path = _T("Resources\\Scripts\\");
		if (scriptFile.Open(path + pathName)) {
			SizeT fileLength = scriptFile.Size();
			script->data = scriptFile.GetAsString(fileLength);
			script->name = pathName;
		}
		scriptFile.Close();

		return UniqueVoidPtr(script, &dr_void_deleter<ScriptInfo>);
	}

	bool
	CodecScript::encode(TString pathName) {
		return false;
	}

	bool
	CodecScript::isCompatible(TString resourceName) {
		return resourceName == _T("as");
	}

	CompatibleType::E
	CodecScript::getType() {
		return CompatibleType::E::SCRIPT;
	}

	
}
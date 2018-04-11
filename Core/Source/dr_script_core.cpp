#include "dr_script_core.h"
#include "dr_script_info.h"

namespace driderSDK {

  void
	ScriptCore::init(void* pData) {

		ScriptInfo* script = static_cast<ScriptInfo*>(pData);
    script->name.erase(script->name.length() - 3,
                       script->name.length());
		m_script = script->data;

	}

  void
	ScriptCore::init(void*, void*) {

	}

  const TString&
	ScriptCore::getScript() {
		return m_script;
	}
}
#include "dr_script_core.h"
#include "dr_script_info.h"

namespace driderSDK {

  void
	ScriptCore::init(void* pData) {

		ScriptInfo* script = static_cast<ScriptInfo*>(pData);
		m_name = script->name;
		m_script = script->data;

	}

  void
	ScriptCore::init(void*, void*) {

	}

  const TString&
	ScriptCore::getScript() {
		return m_script;
	}

  const TString&
  ScriptCore::getName() {
		return m_name;
 }

}
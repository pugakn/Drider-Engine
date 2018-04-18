#include "dr_script_debug.h"
#include "dr_script_engine.h"
#include "angelscript\angelscript.h"

namespace driderSDK
{

	ScriptDebug::ScriptDebug() {
		//This class can only be instantiated by a ScriptEngine, we assume a SE is already up.
	//	DR_ASSERT(!ScriptEngine::isStarted()); //Just in case
	//	pScriptEngine = ScriptEngine::instancePtr();
	}

	ScriptDebug::~ScriptDebug() {}

	void ScriptDebug::setBreakPoint() {

	}

	bool ScriptDebug::checkBreakPoint() {
		return true;
	}

	void ScriptDebug::printCallStack() {

	}

	void ScriptDebug::printLocalVariables() {

	}

	void ScriptDebug::printGlobalVariables() {

	}

	void ScriptDebug::printValue() {

	}

	void ScriptDebug::printToLogger(TString& log) {
		TString Signature = _T("(Debug)"); //custom signature to identify debug logs.
		//pScriptEngine->addScriptLog(Signature + log, asMSGTYPE_INFORMATION);
	}

	void ScriptDebug::debugLineCallback(asIScriptContext* scriptContext) {

	}

}
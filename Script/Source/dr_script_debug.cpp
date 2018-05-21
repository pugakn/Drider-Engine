#include "dr_script_debug.h"
#include "dr_script_engine.h"
#include "angelscript\angelscript.h"

namespace driderSDK
{

	ScriptDebug::ScriptDebug(ScriptEngine* engine) {
		DR_ASSERT(ScriptEngine::isStarted()); //Just in case
		pScriptEngine = engine;
	}

	ScriptDebug::~ScriptDebug() {}

	void 
	ScriptDebug::setBreakPoint() {

		Int32 line = pScriptEngine->m_scriptContext->GetLineNumber();
		TString section; //need to get section

		if (!checkBreakPoint()) {
			m_breakpoints.push_back(BreakPoint(section, line, true));
		}

	}

	void 
	ScriptDebug::sendCommand(DebugCommands::E command) {
		if (command == DebugCommands::CONTINUE) {
			bIsStopped = false;
		}
		//rest of commands
	}

	bool 
	ScriptDebug::checkBreakPoint() {
		asIScriptContext* context = pScriptEngine->m_scriptContext;

		TString section;//need to get section 
		Int32 line = pScriptEngine->m_scriptContext->GetLineNumber(); 
		asIScriptFunction* function = pScriptEngine->m_scriptContext->GetFunction();
		for (UInt16 i = 0; i < m_breakpoints.size(); ++i) {
			if (m_breakpoints[i].section == section) {
				if (m_breakpoints[i].line == line) {
					return true;
				}
			}
		}
		return false;
	}

	void 
	ScriptDebug::printCallStack() {

	}

	void 
	ScriptDebug::printLocalVariables() {

		asIScriptContext* context = pScriptEngine->m_scriptContext;
		if (!context) {
			printToLogger(_T("Couldn't find script context"));
			return;
		}

		asIScriptFunction* function = pScriptEngine->m_scriptContext->GetFunction();
		if (!function) {
			printToLogger(_T("No function selected"));
			return;
		}

		for (UInt16 i = 0; i < function->GetVarCount(); ++i) {
			TOStringstream result;
			result << function->GetVarDecl(i);
			result << TString(_T(" : ")).c_str();
			result << context->GetAddressOfVar(i);
			result << TString(_T("/n")).c_str();
		}

	}

	void 
	ScriptDebug::printGlobalVariables() {

	}

	void 
	ScriptDebug::printValue() {

	}

	void 
	ScriptDebug::printToLogger(TString log) {
		TString Signature = _T("(Debug) "); //custom signature to identify debug logs.
		pScriptEngine->addScriptLog(Signature + log, asMSGTYPE_INFORMATION);
	}



}
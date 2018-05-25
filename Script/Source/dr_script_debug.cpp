#include "dr_script_debug.h"
#include "dr_script_engine.h"
#include "angelscript\angelscript.h"

namespace driderSDK
{

	ScriptDebug::ScriptDebug(ScriptEngine* engine) {
		pScriptEngine = engine;
		lastStackLevel = 0;
	}

	ScriptDebug::~ScriptDebug() {}

	void 
	ScriptDebug::setBreakPoint() {
		const char* pSection;
		Int32 line = pScriptEngine->m_scriptContext->GetLineNumber(0, 0, &pSection);
		TString section(StringUtils::toTString(pSection));

		//if (!checkBreakPoint()) {
			m_breakpoints.push_back(BreakPoint(section, line));
		//}
	}

	bool 
	ScriptDebug::removeBreakPoint(Int32 line, TString& section)
	{
		for (std::vector<BreakPoint>::iterator it = m_breakpoints.begin(); it != m_breakpoints.end(); ++it) {
			if (it->line == line && it->section == section) {
				m_breakpoints.erase(it);
				return true;
			}
		}
		return false;
	}

	void 
	ScriptDebug::interpretInput(TString& input) {

		switch (input[0]) {

			case 'p': //print info


				if (input[1] == 'c') { // callstack
					printCallStack();
				}
				else if (input[1] == 'l') { //local variables
					printLocalVariables();
				}
				else if (input[1] == 'g') { //global variables
					printGlobalVariables();
				}
				else if (input[1] == 't') { //garbage collector
					printGarbageStatus();
				}

				break;

			case 'c': //give command

				DebugCommands::E comm;

				if (input[1] == 'c') { // continue
					comm = DebugCommands::CONTINUE;
				}
				else if (input[1] == 'o') { //step out
					comm = DebugCommands::STEP_OUT;
				}
				else if (input[1] == 'i') { //step into
					comm = DebugCommands::STEP_IN;
				}
				else if (input[1] == 'v') { //step over
					comm = DebugCommands::STEP_OVER;
				}
				else {
					break;
				}

				setCommand(comm);

				break;
		}

	}

	void 
	ScriptDebug::setCommand(DebugCommands::E command) {
		m_command = command;
		if (m_command == DebugCommands::STEP_OVER || m_command == DebugCommands::STEP_IN) {
			asIScriptContext* context = pScriptEngine->m_scriptContext;
			if (context) {
				lastStackLevel = context->GetCallstackSize();
			}
		}
	}

	DebugCommands::E 
	ScriptDebug::getCommand() {
		return m_command;
	}

	bool 
	ScriptDebug::checkBreakPoint() {
		const char* pSection;
		Int32 line = pScriptEngine->m_scriptContext->GetLineNumber(0, 0, &pSection);
		TString section(StringUtils::toTString(pSection));

		for (UInt16 i = 0; i < m_breakpoints.size(); ++i) {
			if (m_breakpoints[i].section == section && m_breakpoints[i].line == line) {
				printToLogger(L"Breakpoint Reached");
				return true;
			}
		}
		return false;
	}

	void 
	ScriptDebug::clearBreakPoints() {
		m_breakpoints.clear();
	}

	void 
	ScriptDebug::printCallStack() {

		asIScriptContext* context = pScriptEngine->m_scriptContext;
		if (!context) {
			printToLogger(_T("No context selected"));
			return;
		}
		TOStringstream result;
		UInt32 line = 0;
		const char* section;

		for (UInt16 i = 0; i < context->GetCallstackSize(); ++i) {
			line = context->GetLineNumber(i, 0, &section);
			result << section << "(" << line << ") : ";
			result << context->GetFunction(i)->GetDeclaration() << "/n";
		}
		printToLogger(result.str());
	}

	void 
	ScriptDebug::printLocalVariables() {

		asIScriptContext* context = pScriptEngine->m_scriptContext;
		if (!context) {
			printToLogger(_T("Couldn't find script context"));
			return;
		}

		asIScriptFunction* function = context->GetFunction();
		if (!function) {
			printToLogger(_T("No function selected"));
			return;
		}

		TOStringstream result;
		for (UInt16 i = 0; i < function->GetVarCount(); ++i) {
			result << function->GetVarDecl(i);
			result << TString(_T(" : ")).c_str();
			result << interpretValue(context->GetAddressOfVar(i), context->GetVarTypeId(i));
			result << TString(_T("/n")).c_str();
		}
		printToLogger(result.str());
	}

	void 
	ScriptDebug::printGlobalVariables() {

		asIScriptContext* context = pScriptEngine->m_scriptContext;
		if (!context) {
			printToLogger(_T("No context selected"));
			return;
		}
		asIScriptFunction* function = context->GetFunction();
		if (!function) {
			printToLogger(_T("No function selected"));
			return;
		}
		asIScriptModule* mod = function->GetModule();
		if (!mod) {
			printToLogger(_T("No module selected"));
			return;
		}

		TOStringstream result;
		Int32 typeId;

		for (UInt16 i = 0; i < mod->GetGlobalVarCount(); ++i) {
			typeId = 0;
			mod->GetGlobalVar(i, 0, 0, &typeId);
			result << mod->GetGlobalVarDeclaration(i) << " : ";
			result << interpretValue(mod->GetAddressOfGlobalVar(i), typeId) << "/n";
		}
		printToLogger(result.str());
	}

	void 
	ScriptDebug::printToLogger(TString log) {
		TString Signature = _T("(Debug) "); //custom signature to identify debug logs.
		pScriptEngine->addScriptLog(Signature + log, asMSGTYPE_INFORMATION);
	}

	TString 
	ScriptDebug::interpretValue(void * value, Int32 typeId)
	{
		TOStringstream result;

		if (typeId == asTYPEID_VOID) {
			return L"void";
		}
		else if (typeId == asTYPEID_BOOL) {
			return *(bool*)value ? L"true" : L"false";
		}
		else if (typeId == asTYPEID_INT8) {
			result << *(Int8*)value;
		}
		else if (typeId == asTYPEID_INT16) {
			result << *(Int16*)value;
		}
		else if (typeId == asTYPEID_INT32) {
			result << *(Int32*)value;
		}
		else if (typeId == asTYPEID_INT64) {
			result << *(Int64*)value;
		}
		else if (typeId == asTYPEID_UINT8) {
			result << *(UInt8*)value;
		}
		else if (typeId == asTYPEID_UINT16) {
			result << *(UInt16*)value;
		}
		else if (typeId == asTYPEID_UINT32) {
			result << *(UInt32*)value;
		}
		else if (typeId == asTYPEID_UINT64) {
			result << *(UInt64*)value;
		}
		else if (typeId == asTYPEID_FLOAT) {
			result << *(float*)value;
		}
		else if (typeId == asTYPEID_DOUBLE) {
			result << *(double*)value;
		}
		else if (typeId & asTYPEID_MASK_OBJECT) {
			result << *(Int32*)value;
			asITypeInfo *typeInfo = pScriptEngine->m_scriptEngine->GetTypeInfoById(typeId);
			Int32 enumValue;
			for (Int32 i = typeInfo->GetEnumValueCount(); --i > 0; ) {
				enumValue = 0;
				const char *enumName = typeInfo->GetEnumValueByIndex(i, &enumValue);
				if (enumValue == *(Int32*)value) {
					result << " (" << enumName << ")";
					break;
				}
			}
		}
		else if (typeId & asTYPEID_SCRIPTOBJECT) {
			result << "(Object)";
			//expand into more details
		}
		else {
			result << "(Other type of value)";
			//expand into more details
		}
		return result.str();
	}

	void 
	ScriptDebug::printGarbageStatus() {

		asIScriptContext* context = pScriptEngine->m_scriptContext;
		if (!context) {
			printToLogger(_T("No context selected"));
			return;
		}

		asIScriptEngine* pEngine = pScriptEngine->m_scriptEngine;
		UInt32 gcSize, gcTotalDestroyed, gcTotalDetected, gcNewObjects, gcTotalNewDestroyed;
		pEngine->GetGCStatistics(&gcSize, 
														 &gcTotalDestroyed, 
														 &gcTotalDetected, 
														 &gcNewObjects, 
														 &gcTotalNewDestroyed);
		TOStringstream result;
		result << "Garbage Collector Status: \n";
		result << "Current size = " << gcSize << "\n";
		result << "Total destroyed = " << gcTotalDestroyed << "\n";
		result << "Total detected = " << gcTotalDetected << "\n";
		result << "New objects = " << gcNewObjects << "\n";
		result << "Total new objects destroyed = " << gcTotalNewDestroyed << "\n";

		printToLogger(result.str());

	}

}
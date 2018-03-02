#include <dr_file.h>
#include <dr_string_utils.h>
#include "dr_script_engine.h"
#include "dr_script_string_factory.h"
#include "scriptstdstring.h"


namespace driderSDK {


void stringPrint_g(asIScriptGeneric* gen) {
	WString *str = (WString*)gen->GetArgAddress(0);
	std::wcout << StringUtils::toTString(*str);
}

ScriptEngine::ScriptEngine() {

	if (!m_scriptLogger.isStarted()) {
		m_scriptLogger.startUp();
	}

}

Int8
ScriptEngine::createEngine() {
	Int8 result;
	m_scriptEngine = asCreateScriptEngine();  
	if(m_scriptEngine == 0) {
		addScriptLog(_T("Failed to create script engine"), asMSGTYPE_ERROR);
		return -1;
	}

	m_scriptEngine->SetEngineProperty(asEP_STRING_ENCODING, 1);
	m_scriptEngine->SetMessageCallback(asMETHOD(ScriptEngine, messageCallback), 
																		 0, 
																		 asCALL_CDECL);


	//Register custom string type
	/*result = m_scriptEngine->RegisterObjectType("TString", 
																							sizeof(TString), 
																							asOBJ_VALUE | asOBJ_POD);
	DR_ASSERT(result >= 0);*/
	//result = m_scriptEngine->RegisterStringFactory("TString", &stringFactory);
	RegisterStdString(m_scriptEngine);
	//

	// Register the functions that the scripts will be allowed to use.
	result = m_scriptEngine->RegisterGlobalFunction("void Print(string &in)",
																									asFUNCTION(stringPrint_g), 
																									asCALL_GENERIC);

  /*
  * Seccion para registrar metodos //eso dice arriba pero en ingles...(asi es comente un comentario)
  */

	return result;
}

Int8
ScriptEngine::addScript(const TString& fileName) {
	File scriptFile;
	if(!scriptFile.Open(fileName)) {
		addScriptLog(_T("Script file doesn't exist : ") + fileName, asMSGTYPE_ERROR);
		m_scriptEngine->Release();
		return -1;
	}
  
	SizeT fileLength = scriptFile.Size();

	TString script;
	script = scriptFile.GetAsString(fileLength);
	scriptFile.Close();

	if(script.size() == 0) {
		addScriptLog(_T("Script file : ") + fileName + _T(" couldn't be loaded."), asMSGTYPE_ERROR);
		m_scriptEngine->Release();
		return -2;
	}
	m_scriptModule = m_scriptEngine->GetModule("module", asGM_CREATE_IF_NOT_EXISTS);

	Int8 result = m_scriptModule->AddScriptSection(StringUtils::toString(fileName).c_str(),
																								&StringUtils::toString(script)[0],
																								fileLength);
	if(result < 0) {
		addScriptLog(_T("AddScriptSection failed on file ") + fileName, asMSGTYPE_ERROR);
		m_scriptEngine->Release();
		return -3;
	}
	return 0;
}

Int8
ScriptEngine::compileScript() {
	Int8 result = m_scriptModule->Build();
	if(result < 0) {
		addScriptLog(_T("Failed to compile script."), asMSGTYPE_ERROR);
		m_scriptEngine->Release();
		return -1;
	}
	return 0;
}

Int8
ScriptEngine::configureContext() {
	m_scriptContext = m_scriptEngine->CreateContext();
	if(m_scriptContext == 0) {
		addScriptLog(_T("Failed to create script context."), asMSGTYPE_ERROR);
		m_scriptEngine->Release();
		return -1;
	}

	/*Int8 result = m_scriptContext->SetLineCallback(asMETHOD(ScriptEngine, lineCallback),
																								 &timeout,
																								 asCALL_THISCALL);
	if(result < 0) {
		addScriptLog(_T("Failed to set the line callback."), asMSGTYPE_ERROR);
		m_scriptContext->Release();
		m_scriptEngine->Release();
		return -2;
	}*/
	return 0;
}

Int8
ScriptEngine::prepareFunction(TString function) {

	auto modul = m_scriptEngine->GetModule("module");
  m_scriptFunction = modul->GetFunctionByName(StringUtils::toString(function).c_str());
	if (m_scriptFunction == 0)
	{
		addScriptLog(_T("Function : ") + function + _T(" not found."), asMSGTYPE_ERROR);
		m_scriptContext->Release();
		m_scriptEngine->Release();
		return -1;
	}
	Int8 result = m_scriptContext->Prepare(m_scriptFunction);

	if (result < 0)
	{
		addScriptLog(_T("Failed to prepare script context."), asMSGTYPE_ERROR);
		m_scriptContext->Release();
		m_scriptEngine->Release();
		return -2;
	}

	return result;
}

Int8
ScriptEngine::executeCall() {
	Int8 result = m_scriptContext->Execute();
	if (result != asEXECUTION_FINISHED) {
		if (result == asEXECUTION_EXCEPTION) {
			addScriptLog(_T("There was an exception with a script call."), asMSGTYPE_ERROR);
		}
	}
	return result;
}

void
ScriptEngine::release() {
	m_scriptContext->Release();
	m_scriptEngine->ShutDownAndRelease();
}

void
ScriptEngine::lineCallback(asIScriptContext *scriptContext, unsigned long *timeOut) {
	if(timeout != /*functionToObtainTime()*/ 0) {
		m_scriptContext->Abort(); //we can also use suspend
	}
}

void 
ScriptEngine::messageCallback(const asSMessageInfo* scriptMessage, void* param) {

	Int8 row = scriptMessage->row, col = scriptMessage->col;

	//Expect something like "mySection (5, 1) : Example message here"
	TString message = StringUtils::toTString(scriptMessage->section) + 
																					 //_T(" (") + StringUtils::toTString(std::to_string(row)) +
																					 //_T(", ") + StringUtils::toTString(std::to_string(col)) +
																					 //_T(") : ") + 
																					 StringUtils::toTString(scriptMessage->message);

	addScriptLog(message, scriptMessage->type);
}

void
ScriptEngine::addScriptLog(const TString& log, int type) {

	driderSDK::Logger ModuleLogger; //testing logger, this will be removed
	if (!ModuleLogger.isStarted()) {
		ModuleLogger.startUp();
	}
	driderSDK::Logger& htmlLogger = ModuleLogger.instance();
	htmlLogger.addWarning(__FILE__, __LINE__, log);

	//if (type == asMSGTYPE_WARNING) {
	//	m_scriptLogger.addWarning(__FILE__, 
	//														__LINE__, 
	//														"[ScriptEngine] " + StringUtils::toString(log));
	//}
	//else if (type == asMSGTYPE_INFORMATION) {
	//	m_scriptLogger.addWarning(__FILE__,
	//														__LINE__, 
	//														"[ScriptEngine] (INFO) " + StringUtils::toString(log));
	//}
	//else if (type == asMSGTYPE_ERROR) {
	//	m_scriptLogger.addError(__FILE__,
	//													__LINE__, 
	//													"[ScriptEngine] " + StringUtils::toString(log));
	//}
}

}
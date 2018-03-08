#include <dr_string_utils.h>
#include <dr_resource_manager.h>
#include <dr_time.h>

#include "dr_script_engine.h"
#include "scriptstdstring.h"
//#include "dr_script_custom_string.h"

#include "dr_object.h"


namespace driderSDK {

	unsigned long g_timeout;

ObjectAS *Ref_Factory() {
  return new ObjectAS();
}

void stringPrint_g(asIScriptGeneric* gen) {
	String *str = (String*)gen->GetArgAddress(0);
	std::wcout << StringUtils::toTString(*str);
}

ScriptEngine::ScriptEngine() {
	m_scriptLogger = nullptr;
	m_scriptTime = nullptr;

	Logger::startUp();

	if (Logger::isStarted()) {
		m_scriptLogger = &Logger::instance();
	}

	if (!Time::isStarted()) {
		Time::startUp();
	}
	m_scriptTime = &Time::instance();


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
																		 this, 
																		 asCALL_THISCALL);


	// Register string type

	RegisterStdString(m_scriptEngine);
	//RegisterTString(m_scriptEngine);

	// Register the functions that the scripts will be allowed to use.
	result = m_scriptEngine->RegisterGlobalFunction("void Print(string &in)",
																									asFUNCTION(stringPrint_g), 
																									asCALL_GENERIC);

  /*result = m_scriptEngine->RegisterObjectType("Object",
                                              0,
                                              asOBJ_REF);

  result = m_scriptEngine->RegisterObjectBehaviour("Object", 
                                                   asBEHAVE_FACTORY,
                                                   "Object@ f()", 
                                                   asFUNCTION(Ref_Factory),
                                                   asCALL_CDECL);

  result = m_scriptEngine->RegisterObjectBehaviour("Object",
                                      asBEHAVE_ADDREF,
                                      "void f()", 
                                      asMETHOD(ObjectAS, AddRef),
                                      asCALL_THISCALL);

  result = m_scriptEngine->RegisterObjectBehaviour("Object", 
                                                   asBEHAVE_RELEASE, 
                                                   "void f()", 
                                                   asMETHOD(ObjectAS, Release), 
                                                   asCALL_THISCALL);

  result = m_scriptEngine->RegisterObjectMethod("Object",
                                                "float Add(float param1)",
                                                asMETHODPR(ObjectAS, Add, (float), float),
                                                asCALL_THISCALL);*/

  /*
  * Seccion para registrar metodos //eso dice arriba pero en ingles...(asi es comente un comentario)
  */

	return result;
}

Int8
ScriptEngine::addScript(const TString& scriptName,
                        const TString& script) {

	m_scriptModule = m_scriptEngine->GetModule("module", asGM_CREATE_IF_NOT_EXISTS);

	Int8 result = m_scriptModule->AddScriptSection(StringUtils::toString(scriptName).c_str(),
                                                 StringUtils::toString(script).c_str());
	if(result < 0) {
		addScriptLog(_T("AddScriptSection failed on file ") + scriptName, asMSGTYPE_ERROR);
		m_scriptEngine->Release();
		return -1;
	}
	return 0;
}

Int8
ScriptEngine::compileScript() {
	Int8 result = m_scriptModule->Build();
	if(result < 0) {
		addScriptLog(_T("Failed to compile script."), asMSGTYPE_ERROR);
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

	Int8 result = m_scriptContext->SetLineCallback(asMETHOD(ScriptEngine, lineCallback),
																								 &g_timeout,
																								 asCALL_THISCALL);
	if(result < 0) {
		addScriptLog(_T("Failed to set the line callback."), asMSGTYPE_ERROR);
		m_scriptContext->Release();
		m_scriptEngine->Release();
		return -2;
	}

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
	g_timeout = (unsigned long)(m_scriptTime->getElapsedMilli() + timeout);
	Int8 result = m_scriptContext->Execute();			

	if (result != asEXECUTION_FINISHED) {
		if (result == asEXECUTION_ABORTED) {
			addScriptLog(_T("The script was aborted, possibly timed out."), asMSGTYPE_ERROR);
		}
		if (result == asEXECUTION_SUSPENDED) {
			addScriptLog(_T("The script was suspended."), asMSGTYPE_ERROR);
		}
		if (result == asEXECUTION_EXCEPTION) {
	    TString NewLine(_T("<br>"));
	    addScriptLog(_T("There was an exception within a script call."), asMSGTYPE_ERROR);
			TString details = _T("Exception details:") + NewLine + _T("Function: ") + 
												StringUtils::toTString(m_scriptFunction->GetDeclaration()) + 
												NewLine + _T("Line: ") + 
												StringUtils::toTString(m_scriptContext->GetExceptionLineNumber()) + 
												NewLine + _T("Description: ") + 
												StringUtils::toTString(m_scriptContext->GetExceptionString());

			addScriptLog(details, asMSGTYPE_INFORMATION);
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
ScriptEngine::lineCallback(asIScriptContext* scriptContext) {
	if(g_timeout < m_scriptTime->getElapsedMilli()) {
		scriptContext->Abort(); 
		//scriptContext->Suspend(); //we can also use suspend
	}
}

void 
ScriptEngine::messageCallback(const asSMessageInfo* scriptMessage) {

	Int8 row = scriptMessage->row, col = scriptMessage->col;

	//Expect something like "myScript.as (5, 1) : Example message here"
	TString message = StringUtils::toTString(scriptMessage->section) + 
																					 _T(" (") + StringUtils::toTString(row) +
																					 _T(", ") + StringUtils::toTString(col) +
																					 _T(") : ") + 
																					 StringUtils::toTString(scriptMessage->message);

	addScriptLog(message, scriptMessage->type);
}

void
ScriptEngine::addScriptLog(const TString& log, int type) {

	if (type == asMSGTYPE_WARNING) {
		m_scriptLogger->addWarning(__FILE__, 
															__LINE__, 
															_T("[ScriptEngine] ") + log);
	}
	else if (type == asMSGTYPE_INFORMATION) {
		m_scriptLogger->addLog(_T("[ScriptEngine] ") + log);
	}
	else if (type == asMSGTYPE_ERROR) {
		m_scriptLogger->addError(__FILE__,
														__LINE__, 
														_T("[ScriptEngine] ") + log);
	}
}

}
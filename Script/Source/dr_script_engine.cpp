#include <dr_string_utils.h>
#include <dr_resource_manager.h>
#include <dr_time.h>

#include "dr_script_engine.h"
#include "scriptstdstring.h"
//#include "dr_script_custom_string.h"
#include <vector>
#include <dr_context_manager.h>

namespace driderSDK {

unsigned long g_timeout;


void stringPrint_g(asIScriptGeneric* gen) {
	String *str = (String*)gen->GetArgAddress(0);
	std::wcout << StringUtils::toTString(*str);
}

ScriptEngine::ScriptEngine() {

	/*if (!Logger::isStarted()) {
    Logger::startUp();
	}

	if (!Time::isStarted()) {
		Time::startUp();
	}*/

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

  /*
  * Seccion para registrar metodos //eso dice arriba pero en ingles...(asi es comente un comentario)
  */

	return result;
}

Int8
ScriptEngine::configurateEngine(ContextManager *ctx) {
  Int8 result = 0;

  // Register the functions for controlling the script threads, e.g. sleep
  ctx->registerThreadSupport(m_scriptEngine);

  return result;
}

Int8
ScriptEngine::addScript(const TString& scriptName,
                        const TString& script,
                        const TString& module) {

  asIScriptModule *mod = m_scriptEngine->GetModule(StringUtils::toString(module).c_str(),
                                                   asGM_CREATE_IF_NOT_EXISTS);

	Int8 result = mod->AddScriptSection(StringUtils::toString(scriptName).c_str(),
                                      StringUtils::toString(script).c_str(),
                                      script.length());

	if(result < 0) {
		addScriptLog(_T("AddScriptSection failed on file ") + scriptName, asMSGTYPE_ERROR);
		m_scriptEngine->Release();
		return -1;
	}

  result = mod->Build();
  if (result < 0) {
    addScriptLog(_T("Build module failed") + module, asMSGTYPE_ERROR);
    return -1;
  }

	return result;
}

void
ScriptEngine::getScriptObject(TString scriptName,
                              asIScriptModule *mod,
                              asIScriptObject **objectRef,
                              asITypeInfo **typeRef) {
  //Get type of the script object
  asITypeInfo* type = mod->GetTypeInfoByDecl(StringUtils::toString(
                                             scriptName).c_str());

  // Get the factory function from the object type
  String path = StringUtils::toString(scriptName) + " @" 
                + StringUtils::toString(scriptName) + "()";
  asIScriptFunction *factory = type->GetFactoryByDecl(path.c_str());

  // Prepare the context to call the factory function
  m_scriptContext->Prepare(factory);
  // Execute the call
  m_scriptContext->Execute();
  // Get the object that was created
  asIScriptObject *obj = *(asIScriptObject**)m_scriptContext->GetAddressOfReturnValue();
  // If you're going to store the object you must increase the reference,
  // otherwise it will be destroyed when the context is reused or destroyed.
  obj->AddRef();
  
  *objectRef = obj;
  *typeRef = type;
}

void
ScriptEngine::setObjectToScript(asITypeInfo *type,
                                TString methodDecl,
                                UInt32 arg,
                                void* appObj,
                                asIScriptObject* scriptObj) {
  // Obtain the function object that represents the class method
  asIScriptFunction *func = type->GetMethodByDecl(StringUtils::toString
                                                  (methodDecl).c_str());
  // Prepare the context for calling the method
  Int8 result = m_scriptContext->Prepare(func);
  result = m_scriptContext->SetArgObject(arg, appObj);
  // Set the object pointer
  m_scriptContext->SetObject(scriptObj);
  // Execute the call
  m_scriptContext->Execute();
}

void
ScriptEngine::executeFunction(TString function,
                              asITypeInfo *type,
                              asIScriptObject* scriptObj) {
  // Obtain the function object that represents the class method
  asIScriptFunction *func = type->GetMethodByDecl(StringUtils::toString(
                                                  function).c_str());
  // Prepare the context for calling the method
  m_scriptContext->Prepare(func);
  // Set the object pointer
  m_scriptContext->SetObject(scriptObj);
  // Execute the call
  m_scriptContext->Execute();
}

void
ScriptEngine::release() {
	m_scriptContext->Release();
	m_scriptEngine->ShutDownAndRelease();
}

void
ScriptEngine::lineCallback(asIScriptContext* scriptContext) {
	/*if(g_timeout < Time::instancePtr()->getElapsedMilli()) {
		scriptContext->Abort(); 
		//scriptContext->Suspend(); //we can also use suspend
	}*/
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
    Logger::instancePtr()->addWarning(__FILE__,
															__LINE__, 
															_T("[ScriptEngine] ") + log);
	}
	else if (type == asMSGTYPE_INFORMATION) {
    Logger::instancePtr()->addLog(_T("[ScriptEngine] ") + log);
	}
	else if (type == asMSGTYPE_ERROR) {
    Logger::instancePtr()->addError(__FILE__,
														__LINE__, 
														_T("[ScriptEngine] ") + log);
	}
}

}
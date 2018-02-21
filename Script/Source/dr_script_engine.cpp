#include <dr_file.h>
#include <dr_string_utils.h>
#include "dr_script_engine.h"
#include "dr_script_string_factory.h"

namespace driderSDK {

ScriptEngine::ScriptEngine() {}

Int8
ScriptEngine::createEngine() {

	m_scriptEngine = asCreateScriptEngine();  
	if(m_scriptEngine == 0) {
		//failed to create script engine
		return -1;
	}
	m_scriptEngine->SetMessageCallback(asMETHOD(ScriptEngine, messageCallback), 
																		 0, 
																		 asCALL_CDECL);
  

	Int8 result = m_scriptEngine->RegisterObjectType("TString",
																									sizeof(TString),
																									asOBJ_VALUE | asOBJ_POD);
	DR_ASSERT(result >= 0);
	result = m_scriptEngine->RegisterStringFactory("TString", &stringFactory);
	// Register the functions that the scripts will be allowed to use.

  result = m_scriptEngine->RegisterGlobalFunction("void print(TString)", asFUNCTIONPR(print, (TString), void),  asCALL_CDECL);

	//
	return result;
}

Int8
ScriptEngine::addScript(const TString& fileName) {
	File scriptFile;
	if(!scriptFile.Open(fileName)) {
		//failed to find file
		m_scriptEngine->Release();
		return -1;
	}
  
	SizeT fileLength = scriptFile.Size();

	TString script;
	//script.resize(fileLength);
	script = scriptFile.GetAsString(fileLength);
	scriptFile.Close();

	if(script.size() == 0) {
		//couldn't load scipt
		m_scriptEngine->Release();
		return -2;
	}
	m_scriptModule = m_scriptEngine->GetModule(0, asGM_ALWAYS_CREATE);
	Int8 result = m_scriptModule->AddScriptSection(StringUtils::toString(fileName).c_str(),
																								StringUtils::toString(script).c_str(),
																								fileLength);
	if(result < 0) {
		//AddscriptSection failed
		m_scriptEngine->Release();
		return -3;
	}
	return 0;
}

Int8
ScriptEngine::compileScript() {
	Int8 result = m_scriptModule->Build();
	if(result < 0) {
		//build failed.
		m_scriptEngine->Release();
		return -1;
	}
	return 0;
}

Int8
ScriptEngine::configureContext() {
	m_scriptContext = m_scriptEngine->CreateContext();
	if(m_scriptContext == 0) {
		//failed to create context.
		m_scriptEngine->Release();
		return -1;
	}

	/*Int8 result = m_scriptContext->SetLineCallback(asMETHOD(ScriptEngine, lineCallback),
																								reinterpret_cast<void*>(&timeout), 
																								asCALL_STDCALL);
	if(result < 0) {
		//failed to set line callback
		m_scriptContext->Release();
		m_scriptEngine->Release();
		return -2;
	}*/
	return 0;
}

Int8
ScriptEngine::prepareFunction(TString function) {

	auto modul = m_scriptEngine->GetModule(0);
  m_scriptFunction = modul->GetFunctionByName(StringUtils::toString(function).c_str());
	if (m_scriptFunction == 0)
	{
		//function not found
		m_scriptContext->Release();
		m_scriptEngine->Release();
		return -1;
	}
	Int8 result = m_scriptContext->Prepare(m_scriptFunction);

	if (result < 0)
	{
		//failed to prepare context
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
			//there was an exception
			return result; //will replace with logger entry
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
ScriptEngine::lineCallback() {
	//if(*timeOut < /*functionToObtainTime()*/ 0) {
	//	scriptContext->Abort(); //we can also use suspend
	//}
}

void 
ScriptEngine::messageCallback(const asSMessageInfo *scriptMessage, void *param) {
	//output function...
}

}
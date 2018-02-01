#include "dr_script_engine.h"
#include "dr_script_string_factory.h"

#include <dr_file.h>


namespace driderSDK {

ScriptEngine::ScriptEngine() {}

ScriptEngine::~ScriptEngine() {}

int 
ScriptEngine::createEngine() {

	m_scriptEngine = asCreateScriptEngine();

	if(m_scriptEngine == 0) {
		//failed to create script engine
		return -1;
	}

	m_scriptEngine->SetMessageCallback(asFUNCTION(messageCallback), 0, asCALL_CDECL);
	return 0;
}

void
ScriptEngine::configureEngine() {

	int result = m_scriptEngine->RegisterObjectType("TString", 
																									sizeof(TString), 
																									asOBJ_VALUE | asOBJ_POD); 
	DR_ASSERT(result >= 0);
	m_scriptEngine->RegisterStringFactory("TString", &stringFactory);

	// Register the functions that the scripts will be allowed to use.

	//

}

void
ScriptEngine::addScript(const TString& fileName) {
	File scriptFile;
	if(!scriptFile.Open(fileName)) {
		//failed to find file
		m_scriptEngine->Release();
		return;
	}

	SizeT fileLength = scriptFile.Size();

	TString script;
	script.resize(fileLength);
	scriptFile.Write(fileLength, (ANSIChar*)&script);
	scriptFile.Close();

	if(script.size() == 0) {
		//couldn't load scipt
		m_scriptEngine->Release();
		return;
	}
	m_scriptModule = m_scriptEngine->GetModule(0, asGM_ALWAYS_CREATE);
	int result = m_scriptModule->AddScriptSection((char*) fileName.c_str(), 
																								(char*) &script[0], 
																								fileLength);
	if(result < 0) {
		//AddscriptSection failed
		m_scriptEngine->Release();
		return;
	}
}

void
ScriptEngine::compileScript() {
	int result = m_scriptModule->Build();
	if(result < 0) {
		//build failed.
		m_scriptEngine->Release();
		return;
	}
}

void
ScriptEngine::configureContext() {
	m_scriptContext = m_scriptEngine->CreateContext();
	if(m_scriptContext == 0) {
		//failed to create context.
		m_scriptEngine->Release();
		return;
	}

	unsigned long timeout;
	int result = m_scriptContext->SetLineCallback(asFUNCTION(lineCallback), 
																				&timeout, 
																				asCALL_CDECL);
	if(result < 0) {
		//failed to set line callback
		m_scriptContext->Release();
		m_scriptEngine->Release();
		return;
	}
}

int
ScriptEngine::prepareFunction(TString function) {

	m_scriptFunction = m_scriptEngine->GetModule(0)->GetFunctionByDecl((const char*)function.c_str());
	if (m_scriptFunction == 0)
	{
		//function not found
		m_scriptContext->Release();
		m_scriptEngine->Release();
		return -1;
	}
	int result = m_scriptContext->Prepare(m_scriptFunction);

	if (result < 0)
	{
		//failed to prepare context
		m_scriptContext->Release();
		m_scriptEngine->Release();
		return -1;
	}

}

int
ScriptEngine::executeCall() {
	int result = m_scriptContext->Execute();
	if (result != asEXECUTION_FINISHED) {
		if (result == asEXECUTION_EXCEPTION) {
			//there was a exception
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
ScriptEngine::lineCallback(asIScriptContext *scriptContext, unsigned long *timeOut) {
	if(*timeOut < /*functionToObtainTime()*/ 0) {
		scriptContext->Abort(); //we can also use suspend
	}
}

void
ScriptEngine::messageCallback(const asSMessageInfo *scriptMessage, void *param) {
	//output function...
}

}
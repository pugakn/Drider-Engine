#include "dr_script_engine.h"
#include <dr_file.h>

namespace driderSDK {

ScriptEngine::ScriptEngine() {}

ScriptEngine::~ScriptEngine() {}

int 
ScriptEngine::createEngine() {

	engine = asCreateScriptEngine();

	if(engine == 0) {
		return -1;
	}

	engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
	return 0;
}

void
ScriptEngine::configureEngine() {

	//register TString

}

void
ScriptEngine::addScript(const TString& fileName) {
	File scriptFile;
	if(!scriptFile.Open(fileName)) {
		//failed to find file
		engine->Release();
		return;
	}

	SizeT fileLenght = scriptFile.Size();

	TString script;
	script.resize(fileLenght);
	scriptFile.Write(fileLenght, (ANSIChar*)&script);
	scriptFile.Close();

	if(script.size() == 0) {
		//couldn't load scipt
		engine->Release();
		return;
	}
	mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
	int result = mod->AddScriptSection((char*) fileName.c_str(),
																		 (char*) &script[0], 
																		 fileLenght);
	if(result < 0) {
		//AddscriptSection failed
		engine->Release();
		return;
	}
}

void
ScriptEngine::compileScript() {
	int result = mod->Build();
	if(result < 0) {
		//build failed.
		engine->Release();
		return;
	}
}

void
ScriptEngine::configureContext() {
	context = engine->CreateContext();
	if(context == 0) {
		//failed to create context.
		engine->Release();
		return;
	}

	unsigned long timeout;
	int result = context->SetLineCallback(asFUNCTION(LineCallback), 
																				&timeout, 
																				asCALL_CDECL);
	if(result < 0) {
		//failed to set line callback
		context->Release();
		engine->Release();
		return;
	}
}

void
ScriptEngine::LineCallback(asIScriptContext *ctx, unsigned long *timeOut) {
	if(*timeOut < /*functionToObtainTime()*/ 0) {
		ctx->Abort(); //we can also use suspend
	}
}

void
ScriptEngine::MessageCallback(const asSMessageInfo *msg, void *param) {
	//output function...
}

}
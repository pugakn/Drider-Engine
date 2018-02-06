#pragma once

#include <angelscript\angelscript.h>
#include <dr_module.h>
#include "dr_script_prerequisites.h"


namespace driderSDK {

/**
*  Script engine class.
*
* Sample usage:
*	ScriptEngine scriptMgr;
* if (!scriptMgr.isStarted()) {
*   scriptMgr.startUp();
* }
*/
class DR_SCRIPT_EXPORT ScriptEngine : public Module<ScriptEngine>
{
public:

	/**
	* Default constructor.
	*
	*/
	ScriptEngine();

	/**
	* Default destructor.
	*
	*/
	virtual
	~ScriptEngine() {}

	/**
	* Create the script engine and register string type and functions.
	*
	* @return
	*	  Negative value if error.
	*/
	int 
	createEngine();

	/**
	* Open the script and adds it to the module.
	*
	* @param fileName
	*  Name of the file to open.
	*/
	int 
	addScript(const TString& fileName);

	/**
	* Compiles the script.
	*
	*/
	int
	compileScript();

	/**
	* Create the context and sets the LineCallback function.
	*
	*/
	int 
	configureContext();

	/**
	* Create the context and sets the LineCallback function.
	*
	* @return
	*	  Negative value if error.
	*/
	int
	prepareFunction(TString function);

	/**
	* Execute a call in the context.
	*
	*/
	int
	executeCall();

	/**
	* Shut down the script's engine.
	*
	*/
	void
	release();

	/**
	* Aborts the script if it runs out of time.
	*
	* @param scriptContext
	*   Script context to check.
	*
	* @param timeOut
	*   Time required to abort the script.
	*/
	void 
	lineCallback(asIScriptContext *scriptContext, unsigned long *timeOut);

	/**
	* Gets the messages from the script's engine and logs them.
	*
	* @param scriptMessage
	*   Message from the script's engine.
	*
	* @param param
	*   Aditional parameters from the message. 
	*/
	void
	messageCallback(const asSMessageInfo *scriptMessage, void *param);

private:
	asIScriptEngine* m_scriptEngine;
	asIScriptContext* m_scriptContext;
	asIScriptFunction* m_scriptFunction;
	asIScriptModule* m_scriptModule;
	unsigned long timeout;
};

}
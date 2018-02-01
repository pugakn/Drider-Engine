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
	~ScriptEngine();

	/**
	* Create the script engine
	*
	* @return
	*	  Returns 0 if the engine was created succesfully, otherwise returns -1.
	*
	*/
	int 
	createEngine();

	/**
  * Register the script's string type and the functions it'll be allowed to use.
  *
  */
	void 
	configureEngine();

	/**
	* Open the script and adds it to the module.
	*
	* @param fileName
	*  Name of the file to open.
	*
	*/
	void 
	addScript(const TString& fileName);

	/**
	* Compiles the script.
	*
	*/
	void
	compileScript();

	/**
	* Create the context and sets the LineCallback function.
	*
	*/
	void 
	configureContext();

	/**
	* Create the context and sets the LineCallback function.
	*
	*/
	int
	prepareFunction(TString scriptName);

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
	*
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

};

}
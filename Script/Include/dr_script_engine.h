#pragma once

#include <angelscript\angelscript.h>
#include <dr_module.h>
#include <dr_logger.h>
#include <string>
#include "dr_script_prerequisites.h"

#include <iostream>

namespace driderSDK {

class ObjectAS;

ObjectAS *Ref_Factory();

void stringPrint_g(asIScriptGeneric* gen);

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
	Int8 
	createEngine();

	/**
	* Open the script and adds it to the module.
	*
	* @param fileName
	*  Name of the file to open.
	*/
	Int8
	addScript(const TString& scriptName,
            const TString& script);

	/**
	* Compiles the script.
	*
	*/
	Int8
	compileScript();

	/**
	* Create the context and sets the LineCallback function.
	*
	*/
	Int8
	configureContext();

	/**
	* Create the context and sets the LineCallback function.
	*
	* @return
	*	  Negative value if error.
	*/
	Int8
	prepareFunction(TString function);

	/**
	* Execute a call in the context.
	*
	*/
	Int8
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

	/**
	* Adds a custom script log into the logger.
	*
	* @param log
	*   Message to insert.
	*
	* @param type
	*   Type of the message.
	*/
	void 
	addScriptLog(const TString& log, const int type);
  asIScriptEngine* m_scriptEngine;
private:
	//asIScriptEngine* m_scriptEngine;
	asIScriptContext* m_scriptContext;
	asIScriptFunction* m_scriptFunction;
	asIScriptModule* m_scriptModule;
	Logger m_scriptLogger;
	unsigned long timeout = 4000;
};

}
#pragma once

#include <angelscript\angelscript.h>
#include <dr_module.h>
#include <dr_logger.h>
#include <string>
#include "dr_script_prerequisites.h"
#include <dr_export_script.h>

#include <iostream>
#include <vector>

namespace driderSDK {

class Time;
class ContextManager;

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

  ScriptEngine(const ScriptEngine&) = delete;

  ScriptEngine& operator=(const ScriptEngine&) = delete;

	/**
	* Default destructor.
	*
	*/
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
  * Configurate Engine
  */
  Int8
  configurateEngine(ContextManager *ctx);
  
	/**
	* Open the script and adds it to the module.
	*
	* @param fileName
	*  Name of the file to open.
	*/
	Int8
	addScript(const TString& scriptName,
            const TString& script,
            const TString& module);
  
  Int8
  compile(TString module);

	/**
	* Compiles the script.
	*
	*/
	/*Int8
	compileScript();*/

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
	*/
	void 
	lineCallback(asIScriptContext *scriptContext);

	/**
	* Gets the messages from the script's engine and logs them.
	*
	* @param scriptMessage
	*   Message from the script's engine.
	*/
	void
	messageCallback(const asSMessageInfo *scriptMessage);

	/**
	* Adds a custom script log into the logger.
	*
	* @param log
	*   Message to insert.
	*
	* @param type
	*   Type of the message.
	*/
	static void 
	addScriptLog(const TString& log, const int type);

  asIScriptEngine* m_scriptEngine;
	unsigned long timeout = 5000;
	
  asIScriptContext* m_scriptContext;
  std::vector<asIScriptModule*> m_scriptModules;

 protected:
  void
  onStartUp() override { }

  void
  onShutDown() override {}
	
 private:
	//asIScriptEngine* m_scriptEngine;
	//asIScriptFunction* m_scriptFunction;
};

}
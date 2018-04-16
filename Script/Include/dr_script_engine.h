#pragma once

#include <vector>

#include <angelscript\angelscript.h>
#include <dr_module.h>
#include <dr_logger.h>
#include "dr_script_prerequisites.h"
#include <dr_export_script.h>

#include <../../Input/Include/dr_input_defines.h>

namespace driderSDK {

class Time;
class ContextManager;
class ScriptDebug;

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
  
  /**
  * Gets script object
  */
  void
  getScriptObject(TString scriptName,
                  asIScriptModule *mod,
                  asIScriptObject **objectRef,
                  asITypeInfo **typeRef);

  /**
  * Sets componet to a script
  */
  void 
  setObjectToScript(asITypeInfo *type,
                    TString methodDecl,
                    UInt32 arg,
                    void* appObj,
                    asIScriptObject* scriptObj);
  
  /**
  * Execute a script function
  */
  void
  executeFunction(TString function,
                  asITypeInfo *type,
                  asIScriptObject* scriptObj);

  /**
  * Execute a script function with param
  */
  void
  executeFunctionParam(TString function,
                       asITypeInfo *type,
                       asIScriptObject* scriptObj,
                       KEY_CODE::E param);

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
	ScriptDebug* Debug;

 protected:
  void
  onStartUp() override { }

  void
  onShutDown() override {}

 private:
};

}
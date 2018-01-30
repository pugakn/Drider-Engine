#pragma once

#include <angelscript\angelscript.h>
#include <dr_module.h>
#include "dr_script_prerequisites.h"


namespace driderSDK {

/**
*  Script engine class.
*
* Sample usage:
*	ScriptEngine script;
*/
class DR_SCRIPT_EXPORT ScriptEngine : public Module<ScriptEngine>
{
public:

	/**
	* TEST::defaultConstructor
	* Default constructor.
	*
	*/
	ScriptEngine();

	/**
	* TEST::defaultDestructor
	* Default destructor.
	*
	*/
	~ScriptEngine();

	/**
	* TEST::configureEngine
	* Create the script engine
	*
	* @return
	*	  Returns 0 if the engine was created succesfully, otherwise returns -1.
	*
	*/
	int 
	createEngine();

	/**
	* TEST::configureEngine
  * Register the script's string type and the functions it'll be allowed to use.
  *
  */
	void 
	configureEngine();

	/**
	* TEST::addScript
	* Open the script and adds it to the module.
	*
	* @param fileName
	*  Name of the file to open.
	*
	*/
	void 
	addScript(const TString& fileName);

	/**
	* TEST::compileScript
	* Compiles the script.
	*
	*/
	void
	compileScript();

	/**
	* TEST::configureContext
	* Create the context and sets the LineCallback function.
	*
	*/
	void 
	configureContext();

	void 
	LineCallback(asIScriptContext *ctx, unsigned long *timeOut);

	void
	MessageCallback(const asSMessageInfo *msg, void *param);


	asIScriptEngine* engine;
	asIScriptContext* context;
	asIScriptFunction* function;
	asIScriptModule* mod;

};

}
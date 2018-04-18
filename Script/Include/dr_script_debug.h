#pragma once
#include "dr_script_prerequisites.h"

namespace driderSDK {

	/**
	*  Class containing debug functions for the script engine class.
	*
	* Sample usage:
	*	ScriptEngine scriptMgr;
	* scriptMgr->Debug.setBreakpoint();
	*/
	class DR_SCRIPT_EXPORT ScriptDebug 
	{
		class asIScriptContext;
		//class ScriptEngine;

	public:

		/**
		*  Constructor
		*
		*/
		ScriptDebug();

		/**
		*  Default destructor
		*
		*/
		~ScriptDebug();

		/**
		*  Adds a breakpoint into the code.
		*
		*/
		void setBreakPoint();

		/**
		*  Checks for a breakpoint.
		*
		*  @return
		*   true if it found a breakpoint.
		*/
		bool checkBreakPoint();

		/**
		*  Print all the Callstack using the Logger.
		*
		*/
		void printCallStack();

		/**
		*  Print all the local variables using the Logger.
		*
		*/
		void printLocalVariables();

		/**
		*  Print all the global variables using the Logger.
		*
		*/
		void printGlobalVariables();

		/**
		*  x
		*
		*/
		void printValue();

		/**
		*  Calls the Logger and adds a new log.
		*
		*  @param log
		*   String containing the message.
		*/
		void printToLogger(TString& log);

		/**
		*  Function to be called for each statement executed, called by AngelScript.
		*
		*  @param scriptContext
		*   pointer to a script context.
		*/
		void debugLineCallback(asIScriptContext* scriptContext);

	private:
		//ScriptEngine* pScriptEngine;
	};

}

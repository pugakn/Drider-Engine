#pragma once
#include "dr_script_prerequisites.h"
#include <vector>
//#include <unordered_map>

namespace driderSDK {

	class ScriptEngine;

	namespace DebugCommands {

		enum E 
		{
			CONTINUE,
			STEP_OVER,
			STEP_IN,
			STEP_OUT
		};

	}

	/**
	*  Class containing debug functions for the script engine class.
	*
	* Sample usage:
	*	ScriptEngine scriptMgr;
	* scriptMgr->Debug.printToLogger(_T("Test log from debug"));
	*/
	class DR_SCRIPT_EXPORT ScriptDebug 
	{
		//class asIScriptContext;

		struct BreakPoint
		{
			BreakPoint(TString _section,
								 UInt32 _line, 
								 bool func) : section(_section), line(_line), function(func) {}
			TString section;
			UInt32 line;
			bool function;
		};

	public:

		/**
		* Constructor
		*
		*/
		ScriptDebug(ScriptEngine* engine);

		/**
		* Default destructor
		*
		*/
		~ScriptDebug();

		/**
		* Adds a breakpoint into the code.
		*
		*/
		void setBreakPoint();

		/**
		* x
		*
		*/
		void sendCommand(DebugCommands::E command);

		/**
		* Checks for a breakpoint.
		*
		* @return
		*   true if it found a breakpoint.
		*/
		bool checkBreakPoint();

		/**
		* Print all the Callstack using the Logger.
		*
		*/
		void printCallStack();

		/**
		* Print all the local variables using the Logger.
		*
		*/
		void printLocalVariables();

		/**
		* Print all the global variables using the Logger.
		*
		*/
		void printGlobalVariables();

		/**
		* x
		*
		*/
		void printValue();

		/**
		* Calls the Logger and adds a new log.
		*
		* @param log
		*   String containing the message.
		*/
		void printToLogger(TString log);

		bool bIsStopped;
	private:
		ScriptEngine* pScriptEngine;
		std::vector<BreakPoint> m_breakpoints;
	};

}

#pragma once
#include "dr_script_prerequisites.h"
#include <vector>
#include "angelscript\angelscript.h"

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

		//class asIScriptGeneric;

		struct BreakPoint
		{
			BreakPoint(TString _section,
								 UInt32 _line) : section(_section), line(_line) {}
			TString section;
			UInt32 line;

		};

	public:

		/**
		* Constructor
		*
		*/
		ScriptDebug(ScriptEngine* engine);

		/**
		* Default constructor
		*
		*/
		ScriptDebug();

		/**
		* Default destructor
		*
		*/
		~ScriptDebug();

		/**
		* Adds a breakpoint into the code.
		*
		*/
		void 
		setBreakPoint();

		/**
		* Removes a breakpoint from the code.
		*
		*/
		bool 
		removeBreakPoint(Int32 line, TString& section);

		/**
		* Takes the input and interpret it's action
		*
		*  @param input
		*   String containing the action
		*/
		void 
		interpretInput(TString& input);

		/**
		* Sets a command to the debugger
		*
		*  @param command
		*   Command to set from an enumerator.
		*/
		void 
		setCommand(DebugCommands::E command);

		/**
		* Gets the current command of the debugger.
		*
		*  @return
		*   Active command
		*/
		DebugCommands::E 
		getCommand();

		/**
		* Checks for a breakpoint.
		*
		* @return
		*   True if it found a breakpoint.
		*/
		bool 
		checkBreakPoint();

		/**
		* Removes all Breakpoints.
		*
		*/
		void 
		clearBreakPoints();

		/**
		* Print all the Callstack using the Logger.
		*
		*/
		void 
		printCallStack();

		/**
		* Print all the local variables using the Logger.
		*
		*/
		void 
		printLocalVariables();

		/**
		* Print all the global variables using the Logger.
		*
		*/
		void 
		printGlobalVariables();

		/**
		* Calls the Logger and adds a new log.
		*
		* @param log
		*   String containing the message.
		*/
		void 
		printToLogger(TString log);

		/**
		* Interprets the value of the object
		*
		*  @param value
		*   pointer to the value to interpret.
		*
		*  @param typeId
		*   Int from an enumerator indicating the type of value.
		*
		*  @return
		*   TString containing the interpreted value.
		*/
		TString 
		interpretValue(void* value, Int32 typeId);

		/**
		* Prints the Garbage Collector's status.
		*
		*/
		void 
		printGarbageStatus();

		/**
		* Prints a string, called from the script
		*
		* @param string
		*   Pointer to the string to print.
		*/
		void 
		printString(TString* string);

		/**
		* Prints the value of a variable, called from the script
		*
		* @param gen
		*   Pointer to the value of the variable.
		*/
		static void 
		logValue(asIScriptGeneric* gen);

		UInt32 lastStackLevel;
	private:
		ScriptEngine* pScriptEngine;
		std::vector<BreakPoint> m_breakpoints;
		DebugCommands::E m_command;
	};

}

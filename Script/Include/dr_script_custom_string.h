#pragma once
#include "dr_script_prerequisites.h"

class asIScriptEngine;

namespace driderSDK
{

	void RegisterTString(asIScriptEngine *engine);
	void RegisterTStringMethodsNative(asIScriptEngine *engine);
}
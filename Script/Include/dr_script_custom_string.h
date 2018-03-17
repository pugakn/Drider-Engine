#pragma once
#include "dr_script_prerequisites.h"

class asIScriptEngine;

namespace driderSDK
{
	void registerTString(asIScriptEngine *engine);
	void registerTStringMethodsNative(asIScriptEngine *engine);
}
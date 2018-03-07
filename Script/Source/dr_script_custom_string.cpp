#include "dr_script_custom_string.h"
#include "dr_script_string_factory.h"

#include <angelscript\angelscript.h>

namespace driderSDK
{
	static StringFactory g_stringFactory;

	void RegisterTString(asIScriptEngine *engine) {
		if (true) {
			RegisterTStringMethodsNative(engine);
		}
		else {

		}
	}

	void RegisterTStringMethodsNative(asIScriptEngine *engine) {

		Int8 result;
		result = engine->RegisterObjectType("TString",
																				sizeof(TString),
																				asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);

		result = engine->RegisterStringFactory("TString", &g_stringFactory);


	}

}
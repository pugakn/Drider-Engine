#include "dr_script_custom_string.h"
#include "dr_script_string_factory.h"

#include <angelscript\angelscript.h>

namespace driderSDK
{
	static StringFactory g_stringFactory;

	void tStringConstructor(TString* pString) {
		new(pString) TString();
	}

	void tStringCopyConstructor(TString* pString, TString& copy) {
		new(pString) TString(copy);
	}

	void tStringDestructor(TString* pString) {
		pString->~basic_string();
	}

	static int tStringCmp(const TString& a, const TString& b) {
		int cmp = 0;
		if (a < b) {
			cmp = -1;
		}
		else if ( a > b) {
			cmp = 1;
		}
		return cmp;
	}

	static bool tStringEquals(const TString& a, const TString& b) {
		return a == b;
	}

	static TString tStringAdd(const TString& a, const TString& b) {
		return a + b;
	}

	static TString& tStringAssign(const TString& in, TString& destiny) {
		destiny = in;
		return destiny;
	}

	static TString& tStringAddAssign(const TString& in, TString& destiny) {
		destiny += in;
		return destiny;
	}

	static asUINT tStringLength(const TString& a) {
		return (asUINT)a.length();
	}

	static void tStringResize(asUINT size, TString &a) {
		a.resize(size);
	}

	static bool tStringIsEmpty(const TString& a) {
		return a.empty();
	}

	static TString::value_type * tStringIndex(Int8 index, TString& a) {
		if (index >= a.size()) {
			asIScriptContext* context = asGetActiveContext();
			context->SetException("Out of range");
			return 0;
		}
		return &a[index];
	}

	void registerTString(asIScriptEngine* engine) {
		if (true) {
			registerTStringMethodsNative(engine);
		}
		else {

		}
	}

	void registerTStringMethodsNative(asIScriptEngine* engine) {

		Int8 result;
		result = engine->RegisterObjectType("TString",
																				sizeof(TString),
																				asOBJ_VALUE | asGetTypeTraits<TString>());

		result = engine->RegisterStringFactory("TString", &g_stringFactory);

		//Behaviors
		result = engine->RegisterObjectBehaviour("TString", 
																						 asBEHAVE_CONSTRUCT, 
																						 "void f()", 
																						 asFUNCTION(tStringConstructor), 
																						 asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectBehaviour("TString",
																						 asBEHAVE_CONSTRUCT,
																						 "void f(const TString &in)",
																						 asFUNCTION(tStringCopyConstructor),
																						 asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectBehaviour("TString", 
																						 asBEHAVE_DESTRUCT, 
																						 "void f()", 
																						 asFUNCTION(tStringDestructor), 
																						 asCALL_CDECL_OBJLAST);
		//Operators
		result = engine->RegisterObjectMethod("TString", 
																					"bool opEquals(const TString &in) const", 
																					asFUNCTIONPR(tStringEquals, 
																					(const TString&, const TString&), bool), 
																					asCALL_CDECL_OBJFIRST);
		result = engine->RegisterObjectMethod("TString", 
																					"int opCmp(const TString &in) const", 
																					asFUNCTION(tStringCmp), 
																					asCALL_CDECL_OBJFIRST);
		result = engine->RegisterObjectMethod("TString", 
																					"TString opAdd(const TString &in) const", 
																					asFUNCTIONPR(tStringAdd, 
																					(const TString&, const TString&), TString),
																					asCALL_CDECL_OBJFIRST);
		result = engine->RegisterObjectMethod("TString", 
																					"string &opAssign(const TString &in)", 
																					asFUNCTION(tStringAssign),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString", 
																					"string &opAddAssign(const TString &in)", 
																					asFUNCTION(tStringAddAssign), 
																					asCALL_CDECL_OBJLAST);
		//String methods
		result = engine->RegisterObjectMethod("TString", 
																					"uint length() const", 
																					asFUNCTION(tStringLength), 
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString", 
																					"void resize(uint)", 
																					asFUNCTION(tStringResize), 
																					asCALL_CDECL_OBJLAST); 
		result = engine->RegisterObjectMethod("TString",
																					"bool isEmpty() const",
																					asFUNCTION(tStringIsEmpty),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString",
																					"uint8 opIndex(uint)",
																					asFUNCTION(tStringIndex),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString",
																					"const uint8 opIndex(uint) const",
																					asFUNCTION(tStringIndex),
																					asCALL_CDECL_OBJLAST);
		//Conversions
		/*result = engine->RegisterObjectMethod("TString",
																					"",
																					asFUNCTION(),
																					asCALL_CDECL_OBJLAST);*/

		/*
			TO DO...
			Assign
			AddAssign
			Add
			Add_r

			TYPES:
			float
			bool


		*/
	}

}
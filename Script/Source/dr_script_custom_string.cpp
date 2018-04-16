#include "dr_script_custom_string.h"
#include "dr_script_string_factory.h"

#include <angelscript\angelscript.h>

namespace driderSDK
{
	static StringFactory g_stringFactory;

	const char* gStringName = "TString";

	void tStringConstructor(TString* pString) {
		new(pString) TString();
	}

	void tStringCopyConstructor(TString* pString, TString& copy) {
		new(pString) TString(copy);
	}

	void tStringDestructor(TString* pString) {
		pString->~basic_string();
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

	static UInt32 tStringLength(const TString& a) {
		return (UInt32)a.length();
	}

	static void tStringResize(UInt32 size, TString &a) {
		a.resize(size);
	}

	static bool tStringIsEmpty(const TString& a) {
		return a.empty();
	}

	static TString::value_type * tStringIndex(Int32 index, TString& a) {
		if (index >= a.size()) {
			asIScriptContext* context = asGetActiveContext();
			context->SetException("Out of range");
			return 0;
		}
		return &a[index];
	}

	static void tStringClear(TString& a) {
		a.clear();
	}

	static UInt32 tStringSize(const TString& a) {
		return (UInt32)a.size();
	}

	static void tStringPushBack(const TString::value_type& character, TString& a) {
		a.push_back(character);
	}

	static Int32 tStringCompare(const TString& a, const TString& b) {
		return a.compare(b);
	}

	static void tStringPopBack(TString& a) {
		if (!a.empty()) {
			a.pop_back();
		}
	}

	static TString tStringSubstr(Int32 position, Int32 length, const TString& string) {
		return string.substr(position, length);
	}

	static Int32 tStringCopy(TString& a, TString::value_type* destiny, Int32 count, Int32 pos) {
		if (count >= a.size() || pos > a.max_size()) {
			asIScriptContext* context = asGetActiveContext();
			context->SetException("Out of range");
			return -1;
		}

    for (Int32 i = pos; i < count; ++i) {
      destiny[i] = a[i];
    }

		return count - pos;
	}
	
	static Int32 tStringFind(TString& a, TString& in, Int32 pos) {
		return a.find(in, pos);
	}
	
	static TString& tStringInsert(TString& a, Int32 pos, const TString& in) {
		//add exceptions
		return a.insert(pos, in);
	}

	static Int32 tStringRFind(TString& a, TString& in, Int32 pos) {
		return a.rfind(in, pos);
	}

	//Conversions
	static TString &AssignBool(bool b, TString &destiny)
	{
		b ? destiny = _T("true") : destiny = _T("false");
		return destiny;
	}

	static TString &AddAssignBool(bool b, TString &destiny)
	{
		b ? destiny += _T("true") : destiny += _T("false");
		return destiny;

	}

	static TString AddBool(bool b, const TString &in)
	{
		TString result;
		b ? result = _T("true") : result = _T("false");
		return in + result;
	}

	static TString &AssignFloat(float f, TString &destiny)
	{
		destiny = f;
		return destiny;
	}

	static TString &AddAssignFloat(float f, TString &destiny)
	{
		destiny += f;
		return destiny;
	}

	static TString AddFloat(float f, const TString &in)
	{
		TString result;
		result = f;
		return in + result;
	}

	static TString &AssignDouble(double d, TString &destiny)
	{
		destiny = d;
		return destiny;
	}

	static TString &AddAssignDouble(double f, TString &destiny)
	{
		destiny += f;
		return destiny;
	}

	static TString AddDouble(double f, const TString &in)
	{
		TString result;
		result = f;
		return in + result;
	}

	static TString &AssignInt32(Int32 i, TString &destiny)
	{
		destiny = i;
		return destiny;
	}

	static TString &AddAssignInt32(Int32 i, TString &destiny)
	{
		destiny += i;
		return destiny;
	}

	static TString AddInt32(Int32 i, const TString &in)
	{
		TString result;
		result = i;
		return in + result;
	}

	static TString &AssignInt64(Int64 i, TString &destiny)
	{
		destiny = i;
		return destiny;
	}

	static TString &AddAssignInt64(Int64 i, TString &destiny)
	{
		destiny += i;
		return destiny;
	}

	static TString AddInt64(Int64 i, const TString &in)
	{
		TString result;
		result = i;
		return in + result;
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
		//Main class and factory
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
																						 "void f(const TString& in)",
																						 asFUNCTION(tStringCopyConstructor),
																						 asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectBehaviour("TString", 
																						 asBEHAVE_DESTRUCT, 
																						 "void f()", 
																						 asFUNCTION(tStringDestructor), 
																						 asCALL_CDECL_OBJLAST);
		//Operators
		result = engine->RegisterObjectMethod("TString", 
																					"bool opEquals(const TString& in) const", 
																					asFUNCTIONPR(tStringEquals, 
																					(const TString&, const TString&), bool), 
																					asCALL_CDECL_OBJFIRST);
		result = engine->RegisterObjectMethod("TString", 
																					"TString opAdd(const TString& in) const", 
																					asFUNCTIONPR(tStringAdd, 
																					(const TString&, const TString&), TString),
																					asCALL_CDECL_OBJFIRST);
		result = engine->RegisterObjectMethod("TString", 
																					"TString& opAssign(const TString& in)", 
																					asFUNCTION(tStringAssign),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString", 
																					"TString& opAddAssign(const TString& in)", 
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
																					"bool empty() const",
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
		result = engine->RegisterObjectMethod("TString",
																					"void clear()",
																					asFUNCTION(tStringClear),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString",
																					"uint size() const",
																					asFUNCTION(tStringSize),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString",
																					"void push_back(char& a)",
																					asFUNCTION(tStringPushBack),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString",
																					"int compare(const TString& in) const",
																					asFUNCTION(tStringCompare),
																					asCALL_CDECL_OBJFIRST); 
		//right now string compare only works with strings
		result = engine->RegisterObjectMethod("TString",
																					"void pop_back()",
																					asFUNCTION(tStringPopBack),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString",
																					"TString substr(uint pos, uint len)",
																					asFUNCTION(tStringSubstr),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString", 
																					"uint copy(char* dest, int count, int pos)",
																					asFUNCTION(tStringCopy),
																				  asCALL_CDECL_OBJFIRST);
		result = engine->RegisterObjectMethod("TString",
																					"int find(TString& in, int pos)",
																					asFUNCTION(tStringFind),
																					asCALL_THISCALL_OBJFIRST);
		result = engine->RegisterObjectMethod("TString",
																					"TString& insert(int pos, const TString& in)",
																					asFUNCTION(tStringInsert),
																					asCALL_CDECL_OBJFIRST);
		result = engine->RegisterObjectMethod("TString",
																					"int rfind(TString& in, int pos)",
																					asFUNCTION(tStringRFind),
																					asCALL_THISCALL_OBJFIRST);
		//...

		//Conversions

		result = engine->RegisterObjectMethod("TString",
																					"TString &opAssign(bool)",
																					asFUNCTION(AssignBool),
																					asCALL_CDECL_OBJLAST); 
		result = engine->RegisterObjectMethod("TString",
																					"TString &opAddAssign(bool)",
																					asFUNCTION(AddAssignBool),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString",
																					"TString opAdd(bool) const",
																					asFUNCTION(AddBool),
																					asCALL_CDECL_OBJLAST); 

		result = engine->RegisterObjectMethod("TString",
																					"TString &opAssign(float)",
																					asFUNCTION(AssignFloat),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString",
																					"TString &opAddAssign(float)",
																					asFUNCTION(AddAssignFloat),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString",
																					"TString opAdd(float) const",
																					asFUNCTION(AddFloat),
																					asCALL_CDECL_OBJLAST);

		result = engine->RegisterObjectMethod("TString",
																					"TString &opAssign(double)",
																					asFUNCTION(AssignDouble),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString",
																					"TString &opAddAssign(double)",
																					asFUNCTION(AddAssignDouble),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString",
																					"TString opAdd(double) const",
																					asFUNCTION(AddDouble),
																					asCALL_CDECL_OBJLAST);

		result = engine->RegisterObjectMethod("TString",
																					"TString &opAssign(int)",
																					asFUNCTION(AssignInt32),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString",
																					"TString &opAddAssign(int)",
																					asFUNCTION(AddAssignInt32),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString",
																					"TString opAdd(int) const",
																					asFUNCTION(AddInt32),
																					asCALL_CDECL_OBJLAST);

		result = engine->RegisterObjectMethod("TString",
																					"TString &opAssign(int64)",
																					asFUNCTION(AssignInt64),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString",
																					"TString &opAddAssign(int64)",
																					asFUNCTION(AddAssignInt64),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod("TString",
																					"TString opAdd(int64) const",
																					asFUNCTION(AddInt64),
																					asCALL_CDECL_OBJLAST);


		//result = engine->RegisterObjectMethod("TString",
		//																			"",
		//																			asFUNCTION(),
		//																			asCALL_CDECL_OBJLAST);

		/*
			TO DO...

			/////METHODS://///

			findFirstOf
			findLastOf
			findFirstNotOf
			findLastNotOF


			////CONVERSIONS:////
			Assign
			AddAssign
			Add

			TYPES:
			[X]bool
			[X]float
			[x]double
			[x]int32
			[x]int64


		*/
	}

}
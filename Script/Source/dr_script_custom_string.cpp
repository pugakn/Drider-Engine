#include "dr_script_custom_string.h"
#include "dr_script_string_factory.h"
#include <dr_string_utils.h>
#include <angelscript\angelscript.h>

namespace driderSDK
{
	static StringFactory g_stringFactory;

	const char* gStrName = "TString";

	void tStringConstructor(TString* pString) {
		new(pString) TString();
	}

	void tStringCopyConstructor(const TString& copy, TString* pString) {
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

	static TString::value_type* tStringIndex(SizeT index, TString& a) {
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

	static void tStringPushBack(const TString& character, TString& a) {
		if (!character.empty()) {
			a.push_back(character[0]);
		}
	}

	static Int32 tStringCompare(const TString& a, const TString& b) {
		return a.compare(b);
	}

	static void tStringPopBack(TString& a) {
		if (!a.empty()) {
			a.pop_back();
		}
	}

	static TString 
  tStringSubstr(Int32 position, 
                Int32 length, 
                const TString& string) {
		return string.substr(position, length);
	}

	static Int32 
  tStringCopy(TString& a, 
              TString& destiny, 
              SizeT count, 
              SizeT pos) {
		if (count >= a.size() || pos > a.max_size()) {
			asIScriptContext* context = asGetActiveContext();
			context->SetException("Out of range");
			return -1;
		}

    for (SizeT i = pos; i < count; ++i) {
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

	static TString 
  AddBool(bool b, const TString &in)
	{
		TString result;
		b ? result = _T("true") : result = _T("false");
		return in + result;
	}

	static TString &AssignFloat(float f, TString &destiny)
	{
		TOStringstream stream;
		stream << f;
		destiny = stream.str();
		return destiny;
	}

	static TString&
  AddAssignFloat(float f, TString &destiny)
	{
		TOStringstream stream;
		stream << f;
		destiny += stream.str();
		return destiny;
	}

	static TString AddFloat(float f, const TString &in)
	{
		TOStringstream stream;
		stream << f;
		return in + stream.str();
	}

	static TString &AssignDouble(Float64 d, TString &destiny)
	{
		TOStringstream stream;
		stream << d;
		destiny = stream.str();
		return destiny;
	}

	static TString &AddAssignDouble(Float64 d, TString &destiny)
	{
		TOStringstream stream;
		stream << d;
		destiny += stream.str();
		return destiny;
	}

	static TString AddDouble(Float64 d, const TString &in)
	{
		TOStringstream stream;
		stream << d;
		return in + stream.str();
	}

	static TString &AssignInt32(Int32 i, TString &destiny)
	{
		TOStringstream stream;
		stream << i;
		destiny = stream.str();
		return destiny;
	}

	static TString &AddAssignInt32(Int32 i, TString &destiny)
	{
		TOStringstream stream;
		stream << i;
		destiny += stream.str();
		return destiny;
	}

	static TString AddInt32(Int32 i, const TString &in)
	{
		TOStringstream stream;
		stream << i;
		return in + stream.str();
	}

	static TString &AssignInt64(Int64 i, TString &destiny)
	{
		TOStringstream stream;
		stream << i;
		destiny = stream.str();
		return destiny;
	}

	static TString &AddAssignInt64(Int64 i, TString &destiny)
	{
		TOStringstream stream;
		stream << i;
		destiny += stream.str();
		return destiny;
	}

	static TString AddInt64(Int64 i, const TString &in)
	{
		TOStringstream stream;
		stream << i;
		return in + stream.str();
	}

	void registerTString(asIScriptEngine* engine) {
		if (true) {
			registerTStringMethodsNative(engine);
		}
		else {
			//generic methods, not implemented
		}
	}

	void registerTStringMethodsNative(asIScriptEngine* engine) {

		Int8 result;
		String declaration, name(gStrName);
		//Main class and factory
		result = engine->RegisterObjectType(gStrName,
																				sizeof(TString),
																				asOBJ_VALUE | asGetTypeTraits<TString>());

		result = engine->RegisterStringFactory(gStrName, &g_stringFactory);

		//Behaviors
		result = engine->RegisterObjectBehaviour(gStrName,
																						 asBEHAVE_CONSTRUCT, 
																						 "void f()", 
																						 asFUNCTION(tStringConstructor), 
																						 asCALL_CDECL_OBJLAST);
		declaration = "void f(const " + name + "& in) ";
		result = engine->RegisterObjectBehaviour(gStrName,
																						 asBEHAVE_CONSTRUCT,
																						 declaration.c_str(),
																						 asFUNCTION(tStringCopyConstructor),
																						 asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectBehaviour(gStrName, 
																						 asBEHAVE_DESTRUCT, 
																						 "void f()", 
																						 asFUNCTION(tStringDestructor), 
																						 asCALL_CDECL_OBJLAST);
		//Operators
		declaration = "bool opEquals(const " + name + "&in) const";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTIONPR(tStringEquals, 
																					(const TString&, 
																					 const TString&), bool), 
																					asCALL_CDECL_OBJFIRST);
		declaration = name + " opAdd(const " + name + "&in) const";
		result = engine->RegisterObjectMethod(gStrName, 
																					declaration.c_str(),
																					asFUNCTIONPR(tStringAdd, 
																					(const TString&, 
																					 const TString&), TString),
																					asCALL_CDECL_OBJFIRST);
		declaration = name + "& opAssign(const " + name + "&in)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(tStringAssign),
																					asCALL_CDECL_OBJLAST);
		declaration = name + "& opAddAssign(const " + name + "&in)";
		result = engine->RegisterObjectMethod(gStrName, 
																					declaration.c_str(), 
																					asFUNCTION(tStringAddAssign), 
																					asCALL_CDECL_OBJLAST);
		//String methods
		result = engine->RegisterObjectMethod(gStrName, 
																					"uint length() const", 
																					asFUNCTION(tStringLength), 
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod(gStrName, 
																					"void resize(uint)", 
																					asFUNCTION(tStringResize), 
																					asCALL_CDECL_OBJLAST); 
		result = engine->RegisterObjectMethod(gStrName,
																					"bool empty() const",
																					asFUNCTION(tStringIsEmpty),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod(gStrName,
																					"uint8 opIndex(uint)",
																					asFUNCTION(tStringIndex),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod(gStrName,
																					"const uint8 opIndex(uint) const",
																					asFUNCTION(tStringIndex),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod(gStrName,
																					"void clear()",
																					asFUNCTION(tStringClear),
																					asCALL_CDECL_OBJLAST);
		result = engine->RegisterObjectMethod(gStrName,
																					"uint size() const",
																					asFUNCTION(tStringSize),
																					asCALL_CDECL_OBJLAST);
		declaration = "void push_back(" + name + "& in)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(tStringPushBack),
																					asCALL_CDECL_OBJLAST);
		declaration = "int compare(const " + name + "& in) const";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(tStringCompare),
																					asCALL_CDECL_OBJFIRST); 
		//right now string compare only works with strings
		result = engine->RegisterObjectMethod(gStrName,
																					"void pop_back()",
																					asFUNCTION(tStringPopBack),
																					asCALL_CDECL_OBJLAST);
		declaration = name + " substr(uint pos, uint len)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(tStringSubstr),
																					asCALL_CDECL_OBJLAST);
		declaration = "uint copy(" + name + "& out, int count, int pos)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(tStringCopy),
																				  asCALL_CDECL_OBJFIRST);
		declaration = "int find(" + name + "& in, int pos)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(tStringFind),
																					asCALL_CDECL_OBJFIRST);
		declaration = name + "& insert(int pos, const " + name + "& in)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str() ,
																					asFUNCTION(tStringInsert),
																					asCALL_CDECL_OBJFIRST);
		declaration = "int rfind(" + name + "& in, int pos)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(tStringRFind),
																					asCALL_CDECL_OBJFIRST);
		//...

		//Conversions

		declaration = name + " &opAssign(bool)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(AssignBool),
																					asCALL_CDECL_OBJLAST); 
		declaration = name + " &opAddAssign(bool)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(AddAssignBool),
																					asCALL_CDECL_OBJLAST);
		declaration = name + " opAdd(bool) const";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(AddBool),
																					asCALL_CDECL_OBJLAST); 
		declaration = name + " &opAssign(float)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(AssignFloat),
																					asCALL_CDECL_OBJLAST);
		declaration = name + " &opAddAssign(float)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(AddAssignFloat),
																					asCALL_CDECL_OBJLAST);
		declaration = name + " opAdd(float) const";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(AddFloat),
																					asCALL_CDECL_OBJLAST);
		declaration = name + " &opAssign(double)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(AssignDouble),
																					asCALL_CDECL_OBJLAST);
		declaration = name + " &opAddAssign(double)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(AddAssignDouble),
																					asCALL_CDECL_OBJLAST);
		declaration = name + " opAdd(double) const";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(AddDouble),
																					asCALL_CDECL_OBJLAST);
		declaration = name + " &opAssign(int)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(AssignInt32),
																					asCALL_CDECL_OBJLAST);
		declaration = name + " &opAddAssign(int)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(AddAssignInt32),
																					asCALL_CDECL_OBJLAST);
		declaration = name + " opAdd(int) const";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(AddInt32),
																					asCALL_CDECL_OBJLAST);
		declaration = name + " &opAssign(int64)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(AssignInt64),
																					asCALL_CDECL_OBJLAST);
		declaration = name + " &opAddAssign(int64)";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(AddAssignInt64),
																					asCALL_CDECL_OBJLAST);
		declaration = name + " opAdd(int64) const";
		result = engine->RegisterObjectMethod(gStrName,
																					declaration.c_str(),
																					asFUNCTION(AddInt64),
																					asCALL_CDECL_OBJLAST);


		//result = engine->RegisterObjectMethod(gStrName,
		//																			"",
		//																			asFUNCTION(),
		//																			asCALL_CDECL_OBJLAST);

		/*
			TO DO... (maybe)

			/////METHODS://///

			findFirstOf
			findLastOf
			findFirstNotOf
			findLastNotOF

		*/
	}

}
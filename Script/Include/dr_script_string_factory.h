#pragma once

#include <angelscript\angelscript.h>
#include <unordered_map>
#include "dr_script_prerequisites.h"

namespace driderSDK {

typedef std::unordered_map<TString, int> map_t;

/**
*  String factory class
*
* Sample usage:
*	Required by the script's engine, therefore it should not be used.
*/
class DR_SCRIPT_EXPORT StringFactory : public asIStringFactory
{
public:

	/**
	* Default constructor.
	*
	*/
	StringFactory();

	/**
	* Destructor.
	*
	*/
	~StringFactory();

	/**
	* Called by the script's engine to instantiate a string constant.
	*
	* @return
	*	  The pointer to the instantiated string constant
	*
	*/
	const void* 
	GetStringConstant(const char* data, asUINT length);

	/**
	* Called by the script's engine when the string constant is no longer used.
	*
	* @return
	*	  A negative value on error
	*
	*/
	int 
	ReleaseStringConstant(const void* str);

	/**
	* Called by the script's engine to get the raw string data.
	*
	* @return
	*	  A negative value on error
	*
	*/
	int 
	GetRawStringData(const void* str, char* data, asUINT* length) const;

	map_t m_stringCache;
};

}
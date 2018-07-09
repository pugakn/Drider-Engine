#include "dr_script_string_factory.h"
#include <dr_string_utils.h>

namespace driderSDK {

StringFactory::StringFactory() {}

StringFactory::~StringFactory() {
	DR_ASSERT(m_stringCache.size() == 0);
}

const void* 
StringFactory::GetStringConstant(const char* data, asUINT length) {

	WString stringData((const wchar_t*)data, length / m_unicode);
	TString string(StringUtils::toTString(stringData));

	map_t::iterator it = m_stringCache.find(string);

	if (it != m_stringCache.end()) {
		it->second++;
	}
	else {
		it = m_stringCache.insert(map_t::value_type(string, 1)).first;
	}

	return static_cast<const void*>(&it->first);
}

int 
StringFactory::ReleaseStringConstant(const void* str) {
	if (str == 0) {
		return asERROR;
	}
	map_t::iterator it = m_stringCache.find(*reinterpret_cast<const TString*>(str));
	if (it == m_stringCache.end()) {
		return asERROR;
	}
	it->second--;
	if (it->second == 0) {
		m_stringCache.erase(it);
	}
	return asSUCCESS;
}

int 
StringFactory::GetRawStringData(const void* str, char* data, asUINT* length) const {
	if (str == 0) {
		return asERROR;
	}
	if (length) {
		*length = (asUINT)reinterpret_cast<const TString*>(str)->length()* m_unicode;
	}
	if (data) {
		memcpy(data,
					 reinterpret_cast<const TString*>(str)->c_str(),
					 reinterpret_cast<const TString*>(str)->length()* m_unicode);
	}
	return asSUCCESS;
}

}
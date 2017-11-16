#pragma once

#include "dr_core_prerequisites.h"

namespace driderSDK {
/**
* TEST::md5Encrypting
*
* Encodes/Encrypts a string with the MD5 algorithm.
*
* @param initial_msg
*   String to be encoded/encrypted.
*
* @return
*   The encoded/encrypted string.
*/
std::string DR_CORE_EXPORT
MD5(const std::string& initial_msg);

}
#pragma once

#include <map>

#include "dr_core_prerequisites.h"

namespace driderSDK {

class DR_CORE_EXPORT Resource 
{
 public:
  /**
  * TEST::resourceConstructor
  *	Default constructor.
  */
  Resource() { }

  /**
  * TEST::resourceDestructor
  *	Default destructor.
  */
  virtual ~Resource() { };

  virtual void create(void* pResourceInfo) = 0;
};

}
#pragma once

#include <map>
#include <dr_memory.h>
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
  virtual ~Resource() { }

  //virtual Int32 getType() = 0;

  /**
  * Initialize the resource
  */
  virtual void
  init(void* /*pData*/)
  {}

  virtual void
  init(void* /*pData*/,
       void* /*extraData*/)
  {}
};

}
#pragma once
#include <dr_prerequisites.h>
#include <map>

namespace driderSDK {

class DR_API_EXPORT Resource 
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
};

}
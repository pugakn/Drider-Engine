#pragma once
#include <dr_prerequisites.h>

namespace driderSDK {

class DR_API_EXPORT Resource {
 public:
  /**
  * TEST::resourceConstructor
  * Default constructor
  */
  Resource();

  /**
  * TEST::resourceDestructor
  * Default destructor
  */
  virtual ~Resource(){};
};
}
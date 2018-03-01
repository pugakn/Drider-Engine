#pragma once
#include <string>
#include <fstream>
#include "dr_core_prerequisites.h"
#include "dr_module.h"
#include "dr_file_system.h"
#include <dr_string_utils.h>

namespace driderSDK
{

/**
* Vector with 2 elements (x, y)
*
* Sample usage:
* Logger sampleName;
* if (!sampleName.isStarted()) {
*   sampleName.startUp();
*   sampleName.reset();
* }
*/
class DR_CORE_EXPORT Logger : public Module<Logger>
{
 public:
  /**
  * TEST::defaultConstructor
  *
  * Default constructor.
  */
  Logger() {};

  /**
  * TEST::defaultDestructor
  *
  * Default destructor.
  */
  virtual
  ~Logger() {};

  /**
  * TEST::reset
  *
  * This function reset the content of the logger's html.
  */
  void
  reset();

  /**
  * TEST::addError
  *
  * Adds a message in the error area.
  * Use __FILE__ in the Filename parameter.
  * Use __LINE__ in the Filename parameter.
  */
  void
  addError(const String Filename,
           int lineNumber,
           const String message);

  /**
  * TEST::addWarning
  *
  * Adds a message in the warning area.
  * Use __FILE__ in the Filename parameter.
  * Use __LINE__ in the Filename parameter.
  */
  void
  addWarning(const String Filename,
             int lineNumber,
             const String message);

 private:
	 String m_filePath;
};

}
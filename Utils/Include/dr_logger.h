#pragma once
#include <fstream>
#include "dr_util_prerequisites.h"
#include "dr_module.h"
#include "dr_file_system.h"
#include <dr_string_utils.h>

namespace driderSDK
{

/**
* Outs information to file.
*
* Sample usage:
* Logger& sampleName = Logger::instance();
* if (!sampleName.isStarted()) {
*   sampleName.startUp();
* }
*/
class DR_UTIL_EXPORT Logger : public Module<Logger>
{
 public:
  /**
  * TEST::defaultConstructor
  *
  * Default constructor.
  */
  Logger(const TString& _filePath = _T("../Docs/Logger/logger.html"))
    : m_filePath(_filePath)
  {}

  /**
  * TEST::defaultDestructor
  *
  * Default destructor.
  */
  virtual
  ~Logger() {}

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
  static void
  addError(const String Filename,
           int lineNumber,
           const TString message);

  /**
  * TEST::addWarning
  *
  * Adds a message in the warning area.
  * Use __FILE__ in the Filename parameter.
  * Use __LINE__ in the Filename parameter.
  */
  static void
  addWarning(const String Filename,
             int lineNumber,
             const TString message);

  static void
  addLog(const TString message);

 protected:
  void
  onStartUp();

 private:
	 TString m_filePath;
};

}
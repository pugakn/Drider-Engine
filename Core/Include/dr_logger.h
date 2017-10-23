#pragma once
#include <string>
#include <fstream>
#include <dr_prerequisites.h>
#include "dr_module.h"

namespace driderSDK
{
class DR_API_EXPORT Logger : public Module<Logger>
{
 public:
  Logger() {};

  virtual
  ~Logger() {};

  /**
  * This function reset the content of the logger's html.
  *
  */
  void
  reset();
  /**
  * Adds a message in the error area.
  * Use __FILE__ in the Filename parameter.
  * Use __LINE__ in the Filename parameter.
  *
  */
  void
  addError(const std::string Filename,
           int lineNumber,
           const std::string message);

  /**
  * Adds a message in the warning area.
  * Use __FILE__ in the Filename parameter.
  * Use __LINE__ in the Filename parameter.
  *
  */
  void
  addWarning(const std::string Filename,
             int lineNumber,
             const std::string message);

 private:
  int LoggerCalls;
  int WarningCalls;
  int ErrorCalls;

  std::string filePath;
};

}
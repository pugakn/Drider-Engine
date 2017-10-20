#include "dr_logger.h"

void
driderSDK::Logger::reset() {
  std::ofstream loggerHtmlFile;
  loggerHtmlFile.open("logger.html", std::ios::out | std::ios::trunc);

  loggerHtmlFile <<
  "<!DOCTYPE html>\n\
  <html>\n\
    <head>\n\
      <meta charset = \"UTF-8\">\n\
      <link rel = \"stylesheet\" href = \"loggerStyle.css\">\n\
      <title>Drider Log</title>\n\
    </head>\n\
    <body>\n\
      <div class = \"bodyContent\">\n\
        <section class = \"section\">\n\
          <a class = \"errorTitle\">ERRORS</a>\n\
        </section>\n\
        <article class = \"section\">\n\
          <a class = \"warningTitle\">WARNINGS</a>\n\
        </article>\n\
      </div>\n\
    </body>\n\
  </html> ";

  loggerHtmlFile.close();
  LoggerCalls = 0;
  WarningCalls = 0;
  ErrorCalls = 0;
}

void
driderSDK::Logger::addError(const std::string Filename,
                 int lineNumber,
                 const std::string message) {
  std::string fullMessage("");
  fullMessage += "<a class = \"file\"> File: ";
  fullMessage += Filename;
  fullMessage += "</a>\n";
  fullMessage += "<a class = \"line\"> Line: ";
  fullMessage += std::to_string(lineNumber);
  fullMessage += "</a>\n";
  fullMessage += "<a class = \"text\"> Description: ";
  fullMessage += message;
  fullMessage += "</a>\n";

  std::ifstream loggerHtmlFile("logger.html");
  std::string fileInString = std::string(std::istreambuf_iterator<char>(loggerHtmlFile),
                                         std::istreambuf_iterator<char>());
  loggerHtmlFile.close();

  size_t lastErrorIndex = fileInString.find("</section>");
  fileInString.insert(lastErrorIndex, fullMessage);

  std::ofstream finalFile("logger.html", std::ofstream::out | std::ofstream::trunc);
  finalFile << fileInString;
  finalFile.close();

  LoggerCalls++;
  ErrorCalls++;
}

void
driderSDK::Logger::addWarning(const std::string Filename,
                   int lineNumber,
                   const std::string message) {
  std::string fullMessage("");
  fullMessage += "<a class = \"file\"> File: ";
  fullMessage += Filename;
  fullMessage += "</a>\n";
  fullMessage += "<a class = \"line\"> Line: ";
  fullMessage += std::to_string(lineNumber);
  fullMessage += "</a>\n";
  fullMessage += "<a class = \"text\"> Description: ";
  fullMessage += message;
  fullMessage += "</a>\n";

  std::ifstream loggerHtmlFile("logger.html");
  std::string fileInString = std::string(std::istreambuf_iterator<char>(loggerHtmlFile),
                                         std::istreambuf_iterator<char>());
  loggerHtmlFile.close();

  size_t lastErrorIndex = fileInString.find("</article>");
  fileInString.insert(lastErrorIndex, fullMessage);

  std::ofstream finalFile("logger.html", std::ofstream::out | std::ofstream::trunc);
  finalFile << fileInString;
  finalFile.close();

  LoggerCalls++;
  WarningCalls++;
}
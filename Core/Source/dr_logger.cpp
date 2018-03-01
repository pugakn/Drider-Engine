#include "dr_logger.h"
#include <dr_string_utils.h>

namespace driderSDK
{

void
Logger::reset() {
  m_filePath = "../Docs/Logger/logger.html";

  FileSystem fSys;
  fSys.Remove(m_filePath);

  File file;
  bool htmlCreated = fSys.CreateAndOpen(m_filePath, file);

  String html =
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

  if (htmlCreated) {
    file.Write(html.size(), html.c_str());
    file.Close();
  }
}

void
Logger::addError(const std::string Filename,
                 int lineNumber,
                 const String message) {
	String fullMessage("");
  fullMessage += "<a class = \"file\"> File: ";
  fullMessage += StringUtils::toString(Filename);
  fullMessage += "</a>\n";
  fullMessage += "<a class = \"line\"> Line: ";
  fullMessage += StringUtils::toString(lineNumber);
  fullMessage += "</a>\n";
  fullMessage += "<a class = \"text\"> Description: ";
  fullMessage += message;
  fullMessage += "</a>\n";

  File file;
  std::ifstream loggerHtmlFile(m_filePath);
	String fileInString = String(std::istreambuf_iterator<char>(loggerHtmlFile),
                               std::istreambuf_iterator<char>());
  loggerHtmlFile.close();
  size_t lastErrorIndex = fileInString.find("</section>");
  fileInString.insert(lastErrorIndex, fullMessage);

  std::ofstream finalFile(m_filePath, std::ofstream::out | std::ofstream::trunc);
  finalFile << StringUtils::toString(fileInString);
  finalFile.close();
}

void
Logger::addWarning(const String Filename,
                   int lineNumber,
                   const String message) {
  String fullMessage("");
  fullMessage += "<a class = \"file\"> File: ";
  fullMessage += StringUtils::toString(Filename);
  fullMessage += "</a>\n";
  fullMessage += "<a class = \"line\"> Line: ";
  fullMessage += StringUtils::toString(lineNumber);
  fullMessage += "</a>\n";
  fullMessage += "<a class = \"text\"> Description: ";
  fullMessage += message;
  fullMessage += "</a>\n";

  std::ifstream loggerHtmlFile(m_filePath);
	String fileInString = String(std::istreambuf_iterator<char>(loggerHtmlFile),
                               std::istreambuf_iterator<char>());
  loggerHtmlFile.close();

  size_t lastErrorIndex = fileInString.find("</article>");
  fileInString.insert(lastErrorIndex, fullMessage);

  std::ofstream finalFile(m_filePath, std::ofstream::out | std::ofstream::trunc);
  finalFile << StringUtils::toString(fileInString);
  finalFile.close();
}

}
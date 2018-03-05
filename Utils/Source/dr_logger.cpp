#include "dr_logger.h"
#include <dr_string_utils.h>

namespace driderSDK
{

void
Logger::reset() {
  m_filePath = _T("../Docs/Logger/logger.html");

  FileSystem fSys;
  fSys.Remove(m_filePath);

  File file;
  bool htmlCreated = fSys.CreateAndOpen(m_filePath, file);

  TString html =
  _T("<!DOCTYPE html>\n\
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
  </html> ");

  if (htmlCreated) {
    file.Write(html.size(), StringUtils::toString(html).c_str());
    file.Close();
  }
}

void
Logger::addError(const String Filename,
                 int lineNumber,
                 const TString message) {
	TString fullMessage(_T(""));
  fullMessage += _T("<a class = \"file\"> File: ");
  fullMessage += StringUtils::toTString(Filename);
  fullMessage += _T("</a>\n");
  fullMessage += _T("<a class = \"line\"> Line: ");
  fullMessage += lineNumber;
  fullMessage += _T("</a>\n");
  fullMessage += _T("<a class = \"text\"> Description: ");
  fullMessage += message;
  fullMessage += _T("</a>\n");

  File file;
  std::ifstream loggerHtmlFile(m_filePath);
	TString fileInString = TString(std::istreambuf_iterator<char>(loggerHtmlFile),
                                 std::istreambuf_iterator<char>());
  loggerHtmlFile.close();
  size_t lastErrorIndex = fileInString.find(_T("</section>"));
  fileInString.insert(lastErrorIndex, fullMessage);

  std::ofstream finalFile(m_filePath, std::ofstream::out | std::ofstream::trunc);
  finalFile << StringUtils::toString(fileInString);
  finalFile.close();
}

void
Logger::addWarning(const String Filename,
                   int lineNumber,
                   const TString message) {
  TString fullMessage(_T(""));
  fullMessage += _T("<a class = \"file\"> File: ");
  fullMessage += StringUtils::toTString(Filename);
  fullMessage += _T("</a>\n");
  fullMessage += _T("<a class = \"line\"> Line: ");
  fullMessage += lineNumber;
  fullMessage += _T("</a>\n");
  fullMessage += _T("<a class = \"text\"> Description: ");
  fullMessage += message;
  fullMessage += _T("</a>\n");

  std::ifstream loggerHtmlFile(m_filePath);
	TString fileInString = TString(std::istreambuf_iterator<char>(loggerHtmlFile),
                                 std::istreambuf_iterator<char>());
  loggerHtmlFile.close();

  size_t lastErrorIndex = fileInString.find(_T("</article>"));
  fileInString.insert(lastErrorIndex, fullMessage);

  std::ofstream finalFile(m_filePath, std::ofstream::out | std::ofstream::trunc);
  finalFile << StringUtils::toString(fileInString);
  finalFile.close();
}

}
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

  TString html = _T("\
<!DOCTYPE html>\
<html>\
<head>\
  <meta charset = \"UTF-8\">\
  <link rel = \"stylesheet\" href = \"loggerStyle.css\">\
  <title>Drider Log</title>\
</head>\
<body>\
  <div class = \"bodyContent\">\
  </div>\
</body>\
</html>\
");

  if (htmlCreated) {
    file.Write(html.size(), StringUtils::toString(html).c_str());
    file.Close();
  }
}

void
Logger::addError(const String Filename,
                 int lineNumber,
                 const TString message) {
  printf("Error: %s", StringUtils::toString(message).c_str());

  printf("%s\n", message);
	TString fullMessage(_T(""));
  fullMessage += _T("<section class = \"section\">");
  fullMessage += _T("<a class = \"errorTitle\">Error</a>");
  fullMessage += _T("<a class = \"line\">Line: ");
  fullMessage += StringUtils::toTString(lineNumber);
  fullMessage += _T("</a>");
  fullMessage += _T("<a class = \"file\">File: ");
  fullMessage += StringUtils::toTString(Filename);
  fullMessage += _T("</a>");
  fullMessage += _T("<a class = \"text\">");
  fullMessage += message;
  fullMessage += _T("</a>\n");
  fullMessage += _T("</section>");

  File file;
  std::ifstream loggerHtmlFile(m_filePath);
	TString fileInString = TString(std::istreambuf_iterator<char>(loggerHtmlFile),
                                 std::istreambuf_iterator<char>());
  loggerHtmlFile.close();
  size_t lastErrorIndex = fileInString.find(_T("</div>"));
  fileInString.insert(lastErrorIndex, fullMessage);

  std::ofstream finalFile(m_filePath, std::ofstream::out | std::ofstream::trunc);
  finalFile << StringUtils::toString(fileInString);
  finalFile.close();
}

void
Logger::addWarning(const String Filename,
                   int lineNumber,
                   const TString message) {
  printf("Warning: %s\n", StringUtils::toString(message).c_str());

  TString fullMessage(_T(""));
  fullMessage += _T("<section class = \"section\">");
  fullMessage += _T("<a class = \"warningTitle\">Warning</a>");
  fullMessage += _T("<a class = \"line\">Line: ");
  fullMessage += StringUtils::toTString(lineNumber);
  fullMessage += _T("</a>");
  fullMessage += _T("<a class = \"file\">File: ");
  fullMessage += StringUtils::toTString(Filename);
  fullMessage += _T("</a>");
  fullMessage += _T("<a class = \"text\">");
  fullMessage += message;
  fullMessage += _T("</a>\n");
  fullMessage += _T("</section>");

  std::ifstream loggerHtmlFile(m_filePath);
	TString fileInString = TString(std::istreambuf_iterator<char>(loggerHtmlFile),
                                 std::istreambuf_iterator<char>());
  loggerHtmlFile.close();

  size_t lastErrorIndex = fileInString.find(_T("</div>"));
  fileInString.insert(lastErrorIndex, fullMessage);

  std::ofstream finalFile(m_filePath, std::ofstream::out | std::ofstream::trunc);
  finalFile << StringUtils::toString(fileInString);
  finalFile.close();
}

void
Logger::addLog(const TString message) {
  printf("%s\n", StringUtils::toString(message).c_str());

  TString fullMessage(_T(""));
  fullMessage += _T("<section class = \"section\">");
  fullMessage += _T("<a class = \"logTitle\">Log</a>");
  fullMessage += _T("<a class = \"line\"></a>");
  fullMessage += _T("<a class = \"file\"></a>");
  fullMessage += _T("<a class = \"text\">");
  fullMessage += message;
  fullMessage += _T("</a>\n");
  fullMessage += _T("</section>");

  std::ifstream loggerHtmlFile(m_filePath);
  TString fileInString = TString(std::istreambuf_iterator<char>(loggerHtmlFile),
    std::istreambuf_iterator<char>());
  loggerHtmlFile.close();

  size_t lastErrorIndex = fileInString.find(_T("</div>"));
  fileInString.insert(lastErrorIndex, fullMessage);

  std::ofstream finalFile(m_filePath, std::ofstream::out | std::ofstream::trunc);
  finalFile << StringUtils::toString(fileInString);
  finalFile.close();
}

void
Logger::onStartUp() {
  reset();
}

}
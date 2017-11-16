#include "dr_application.h"

namespace driderSDK {
void Application::startApplication()
{
  m_alive = true;
  onInit();
  while (m_alive) {
    while (m_running)
    {
      onInput();
      onUpdate();
    }
    onPause();
  }
  onDestroy();
}
void Application::stopApplication()
{
  pauseApplication();
  m_alive = false;
}
void Application::pauseApplication()
{
  m_running = false;
}
void Application::resumeApplication()
{
  m_running = true;
  onResume();
}
}
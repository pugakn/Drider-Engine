#pragma once
#include "dr_core_prerequisites.h"
#include <atomic>
namespace driderSDK {
class DR_CORE_EXPORT Application
{
public:
  virtual ~Application() {}
  void initWindow();
  virtual void onInit() = 0;
  virtual void onInput() = 0;
  virtual void onUpdate() = 0;
  virtual void onDraw() = 0;
  virtual void onDestroy() = 0;
  virtual void onPause() = 0;
  virtual void onResume() = 0;

  void startApplication();
  void stopApplication();
  void pauseApplication();
  void resumeApplication();
private:
  std::atomic<bool> m_alive;
  std::atomic<bool> m_running;
};
}
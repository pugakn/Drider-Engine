#pragma once

#include <atomic>
#include <dr_graphics_api.h>
#include <dr_memory.h>
#include "dr_core_prerequisites.h"
#include "dr_viewport.h"

namespace driderSDK {
class DR_CORE_EXPORT Application
{
 public:
  virtual ~Application() {}
  
  Int32 run();

 protected:
  virtual void postInit() = 0;
  virtual void postUpdate() = 0;
  virtual void postRender() = 0;
  virtual void postDestroy() = 0;
 private:
  void init();
  void createWindow();
  void update();
  void render();
  void destroy();
 protected:
  Viewport m_viewport;
  void* m_hwnd;
 private:
  std::atomic<bool> m_running;
};
}
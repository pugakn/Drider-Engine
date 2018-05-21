#pragma once

#include <atomic>
#include <dr_memory.h>
#include "dr_core_prerequisites.h"
#include "dr_viewport.h"
class SDL_Window;
namespace driderSDK {
class DR_CORE_EXPORT Application
{
 public:
  virtual ~Application() {}
  
  Int32 run(const Viewport& _viewport);
  static void setViewport(const Viewport& _viewport);
  static Viewport getViewPort();
 protected:
  virtual void postInit() = 0;
  virtual void postUpdate() = 0;
  virtual void postRender() = 0;
  virtual void postDestroy() = 0;

  virtual void onResize() = 0;
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
  static Application* application;
  static Application& getApplication();
  std::atomic<bool> m_running;
  SDL_Window* m_window;
};
}
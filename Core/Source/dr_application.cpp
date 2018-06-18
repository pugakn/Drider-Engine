#include "dr_application.h"
#include <iostream>
#include <Windows.h>
#include <SDL2/SDL.h>

namespace driderSDK {
Application* Application::application;
Int32 Application::run(const Viewport& _viewport) {
  if (Application::application) {
    std::cout << "Application already running" << std::endl;
    return 0;
  }
  Application::application = this;
  m_viewport = _viewport;
  init();

  while (m_running) {
    update();
    render();
  }

  destroy();

  return 0;
}
Viewport Application::getViewPort()
{
  return getApplication().m_viewport;
}
void Application::setViewport(const Viewport & _viewport)
{
  Application& app = getApplication();
  app.m_viewport = _viewport;
  SDL_SetWindowSize(app.m_window, app.m_viewport.width, app.m_viewport.height);
}
void 
Application::init() {
  m_running = true;
    
  createWindow();

  m_hwnd = GetActiveWindow();

  postInit();
}
void 
Application::createWindow() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Video initialization failed: " << SDL_GetError() << std::endl;
  }

  m_window = SDL_CreateWindow("Drider Engine", 
                              SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED, 
                              m_viewport.width, 
                              m_viewport.height, 
                              SDL_WINDOW_RESIZABLE);

  if (!m_window) {
    std::cout << "Error creating SDL window " << std::endl;
    exit(666);
  }
}

void 
Application::update() { 
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      m_running = false;
    }
    if (event.type == SDL_WINDOWEVENT) {
      if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
        m_viewport.width = event.window.data1;
        m_viewport.height = event.window.data2;
        onResize();
      }
    }
  }

  postUpdate();
}

void 
Application::render() {
  postRender();
}

void 
Application::destroy() {
  SDL_Quit();
  
  postDestroy();
}

Application&
Application::getApplication() {
  return *Application::application;
}

}
#include "dr_application.h"
#include <iostream>
#include <Windows.h>
#include <SDL\SDL.h>
#include <dr_d3d_graphics_api.h>

namespace driderSDK {

int Application::run() {

  init();

  while (m_running) {
    update();
    render();
  }

  destroy();

  return 0;
}
void 
Application::init() {
  m_running = true;

  m_viewport.width = 1280;
  m_viewport.height = 720;
    
  createWindow();

  m_graphicsAPI = dr_make_unique<D3DGraphicsAPI>();

  HWND win = GetActiveWindow();

  m_graphicsAPI->init(m_viewport.width, 
                      m_viewport.height, 
                      win);

  postInit();
}
void 
Application::createWindow() {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Video initialization failed: " << SDL_GetError() << std::endl;
  }

  SDL_WM_SetCaption("Drider 2018", 0);

  Int32 flags = SDL_HWSURFACE;
  
  //flags |= SDL_FULLSCREEN;
  //flags |= SDL_RESIZABLE;

  SDL_ShowCursor(SDL_ENABLE);
  
  int width = 1280;
  int height = 720;

  if (SDL_SetVideoMode(width, height, 32, flags) == 0) {
    std::cout << "Video mode set failed: " << SDL_GetError() << std::endl;
  }
}
void 
Application::update() {
 
  SDL_Event event;

  while (SDL_PollEvent(&event)){}

  postUpdate();
}

void 
Application::render() {

  m_graphicsAPI->clear();

  postRender();
  
  m_graphicsAPI->swapBuffers();
}

void 
Application::destroy() {
  SDL_Quit();
  m_graphicsAPI->destroy();
  postDestroy();
}

}
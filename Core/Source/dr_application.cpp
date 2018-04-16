#include "dr_application.h"
#include <iostream>
#include <Windows.h>
#include <SDL\SDL.h>

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

  //m_viewport.width = 1920;
  //m_viewport.height = 1024;
  m_viewport.width = 1280;
  m_viewport.height = 720;
    
  createWindow();

  m_hwnd = GetActiveWindow();

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
  flags |= SDL_RESIZABLE;

  SDL_ShowCursor(SDL_ENABLE);

  if (SDL_SetVideoMode(m_viewport.width, m_viewport.height, 32, flags) == 0) {
    std::cout << "Video mode set failed: " << SDL_GetError() << std::endl;
  }
}
void 
Application::update() {
 
  SDL_Event event;

  while (SDL_PollEvent(&event)){
    if (event.type == SDL_QUIT) {
      m_running = false;
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

}
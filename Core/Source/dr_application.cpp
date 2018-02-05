#include "dr_application.h"
#include <SDL\SDL.h>
#include <iostream>
namespace driderSDK {
void Application::startApplication()
{
  initWindow();
  m_alive = true;
  m_running = true;
  onInit();
  while (m_alive) {
    while (m_running)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
      }
      onInput();
      onUpdate();
      onDraw();
    }
    onPause();
  }
  onDestroy();
  SDL_Quit();
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
void Application::initWindow()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Video initialization failed: " << SDL_GetError() << std::endl;
  }
  SDL_WM_SetCaption("Drider 2017", 0);
  int flags = SDL_HWSURFACE;
  //flags |= SDL_FULLSCREEN;
  //flags |= SDL_RESIZABLE;
  SDL_ShowCursor(SDL_ENABLE);
  int width = 1280;
  int height = 720;
  if (SDL_SetVideoMode(width, height, 32, flags) == 0) {
    std::cout << "Video mode set failed: " << SDL_GetError() << std::endl;
  }
}
}
#include "TestApplication.h"
#include <SDL\SDL.h>
#include <iostream>
#include <Windows.h> //TODO: remove

#include <dr_d3d_swap_chain.h>

namespace driderSDK {

TestApplication::TestApplication()
{
}


TestApplication::~TestApplication()
{
}
void TestApplication::onInit()
{
  initWindow();
  driver = new D3DGraphicsAPI;
  HWND win = GetActiveWindow();
  driver->init(1280, 720,win );

  quad.init(*driver->device);
  
}
void TestApplication::onInput()
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
  }
}
void TestApplication::onUpdate()
{
}
void TestApplication::onDraw()
{
  driver->clear();
  quad.draw(*driver->deviceContext);
  driver->swapBuffers();
}
void TestApplication::onDestroy()
{
  SDL_Quit();
}
void TestApplication::onPause()
{
}
void TestApplication::onResume()
{
}




void TestApplication::initWindow()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Video initialization failed: " << SDL_GetError()<<std::endl;
  }
  SDL_WM_SetCaption("Drider 2017", 0);
  int flags = SDL_HWSURFACE;
  //flags |= SDL_FULLSCREEN;
  //flags |= SDL_RESIZABLE;
  int width = 1280;
  int height = 720;
  if (SDL_SetVideoMode(width, height, 32, flags) == 0) {
    std::cout << "Video mode set failed: " << SDL_GetError() << std::endl;
  }
}
}

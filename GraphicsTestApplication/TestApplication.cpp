#include "TestApplication.h"
#include <SDL\SDL.h>
#include <iostream>
#include <Windows.h> //TODO: remove
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
  driver->init(1280, 720, GetActiveWindow());
}
void TestApplication::onInput()
{
}
void TestApplication::onUpdate()
{
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
  SDL_WM_SetCaption("Drider Confidential 2017", 0);
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

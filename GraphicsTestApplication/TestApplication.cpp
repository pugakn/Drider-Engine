#include "TestApplication.h"
#include <SDL\SDL.h>
#include <iostream>
#include <tuple>
#include <Windows.h> //TODO: remove
#include <dr_d3d_swap_chain.h>
#include <dr_rasterizer_state.h>
#include <dr_radian.h>
#include <dr_quaternion.h>
#include <dr_file.h>
#include <dr_vertex_shader.h>
#include <dr_fragment_shader.h>
#include <dr_string_utils.h>

namespace driderSDK {

TestApplication::TestApplication() 
  : viewport{0,0,1280, 720},
    camera(_T("MainCamera"), {0, 100, 200}, {0,0,0}, viewport, 45.f, 0.1f, 1000.f )
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
  driver->init(viewport.width, viewport.height, win );

  ResourceManager::startUp();
  ResourceManager* pInstance;
  if (ResourceManager::isStarted()) {
     pInstance = &ResourceManager::instance();
  }
  
  //createShaders();

  std::vector<TString> modelsFiles{_T("dwarf.x")};

  models.resize(modelsFiles.size());

  quad.init(*driver->device);

  for (SizeT i = 0; i < modelsFiles.size(); ++i) {
    models[i].init(*driver->device, modelsFiles[i]);
  }

}
void TestApplication::onInput()
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_UP)
      {
        camera.move(5.f, 0.f);
      }
      if (event.key.keysym.sym == SDLK_DOWN)
      {
        camera.move(-5.f, 0.f);
      }
      if (event.key.keysym.sym == SDLK_r)
      {
        std::vector<TString> modelsFiles{_T("Croc.X")};

        models[0].destroy();
        models[0].init(*driver->device, modelsFiles[0]);
      }
    break;
    }
  }
}
void TestApplication::onUpdate()
{
  for (auto& model : models)
  {
    //model.transform.rotate(Radian(0.005f), AXIS::kY);
    model.update();
  }
    
  camera.update(0);
}
void TestApplication::onDraw()
{
  driver->clear();
  //quad.draw(*driver->deviceContext, camera.getVP());
  for (auto& model : models)
    model.draw(*driver->deviceContext, camera);
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
void TestApplication::createShaders() {

  if(!m_shaders.empty()) {
    //Free shaders
    m_shaders.clear();
  }

  static std::vector<std::tuple<String, String, DR_SHADER_TYPE_FLAG::E>> 
    shadersFiles = {{"vs.hlsl", "AnimMeshVS", DR_SHADER_TYPE_FLAG::kVertex},
                    {"fs.hlsl", "MeshFS", DR_SHADER_TYPE_FLAG::kFragment},
                    {"mesh.hlsl", "StaticMeshVS", DR_SHADER_TYPE_FLAG::kVertex}};

  auto shaderFactory = 
  [&](DR_SHADER_TYPE_FLAG::E shaderType, const String& shaderFile) -> Shader* { 

    File file;
    file.Open(StringUtils::toTString(shaderFile));
    auto shaderSrc = StringUtils::toString(file.GetAsString(file.Size()));
    return driver->device->createShaderFromMemory(shaderSrc.data(), 
                                                  shaderSrc.size(),
                                                  shaderType);  
  };

  for(auto& shaderFile : shadersFiles) {
    m_shaders[std::get<1>(shaderFile)] = shaderFactory(std::get<2>(shaderFile), 
                                                       std::get<0>(shaderFile));
  }
}

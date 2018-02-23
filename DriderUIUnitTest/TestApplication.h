#pragma once
#include <unordered_map>
#include <vector>
#include <dr_util_prerequisites.h>
#include <dr_d3d_graphics_api.h>
#include <dr_camera.h>
#include <dr_viewport.h>
#include "dr_application.h"
#include <dr_graph.h>
#include <dr_memory.h>
#include <dr_shader.h>

#include <Windows.h>
#include <iostream>
#include <dr_web_renderer.h>
#include <dr_quad.h>

#include"dr_input_manager.h"
#include "dr_input_keyboard.h"
#include "dr_input_mouse.h"
namespace driderSDK {

class Technique;
  
class TestApplication : public Application
{
public:
  TestApplication();
  ~TestApplication();

  void onInit() override;
  void onInput() override;
  void onUpdate() override;
  void onDraw() override;
  void onDestroy() override;
  void onPause() override;
  void onResume() override;

  
  GraphicsAPI* driver;
  Viewport viewport;
  WebRenderer webRenderer;
  Quad quad;

};

}


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
namespace driderSDK {

class Technique;
  
class TestApplication : public Application
{
public:
  TestApplication();
  ~TestApplication();

  void postInit() override;
  void postUpdate() override;
  void postRender() override;
  void postDestroy() override;

  
  Viewport viewport;
  WebRenderer webRenderer;
  Quad quad;
private:


};

}


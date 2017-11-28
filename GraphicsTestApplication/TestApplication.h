#pragma once
#include <dr_util_prerequisites.h>
#include <dr_d3d_graphics_api.h>

#include "dr_application.h"
#include "dr_quad.h"
namespace driderSDK {
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

  void initWindow();

  GraphicsAPI* driver;
  Quad quad;
};

}


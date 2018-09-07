#pragma once
#include "dr_application.h"
#include <Ultralight/Ultralight.h>

namespace driderSDK {
  
class TestApplication : public Application
{
public:
  /*
  * Default class constructor.
  */
  TestApplication();

  /*
  * Default class destructor.
  */
  ~TestApplication();

  void
  postInit() override;

  void
  postUpdate() override;

  void
  postRender() override;

  void
  postDestroy() override;

  void
  onResize() override;

private:
  ultralight::Platform& platform;
};

}
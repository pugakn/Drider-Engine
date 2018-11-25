#pragma once
#include "dr_engine_prerequisites.h"

#include <dr_serializable.h>

namespace driderSDK {

class SceneCore;

class DR_ENGINE_EXPORT Scene {

public: 
  void
  save();

  void 
  open();

private:
  SceneCore *m_scene;

};

}
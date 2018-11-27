#pragma once
#include "dr_engine_prerequisites.h"

#include <memory>

namespace driderSDK {

class GameObject;
class File;

class DR_ENGINE_EXPORT SerializableData {
  public:
    virtual void
    load(File &file, std::shared_ptr<GameObject> obj) = 0;
};

}
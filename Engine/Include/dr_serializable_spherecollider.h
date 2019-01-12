#pragma once
#include "dr_engine_prerequisites.h"
#include <dr_serializable_data.h>

namespace driderSDK {

class DR_ENGINE_EXPORT sSphereCollider : public SerializableData {
  public:
    void
    load(File &file,
         std::shared_ptr<GameObject> obj) override;
};

}

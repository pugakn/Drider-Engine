#pragma once

#include <dr_vector3d.h>
#include <dr_vector4d.h>
#include "dr_core_prerequisites.h"

namespace driderSDK {

  struct Vertex 
  {
    Vector4D position;
    Vector4D normal;
    Vector4D bonesWeights;
    Int32 bonesIDs[4]{-1, -1, -1, -1};

    void addBone(Int32 id, float weight) { 
      for (Int32 i = 0; i < 4; ++i){
        if (bonesIDs[i] == -1){
          bonesIDs[i] = id;
          bonesWeights[i] = weight;
          return;
        }
      }
      DR_ASSERT(false);
    }
  };
}

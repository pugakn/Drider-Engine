#pragma once

#include <vector>
#include <dr_vector3d.h>
#include <dr_vector4d.h>
#include <dr_vector2d.h>
#include <dr_graphics_defines.h>
#include "dr_core_prerequisites.h"

namespace driderSDK {

struct DR_CORE_EXPORT Vertex 
{
  using InputLayoutDesc = std::vector<DrInputElementDesc>;
  
  Vector4D position;
  Vector4D normal;
  Vector4D binormal;
  Vector4D tangent;
  Vector4D bonesWeights;
  Int32 bonesIDs[4]{-1, -1, -1, -1};
  Vector2D uv;
  
  void
  addBone(Int32 id, float weight) { 
    for (Int32 i = 0; i < 4; ++i){
      if (bonesIDs[i] == -1){
        bonesIDs[i] = id;
		bonesWeights[i] = weight;
		return;
      }
    }
    DR_ASSERT(false);
  }
  
  static const InputLayoutDesc&
  getInputDesc();
};

}
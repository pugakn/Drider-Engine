#pragma once

#include <dr_vector3d.h>
#include <dr_vector4d.h>

namespace driderSDK {

struct Vertex 
{
 public:

  Vertex() 
  : bonesIDs{-1, -1, -1, -1}  
  {
  }

  void addBoneData(Int32 boneID, float boneWeight) 
  {
    for (Int8 i = 0; i < 4; ++i) {
      if(bonesIDs[i] == -1){
        bonesIDs[i] = boneID;
        bonesWeights[i] = boneWeight;
        return;
      }
    }
  }

  Vector3D position;
  Vector3D normal;
  Int32 bonesIDs[4];
  Vector4D bonesWeights;
};

}

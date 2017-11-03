#pragma once

#include <dr_prerequisites.h>
#include <dr_vector3d.h>

namespace driderSDK {

class Matrix4x4;

struct Transform 
{
public:

  Matrix4x4 getTransformMatrix() const;
  
  Vector3D position;
  Vector3D rotation;
  Vector3D scale;
};

}

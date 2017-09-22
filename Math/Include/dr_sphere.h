#pragma once

#include <dr_prerequisites.h>
#include <dr_vector3d.h>

namespace driderSDK {

class DR_API_EXPORT Sphere
{

 public:
  Sphere();
  //Sphere(Float32 radio, Vector3D position);
  Float32 GetRadio();
  Vector3D GetPosition();

  //void SetRadio();
  //void SetPosition();

 protected:
 private:
  Float32 m_radio;
  Vector3D m_position;

};

}
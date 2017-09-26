#pragma once

#include <dr_prerequisites.h>
#include "dr_vector3d.h"

namespace driderSDK {

class DR_API_EXPORT Sphere
{

 public:
  //Uncommented
  Sphere();

  //Uncommented
  FORCEINLINE float 
  getRadio() const
  {
   return m_radio;
  }
  
  //Uncommented
  FORCEINLINE Vector3D
  getCenter() const
  {
   return m_center;
  }

  //void SetRadio();
  //void SetPosition();

 protected:
 private:
  float m_radio;
  Vector3D m_center;

};

}
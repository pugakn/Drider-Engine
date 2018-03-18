#pragma once
#include "dr_renderman_prerequisites.h"
#include <dr_vector4d.h>

namespace driderSDK {

/**
* Temporally "point" light
*/
class DR_RENDERMAN_EXPORT Light
{
 public:
  
  /**
  * TEST::TestName
  *  Description
  *
  * @return
  *  Description
  */
  Light()
   : m_vec4Position({ 0.0f, 0.0f, 0.0f, 1.0f }),
     m_fIntensity(1.0f),
     m_vec4Color({1.0f, 1.0f, 1.0f, 1.0f}) {}

  /**
  * TEST::TestName
  *  Description
  *
  * @return
  *  Description
  */
  virtual ~Light() {};

  Vector4D m_vec4Position;

  float m_fIntensity;

  Vector4D m_vec4Color;
};
 
}
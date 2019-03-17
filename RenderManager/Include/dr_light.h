#pragma once
#include "dr_renderman_prerequisites.h"
#include <dr_vector4d.h>

namespace driderSDK {

/**
* Temporally "point" light
*/
class DR_RENDERMAN_EXPORT PointLight
{
 public:
  
  /**
  * TEST::TestName
  *  Description
  *
  * @return
  *  Description
  */
   PointLight()
   : m_vec4Position({ 0.0f, 0.0f, 0.0f, 1.0f }),
     m_vec4Color({1.0f, 1.0f, 1.0f, 1.0f}) {}

  /**
  * TEST::TestName
  *  Description
  *
  * @return
  *  Description
  */
  virtual ~PointLight() {};

  Vector4D m_vec4Position;

  Vector4D m_vec4Color;
};

/**
* Temporally "point" light
*/
class DR_RENDERMAN_EXPORT DirectionalLight
{
 public:
  
  /**
  * TEST::TestName
  *  Description
  *
  * @return
  *  Description
  */
   DirectionalLight()
   : m_vec4Direction({ 0.0f, 0.0f, 0.0f, 1.0f }),
     m_vec4Color({1.0f, 1.0f, 1.0f, 1.0f}) {}

  /**
  * TEST::TestName
  *  Description
  *
  * @return
  *  Description
  */
  virtual ~DirectionalLight() {};

  Vector4D m_vec4Direction;

  Vector4D m_vec4Color;
};
 
}
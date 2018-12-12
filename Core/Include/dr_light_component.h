#pragma once

#include <dr_memory.h>
#include <dr_vector3d.h>

#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"
#include <dr_id_object.h>

namespace driderSDK {

class DR_CORE_EXPORT LightComponent : public GameComponent,
								      public IDClass<LightComponent>
{
 public:
  LightComponent(GameObject& _gameObject);

  /**
  * Sets the light color.
  *
  * @param newColor
  *  The light new color.
  */
  void
  SetColor(const Vector3D& newColor);

  /**
  * Sets the light range.
  *
  * @param newRange
  *  The light new range.
  */
  void
  SetRange(const float newRange);

  /**
  * Sets the light intensity.
  *
  * @param newIntensity
  *  The light new intensity.
  */
  void
  SetIntensity(const float newIntensity);
  
  /**
  * Returns the position and range of the light
  * compressed in a vec4.
  *
  * @return
  *   Compressed vector4D.
  */
  Vector4D
  GetPositionRange();

  /**
  * Returns the color and intensity of the light
  * compressed in a vec4.
  *
  * @return
  *   Compressed vector4D.
  */
  Vector4D
  GetColorIntensity();
  
  /*
  * Returns the light color.
  *
  * @return
  *   Light color.
  */
  Vector3D
  GetColor();
  
  /*
  * Returns the light range.
  *
  * @return
  *   Light range.
  */
  float
  GetRange();

  /*
  * Returns the light intensity.
  *
  * @return
  *   Light intensity.
  */
  float
  GetIntensity();

  void
  serialize(File &file) override;

  void
  deserialize(TString &data) override;

 private:

  // Inherited via GameComponent
  virtual void 
  onCreate() override;

  virtual void 
  onUpdate() override;

  virtual void 
  onRender() override;

  virtual void 
  onDestroy() override;

  virtual UInt32
  getClassID() override;

  virtual GameComponent* 
  cloneIn(GameObject& _go) override;

  Vector3D m_vec3Color;
  float m_fRange;
  float m_fIntensity;
};

}
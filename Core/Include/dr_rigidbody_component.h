#pragma once

#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"

#include <dr_physics_manager.h>
#include <dr_vector3d.h>
#include <dr_quaternion.h>
#include <dr_matrix3x3.h>
#include <vector>
namespace driderSDK {
  class DR_CORE_EXPORT RigidBody3DComponent : public GameComponent
  {
   
  public:
    const float GRAVITY = -0.081;
    RigidBody3DComponent(GameObject& _gameObject);

    virtual void
      onCreate() override;

    virtual void
      onUpdate() override;

    virtual void
      onRender() override;

    virtual void
      onDestroy() override;

    virtual void
      cloneIn(GameObject& _go) override;

    void 
    addForce(Vector3D _force);


    DrRigidBody* m_rigidBody;
  protected:

  };

}

#pragma once

#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"

#include <dr_id_object.h>
#include <dr_physics_manager.h>
#include <dr_vector3d.h>
#include <dr_quaternion.h>
#include <dr_matrix3x3.h>
#include <vector>
namespace driderSDK {
  class DR_CORE_EXPORT RigidBody3DComponent : public GameComponent,
    public IDClass<RigidBody3DComponent>
  {
   
  public:
    RigidBody3DComponent(GameObject& _gameObject);

    virtual void
    onCreate() override;

    virtual void
    onUpdate() override;

    virtual void
    onRender() override;

    virtual void
    onDestroy() override;

    virtual GameComponent*
    cloneIn(GameObject& _go) override;

    virtual UInt32
    getClassID() override;

    void 
    addForce(Vector3D _force);

    void
    addTorque(Vector3D torque);

    void
    setTransform(Transform transform);

    void 
    enableGravity(bool bUseGravity);

    void 
    setType(RIGID_BODY_TYPE::E type);


    DrRigidBody* m_rigidBody;
  protected:

  };

}


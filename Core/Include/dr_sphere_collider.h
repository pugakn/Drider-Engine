#pragma once

#include <dr_id_object.h>
#include "dr_collider_component.h"
#include "dr_core_prerequisites.h"

namespace driderSDK {

  class DR_CORE_EXPORT SphereCollider : public ColliderComponent,
    public IDClass<SphereCollider>
  {

  public:
    SphereCollider(GameObject& _gameObject ,const float radius, const Vector3D& position);
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

  private:
    virtual GameComponent*
      cloneIn(GameObject& _go) override;

    virtual COLLIDER_TYPE::E
      getType() override;
  private:
    float m_radius;
    Vector3D m_center;
    DrCollisionBody * m_body;
    // Inherited via ColliderComponent
  };

}

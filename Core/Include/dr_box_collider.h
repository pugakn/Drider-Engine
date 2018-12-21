#pragma once

#include <dr_aabb.h>
#include <dr_id_object.h>
#include "dr_collider_component.h"
#include "dr_core_prerequisites.h"

namespace driderSDK {

  class DR_CORE_EXPORT BoxCollider : public ColliderComponent,
    public IDClass<BoxCollider>
  {

  public:
    BoxCollider(GameObject& _gameObject, const AABB& aabb);
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

    void
    serialize(File &file) override;

    void
    deserialize(TString &data) override;

    void
      setAABB(const AABB& aabb);
    
    AABB
    getAABB();


  private:
    virtual GameComponent*
      cloneIn(GameObject& _go) override;

    virtual COLLIDER_TYPE::E
      getType() override;

  private:
    AABB m_aabb;

    // Inherited via ColliderComponent
  };

}

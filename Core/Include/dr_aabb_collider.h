#pragma once

#include <dr_aabb.h>
#include <dr_id_object.h>

#include "dr_collider_component.h"
#include "dr_core_prerequisites.h"

namespace driderSDK {

class DR_CORE_EXPORT AABBCollider : public ColliderComponent, 
                                    public IDClass<AABBCollider>
{

 public:
  AABBCollider(GameObject& _gameObject, const AABB& aabb);
  
  const AABB& 
  getTransformedAABB();

  AABB& 
  getAABB();

  virtual void 
  onCreate() override;
    
  virtual void 
  onUpdate() override;

  virtual void 
  onRender() override {}
  
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
  AABB m_originalAABB;
  AABB m_transformedAABB;

  // Inherited via ColliderComponent
};

}

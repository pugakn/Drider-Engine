#pragma once

#include <vector>
#include <AABBDebug.h>

namespace driderSDK {

class Skeleton;
class Technique;
class Transform;

class SkeletonDebug : public GameComponent {

public:
  SkeletonDebug(GameObject& _go);

  SkeletonDebug(const SkeletonDebug&) = delete;

  SkeletonDebug& operator=(const SkeletonDebug&) = delete;

  void
  setShaderTechnique(Technique* technique);


  virtual void 
  onCreate() override;

  virtual void 
  onUpdate() override;
  
  virtual void 
  onRender() override;

  virtual void 
  onDestroy() override;

  virtual void 
  cloneIn(GameObject& _go);
private:
  void
  create();
  using AABBDebugPtr = std::unique_ptr<AABBDebug>;
  using AABBDebugList = std::vector<AABBDebugPtr>;
  Technique* m_technique{nullptr};
  AABBDebugList m_aabbs;
  AABBDebugPtr m_globalAABB;
};

}


#pragma once

#include <vector>
#include <dr_vertex.h>
//#include <dr_gameComponent.h>
#include "DrawableComponent.h"

namespace driderSDK {

class Skeleton;
class Technique;
class Transform;

class SkeletonDebug : public DrawableComponent {

 public:

  SkeletonDebug(GameObject& _go);

  SkeletonDebug(const SkeletonDebug&) = delete;

  SkeletonDebug& operator=(const SkeletonDebug&) = delete;

  virtual void 
  onCreate() override;

  virtual void 
  onUpdate() override;
  
  virtual void 
  cloneIn(GameObject& _go);
 private:
  
  void
  create();

  void 
  buildSkeleton(void* bone, 
                Skeleton& skeleton,
                Mesh& mesh);

  //std::vector<Vertex> m_points;
};

}


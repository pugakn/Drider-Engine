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
  
  virtual GameComponent* 
  cloneIn(GameObject& _go);

  virtual UInt32
  getClassID();
 private:
  
  void
  create();

  void 
  buildSkeleton(void* bone, 
                Skeleton& skeleton,
                Mesh& mesh);


  // Inherited via DrawableComponent
  virtual void serialize(File & file) override;

  virtual void deserialize(TString & data) override;

  //std::vector<Vertex> m_points;
};

}


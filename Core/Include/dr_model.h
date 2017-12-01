#pragma once

#include <vector>
#include <dr_memory.h>
#include "dr_core_prerequisites.h"
#include "dr_resource.h"
#include "dr_mesh.h"
#include "dr_skeleton.h"

namespace driderSDK {

class DR_CORE_EXPORT Model : public Resource 
{
 public:
  Model();
  ~Model();
 private:

   virtual void create(void* pResourceInfo);

   std::vector<Mesh> m_meshes;
   std::unique_ptr<Skeleton> m_pSkeleton;
};

}

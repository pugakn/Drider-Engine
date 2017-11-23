#pragma once

#include <vector>
#include <memory>
#include "dr_core_prerequisites.h"
#include "dr_resource.h"
#include "dr_mesh.h"

namespace driderSDK {

class DR_CORE_EXPORT Model : public Resource 
{
 public:
  Model();
  ~Model();
 private:
   virtual void create(void* pResourceInfo);

   std::vector<Mesh> m_meshes;
};

}

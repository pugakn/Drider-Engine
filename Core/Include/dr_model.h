#pragma once

#include <vector>
#include <dr_prerequisites.h>
#include "dr_resource.h"

namespace driderSDK {

class Mesh;

class Model : public Resource 
{
 public:
  std::vector<Mesh> meshes; 
 private:
};

}

#pragma once

#include <vector>
#include <dr_prerequisites.h>

namespace driderSDK {

class Vertex;

class Mesh
{
 public:
  std::vector<Vertex> vertices;
  std::vector<Int16> indices; 
};

}

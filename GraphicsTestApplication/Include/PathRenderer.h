#pragma once

#include <dr_util_prerequisites.h>

namespace driderSDK {

class PathHolder;
class IndexBuffer;
class VertexBuffer;
class Technique;

class PathRenderer 
{
public:
  PathRenderer(PathHolder& _path);

  void draw(Technique* technique);

  ~PathRenderer();
private:

  void
  createBuffers();

  void 
  destroyBuffers();

  PathHolder& m_path;
  VertexBuffer* m_vertexBuffer{nullptr};
  IndexBuffer* m_indexBuffer{nullptr};
  UInt32 m_indicesCount;
  SizeT m_lastPoints;
};

}

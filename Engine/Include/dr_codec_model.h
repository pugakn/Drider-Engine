#pragma once

#include <unordered_map>
#include <dr_skeleton.h>
#include "dr_engine_prerequisites.h"
#include "dr_codec.h"

struct aiMesh;
struct aiScene;
struct aiNode;

namespace driderSDK {

struct ModelInfo;
struct MeshInfo;

class DR_ENGINE_EXPORT CodecModel : public Codec
{
 public:
  /** 
  * Default constructor
  */
  CodecModel() {}

  /**
  * Destructor
  */
  ~CodecModel() {}

 private:
  
  virtual UniqueVoidPtr 
  decode(TString pathName) override;

  virtual bool 
  encode(TString pathName) override;

  virtual bool 
  isCompatible(TString extension) override;

  /**
  * TEST::
  * Gets the type of the resource
  *
  * @return
  * Returns the type of the resource
  */
  CompatibleType::E
  getType() override;

  void 
  loadVertices(const aiMesh& inMesh, MeshInfo& outMesh);

  void 
  loadIndices(const aiMesh& inMesh, MeshInfo& outMesh);

  void
  loadSkeleton(const aiScene& model, 
               ModelInfo& outModel,
               Skeleton& outSkeleton);

  using NodesRefMap = std::unordered_map<String, Skeleton::NodeData*>;

  void 
  buildTree(const aiNode* pNodeSrc, 
            Skeleton::NodeData* pNode,
            NodesRefMap& nodesRefs);
};

}
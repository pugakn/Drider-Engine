#pragma once

#include <unordered_map>
#include "dr_engine_prerequisites.h"
#include "dr_codec.h"
#include "dr_skeleton_info.h"

struct aiMesh;
struct aiScene;
struct aiNode;

namespace driderSDK {

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
  loadSkeleton(const aiScene& model, SkeletonInfo& outSkeleton);

  using NodesRefMap = std::unordered_map<String,SkeletonInfo::NodeInfo*>;

  void 
  buildTree(const aiNode* pNodeSrc, 
            SkeletonInfo::NodeInfo* pNode,
            NodesRefMap& nodesRefs);
};

}
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
  loadVertices(const aiMesh& inMesh, 
               MeshInfo& outMesh, 
               Vector3D& minPos, 
               Vector3D& maxPos);

  void 
  loadIndices(const aiMesh& inMesh, MeshInfo& outMesh);

  void
  loadSkeleton(const aiScene& model, 
               ModelInfo& outModel,
               Skeleton& outSkeleton);

  void 
  loadAnimations(const aiScene& model, ModelInfo& outModel);

  /*void
  loadMaterials(const aiScene& model, ModelInfo& outModel);*/

  using NodesRefMap = std::unordered_map<TString, Skeleton::NodeData*>;

  void 
  buildTree(const aiNode* nodeSrc, 
            Skeleton::NodeData* node,
            NodesRefMap& nodesRefs);
};

}
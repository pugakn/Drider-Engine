#include "dr_model.h"
#include "dr_mesh.h"
#include "dr_model_info.h"
#include "dr_mesh_info.h"

namespace driderSDK {

Model::Model() {}

Model::~Model() {}

void 
Model::init(void* pModelData) {

  ModelInfo* pModel = static_cast<ModelInfo*>(pModelData);

  for (auto& meshInfo : pModel->meshes){
    Mesh mesh;
    mesh.vertices = meshInfo.vertices;
    mesh.indices = meshInfo.indices;
    meshes.push_back(std::move(mesh));
  }

  animationsNames = pModel->animationsNames;
  skeletonName = pModel->skeletonName;
}

}
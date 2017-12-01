#include "dr_model.h"
#include "dr_mesh.h"
#include "dr_model_info.h"
#include "dr_mesh_info.h"
#include "dr_skeleton_info.h"

namespace driderSDK {

Model::Model() {}

Model::~Model() {}

void Model::create(void* pResourceInfo) {

  ModelInfo* pModelInfo = static_cast<ModelInfo*>(pResourceInfo);

  m_meshes.resize(pModelInfo->meshes.size());

  for(SizeT i = 0; i < m_meshes.size(); ++i) {
    m_meshes[i].indices = std::move(pModelInfo->meshes[i].indices);
    m_meshes[i].vertices = std::move(pModelInfo->meshes[i].vertices);
  }
}

}
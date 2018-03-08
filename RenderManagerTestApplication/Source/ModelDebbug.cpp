#include "ModelDebbug.h"
#include <vector>
#include <dr_aabb_collider.h>
#include <dr_device_context.h>
#include <dr_graphics_api.h>
#include <dr_mesh.h>
#include <dr_aabb.h>
#include <dr_model.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_gameObject.h>
#include <dr_string_utils.h>
#include <iostream>

namespace driderSDK {

void 
ModelDebbug::create() {
  std::vector<Mesh> meshes{1};

  auto& mesh = meshes[0];

  auto aabbCollider = m_gameObject.getComponent<AABBCollider>();

  auto points = aabbCollider->getTransformedAABB().getBounds();

  mesh.vertices.resize(8);

  for (Int32 i = 0; i < 8; ++i) {
    mesh.vertices[i] = {Vector4D{points[i], 1.0}};
  }

  mesh.indices.push_back(AABB_POINT::kXMinYMinZMin);
  mesh.indices.push_back(AABB_POINT::kXMaxYMinZMin);

  mesh.indices.push_back(AABB_POINT::kXMaxYMinZMin);
  mesh.indices.push_back(AABB_POINT::kXMaxYMinZMax);

  mesh.indices.push_back(AABB_POINT::kXMaxYMinZMax);
  mesh.indices.push_back(AABB_POINT::kXMinYMinZMax);

  mesh.indices.push_back(AABB_POINT::kXMinYMinZMax);
  mesh.indices.push_back(AABB_POINT::kXMinYMinZMin);
  /////////////////////////////////////////////////
  mesh.indices.push_back(AABB_POINT::kXMinYMaxZMin);
  mesh.indices.push_back(AABB_POINT::kXMaxYMaxZMin);

  mesh.indices.push_back(AABB_POINT::kXMaxYMaxZMin);
  mesh.indices.push_back(AABB_POINT::kXMaxYMaxZMax);

  mesh.indices.push_back(AABB_POINT::kXMaxYMaxZMax);
  mesh.indices.push_back(AABB_POINT::kXMinYMaxZMax);

  mesh.indices.push_back(AABB_POINT::kXMinYMaxZMax);
  mesh.indices.push_back(AABB_POINT::kXMinYMaxZMin);
  /////////////////////////////////////////////////
  mesh.indices.push_back(AABB_POINT::kXMinYMinZMin);
  mesh.indices.push_back(AABB_POINT::kXMinYMaxZMin);

  mesh.indices.push_back(AABB_POINT::kXMaxYMinZMin);
  mesh.indices.push_back(AABB_POINT::kXMaxYMaxZMin);

  mesh.indices.push_back(AABB_POINT::kXMaxYMinZMax);
  mesh.indices.push_back(AABB_POINT::kXMaxYMaxZMax);

  mesh.indices.push_back(AABB_POINT::kXMinYMinZMax);
  mesh.indices.push_back(AABB_POINT::kXMinYMaxZMax);

  createMeshBuffers(meshes);
}

void
ModelDebbug::onCreate() {
  create();
}

void
ModelDebbug::onUpdate() {
  if (m_gameObject.changed()) {
    create();
  }
}

void
ModelDebbug::onRender() {
}
}
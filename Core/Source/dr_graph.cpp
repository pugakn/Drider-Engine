#include "dr_graph.h"
#include "dr_model.h"
#include "dr_root_node.h"
#include "dr_gameObject.h"
#include "dr_render_component.h"

namespace driderSDK {

SceneGraph::SceneGraph() {}

SceneGraph::~SceneGraph() {}

void 
SceneGraph::init() {
  m_root = std::make_shared<RootNode>();
}

SceneGraph::SharedNode 
SceneGraph::getRoot() const {
  return m_root;
}

void 
SceneGraph::update() {
  m_root->update();
}

void 
SceneGraph::draw() {
  m_root->draw();
}

SceneGraph::SharedGameObject
SceneGraph::createNode(SharedGameObject parent, SharedModel model) {

  SharedGameObject node = std::make_shared<GameObject>();

  RenderComponent::IndexBuffer ib;
  RenderComponent::VertexBuffer vb;
  RenderComponent::MaterialList ml;
  
  Int32 indexStart = 0;
  Int32 vertexStart = 0;

  for (auto& mesh : model->meshes) {
    MaterialRenderInfo info;
  
    info.indexStart = indexStart;
    info.indexSize = static_cast<Int32>(mesh.indices.size());
    ib.insert(ib.end(), mesh.indices.begin(), mesh.indices.end());
    
    info.vertexStart = vertexStart;
    info.vertexSize = static_cast<Int32>(mesh.vertices.size());
    vb.insert(vb.begin(), mesh.vertices.begin(), mesh.vertices.end());
    
    vertexStart += info.vertexStart;
    indexStart += info.indexSize;

    info.material = mesh.material;

    ml.push_back(info);
  }
  
  node->createComponent<RenderComponent>(std::move(ib), 
                                         std::move(vb), 
                                         std::move(ml));

  return node;
}

}
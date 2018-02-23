#include "dr_render_component.h"

namespace driderSDK {



RenderComponent::RenderComponent(GameObject& _gameObject,
                                 IndexBuffer&& _indexBuffer, 
                                 VertexBuffer&& _vertexBuffer, 
                                 MaterialList&& _materials) 
  : GameComponent(_gameObject),
    m_materials(std::move(_materials)),
    m_indexBuffer(std::move(_indexBuffer)),
    m_vertexBuffer(std::move(_vertexBuffer)) {
}

void 
RenderComponent::onCreate() {}

void 
RenderComponent::onUpdate() {}

void 
RenderComponent::onRender() {}

void 
RenderComponent::onDestroy() {}

}
#include "dr_renderman.h"
#include <dr_swap_chain.h>
#include <dr_graphics_driver.h>

namespace driderSDK {

RenderMan::RenderMan() : m_sceneGraph(nullptr){
}

RenderMan::RenderMan(SceneGraph* sceneGraph) : m_sceneGraph(sceneGraph) {
}

RenderMan::~RenderMan() {
}

void
RenderMan::init() {
  DrTextureDesc PositionDesc;
  PositionDesc.width = 1920;
  PositionDesc.height = 1080;
  PositionDesc.pitch = 1920 * 4;
  PositionDesc.dimension = DR_DIMENSION::k2D;
  PositionDesc.Format = DR_FORMAT::kR8G8B8A8_UNORM;
  PositionDesc.mipLevels = 0;
  PositionDesc.genMipMaps = true;

  PositionTex = GraphicsAPI::getDevice().createEmptyTexture(PositionDesc);
  PositionRT  = GraphicsAPI::getDevice().createRenderTarget({PositionTex});


  Viewport vp;
  vp.minDepth = 0.1f;
  vp.maxDepth = 1000.0f;
  vp.width = 1280;
  vp.width = 720;
  Sauron.setName(L"MainCam");
  Sauron.setViewport(vp);

  m_GBufferPass.init(&m_GBufferInitData);
}

void
RenderMan::draw() {
  queryRequest = m_sceneGraph->query(Sauron,
                                     QUERY_ORDER::kFrontToBack,
                                     QUERY_PROPERTYS::kOpaque |
                                     QUERY_PROPERTYS::kStatic);

  m_GBufferDrawData.activeCam = &Sauron;
  m_GBufferDrawData.models = &queryRequest;
  m_GBufferPass.draw(&m_GBufferDrawData);

  /*
  Render order:

  GBuffer
  Depth Pre-Pass
  Shadow Maps
  Screen Space Ambient Occlusion
  Light Pre-Pass
  Forward-Rendering of Opaque Objects
  Transparent Objects
  Light Bloom
  Anti-Aliasing
  Color Correction
  User Interface

  Depth of Field
  */

  GraphicsDriver::API().swapBuffers();
}

void
RenderMan::exit() {
}

}
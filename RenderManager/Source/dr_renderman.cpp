#include "dr_renderman.h"
#include <dr_swap_chain.h>
#include <dr_graphics_driver.h>
#include <dr_d3d_depth_stencil.h>
#include <dr_resource_manager.h>
#include <dr_model.h>

namespace driderSDK {

RenderMan::RenderMan() {
}

RenderMan::~RenderMan() {
}

void
RenderMan::init() {
  Device& dc = GraphicsAPI::getDevice();

  DrTextureDesc PositionDesc;
  PositionDesc.width = 1920;
  PositionDesc.height = 1080;
  PositionDesc.pitch = 1920 * 4;
  PositionDesc.dimension = DR_DIMENSION::k2D;
  PositionDesc.Format = DR_FORMAT::kR8G8B8A8_UNORM;
  PositionDesc.mipLevels = 0;
  PositionDesc.genMipMaps = true;

  ColorTex        = dc.createEmptyTexture(PositionDesc);
  PositionTex     = dc.createEmptyTexture(PositionDesc);
  NormalTex       = dc.createEmptyTexture(PositionDesc);

  m_GBufferDSoptions = dc.createDepthStencil(*ColorTex);

  m_RTs = GraphicsAPI::getDevice().createRenderTarget( { ColorTex, PositionTex, NormalTex } );

  m_viewport.width = 1920;
  m_viewport.height = 1080;
  Sauron = std::make_shared<Camera>(_T("PATO_CAM"), m_viewport);

  Sauron->createProyection(45.f, 20.f, 3000.f);
  Sauron->getTransform().setPosition({ 0.0f, 100.0f, -200.0f });
  Sauron->setTarget({ 0.0f, 50.f, 0.0f });
  SceneGraph::addObject(Sauron);

  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));

  m_GBufferPass.init(&m_GBufferInitData);
  m_SSAOPass.init(&m_SSAOInitData);
}

void
RenderMan::draw() {
  auto queryRequest = SceneGraph::query(*Sauron,
                                 QUERY_ORDER::kFrontToBack,
                                 QUERY_PROPERTY::kOpaque | 
                                 QUERY_PROPERTY::kDynamic | 
                                 QUERY_PROPERTY::kStatic);

  m_GBufferDrawData.activeCam = Sauron;
  m_GBufferDrawData.models = &queryRequest;
  m_GBufferDrawData.OutRt = m_RTs;
  m_GBufferDrawData.dsOptions = m_GBufferDSoptions;
  m_GBufferPass.draw(&m_GBufferDrawData);

  m_SSAODrawData.GbufferRT = m_RTs;
  m_SSAOPass.draw(&m_SSAODrawData);

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
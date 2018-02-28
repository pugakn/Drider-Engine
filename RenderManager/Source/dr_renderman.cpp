#include "dr_renderman.h"

namespace driderSDK {

RenderMan::RenderMan() {
}

RenderMan::~RenderMan() {
}

template<typename T, typename... Args> void
RenderMan::init(T t, Args... args) {
  DrTextureDesc PositionDesc;
  PositionDesc.width = 1920;
  PositionDesc.height = 1080;
  PositionDesc.pitch = 1920 * 4;
  PositionDesc.dimension = DR_DIMENSION::k2D;
  PositionDesc.Format = DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
  PositionDesc.mipLevels = 0;
  PositionDesc.genMipMaps = true;

  RenderTarget* PositionRT;

  Texture* PositionTex = GraphicsAPI::getDevice().createEmptyTexture(PositionDesc);
  GraphicsAPI::getDevice().createRenderTarget(*PositionTex);


  Viewport vp;
  vp.minDepth = 0.1f;
  vp.maxDepth = 1000.0f;
  vp.width = 1280;
  vp.width = 720;
  Camera Sauron(L"MainCam", vp);

  m_GBufferPass.init(0);
}


template<typename T, typename... Args> void
RenderMan::draw(T t, Args... args) {
  queryRequest = m_sceneGraph->query(Sauron,
                                     QUERY_ORDER::kFrontToBack,
                                     QUERY_PROPERTYS::kOpaque |
                                     QUERY_PROPERTYS::kStatic);

  std::weak_ptr<Material> meshMat = queryRequest.back().second.material;

  //GBP.draw( { &Sauron, &meshMat } );
  /*
  Render order:

  GBuffer;
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
}

template<typename T, typename... Args>
void
RenderMan::exit(T t, Args... args) {
}

template<typename T>
void
driderSDK::RenderMan::extractParam(int index, void * out, T t) {
  *static_cast<T*>(out) = t;
}

template<typename T, typename ...Args>
void
driderSDK::RenderMan::extractParam(int index, void * out, T t, Args ...args) {
  if (index != 0) {
    extractParam(index - 1, out, args...);
    return;
  }

  *static_cast<T*>(out) = t;
}

}
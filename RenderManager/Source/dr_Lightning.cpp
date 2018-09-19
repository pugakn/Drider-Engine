#include "dr_Lightning.h"
#include <dr_device.h>
#include <dr_graphics_api.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_device_context.h>
#include <dr_resource_manager.h>
#include <dr_model.h>
#include <dr_texture.h>
#include <dr_depth_stencil.h>
#include <dr_camera_manager.h>

namespace driderSDK {

LightningPass::LightningPass() {
}

LightningPass::~LightningPass() {
}

void
LightningPass::init(PassInitData* initData) {
  LightningInitData* data = static_cast<LightningInitData*>(initData);
  Device& device = GraphicsAPI::getDevice();

  File file;
  String shaderSrc;
  TString csName;

  String precomputeString = "#define RM_MAX_LIGHTS " +
                            StringUtils::toString(RM_MAX_LIGHTS) +
                            "\n" +
                            "#define RM_MAX_LIGHTS_PER_BLOCK " +
                            StringUtils::toString(RM_MAX_LIGHTS_PER_BLOCK) +
                            "\n" +
                            "#define RM_TILE_LIGHTS_SZ " +
                            StringUtils::toString(RM_TILE_LIGHTS_SZ) +
                            "\n";

  //////////Compile WS Lights to SS Shader//////////
  csName = _T("Resources\\Shaders\\WorldLightToSS_cs.hlsl");

  file.Open(csName);
  shaderSrc = precomputeString + StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  m_csWorldLightsToSS = dr_gfx_unique(device.createShaderFromMemory(shaderSrc.data(),
                                                                    shaderSrc.size(),
                                                                    DR_SHADER_TYPE_FLAG::kCompute));

  shaderSrc.clear();

  //////////Compile Tile Lights Shader//////////
  csName = _T("Resources\\Shaders\\TileLights_cs.hlsl");

  file.Open(csName);
  shaderSrc = precomputeString + StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();

  m_csTiledLights = dr_gfx_unique(device.createShaderFromMemory(shaderSrc.data(),
                                  shaderSrc.size(),
                                  DR_SHADER_TYPE_FLAG::kCompute));

  shaderSrc.clear();

  //////////Compile Draw Shader//////////
  m_csFilename = _T("Resources\\Shaders\\Lightning_cs.hlsl");
  recompileShader();


  DrBufferDesc bdesc;

  //////////Create WS Lights to SS ConstantBuffer//////////
  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBufferWSLightsToSS);
  m_CBWSLightsToSS = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));

  //////////Create WS Lights to SS ConstantBuffer//////////
  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBufferTiledLights);
  m_CBTileLights = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));

  //////////Create Draw ConstantBuffer//////////
  bdesc.type = DR_BUFFER_TYPE::kCONSTANT;
  bdesc.sizeInBytes = sizeof(CBufferDraw);
  m_CBDraw = dr_gfx_unique((ConstantBuffer*)device.createBuffer(bdesc));

  //SamplerState
  DrSampleDesc SSdesc;
  SSdesc.Filter = DR_TEXTURE_FILTER::kMIN_MAG_LINEAR_MIP_POINT;
  SSdesc.maxAnisotropy = 16;
  SSdesc.addressU = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressV = DR_TEXTURE_ADDRESS::kWrap;
  SSdesc.addressW = DR_TEXTURE_ADDRESS::kWrap;

  m_samplerState = dr_gfx_unique(device.createSamplerState(SSdesc));

  m_ComputeWidthDivisions = 8;
  m_ComputeHeightDivisions = 4;

  //////////Lights Textures//////////
  std::vector<Texture*> m_vecTexture;

  //Lights in ScreenSpace
  DrTextureDesc m_TexDesc;
  m_TexDesc.dimension = DR_DIMENSION::k2D;
  m_TexDesc.width = RM_MAX_LIGHTS;
  m_TexDesc.height = 1;
  m_TexDesc.Format = DR_FORMAT::kR32G32B32A32_FLOAT;
  m_TexDesc.pitch = m_TexDesc.width * 4 * 4;
  m_TexDesc.mipLevels = 0;
  m_TexDesc.CPUAccessFlags = 0;
  m_TexDesc.genMipMaps = false;
  m_TexDesc.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE |
                        DR_BIND_FLAGS::RENDER_TARGET |
                        DR_BIND_FLAGS::UNORDERED_ACCESS;

  
  GFXUnique<Texture> lightsTransformedTex = dr_gfx_unique<Texture>(device.createEmptyTexture(m_TexDesc));
  m_vecTexture.push_back(lightsTransformedTex.get());
  m_RTLightsInSS = dr_gfx_shared(device.createRenderTarget(m_vecTexture));

  m_vecTexture.clear();
  lightsTransformedTex.release();

  m_RTWidth = data->RTWidth;
  m_RTHeight = data->RTHeight;
  SizeT totalTiles = Math::ceil(m_RTWidth / ((float)RM_TILE_LIGHTS_SZ)) *
                     Math::ceil(m_RTHeight / ((float)RM_TILE_LIGHTS_SZ));

  //Lights index
  m_TexDesc.dimension = DR_DIMENSION::k2D;
  m_TexDesc.width = totalTiles;
  m_TexDesc.height = 1 + RM_MAX_LIGHTS_PER_BLOCK;
  m_TexDesc.Format = DR_FORMAT::kR32_SINT;
  m_TexDesc.pitch = m_TexDesc.width * 4;
  
  GFXUnique<Texture> LightsIndexTex = dr_gfx_unique<Texture>(device.createEmptyTexture(m_TexDesc));
  m_vecTexture.push_back(LightsIndexTex.get());
  m_RTLightsIndex = dr_gfx_shared(device.createRenderTarget(m_vecTexture));

  m_vecTexture.clear();
  LightsIndexTex.release();

  //Lights index aux
  m_TexDesc.height = RM_MAX_LIGHTS_PER_BLOCK;

  GFXUnique<Texture> LightsIndexAuxTex = dr_gfx_unique<Texture>(device.createEmptyTexture(m_TexDesc));
  m_vecTexture.push_back(LightsIndexAuxTex.get());
  m_RTLightsIndexAux = dr_gfx_shared(device.createRenderTarget(m_vecTexture));

  m_vecTexture.clear();
  LightsIndexAuxTex.release();
}

void
LightningPass::recompileShader(String vsPreText,
                               String psPreText,
                               String csPreText) {
  Device& dc = GraphicsAPI::getDevice();

  
  String precomputeString = "#define RM_MAX_LIGHTS " +
                            StringUtils::toString(RM_MAX_LIGHTS) +
                            "\n" +
                            "#define RM_MAX_LIGHTS_PER_BLOCK " +
                            StringUtils::toString(RM_MAX_LIGHTS_PER_BLOCK) +
                            "\n" +
                            "#define RM_TILE_LIGHTS_SZ " +
                            StringUtils::toString(RM_TILE_LIGHTS_SZ) +
                            "\n";

  RenderPass::recompileShader("", "", precomputeString);
}

void
LightningPass::lightsToScreenSpace(LightningLightsToSSData* data) {
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setUAVsNull();
  dc.setResourcesNull();

  m_csWorldLightsToSS->set(dc);

  m_RTLightsInSS->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute);
  
  m_CBWSLightsToSSData.CameraUp = data->ActiveCam->getLocalUp().normalize();
  
  m_CBWSLightsToSSData.CameraVP = data->ActiveCam->getVP();

  for (SizeT lighIndex = 0; lighIndex < RM_MAX_LIGHTS; ++lighIndex) {
    m_CBWSLightsToSSData.LightPosition[lighIndex] = (*data->Lights)[lighIndex].m_vec4Position;
  }

  m_CBWSLightsToSS->updateFromBuffer(dc, reinterpret_cast<byte*>(&m_CBWSLightsToSSData));
  m_CBWSLightsToSS->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);

  SizeT csDivisions = static_cast<SizeT>(Math::ceil(RM_MAX_LIGHTS / 32.0f));
  dc.dispatch(csDivisions, 1, 1);

  dc.setUAVsNull();
  dc.setResourcesNull();
}

//////////KILLER FUNCTION//////////
void
LightningPass::tileLights(LightningTileLightsSSData* data) {
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setUAVsNull();
  dc.setResourcesNull();

  m_csTiledLights->set(dc);

  m_RTLightsInSS->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute, true);

  m_RTLightsIndex->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute);
  m_RTLightsIndexAux->getTexture(0).set(dc, 1, DR_SHADER_TYPE_FLAG::kCompute);

  DrTextureDesc outRTDesc = data->OutRt->getDescriptor();

  m_RTWidth = outRTDesc.width;
  m_RTHeight = outRTDesc.height;

  m_ComputeWidthBlocks = Math::ceil(m_RTWidth / ((float)RM_TILE_LIGHTS_SZ));
  m_ComputeHeightBlocks = Math::ceil(m_RTHeight / ((float)RM_TILE_LIGHTS_SZ));

  m_CBTileLightsData.threadsInfo.x = m_ComputeWidthBlocks;
  m_CBTileLightsData.threadsInfo.y = m_ComputeHeightBlocks;

  m_CBTileLights->updateFromBuffer(dc, reinterpret_cast<byte*>(&m_CBTileLightsData));
  m_CBTileLights->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);

  dc.dispatch(m_ComputeWidthBlocks, m_ComputeHeightBlocks, 1);

  dc.setUAVsNull();
  dc.setResourcesNull();
}

void
LightningPass::draw(PassDrawData* drawData) {
  LightningDrawData* data = static_cast<LightningDrawData*>(drawData);
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  dc.setUAVsNull();
  dc.setResourcesNull();

  m_computeShader->set(dc);

  DrTextureDesc outRTDesc = data->OutRt->getDescriptor();

  m_RTWidth = outRTDesc.width;
  m_RTHeight = outRTDesc.height;

  m_ComputeWidthBlocks = m_RTWidth / m_ComputeWidthDivisions;
  m_ComputeHeightBlocks = m_RTHeight / m_ComputeHeightDivisions;

  m_ComputeTotalBlocks = m_ComputeWidthBlocks * m_ComputeHeightBlocks;

  m_samplerState->set(dc, DR_SHADER_TYPE_FLAG::kCompute);

  m_CBDrawData.ViewportDimensions.x = m_RTWidth;
  m_CBDrawData.ViewportDimensions.y = m_RTHeight;

  m_CBDrawData.EyePosition = data->ActiveCam->getPosition();
  m_CBDrawData.EyePosition.w = data->ActiveLights;

  for (SizeT lighIndex = 0; lighIndex < RM_MAX_LIGHTS; ++lighIndex) {
    m_CBDrawData.LightPosition[lighIndex] = (*data->Lights)[lighIndex].m_vec4Position;
    m_CBDrawData.LightColor[lighIndex] = (*data->Lights)[lighIndex].m_vec4Color;
  }

  m_CBDrawData.ThreadsInfo.x = m_ComputeWidthBlocks;
  m_CBDrawData.ThreadsInfo.y = m_ComputeHeightBlocks;
  m_CBDrawData.ThreadsInfo.z = Math::ceil(m_RTWidth / ((float)RM_TILE_LIGHTS_SZ));
  m_CBDrawData.ThreadsInfo.w = Math::ceil(m_RTHeight / ((float)RM_TILE_LIGHTS_SZ));

  m_CBDraw->updateFromBuffer(dc, reinterpret_cast<byte*>(&m_CBDrawData));
  m_CBDraw->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);

  data->GbufferRT->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute, true);        //Position, linear depth
  data->GbufferRT->getTexture(1).set(dc, 1, DR_SHADER_TYPE_FLAG::kCompute, true);        //Normal, CoC
  data->GbufferRT->getTexture(2).set(dc, 2, DR_SHADER_TYPE_FLAG::kCompute, true);        //Albedo, Metallic
  data->GbufferRT->getTexture(3).set(dc, 3, DR_SHADER_TYPE_FLAG::kCompute, true);        //Emissivve, Roughness
  data->SSAO_SSShadowRT->getTexture(0).set(dc, 4, DR_SHADER_TYPE_FLAG::kCompute, true);  //SSAO & Shadow
  m_RTLightsIndex->getTexture(0).set(dc, 5, DR_SHADER_TYPE_FLAG::kCompute, true);
  data->EnviromentCubemap->textureGFX->set(dc, 6, DR_SHADER_TYPE_FLAG::kCompute, true);  //Cubemap
  data->IrradianceCubemap->textureGFX->set(dc, 7, DR_SHADER_TYPE_FLAG::kCompute, true);  //CubemapDiffuse

  data->OutRt->getTexture(0).set(dc, 0, DR_SHADER_TYPE_FLAG::kCompute, false);

  dc.dispatch(m_ComputeWidthBlocks, m_ComputeHeightBlocks, 1);

  dc.setUAVsNull();
  dc.setResourcesNull();
}

}
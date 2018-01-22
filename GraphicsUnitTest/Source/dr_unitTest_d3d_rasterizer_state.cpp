#pragma once
#include <dr_d3d_rasterizer_state.h>
#include <gtest\gtest.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <d3d11.h>
#include <dxgi.h>

TEST(D3DRasterizerState, create) {
  driderSDK::D3DRasterizerState obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);
  driderSDK::DrRasterizerDesc desc;
  desc.fillMode = driderSDK::DR_FILL_MODE::kSolid;
  desc.cullMode = driderSDK::DR_CULL_MODE::kBack;
  desc.frontCounterClockwise = true;
  desc.depthBias = 0;
  desc.depthBiasClamp = 0.0f;
  desc.slopeScaledDepthBias = 0.0f;
  desc.depthClipEnable = TRUE;
  desc.scissorEnable = FALSE;
  desc.multisampleEnable = FALSE;
  desc.antialiasedLineEnable = FALSE;
  
  EXPECT_TRUE(!obj.create(device, desc));
}

TEST(D3DRasterizerState, set) {
  driderSDK::D3DRasterizerState obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);
  driderSDK::DrRasterizerDesc desc;
  desc.fillMode = driderSDK::DR_FILL_MODE::kSolid;
  desc.cullMode = driderSDK::DR_CULL_MODE::kBack;
  desc.frontCounterClockwise = true;
  desc.depthBias = 0;
  desc.depthBiasClamp = 0.0f;
  desc.slopeScaledDepthBias = 0.0f;
  desc.depthClipEnable = TRUE;
  desc.scissorEnable = FALSE;
  desc.multisampleEnable = FALSE;
  desc.antialiasedLineEnable = FALSE;
  obj.create(device, desc);
  obj.set(context);
  EXPECT_TRUE(true);
}

TEST(D3DRasterizerState, release) {
  driderSDK::D3DRasterizerState obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);
  driderSDK::DrRasterizerDesc desc;
  desc.fillMode = driderSDK::DR_FILL_MODE::kSolid; 
  desc.cullMode = driderSDK::DR_CULL_MODE::kBack;
  desc.frontCounterClockwise = true;
  desc.depthBias = 0;
  desc.depthBiasClamp = 0.0f;
  desc.slopeScaledDepthBias = 0.0f;
  desc.depthClipEnable = TRUE;
  desc.scissorEnable = FALSE;
  desc.multisampleEnable = FALSE;
  desc.antialiasedLineEnable = FALSE;
  obj.create(device,desc);
  obj.release();
  EXPECT_TRUE(obj.APIState);
}
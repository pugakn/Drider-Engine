#pragma once
#include <dr_d3d_depth_stencil.h>
#include <gtest\gtest.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <d3d11.h>
#include <dxgi.h>

TEST(D3DDepthStencil, create) {
  driderSDK::D3DDepthStencil obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);
  driderSDK::DrTextureDesc desc;
  desc.width = 1920;
  desc.pitch = 1920 * 4;
  desc.height = 1080;
  desc.mipLevels = 1;
  desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
  desc.CPUAccessFlags = 0;
  
  EXPECT_TRUE(!obj.create(device, desc));
}

TEST(D3DDepthStencil, release) {
  driderSDK::D3DDepthStencil obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);
  driderSDK::DrTextureDesc desc;
  desc.width = 1920;
  desc.pitch = 1920 * 4;
  desc.height = 1080;
  desc.mipLevels = 1;
  desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
  desc.CPUAccessFlags = 0;
  obj.create(device, desc);
  EXPECT_TRUE(!obj.APIDepthView.Get());
}